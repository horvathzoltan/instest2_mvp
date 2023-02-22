#include <QCommandLineParser>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "dowork.h"
#include "logger.h"
#include "models/responsemodel.h"
#include "models/model.h"
#include "ipscanner.h"

const QString DoWork::APIVER = QStringLiteral("apiver");
QList<Model::InsoleType> DoWork::_insoleTypes;

DoWork::DoWork(QObject *parent) :QObject(parent)
{
    timer_L.setInterval(64);
    timer_R.setInterval(64);
    connect(&timer_L, &QTimer::timeout, this, &DoWork::onTimeout_L);
    connect(&timer_R, &QTimer::timeout, this, &DoWork::onTimeout_R);
}

auto DoWork::init(const DoWorkInit& m) -> bool
{
    Q_UNUSED(m)

    _isInited = false;

//    if(!_httpHelper.init(m.settings._host, m.settings._port)) return _isInited;

    //QObject::connect(&httpHelper, SIGNAL(ResponseOk(QUuid,QString,QByteArray)),
    //                 this, SLOT(ResponseOkAction(QUuid,QString,QByteArray)));

    _insoleTypes = Model::InsoleType::ParseList(Model::InsoleType::CSV);
    _isInited = true;

    return true;
}

ResponseModel::FindPi DoWork::FindPi(const QSet<int>& ports, int timeout, int steps)
{       
    _findPiPresenterGuid=QUuid::createUuid();
    ResponseModel::FindPi r(_findPiPresenterGuid);

    _insoleApis.clear();
    _apiKey_L.clear();
    _apiKey_R.clear();
    timer_L.stop();
    timer_R.stop();

    //FindPiModelR result;

    auto a = IpScanner::GetLocalAddresses();
    if(a.isEmpty())
    {
        r.message = QStringLiteral("No local ip address");
    }
    else
    {
        FindPiThread::Model m(a.first(), ports, timeout, steps);
        StartFindPiThread(m, _findPiPresenterGuid);
        r.message= QStringLiteral("searching...");
    }
    return r;
}

void DoWork::StartFindPiThread(FindPiThread::Model m, QUuid guid)
{
    FindPiThread *workerThread = new FindPiThread(m, guid);
    connect(workerThread, &FindPiThread::resultReady, this, &DoWork::FindPiThreadResults);
    connect(workerThread, &FindPiThread::finished, workerThread, &QObject::deleteLater);
    workerThread->start();
}

void DoWork::FindPiThreadResults(const FindPiThread::Result& m)
{
    ResponseModel::FindPi r(m.guid);
    for(auto&key:m.ipList.keys()){
        QSet<int> values = m.ipList[key];
        QString str;//ports
        for(auto&v:values){
            if(!str.isEmpty()) str+=',';
            str+=QString::number(v);
        };
        auto txt = "ip:" + key + ":" +str;

        //r.iplist.append(txt);
        zInfo("pi found: "+txt);
    }

    r.status=ResponseModel::FindPi::Started;
    emit ResponseFindPi(r);

    for(auto&key:m.ipList.keys()){
        HttpHelper* h = new HttpHelper();
        h->init("http://"+key, 8080);
        InsoleApi insoleApi{ .key=key,
                    //.presenterGuid=m.guid,
                    .httpHelper=h,
                    .checked=false,
                    .isError=false};

        _insoleApis.insert(key, insoleApi);
        QObject::connect(h, SIGNAL(ResponseOk(QUuid,QString,QByteArray)),
                             this, SLOT(ResponseOkAction(QUuid,QString,QByteArray)));
        QObject::connect(h, SIGNAL(ResponseErr(QUuid,QString)),
                             this, SLOT(ResponseNotOkAction(QUuid,QString)));
        // response err
        zInfo("trying: "+key);
        QUuid actionKey = h->GetAction("version");
        _actions.insert(actionKey, key);        
    }
    // ha van pi, kérjük le a típust, és a talpbetétet!
}

DoWork::InsoleApi* DoWork::GetApi(const QString &key)
{
    QMap<QString, InsoleApi>::iterator a = _insoleApis.find(key);
    if(a==_insoleApis.end()) return nullptr;
    auto c = &a.value();

    return c;
}



//QUuid DoWork::GetApiver()
//{
//    return _httpHelper.GetAction(APIVER);
//}
void DoWork::ResponseNotOkAction(const QUuid& guid, const QString& action){
    Q_UNUSED(action)

    if(_actions.contains(guid)){
        auto insoleApiKey = _actions[guid];
        auto& insoleApi = _insoleApis[insoleApiKey];
        insoleApi.checked=true;
        insoleApi.isError=true;
        CheckReady();
    }
}

void DoWork::ResponseOkAction(const QUuid& guid, const QString& action,  QByteArray response)
{
    if(_actions.contains(guid)){
        auto insoleApiKey = _actions[guid];
        auto& insoleApi = _insoleApis[insoleApiKey];
//        insoleApi.checked=true;
        insoleApi.isError=false;
        //auto responseString = QString(s);
        //zInfo("ResponseOkAction: "+a+": "+responseString);
        if(action=="version") //pizero_mcp3008_api:1488
        {
            auto responseString = QString(response);
            zInfo("version: "+insoleApiKey+": "+responseString);
            QStringList r = responseString.split(':');
            if(r.length()>=2)
            {
                if(r[0]=="pizero_mcp3008_api")
                {
                    insoleApi.name = r[0];
                    bool ok;
                    auto v = r[1].toInt(&ok);
                    if(ok){
                        insoleApi.buildnum = v;
                        QUuid actionKey = insoleApi.httpHelper->GetAction("get_data_length");
                        _actions.insert(actionKey, insoleApiKey);
                    }
                }
            }
        }
        else if(action=="get_data_length") //get_data_length 16
        {
            auto responseString = QString(response);
            zInfo("get_data_length: "+insoleApiKey+": "+responseString);
            bool ok;
            auto v = responseString.toInt(&ok);
            if (ok) {
                insoleApi.datalength = v;
                QUuid actionKey = insoleApi.httpHelper->GetAction("get_data");
                _actions.insert(actionKey, insoleApiKey);
            }
        }

        else if(action=="get_data") //50;138;142;121;122;123;121;123;124;121;132;141;123;119;122;123
        {
            insoleApi.insoleData = Model::InsoleData::Parse(response, insoleApi.datalength);
            if(insoleApi.status == Waiting){
                insoleApi.checked=true;
                insoleApi.insoleType = GetInsoleType(insoleApi.insoleData.V);

                QString msg = insoleApi.toString();
                zInfo("insoleApi: "+msg);

                // ha egy api válaszol, lecsekkoljuk, hogy van-e még aki nem
                // ha mindegyik válaszolt, akkor ok, készen vagyunk
                bool ok = CheckReady();
                if(ok)
                {
                    ResponseModel::FindPi r = CheckReady2();
                    r.status=ResponseModel::FindPi::Finished;
                    emit ResponseFindPi(r);
                    if(r.apiKey_L.count()==1)
                    {
                        _apiKey_L=r.apiKey_L.first();
                    }
                    if(r.apiKey_R.count()==1)
                    {
                        _apiKey_R=r.apiKey_R.first();
                    }
                    if(!(_apiKey_L.isEmpty() || _apiKey_R.isEmpty())){
                        zInfo("start getdata");
                        startTimers();
                    }
                }
            }
            else if(insoleApi.status == Started){
                ResponseModel::PiData r(_findPiPresenterGuid);
                r.direction = insoleApi.direction;
                r.message = insoleApi.insoleData.toString();
                emit ResponsePiData(r);
            }
        }
    }
}
// lecsekkolja az apikat, ha mind válaszolt, akkor ok
bool DoWork::CheckReady()
{
    int checkedApiCounter = 0;

    for(auto&key:_insoleApis.keys())
    {
        auto& api = _insoleApis[key];
        if(!api.checked) return false; // ha van olyan api ami nem volt még csekkolva
        checkedApiCounter++;
        if(api.isError) continue;        
    }
    bool ok = checkedApiCounter>0;
    return ok;
}

ResponseModel::FindPi DoWork::CheckReady2()
{
    ResponseModel::FindPi r(_findPiPresenterGuid);

    for(auto&key:_insoleApis.keys())
    {
        auto& api = _insoleApis[key];
        if(api.isError) continue;
        //QString msg = api.toString();
        if(api.insoleType!=nullptr)
        {
            auto d = api.insoleType->direction();
            switch(d){
            case Model::PhysDirection::Left: r.apiKey_L.append(key); break;
            case Model::PhysDirection::Right: r.apiKey_R.append(key); break;
            default: r.apiKey_U.append(key); break;
            }
        } else {
            r.apiKey_E.append(key);
        }
    }

    return r;
}

void DoWork::onTimeout_L()
{
    //InsoleApi* api = _insoleApis[_apiKey_L];
    onTimeout(Model::PhysDirection::Directions::Left);
}

void DoWork::onTimeout_R()
{
    onTimeout(Model::PhysDirection::Directions::Right);
}

void DoWork::startTimers(){
    InsoleApi& api_L = _insoleApis[_apiKey_L];
    api_L.direction = Model::PhysDirection::Directions::Left;
    api_L.status = Started;
    api_L.httpHelper->setVerbose(false);

    InsoleApi& api_R = _insoleApis[_apiKey_R];
    api_R.direction = Model::PhysDirection::Directions::Right;
    api_R.status = Started;
    api_R.httpHelper->setVerbose(false);

    timer_L.start();
    timer_R.start();
}
//https://stackoverflow.com/questions/15835267/qthread-and-qtimer
void DoWork::onTimeout(Model::PhysDirection::Directions d)
{
    if(d==Model::PhysDirection::Directions::Left){
        InsoleApi& api = _insoleApis[_apiKey_L];
        QUuid actionKey = api.httpHelper->GetAction("get_data");
        _actions.insert(actionKey, _apiKey_L);
    } else if(d==Model::PhysDirection::Directions::Right){
        InsoleApi& api = _insoleApis[_apiKey_R];
        QUuid actionKey = api.httpHelper->GetAction("get_data");
        _actions.insert(actionKey, _apiKey_R);
    }

//    ResponseModel::PiData r(_findPiPresenterGuid);
//    r.direction = d;
//    static int i_L,i_R;
//    if(d==Model::PhysDirection::Directions::Left){
//        r.message = QString::number(i_L++);
//    } else if(d==Model::PhysDirection::Directions::Right){
//        r.message = QString::number(i_R++);
//    }

//    emit ResponsePiData(r);
}

//void DoWork::GetApiverResponse(const QUuid& guid, QByteArray s){
//    QJsonParseError errorPtr;
//    QJsonDocument doc = QJsonDocument::fromJson(s, &errorPtr);
//    QJsonObject rootobj = doc.object();
//    ResponseModel::GetApiVer r(guid);

//    //Model::ApiVer m;
//    if(rootobj.isEmpty()){
//        r.msg = "no response";
//    }else{
//        r.apiVer = {55};//Model::ApiVer::JsonParse(rootobj);
//        r.msg = "apiver: "+r.apiVer.toString();
//    }

//    emit ResponseGetApiverAction(r);
//}


Model::InsoleType* DoWork::GetInsoleTypeR(int r){
    for(auto&insoleType:_insoleTypes)
        if(insoleType.R==r) return &insoleType;
    return nullptr;
}

Model::InsoleType* DoWork::GetInsoleType(int v){
    for(auto&insoleType:_insoleTypes)
        if(insoleType.VMin<=v && v<=insoleType.VMax) return &insoleType;
    return nullptr;
}

QString DoWork::InsoleApi::toString(){
    QString msg =  name
            +","+QString::number(buildnum)
            +","+(checked?"ok":"Nok")
            +","+QString::number(datalength);
    //+","+data;
    if(insoleType!=nullptr){
        msg+=' '+insoleType->toString();
    }
    return msg;
}

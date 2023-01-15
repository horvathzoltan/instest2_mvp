#include "dowork.h"
#include "logger.h"
#include "models/responsemodel.h"
#include <QCommandLineParser>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <models/model.h>
#include <models/featurerequest.h>
#include "ipscanner.h"

const QString DoWork::APIVER = QStringLiteral("apiver");


DoWork::DoWork(QObject *parent) :QObject(parent)
{

}

auto DoWork::init(const DoWorkInit& m) -> bool
{
    _isInited = false;

//    if(!_httpHelper.init(m.settings._host, m.settings._port)) return _isInited;

    //QObject::connect(&httpHelper, SIGNAL(ResponseOk(QUuid,QString,QByteArray)),
    //                 this, SLOT(ResponseOkAction(QUuid,QString,QByteArray)));

    _isInited = true;
    return true;
}

ResponseModel::FindPi DoWork::FindPi(const QSet<int>& ports, int timeout)
{   
    _findPiPresenterGuid=QUuid::createUuid();
    ResponseModel::FindPi r(_findPiPresenterGuid);

    //FindPiModelR result;

    auto a = IpScanner::GetLocalAddresses();
    if(a.isEmpty())
    {
        r.message = QStringLiteral("No local ip address");
    }
    else
    {
        FindPiThread::Model m(a.first(), ports, timeout);
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
        r.iplist.append(txt);
        zInfo("pi found: "+txt);
    }

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
        QObject::connect(h, SIGNAL(ResponseErr(QUuid,QString,QByteArray)),
                             this, SLOT(ResponseNotOkAction(QUuid,QString,QByteArray)));
        // response err
        zInfo("trying: "+key);
        QUuid actionKey = h->GetAction("version");
        _actions.insert(actionKey, key);
    }
    // ha van pi, kérjük le a típust, és a talpbetétet!
}



//QUuid DoWork::GetApiver()
//{
//    return _httpHelper.GetAction(APIVER);
//}
void DoWork::ResponseNotOkAction(const QUuid& guid, const QString& action,  QByteArray s){
    if(_actions.contains(guid)){
        auto insoleApiKey = _actions[guid];
        auto& insoleApi = _insoleApis[insoleApiKey];
        insoleApi.checked=true;
        insoleApi.isError=true;
        CheckReady();
    }
}

void DoWork::ResponseOkAction(const QUuid& guid, const QString& action,  QByteArray s)
{
    if(_actions.contains(guid)){
        auto insoleApiKey = _actions[guid];
        auto& insoleApi = _insoleApis[insoleApiKey];
        insoleApi.checked=true;
        insoleApi.isError=false;
        auto responseString = QString(s);
        //zInfo("ResponseOkAction: "+a+": "+responseString);
        if(action=="version") //pizero_mcp3008_api:1488
        {
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
            zInfo("get_data: "+insoleApiKey+": "+responseString);
            insoleApi.data = responseString;

            QString msg = insoleApi.toString();
            zInfo("insoleApi: "+msg);

            // ha minden api adott választ- ok, vagy err
            CheckReady();
        }
    }
    //if(action==APIVER) GetApiverResponse(guid,s);
    //else

}

void DoWork::CheckReady()
{
    ResponseModel::FindPi r(_findPiPresenterGuid);
    int checkedApiCounter = 0;

    for(auto&key:_insoleApis.keys())
    {
        auto& api = _insoleApis[key];
        if(!api.checked) return; // ha van olyan api ami nem volt még csekkolva
        checkedApiCounter++;
        if(api.isError) continue;
        QString msg = api.toString();
        r.iplist.append(msg);
    }
    if(checkedApiCounter>0) emit ResponseFindPi(r);
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




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

    QObject::connect(&_httpHelper, SIGNAL(ResponseOk(QUuid,QString,QByteArray)),
                     this, SLOT(ResponseOkAction(QUuid,QString,QByteArray)));

    _isInited = true;
    return true;
}

DoWork::FindPiModelR DoWork::FindPi(const QSet<int>& ports, int timeout)
{
    FindPiModelR result;

    auto a = IpScanner::GetLocalAddresses();
    if(a.isEmpty())
    {
        result.errors.append(QStringLiteral("No local ip address"));
    }
    else
    {
        WorkerThread::Model m(a.first(), ports, timeout);
        startWorkInAThread(m);
        result.errors.append(QStringLiteral("searching..."));
    }
    return result;
}

void DoWork::startWorkInAThread(WorkerThread::Model m)
{
    WorkerThread *workerThread = new WorkerThread(m);
    connect(workerThread, &WorkerThread::resultReady, this, &DoWork::handleResults);
    connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
    workerThread->start();
}

void DoWork::handleResults(const WorkerThread::ModelR &result){

    for(auto&key:result.ipList.keys()){
        QSet<int> values = result.ipList[key];
        QString str;//ports
        for(auto&v:values){
            if(!str.isEmpty()) str+=',';
            str+=QString::number(v);
        };
        zInfo("ip:" + key + ":" +str);
    }
}

QUuid DoWork::GetApiver()
{
    return _httpHelper.GetAction(APIVER);
}

void DoWork::ResponseOkAction(const QUuid& guid, const QString& action,  QByteArray s){
    //if(action==APIVER) GetApiverResponse(guid,s);
    //else zInfo("unknown action: "+action);
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




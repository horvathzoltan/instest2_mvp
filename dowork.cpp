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


const QString DoWork::APIVER = QStringLiteral("apiver");


DoWork::DoWork(QObject *parent) :QObject(parent)
{

}

auto DoWork::init(const DoWorkInit& m) -> bool
{
    _isInited = false;

    if(!_httpHelper.init(m.settings._host, m.settings._port)) return _isInited;

    QObject::connect(&_httpHelper, SIGNAL(ResponseOk(QUuid,QString,QByteArray)),
                     this, SLOT(ResponseOkAction(QUuid,QString,QByteArray)));

    _isInited = true;
    return true;
}


QUuid DoWork::GetApiver()
{
    return _httpHelper.GetAction(APIVER);
}

void DoWork::ResponseOkAction(const QUuid& guid, const QString& action,  QByteArray s){
    if(action==APIVER) GetApiverResponse(guid,s);
    else zInfo("unknown action: "+action);
}


void DoWork::GetApiverResponse(const QUuid& guid, QByteArray s){
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(s, &errorPtr);
    QJsonObject rootobj = doc.object();
    ResponseModel::GetApiVer r(guid);

    //Model::ApiVer m;
    if(rootobj.isEmpty()){
        r.msg = "no response";
    }else{
        r.apiVer = Model::ApiVer::JsonParse(rootobj);
        r.msg = "apiver: "+r.apiVer.toString();
    }

    emit ResponseGetApiverAction(r);
}




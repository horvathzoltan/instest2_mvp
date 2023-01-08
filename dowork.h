#ifndef DOWORK_H
#define DOWORK_H

#include <QCoreApplication>
#include <models/responsemodel.h>
#include <models/settings.h>
#include <models/viewmodel.h>
#include "models/commandlineargs.h"
#include "helpers/httphelper.h"


class DoWork: public QObject
{
    Q_OBJECT
private:
    static const QString APIVER;


    //enum GetRequestType{checkin, apiver};
public:
    explicit DoWork(QObject *parent = nullptr);   

    struct DoWorkInit{
        Settings settings;
    };

    bool init(const DoWorkInit& m);

    QUuid GetApiver();

    void setData(const Model::ApiVer& m){_data.apiVer = m;}    

    Model::ApiVer apiVer(){ return _data.apiVer; }    

private:
    bool _isInited = false;
    //CommadLineArgs params;
    HttpHelper _httpHelper;        

    Model::Data _data;

    void GetApiverResponse(const QUuid& guid, QByteArray s);

// (wiew)action -> (presenter)processAction -> [ (dowork)ResponseAction -> (presenter)onResponseAction -> ] (wiew)set_view
//                                               ----------------------
signals:
    //2//apiver
    void ResponseGetApiverAction(ResponseModel::GetApiVer);

private slots:
    void ResponseOkAction(const QUuid& guid, const QString& action, QByteArray s);

};

#endif // DOWORK_H

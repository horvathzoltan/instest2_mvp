#ifndef DOWORK_H
#define DOWORK_H

#include <QCoreApplication>
#include <models/responsemodel.h>
#include <models/settings.h>
#include <models/viewmodel.h>
//#include "models/commandlineargs.h"
#include "helpers/httphelper.h"
#include "workerthread.h"
#include <QMap>
#include "models/model.h"

class DoWork: public QObject
{
    Q_OBJECT
private:
    static const QString APIVER;
    static QList<Model::InsoleType> _insoleTypes;

    //enum GetRequestType{checkin, apiver};
public:
    explicit DoWork(QObject *parent = nullptr);   

    struct DoWorkInit{
        Settings settings;
    };

    bool init(const DoWorkInit& m);

    //QUuid GetApiver();

    //void setData(const Model::ApiVer& m){_data.apiVer = m;}

//    struct FindPiModelR{
//        QString localAddress;
//        QMap<QString, QSet<int>> ipAddresses;
//        QStringList errors;
//    };

    ResponseModel::FindPi FindPi(const QSet<int>& ports, int timeout);
  //  Model::ApiVer apiVer(){ return _data.apiVer; }



    void StartFindPiThread(FindPiThread::Model m, QUuid guid);

    Model::InsoleType* GetInsoleTypeR(int r);
    Model::InsoleType* GetInsoleType(int v);

private:
    bool _isInited = false;

    struct InsoleApi{
        QString key;
        //QUuid presenterGuid;
        HttpHelper* httpHelper;
        bool checked;
        bool isError;
        QString name;
        int buildnum;
        int datalength;
        //QString data;
        Model::InsoleData insoleData;
        Model::InsoleType* insoleType;
        //int insoletypeV
        //int[] prssures

        QString toString();
    };

    QUuid _findPiPresenterGuid;
    QMap<QString, InsoleApi> _insoleApis;
    QMap<QUuid, QString> _actions;
    //Model::Data _data;

    bool CheckReady();
    ResponseModel::FindPi CheckReady2();
   // void GetApiverResponse(const QUuid& guid, QByteArray s);

// (wiew)action -> (presenter)processAction -> [ (dowork)ResponseAction -> (presenter)onResponseAction -> ] (wiew)set_view
//                                               ----------------------
signals:
    void ResponseFindPi(ResponseModel::FindPi);
    //2//apiver
    //void ResponseGetApiverAction(ResponseModel::GetApiVer);

private slots:
    void ResponseOkAction(const QUuid& guid, const QString& action, QByteArray s);
    void ResponseNotOkAction(const QUuid& guid, const QString& action, QByteArray s);
    void FindPiThreadResults(const FindPiThread::Result &s);

public:
    InsoleApi* GetApi(const QString& key);
};

#endif // DOWORK_H

#ifndef DOWORK_H
#define DOWORK_H

#include <QCoreApplication>
#include <QString>
#include <QTimer>
#include <helpers/bitmapgen.h>
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
    BitMapGen _bitmapGen_L;
    BitMapGen _bitmapGen_R;
    //enum GetRequestType{checkin, apiver};
public:
    explicit DoWork(QObject *parent = nullptr);   

    QString _apiKey_L;
    QString _apiKey_R;

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

    ResponseModel::FindPi FindPi(const QSet<int>& ports, int timeout, int steps);
  //  Model::ApiVer apiVer(){ return _data.apiVer; }



    void StartFindPiThread(FindPiThread::Model m, QUuid guid);

    Model::InsoleType* GetInsoleTypeR(int r);
    Model::InsoleType* GetInsoleType(int v);

private:
    bool _isInited = false;

    enum ApiStatus:int{
        Waiting=0, Started
    };

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
        Model::PhysDirection::Directions direction;
        ApiStatus status = Waiting;
        QString toString();
    };

    QUuid _findPiPresenterGuid;
    QMap<QString, InsoleApi> _insoleApis;
    QMap<QUuid, QString> _actions;
    //Model::Data _data;

    void startTimers();
    bool CheckReady();
    ResponseModel::FindPi CheckReady2();

    QTimer timer_L;
    QTimer timer_R;

    void onTimeout_L();
    void onTimeout_R();
    void onTimeout(Model::PhysDirection::Directions d);
   // void GetApiverResponse(const QUuid& guid, QByteArray s);

// (wiew)action -> (presenter)processAction -> [ (dowork)ResponseAction -> (presenter)onResponseAction -> ] (wiew)set_view
//                                               ----------------------
signals:
    void ResponseFindPi(ResponseModel::FindPi);
    void ResponsePiData(ResponseModel::PiData);
    //2//apiver
    //void ResponseGetApiverAction(ResponseModel::GetApiVer);

private slots:
    void ResponseOkAction(const QUuid& guid, const QString& action, QByteArray s);
    void ResponseNotOkAction(const QUuid& guid, const QString& action);
    void FindPiThreadResults(const FindPiThread::Result &s);

public:
    InsoleApi* GetApi(const QString& key);
};

#endif // DOWORK_H

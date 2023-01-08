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
    static const QString CHECKIN;
    static const QString APIVER;
    static const QString FEATURE_REQUEST;

    static const QString CALLS_ANSWER;
    static const QString CALLS_HANGUP;
    static const QString CALLS_HANGUPALL;
    static const QString CALLS_DIAL;

    static const QString MEDIA_PLAY;
    static const QString MEDIA_PAUSE;
    static const QString MEDIA_SKIP;
    static const QString MEDIA_BACK;

    static const QString CURRENTWEATHER;
    static const QString CURRENTWEATHERICON;

    static const QString CURRENTALERT;
    static const QString CURRENTALERTMAP;
    static const QString CURRENTWARNING;
    static const QString CURRENTWARNINGMAP;

    //enum GetRequestType{checkin, apiver};
public:
    explicit DoWork(QObject *parent = nullptr);   

    struct DoWorkInit{
        Settings settings;
    };

    bool init(const DoWorkInit& m);
    QUuid GetCheckin();
    QUuid GetApiver();
    QUuid GetFeatureRequest();

    QUuid GetCallsAnswer();
    QUuid GetCallsHangup();
    QUuid GetCallsHangupAll();
    QUuid GetCallsDial();

    QUuid GetMediaPlay();
    QUuid GetMediaPause();
    QUuid GetMediaSkip();
    QUuid GetMediaBack();    

    //7//weather
    QUuid GetCurrentWeather();
    QUuid GetCurrentWeatherIcon(const QString& iconpath);    
    //8//alert
    QUuid GetCurrentAlert();
    QUuid GetCurrentAlertMap(const QString& iconpath);
    //9//warning
    QUuid GetCurrentWarning();
    QUuid GetCurrentWarningMap(const QString& iconpath);

    ViewModel::State GetState(const Model::Device& device,
                      const Model::Media& media,
                      const Model::Calls& calls
                      );

    void setData(const Model::Device& m){_data.device = m;}
    void setData(const Model::Media& m){_data.media = m;}
    void setData(const Model::ApiVer& m){_data.apiVer = m;}
    void setData(const Model::Features& m){_data.features = m;}
    void setData(const Model::CurrentWeather& m){_data.currentWeather = m;}
    void setData(const Model::CurrentAlert& m){_data.currentAlert = m;}
    void setData(const Model::CurrentWarning& m){_data.currentWarning = m;}

    Model::Media media(){ return _data.media; }
    Model::Device device(){ return _data.device; }
    Model::Calls calls(){ return _data.calls; }
    Model::ApiVer apiVer(){ return _data.apiVer; }
    Model::Features features(){ return _data.features; }
    Model::CurrentWeather currentWeather(){ return _data.currentWeather; }
    Model::CurrentAlert currentAlert(){ return _data.currentAlert; }
    Model::CurrentWarning currentWarning(){ return _data.currentWarning; }

    Model::WarningKeys ParseWarningKeys(const QString& txt);
private:
    bool _isInited = false;
    //CommadLineArgs params;
    HttpHelper _httpHelper;
    HttpHelper _httpHelper_idokep;
    HttpHelper _httpHelper_met;
    Settings::CurrentWeatherKeys _currentWeatherKeys;
    Settings::CurrentAlertKeys _currentAlertKeys;
    Settings::CurrentWarningKeys _currentWarningKeys;

    Model::Data _data;


    void GetCheckinResponse(const QUuid& guid, QByteArray s);
    void GetApiverResponse(const QUuid& guid, QByteArray s);
    void GetFeatureRequestResponse(const QUuid& guid, QByteArray s);
    //7//weather
    void GetCurrentWeatherResponse(const QUuid& guid, QByteArray s);
    void GetCurrentWeatherIconResponse(const QUuid& guid, QByteArray s);
    //8//alert
    void GetCurrentAlertResponse(const QUuid& guid, QByteArray s);
    void GetCurrentAlertMapResponse(const QUuid& guid, QByteArray s);
    //9//warning
    void GetCurrentWarningResponse(const QUuid& guid, QByteArray s);
    void GetCurrentWarningMapResponse(const QUuid& guid, QByteArray s);

// (wiew)action -> (presenter)processAction -> [ (dowork)ResponseAction -> (presenter)onResponseAction -> ] (wiew)set_view
//                                               ----------------------
signals:
    //1//checkin
    void ResponseConnectionAction(ResponseModel::Checkin);
    //2//apiver
    void ResponseGetApiverAction(ResponseModel::GetApiVer);
    //3//features
    void ResponseGetFeatureRequestAction(ResponseModel::GetFeature);
    //7//weather
    void ResponseGetCurrentWeatherRequestAction(ResponseModel::GetCurrentWeather);
    //7a//weather_icon
    void ResponseGetCurrentWeatherIconRequestAction(ResponseModel::GetCurrentWeatherIcon);
    //8//aleret
    void ResponseGetCurrentAlertRequestAction(ResponseModel::GetCurrentAlert);
    void ResponseGetCurrentAlertMapRequestAction(ResponseModel::GetCurrentAlertMap);
    //9//warning
    void ResponseGetCurrentWarningRequestAction(ResponseModel::GetCurrentWarning);
    void ResponseGetCurrentWarningMapRequestAction(ResponseModel::GetCurrentWarningMap);

private slots:
    void ResponseOkAction(const QUuid& guid, const QString& action, QByteArray s);

};

#endif // DOWORK_H

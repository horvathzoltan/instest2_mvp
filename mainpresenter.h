#ifndef MAINPRESENTER_H
#define MAINPRESENTER_H

#include <QObject>
#include <models/responsemodel.h>
#include "imainview.h"
#include "models/commandlineargs.h"
#include "dowork.h"

class IMainView;

class MainPresenter : public QObject
{
    Q_OBJECT

public:
    explicit MainPresenter(QObject *parent = nullptr);
    void appendView(IMainView *w);
    void initView(IMainView *w) const;
    struct MainPresenterInit{
        Settings settings;
    };

    bool init(const MainPresenterInit& m);
private:
    QList<IMainView*> _views;

    ViewModel::Page GetActivePage(ViewModel::State state);
    bool IsGoAlertPage(const Model::CurrentAlert& w);
    bool IsGoWarningPage(const Model::CurrentWarning& w);

private slots:
    // (wiew)action -> (presenter)processAction -> [ (dowork)ResponseAction -> (presenter)onResponseAction -> ] (wiew)set_view
    //                 ------------------------                                ---------------------------
    //1//checkin
    void processGetConnectionAction(IMainView *sender);
    void onResponseConnectionAction(ResponseModel::Checkin);
    //2//apiver
    void processGetApiverAction(IMainView *sender);
    void onResponseGetApiverAction(ResponseModel::GetApiVer);
    //3//features
    void processGetFeatureRequestAction(IMainView *sender);
    void onResponseGetFeatureRequestAction(ResponseModel::GetFeature);
    //4//media
    void processMediaRefreshAction(IMainView *sender);
    //5//device
    void processDeviceRefreshAction(IMainView *sender);
    //6//calls
    void processCallsRefreshAction(IMainView *sender);
    //7//weather
    void processGetCurrentWeatherAction(IMainView *sender);    
    void onResponseGetCurrentWeatherRequestAction(ResponseModel::GetCurrentWeather);    
    void onResponseGetCurrentWeatherIconRequestAction(ResponseModel::GetCurrentWeatherIcon);
    //8//alert
    void processGetCurrentAlertAction(IMainView *sender);
    void onResponseGetCurrentAlertRequestAction(ResponseModel::GetCurrentAlert);
    void onResponseGetCurrentAlertMapRequestAction(ResponseModel::GetCurrentAlertMap);
    //9//warning
    void processGetCurrentWarningAction(IMainView *sender);
    void onResponseGetCurrentWarningRequestAction(ResponseModel::GetCurrentWarning);
    void onResponseGetCurrentWarningMapRequestAction(ResponseModel::GetCurrentWarningMap);

private:
    bool _isInited = false;
    DoWork _dowork;
    QMap<QUuid, IMainView*> _senders;

    //Model::Data _data;
};

#endif // MAINPRESENTER_H

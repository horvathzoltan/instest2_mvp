#ifndef IMAINVIEW_H
#define IMAINVIEW_H

#include "models/viewmodel.h"


class IMainView
{
public:
// (wiew)action -> (presenter)processAction -> [ (dowork)ResponseAction -> (presenter)onResponseAction -> ] (wiew)set_view
// ------------                                                                                             --------------

    //1//checkin <- actions
    virtual void GetConnectionActionTriggered(IMainView *sender) = 0;
    virtual void set_ConnectionView(const ViewModel::ConnectionR& m) = 0;
    //2//apiver
    virtual void GetApiverActionTriggered(IMainView *sender) = 0;
    virtual void set_ApiverView(const ViewModel::Apiver& m) = 0;
    //3//features
    virtual void GetFeatureRequestActionTriggered(IMainView *sender) =0;
    virtual void set_FeatureRequestView(const ViewModel::Features& m) = 0;
    //4//media
    virtual void MediaRefreshActionTriggered(IMainView *sender) =0;
    virtual void set_MediaView(const ViewModel::Media& m) = 0;
    //5//device
    virtual void DeviceRefreshActionTriggered(IMainView *sender) =0;
    virtual void set_DeviceView(const ViewModel::Device& m) = 0;
    //6//calls
    virtual void CallsRefreshActionTriggered(IMainView *sender) =0;
    virtual void set_CallsView(const ViewModel::Calls& m) = 0;
    //7//weather
    virtual void GetCurrentWeatherActionTriggered(IMainView *sender) =0;
    virtual void set_CurrentWeatherView(const ViewModel::CurrentWeather& m) = 0;
    virtual void set_CurrentWeatherIconView(const ViewModel::CurrentWeatherIcon& m) = 0;
    //8//alert
    virtual void GetCurrentAlertActionTriggered(IMainView *sender) =0;
    virtual void set_CurrentAlertView(const ViewModel::CurrentAlert& m) = 0;
    virtual void set_CurrentAlertMapView(const ViewModel::CurrentAlertMap& m) = 0;
    //9//warning
    virtual void GetCurrentWarningActionTriggered(IMainView *sender) =0;
    virtual void set_CurrentWarningView(const ViewModel::CurrentWarning& m) = 0;
    virtual void set_CurrentWarningMapView(const ViewModel::CurrentWarningMap& m) = 0;
};

#endif // IMAINVIEW_H

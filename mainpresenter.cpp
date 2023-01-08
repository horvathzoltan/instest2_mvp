#include "dowork.h"
#include "logger.h"
#include "mainpresenter.h"
#include <QDebug>
#include <models/viewmodel.h>
#include <models/responsemodel.h>
#include <models/model.h>

MainPresenter::MainPresenter(QObject *parent) :QObject(parent)
{

}

void MainPresenter::appendView(IMainView *w)
{
    if(_views.contains(w)) return;
    _views.append(w);

    auto *view_obj = dynamic_cast<QObject*>(w);

    //1//checkin <- (view)actions - (presenter)processActions
    QObject::connect(view_obj, SIGNAL(GetConnectionActionTriggered(IMainView*)),
                     this, SLOT(processGetConnectionAction(IMainView*)));
    //2//apiver
    QObject::connect(view_obj, SIGNAL(GetApiverActionTriggered(IMainView*)),
                     this, SLOT(processGetApiverAction(IMainView*)));
    //3//features
    QObject::connect(view_obj, SIGNAL(GetFeatureRequestActionTriggered(IMainView*)),
                     this, SLOT(processGetFeatureRequestAction(IMainView*)));
    //4//media
    QObject::connect(view_obj, SIGNAL(MediaRefreshActionTriggered(IMainView*)),
                     this, SLOT(processMediaRefreshAction(IMainView*)));
    //5//device
    QObject::connect(view_obj, SIGNAL(DeviceRefreshActionTriggered(IMainView*)),
                     this, SLOT(processDeviceRefreshAction(IMainView*)));
    //6//calls
    QObject::connect(view_obj, SIGNAL(CallsRefreshActionTriggered(IMainView*)),
                     this, SLOT(processCallsRefreshAction(IMainView*)));
    //7//weather
    QObject::connect(view_obj, SIGNAL(GetCurrentWeatherActionTriggered(IMainView*)),
                     this, SLOT(processGetCurrentWeatherAction(IMainView*)));
    QObject::connect(view_obj, SIGNAL(GetCurrentWeatherActionTriggered(IMainView*)),
                     this, SLOT(processGetCurrentWeatherAction(IMainView*)));
    //8//Alert
    QObject::connect(view_obj, SIGNAL(GetCurrentAlertActionTriggered(IMainView*)),
                     this, SLOT(processGetCurrentAlertAction(IMainView*)));

    //9//Warning
    QObject::connect(view_obj, SIGNAL(GetCurrentWarningActionTriggered(IMainView*)),
                     this, SLOT(processGetCurrentWarningAction(IMainView*)));

    //refreshView(w);    
}


auto MainPresenter::init(const MainPresenterInit& m) -> bool
{
    _isInited = false;
    _dowork.init({m.settings});
    // (dowork)ResponseAction - (presenter)onResponseAction
    //1//checkin
    connect(&_dowork,SIGNAL(ResponseConnectionAction(ResponseModel::Checkin)),
            this,SLOT(onResponseConnectionAction(ResponseModel::Checkin)));
    //2//apiver
    connect(&_dowork,SIGNAL(ResponseGetApiverAction(ResponseModel::GetApiVer)),
            this,SLOT(onResponseGetApiverAction(ResponseModel::GetApiVer)));
    //3//features
    connect(&_dowork,SIGNAL(ResponseGetFeatureRequestAction(ResponseModel::GetFeature)),
            this,SLOT(onResponseGetFeatureRequestAction(ResponseModel::GetFeature)));
    //7//weather
    connect(&_dowork,SIGNAL(ResponseGetCurrentWeatherRequestAction(ResponseModel::GetCurrentWeather)),
            this,SLOT(onResponseGetCurrentWeatherRequestAction(ResponseModel::GetCurrentWeather)));
    //7a//weather_icon
    connect(&_dowork,SIGNAL(ResponseGetCurrentWeatherIconRequestAction(ResponseModel::GetCurrentWeatherIcon)),
            this,SLOT(onResponseGetCurrentWeatherIconRequestAction(ResponseModel::GetCurrentWeatherIcon)));
    //8//alert
    connect(&_dowork,SIGNAL(ResponseGetCurrentAlertRequestAction(ResponseModel::GetCurrentAlert)),
            this,SLOT(onResponseGetCurrentAlertRequestAction(ResponseModel::GetCurrentAlert)));    
    connect(&_dowork,SIGNAL(ResponseGetCurrentAlertMapRequestAction(ResponseModel::GetCurrentAlertMap)),
            this,SLOT(onResponseGetCurrentAlertMapRequestAction(ResponseModel::GetCurrentAlertMap)));
    //9//warning
    connect(&_dowork,SIGNAL(ResponseGetCurrentWarningRequestAction(ResponseModel::GetCurrentWarning)),
            this,SLOT(onResponseGetCurrentWarningRequestAction(ResponseModel::GetCurrentWarning)));
    connect(&_dowork,SIGNAL(ResponseGetCurrentWarningMapRequestAction(ResponseModel::GetCurrentWarningMap)),
            this,SLOT(onResponseGetCurrentWarningMapRequestAction(ResponseModel::GetCurrentWarningMap)));
    //_result = { Result::State::NotCalculated, -1};
    _isInited = true;
    return true;
}

void MainPresenter::initView(IMainView *w) const {
   // MainViewModel::DoWorkRModel rm{"started",{}};
    //w->set_DoWorkRModel(rm);
};

/*GetConnection*/
void MainPresenter::processGetConnectionAction(IMainView *sender){
    auto guid = _dowork.GetCheckin();
    _senders.insert(guid,sender);
}


ViewModel::Page MainPresenter::GetActivePage(ViewModel::State state)
{
    if(state.callsState==ViewModel::State::ConnectionState::created)
        return ViewModel::Page::calls;
    //ha amúgy van és a változott az nem megszűnés
    if(state.callsState==ViewModel::State::ConnectionState::changed)
        return ViewModel::Page::calls;
//    if(state.callsState==ViewModel::State::ConnectionState::deleted)
//        return ViewModel::Page::noChange;
    //ha bejön vagy van egy hívás de elnavigálok - és nincs változás, ne menjen vissza, de ha valami változik, igen
    if(state.callsState==ViewModel::State::ConnectionState::unchanged)
        return ViewModel::Page::noChange;
    if(state.mediaState==ViewModel::State::ConnectionState::created)
        return ViewModel::Page::media;
    //ha amúgy van és a változott az nem megszűnés
    // és nem paused, akkor
    if(state.mediaState==ViewModel::State::ConnectionState::changed){
        //    return ViewModel::Page::media;
        auto s = _dowork.media().status;
        if(s==Model::Media::Status::playing) return ViewModel::Page::media;
        return ViewModel::Page::noChange;
    }


    if(state.mediaState==ViewModel::State::ConnectionState::unchanged)
        return ViewModel::Page::noChange;
//ha sokáig áll, vagy stopped, vagy deleted, akkor conn
//    if(state.mediaState==DoWork::State::ConnectionState::deleted)
//        return ViewModel::Page::connection;
    return ViewModel::Page::noChange;
}

void MainPresenter::onResponseConnectionAction(ResponseModel::Checkin m)
{

    if(_senders.contains(m.guid)){
        auto state = _dowork.GetState(m.device, m.media, m.calls);

        _dowork.setData(m.device);
        _dowork.setData(m.media);

        auto page = GetActivePage(state);

        ViewModel::ConnectionR rm {.page = page, .state = state};
        _senders[m.guid]->set_ConnectionView(rm);                
        _senders.remove(m.guid);
    }
}

/*GetApiver*/
void MainPresenter::processGetApiverAction(IMainView *sender){
    auto guid = _dowork.GetApiver();
    _senders.insert(guid,sender);
}

void MainPresenter::onResponseGetApiverAction(ResponseModel::GetApiVer m)
{    
    if(_senders.contains(m.guid)){
        //_data.apiVer = m.apiVer;
        _dowork.setData(m.apiVer);
        ViewModel::Apiver rm = {m.apiVer};
        _senders[m.guid]->set_ApiverView(rm);
        _senders.remove(m.guid);
    }
}

/*GetFeature*/
void MainPresenter::processGetFeatureRequestAction(IMainView *sender){
    auto guid = _dowork.GetFeatureRequest();
    _senders.insert(guid,sender);
}

void MainPresenter::onResponseGetFeatureRequestAction(ResponseModel::GetFeature m)
{
    if(_senders.contains(m.guid)){
        //_data.features = m.features;
         _dowork.setData(m.features);

        ViewModel::Features rm = {m.features};
        _senders[m.guid]->set_FeatureRequestView(rm);
        _senders.remove(m.guid);
    }
}

void MainPresenter::processMediaRefreshAction(IMainView *sender)
{
    Model::Media m = _dowork.media();
    ViewModel::Media rm{
        .media = m
    };
    sender->set_MediaView(rm);
}

void MainPresenter::processDeviceRefreshAction(IMainView *sender)
{
    Model::Device m = _dowork.device();
    ViewModel::Device rm{
        .device = m
    };
    sender->set_DeviceView(rm);
}

void MainPresenter::processCallsRefreshAction(IMainView *sender)
{
    Model::Calls m = _dowork.calls();
    ViewModel::Calls rm{
        .calls = m
    };
    sender->set_CallsView(rm);
}

//7//weather

void MainPresenter::processGetCurrentWeatherAction(IMainView *sender){
    auto guid = _dowork.GetCurrentWeather();
    _senders.insert(guid,sender);
}

void MainPresenter::onResponseGetCurrentWeatherRequestAction(ResponseModel::GetCurrentWeather m)
{
    if(_senders.contains(m.guid)){
        auto sender = _senders[m.guid];

        //_data.apiVer = m.apiVer;
        _dowork.setData(m.currentWeather);
        ViewModel::CurrentWeather rm = {m.currentWeather};

        sender->set_CurrentWeatherView(rm);
        _senders.remove(m.guid);
        //m.currentWeather.icon;
        auto guid_icon = _dowork.GetCurrentWeatherIcon(m.currentWeather.icon);
        _senders.insert(guid_icon,sender);
    }
}

void MainPresenter::onResponseGetCurrentWeatherIconRequestAction(ResponseModel::GetCurrentWeatherIcon m)
{
    if(_senders.contains(m.guid)){
        ViewModel::CurrentWeatherIcon rm = {m.pixmap};
        _senders[m.guid]->set_CurrentWeatherIconView(rm);
        _senders.remove(m.guid);
    }
}

//8//alert

void MainPresenter::processGetCurrentAlertAction(IMainView *sender){
    auto guid = _dowork.GetCurrentAlert();
    _senders.insert(guid,sender);
}

void MainPresenter::onResponseGetCurrentAlertRequestAction(ResponseModel::GetCurrentAlert m)
{
    if(_senders.contains(m.guid)){
        auto sender = _senders[m.guid];

        bool a = IsGoAlertPage(m.currentAlert);

        _dowork.setData(m.currentAlert);

        ViewModel::CurrentAlert rm = {m.currentAlert};

        sender->set_CurrentAlertView(rm);
        _senders.remove(m.guid);
        auto guid_icon = _dowork.GetCurrentAlertMap(m.currentAlert.map);
        _senders.insert(guid_icon,sender);
    }
}

bool MainPresenter::IsGoAlertPage(const Model::CurrentAlert& m){
    bool go;

    auto prev_m = _dowork.currentAlert().alerts;
    for(auto&w:m.alerts){
        if(prev_m.contains(w.key)){

        }
    }


    return go;
}

void MainPresenter::onResponseGetCurrentAlertMapRequestAction(ResponseModel::GetCurrentAlertMap m)
{
    if(_senders.contains(m.guid)){
        ViewModel::CurrentAlertMap rm = {m.pixmap};
        _senders[m.guid]->set_CurrentAlertMapView(rm);
        _senders.remove(m.guid);
    }
}

//9//warning

void MainPresenter::processGetCurrentWarningAction(IMainView *sender){
    auto guid = _dowork.GetCurrentWarning();
    _senders.insert(guid,sender);
}

void MainPresenter::onResponseGetCurrentWarningRequestAction(ResponseModel::GetCurrentWarning m)
{
    if(_senders.contains(m.guid)){
        auto sender = _senders[m.guid];

        bool a = IsGoWarningPage(m.currentWarning);

        _dowork.setData(m.currentWarning);

        ViewModel::CurrentWarning rm = {m.currentWarning};

        sender->set_CurrentWarningView(rm);
        _senders.remove(m.guid);
        // todo 11  összeset leszedni
        auto map1 = m.currentWarning.maps.first();
        auto guid_icon = _dowork.GetCurrentWarningMap(map1);
        _senders.insert(guid_icon,sender);
    }
}

bool MainPresenter::IsGoWarningPage(const Model::CurrentWarning& m){
    bool go;

    auto prev_warnings = _dowork.currentWarning().warnings;
    for(auto&w:m.warnings){
        if(prev_warnings.contains(w.key)){

        }
    }


    return go;
}

void MainPresenter::onResponseGetCurrentWarningMapRequestAction(ResponseModel::GetCurrentWarningMap m)
{
    if(_senders.contains(m.guid)){
        ViewModel::CurrentWarningMap rm = {m.pixmap};
        _senders[m.guid]->set_CurrentWarningMapView(rm);
        _senders.remove(m.guid);
    }
}

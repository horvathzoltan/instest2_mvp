#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imainview.h"

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public IMainView
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

// (wiew)action -> (presenter)processAction -> [ (dowork)ResponseAction -> (presenter)onResponseAction -> ] (wiew)set_view
// ------------                                                                                             --------------

    void set_ConnectionView(const ViewModel::ConnectionR& m);
    void set_ApiverView(const ViewModel::Apiver& m);
    void set_FeatureRequestView(const ViewModel::Features& m);
    void set_MediaView(const ViewModel::Media& m);
    void set_DeviceView(const ViewModel::Device& m);
    void set_CallsView(const ViewModel::Calls& m);
    void set_DateTime();
    //weather
    void set_CurrentWeatherView(const ViewModel::CurrentWeather& m);
    void set_CurrentWeatherIconView(const ViewModel::CurrentWeatherIcon& m);
    //alert
    void set_CurrentAlertView(const ViewModel::CurrentAlert& m);
    void set_CurrentAlertMapView(const ViewModel::CurrentAlertMap& m);
    //warning
    void set_CurrentWarningView(const ViewModel::CurrentWarning& m);
    void set_CurrentWarningMapView(const ViewModel::CurrentWarningMap& m);

signals:
    void GetConnectionActionTriggered(IMainView *sender);
    void GetApiverActionTriggered(IMainView *sender);
    void GetFeatureRequestActionTriggered(IMainView *sender);
    void MediaRefreshActionTriggered(IMainView *sender);
    void DeviceRefreshActionTriggered(IMainView *sender);
    void CallsRefreshActionTriggered(IMainView *sender);
    //weather
    void GetCurrentWeatherActionTriggered(IMainView *sender);
    //alert
    void GetCurrentAlertActionTriggered(IMainView *sender);    
    //warning
    void GetCurrentWarningActionTriggered(IMainView *sender);

private slots:
    void onTimerTimeout();

private:
    Ui::MainWindow *ui;
    QTimer _timer;
    unsigned long int _tick=0;
    QLocale _locale;//("hu-HU");

    void setPage(ViewModel::Page page);
    bool IsRefresh(ViewModel::State::ConnectionState s);
};
#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "logger.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_status_2->setText("started");
    setPage(ViewModel::Page::main);
    _timer.setParent(this);
    QObject::connect(&_timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
    _timer.start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*UserActions*/


/*GetViewModel*/
//auto MainWindow::get_DoWorkModel() -> ViewModel::DoWorkModel
//{
//    auto t = ui->label->text();
//    bool isok;
//    int i = t.toInt(&isok, 10);
//    if(!isok) return {-1};
//    return {i};
//};


/*SetView*/
//void MainWindow::set_PhoneView(const MainViewModel::PhoneViewModelR &m)
//{
//    ui->label->setText(m.msg);
//}

void MainWindow::set_ConnectionView(const ViewModel::ConnectionR &m)
{
    setPage(m.page);

    ui->label_media->setText(m.state.mediaMsg);
    ui->label_device->setText(m.state.deviceMsg);
    ui->label_calls->setText(m.state.callsMsg);

    if(IsRefresh(m.state.mediaState)){
        emit MediaRefreshActionTriggered(this);
    }
    if(IsRefresh(m.state.deviceState)){
        emit DeviceRefreshActionTriggered(this);
    }
    if(IsRefresh(m.state.callsState)){
        emit CallsRefreshActionTriggered(this);
    }
}

bool MainWindow::IsRefresh(ViewModel::State::ConnectionState s){
    if(s==ViewModel::State::ConnectionState::changed) return true;
    if(s==ViewModel::State::ConnectionState::created) return true;
    if(s==ViewModel::State::ConnectionState::deleted) return true;
    return false;
}

void MainWindow::setPage(ViewModel::Page page){
    if(page==ViewModel::Page::noChange) return;
    QString pageName = ViewModel::PageToPageName(page);
    auto p = ui->tabWidget->findChild<QWidget *>(pageName);
    if(p==nullptr) return;
    if(ui->tabWidget->currentWidget()==p) return;
    int page_ix = ui->tabWidget->indexOf(p);
    if(page_ix==-1) return;
    ui->tabWidget->setCurrentIndex(page_ix);
}

void MainWindow::set_ApiverView(const ViewModel::Apiver &m)
{
    QString msg2 = m.apiver.toString();
    ui->label_api->setText(msg2);
}

void MainWindow::set_FeatureRequestView(const ViewModel::Features &m)
{
    QString serverString = m.features.toServerString();
    ui->label_server->setText(serverString);

    QString featureString = m.features.toFeatureString();
    ui->label_features->setText(featureString);
}


void MainWindow::onTimerTimeout()
{
    if(_tick==0){
        emit GetApiverActionTriggered(this);
        emit GetFeatureRequestActionTriggered(this);
        emit GetCurrentWeatherActionTriggered(this);
        emit GetCurrentAlertActionTriggered(this);
        emit GetCurrentWarningActionTriggered(this);
    }

    emit GetConnectionActionTriggered(this);
    ui->label_status_2->setText("tick: "+QString::number(_tick));

    _tick++;

    set_DateTime();
}

/*set media*/
void MainWindow::set_MediaView(const ViewModel::Media &m)
{
    ui->label_album->setText(m.media.album);
    ui->label_artist->setText(m.media.artist);
    ui->label_title->setText(m.media.title);
    ui->label_status->setText(Model::Media::StatusToString(m.media.status));
}

void MainWindow::set_DeviceView(const ViewModel::Device &m)
{
    QString msg = m.device.toString();
    ui->label_connected->setText(msg);
}

void MainWindow::set_CallsView(const ViewModel::Calls &m)
{
    ui->label_device->setText(m.calls.msg);
}

void MainWindow::set_DateTime()
{
    auto t = QDateTime::currentDateTime();
    QString msg = _locale.toString(t);
    ui->label_datetime->setText(msg);
}

void MainWindow::set_CurrentWeatherView(const ViewModel::CurrentWeather &m)
{
    QString msg2 = m.currentWeather.toString();
    ui->label_weather->setText("currentWeather:"+msg2);
}

void MainWindow::set_CurrentWeatherIconView(const ViewModel::CurrentWeatherIcon &m)
{
    ui->label_img->setPixmap(m.pixmap);
}

//8//alert

void MainWindow::set_CurrentAlertView(const ViewModel::CurrentAlert& m)
{
    QString msg;
    for(auto&w:m.currentAlert.alerts){
        if(w.value<1) continue;
        if(!msg.isEmpty()) msg+=", ";
        msg+=w.title+": "+QString::number(w.value);
    }
    if(m.currentAlert.uvBlevel>0){
        if(!msg.isEmpty()) msg+=", ";
        msg+="uvB: "+QString::number(m.currentAlert.uvBlevel);
    }
    ui->label_alerts->setText(msg);
}

void MainWindow::set_CurrentAlertMapView(const ViewModel::CurrentAlertMap &m)
{
    ui->label_alert_map->setPixmap(m.pixmap);
}
//9//warning
void MainWindow::set_CurrentWarningView(const ViewModel::CurrentWarning& m)
{
    QString msg;
    for(auto&w:m.currentWarning.warnings){
        if(w.value<1) continue;
        if(!msg.isEmpty()) msg+=", ";
        msg+=w.title+": "+QString::number(w.value);
    }

    ui->label_warnings->setText(msg);
}

void MainWindow::set_CurrentWarningMapView(const ViewModel::CurrentWarningMap &m)
{
    ui->label_warning_map->setPixmap(m.pixmap);
}


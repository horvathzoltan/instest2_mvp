#include "settings.h"
#include "logger.h"

#include <QFileInfo>
#include <QDir>
#include <QSettings>

#include <helpers/textfilehelper.h>
#include <helpers/filenamehelper.h>
#include <QCoreApplication>

Settings::Settings()
{

}

Settings Settings::Load(const QString& dir)
{
    const QString SETTINGS_FN = "settings.ini";

    QFileInfo fi(dir);
    if(fi.isFile()){
        zInfo("not a directory:"+dir);
        return{};
    }

    QString p(dir);
    if(dir.isEmpty()) p = qApp->applicationDirPath();
    QString absoluteFilePath = FileNameHelper::GetAbsolutFn(p+ "/" + SETTINGS_FN);


    QFileInfo fi2(absoluteFilePath);
    if(!fi2.exists()){
        zInfo("settings file not exisits:"+absoluteFilePath);
        return{};
    }

    QSettings::Format f;
    auto suffix = fi2.suffix();
    if(suffix=="ini")
        f = QSettings::IniFormat;
    else
        f = QSettings::InvalidFormat;

    if(f==QSettings::InvalidFormat) return {};

    QSettings q(absoluteFilePath, f);

    Settings s;

    s._host=q.value("host").toString();
    s._port=q.value("port").toInt();

    s._hostIdokep=q.value("host_idokep").toString();
    s._hostMet=q.value("host_met").toString();

    s._currentWeatherKeys.div=q.value("current-weather-div").toString();
    s._currentWeatherKeys.shortDesc=q.value("current-weather-short-desc").toString();
    s._currentWeatherKeys.title=q.value("current-weather-title").toString();
    s._currentWeatherKeys.value=q.value("current-weather").toString();
    s._currentWeatherKeys.icon=q.value("current-weather-icon").toString();
    s._currentWeatherKeys.temperature=q.value("current-temperature").toString();
    s._currentWeatherKeys.sunrise=q.value("sunrise").toString();
    s._currentWeatherKeys.sunset=q.value("sunset").toString();

    s._currentAlertKeys.div=q.value("alert-div").toString();
    s._currentAlertKeys.tags = q.value("alert-tag").toStringList();
    s._currentAlertKeys.title=q.value("alert-title").toString();
    s._currentAlertKeys.map_div=q.value("alert-map-div").toString();
    s._currentAlertKeys.map=q.value("alert-map").toString();
    s._currentAlertKeys.uvB_div=q.value("alert-uvb-div").toString();

    s._currentWarningKeys.div=q.value("warning-div").toString();
    s._currentWarningKeys.query=q.value("warning-query").toString();
    s._currentWarningKeys.tags = q.value("warning-tag").toStringList();
    s._currentWarningKeys.title=q.value("warning-title").toString();
    s._currentWarningKeys.map_div=q.value("warning-map-div").toString();
    s._currentWarningKeys.map=q.value("warning-map").toString();
    return s;
}



bool Settings::isValid()
{
    if(_host.isEmpty()) return false;
    if(_port<0||_port>65535) return false;
    return true;
}

bool Settings::CurrentAlertKeys::isValid(){
    if(div.isEmpty()) return false;
    if(tags.isEmpty()) return false;
    if(title.isEmpty()) return false;
    if(map_div.isEmpty()) return false;
    if(map.isEmpty()) return false;
    if(uvB_div.isEmpty()) return false;
    return true;
}

bool Settings::CurrentWarningKeys::isValid(){
    if(div.isEmpty()) return false;
    if(query.isEmpty()) return false;
    if(tags.isEmpty()) return false;
    if(title.isEmpty()) return false;
    if(map_div.isEmpty()) return false;
    if(map.isEmpty()) return false;
    return true;
}

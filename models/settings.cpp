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

    s._port=q.value("port").toInt();
    return s;
}



bool Settings::isValid()
{
    if(_port<0||_port>65535) return false;
    return true;
}

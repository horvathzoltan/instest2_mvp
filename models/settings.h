#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QStringList>



class Settings
{
public:
    QString _host;
    int _port=-1;
    QString _hostIdokep;
    QString _hostMet;

    struct CurrentWeatherKeys{
        QString div;
        QString shortDesc;
        QString title;
        QString value;
        QString icon;
        QString temperature;
        QString sunrise;
        QString sunset;
    };

    CurrentWeatherKeys _currentWeatherKeys;

    struct CurrentAlertKeys{
        QString div;
        QStringList tags;
        QString title;
        QString map_div;
        QString map;
        QString uvB_div;

        bool isValid();
    };

    CurrentAlertKeys _currentAlertKeys;

    struct CurrentWarningKeys{
        QString div;
        QString query;
        QStringList tags;
        QString title;
        QString map_div;
        QString map;

        bool isValid();
    };

    CurrentWarningKeys _currentWarningKeys;

    Settings();
    static Settings Load(const QString& dir);
    bool isValid();
};

#endif // SETTINGS_H

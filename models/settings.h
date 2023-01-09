#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QStringList>



class Settings
{
public:
    int _port=-1;      

    Settings();
    static Settings Load(const QString& dir);
    bool isValid();
};

#endif // SETTINGS_H

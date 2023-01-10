#ifndef MODELS_H
#define MODELS_H

#include <QJsonObject>
#include <QString>

namespace Model
{       
    struct Device{
        QString address = "";
        bool connected = false;
        QString name = "";

        QString toString() const;
    };

    struct Data{
            //ApiVer apiVer;
            Device device;
        };
};

#endif // MODELS_H

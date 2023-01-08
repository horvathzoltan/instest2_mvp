#ifndef RESPONSEMODEL_H
#define RESPONSEMODEL_H

#include "model.h"

#include <QPixmap>
#include <QString>
#include <QUuid>

namespace ResponseModel{

    struct Base{

        Base(const QUuid _guid){
            guid = _guid;
        }
        QString msg;
        QUuid guid;
    };

    struct Checkin : public Base{
        Checkin(const QUuid _guid):Base(_guid){};
        Model::Device device;
        Model::Media media;
        Model::Calls calls;
    };

    struct GetApiVer: public Base{
        GetApiVer(const QUuid _guid):Base(_guid){};
        Model::ApiVer apiVer;
    };

    struct GetFeature: public Base{
        GetFeature(const QUuid _guid):Base(_guid){};
        Model::Features features;
    };

    //7//weather
    struct GetCurrentWeather: public Base{
        GetCurrentWeather(const QUuid _guid):Base(_guid){};
        Model::CurrentWeather currentWeather;
    };

    struct GetCurrentWeatherIcon: public Base{
        GetCurrentWeatherIcon(const QUuid _guid):Base(_guid){};
        QPixmap pixmap;
    };

    //8//alert
    struct GetCurrentAlert: public Base{
        GetCurrentAlert(const QUuid _guid):Base(_guid){};
        Model::CurrentAlert currentAlert;
    };

    struct GetCurrentAlertMap: public Base{
        GetCurrentAlertMap(const QUuid _guid):Base(_guid){};
        QPixmap pixmap;
    };

    //9//warning
    struct GetCurrentWarning: public Base{
        GetCurrentWarning(const QUuid _guid):Base(_guid){};
        Model::CurrentWarning currentWarning;
    };

    struct GetCurrentWarningMap: public Base{
        GetCurrentWarningMap(const QUuid _guid):Base(_guid){};
        QPixmap pixmap;
    };

};
#endif // RESPONSEMODEL_H

#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include "model.h"

#include <QPixmap>
#include <QString>


namespace ViewModel
{
    enum Page{main, connection, media, calls, alert,warning,weather, noChange};
    static QString PageToPageName(Page page)
    {
        switch(page){
        case ViewModel::Page::main: return "tab_main";
        case ViewModel::Page::calls: return "tab_calls";
        case ViewModel::Page::connection: return "tab_connection";
        case ViewModel::Page::media: return "tab_media";
        case ViewModel::Page::alert: return "tab_media";
        case ViewModel::Page::warning: return "tab_media";
        case ViewModel::Page::weather: return "tab_media";
        default: return{};
        }
    }

    struct State{
        enum ConnectionState {unknown, unchanged, created, changed, deleted};

        ConnectionState deviceState = unknown;
        QString deviceMsg;
        ConnectionState mediaState = unknown;
        QString mediaMsg;
        ConnectionState callsState = unknown;
        QString callsMsg;
    };

    struct ConnectionR{
        Page page;
        State state;
    };

    struct Apiver{
        Model::ApiVer apiver;
    };

    struct Features{
        Model::Features features;
    };

    struct Media{
        Model::Media media;
    };

    struct Device{
        Model::Device device;
    };

    struct Calls{
        Model::Calls calls;
    };

    struct CurrentWeather{
        Model::CurrentWeather currentWeather;
    };

    struct CurrentWeatherIcon{
        QPixmap pixmap;
    };

    struct CurrentAlert{
        Model::CurrentAlert currentAlert;
        bool goToAlertPage=false;
    };

    struct CurrentAlertMap{
        QPixmap pixmap;
    };

    struct CurrentWarning{
        Model::CurrentWarning currentWarning;
        bool goToWarningPage=false;
    };

    struct CurrentWarningMap{
        QPixmap pixmap;
    };
};

#endif // VIEWMODEL_H

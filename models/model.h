#ifndef MODELS_H
#define MODELS_H

#include <QJsonObject>
#include <QString>

namespace Model
{    

    struct ApiVer{
        int major=0;
        int minor = 0;

        static Model::ApiVer JsonParse(const QJsonObject &j);
        QString toString() const;
    };

    struct Media{
        enum Status {unknown,playing,stopped,paused};
        static Status ParseStatus(const QString& s);
        static QString StatusToString(Status s);

        QString album;
        QString artist;
        Status status=Status::unknown;
        QString title;

        static Model::Media JsonParse(const QJsonObject &j);
        QString toString();
        bool operator==(Media m);
    };

    struct Device{
        QString address="";
        bool connected = false;
        QString name="";
        int signal_strength=-1;

        static Model::Device JsonParse(const QJsonObject &j);
        QString toString() const;
        bool operator==(Device m);
    };

    struct Features{
        bool audio = false;
        bool call_support = false;
        QString server="";
        QString version="";

        static Model::Features JsonParse(const QJsonObject &j);
        QString toString() const;
        QString toServerString() const;
        QString toFeatureString() const;
    };    

    struct Calls{
        QString msg;

        static Model::Calls JsonParse(const QJsonObject &j);
        QString toString();
    };

    struct Sun{
        QString title;
        QString icon;
        QTime time;
    };

    struct CurrentWeather{
        QString city;
        QString shortDesc;
        QString title;
        QString value;
        QString icon;
        QString temperature;
        Sun sunrise;
        Sun sunset;

        QString toString() const {
            QString msg = title+": "+temperature+", "+value;
            if(value!=shortDesc)msg+=" - "+ shortDesc;
            return msg;
        };
    };

    //        Warning ts; //zivatar
    //        Warning wind;//széllökés
    //        Warning rainstorm; //felhőszakadás
    //        Warning fzra;//ónos eső,
    //        Warning snowdrift;//hófúvás
    //Havazásra és nagy mennyiségű esőre
    //hőség, extrém hideg, továbbá a tartós sűrű köd

    enum WarningKeys {unknown,ts,rainstorm,wind,fzra,snowdrift,rain,snow,coldx,hotx,fog};

    struct Warning{
        QString title;
        QString icon;
        QString value_icon;
        int value=0;
        int index=0;
        WarningKeys key;

        //src="/images/warningb/w0.gif"
        static int ParseValue(const QString& txt){
            static const QString rtxt = R"(\/[^\d]*(\d+)\.)";
            static const QRegularExpression rx(rtxt);

            auto m = rx.match(txt);
            if(!m.hasMatch()) return -1;
            bool ok;
            int i = m.captured(1).toInt(&ok);
            if(!ok) return -1;
            return i;
        }

        static QString ParseKey(const QString& txt){
            static const QString rtxt = R"(\/[^\/]*\W([^\d\/]*)(?:\d+)?\.)";//R"(\/[^\/]*\W([^\d]*)(?:\d+)?\.)";
            static const QRegularExpression rx(rtxt);

            auto m = rx.match(txt);
            if(!m.hasMatch()) return {};
            return m.captured(1);
        }
    };

    struct CurrentAlert{
        QMap<WarningKeys, Warning> alerts;
        QString map;
        int uvBlevel;
    };

    struct CurrentWarning{
        QMap<WarningKeys, Warning> warnings;
        QMap<WarningKeys, QString> maps;
    };

    struct Data{
        ApiVer apiVer;
        Features features;
        Media media;
        Device device;
        Calls calls;
        CurrentWeather currentWeather;
        CurrentAlert currentAlert;
        CurrentWarning currentWarning;
    };

    struct WarningMeta{
        QString title;
        QString iconMask;
        QMap<int, QString> description;
    };


    class Descriptions{
        static const QMap<int, QString> warningLevelDescriptions;
        static const QMap<Model::WarningKeys, Model::WarningMeta> warningMeta;
    };

    ///images/warning_header/ts1.gif


};

#endif // MODELS_H

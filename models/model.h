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

    struct InsoleType{
        static const QString CSV;
        int Id;
        QDateTime LastModified;
        QString Name;
        int InsoleGenderId;
        int InsoleAgeCategoryId;
        int InsoleSideId;
        qreal EUSize;
        QString GeometryCSV;
        int R;
        int VMax;
        int VMin;

        static QList<InsoleType> ParseList(const QString& str);
        static InsoleType Parse(const QString& str);
    };
//    struct Data{
//            //ApiVer apiVer;
//            Device device;
//        };
};

#endif // MODELS_H

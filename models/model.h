#ifndef MODELS_H
#define MODELS_H

#include "helpers/meta.h"
#include <QDateTime>
#include <QJsonObject>
#include <QString>

namespace Model
{       
    enum PhysDirection : int {
        Undefined = -1, Front = 1, Back, Right, Left
    };

    struct Device{
        QString address = "";
        bool connected = false;
        QString name = "";

        QString toString() const;
    };

    struct InsoleType{
        META
        static const QString CSV;
        static const QString CSV_header;
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
        static Meta<Model::InsoleType> GetMeta();
        QString toString();
    };

    struct InsoleData{
        int V;
        QVarLengthArray<int> pressures;

        static InsoleData Parse(const QByteArray& str, int dataLength);
    };
};

#endif // MODELS_H

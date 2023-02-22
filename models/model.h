#ifndef MODELS_H
#define MODELS_H

#include "helpers/meta.h"
#include <QDateTime>
#include <QJsonObject>
#include <QString>

#define nameof(x) nameof2(#x, sizeof(x))
#define nameof2(x,y) QStringLiteral(x);void(y)

namespace Model
{       
    class PhysDirection{
    public:
        enum Directions : int {
            Undefined = -1, Front = 1, Back, Right, Left
        };

        static Directions ToPhysDirection(int i);
        static QString toString(Directions d);
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

        //PhysDirection::Directions direction;
        PhysDirection::Directions direction();

        static QList<InsoleType> ParseList(const QString& str);
        static InsoleType Parse(const QString& str);
        static Meta<Model::InsoleType> GetMeta();
        QString toString();
    };

    struct InsoleData{
        int V;
        QVarLengthArray<int> pressures;

        static InsoleData Parse(const QByteArray& str, int dataLength);
        QString toString();
    };
};

#endif // MODELS_H

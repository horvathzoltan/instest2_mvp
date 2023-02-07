#ifndef MODELS_H
#define MODELS_H

#include "helpers/meta.h"
#include <QDateTime>
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

    struct InsoleType_Model{
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

        static QList<InsoleType_Model> ParseList(const QString& str);
        static InsoleType_Model Parse(const QString& str);
        static Meta<Model::InsoleType_Model> GetMeta();
    };
};

#endif // MODELS_H

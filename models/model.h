#ifndef MODELS_H
#define MODELS_H

#include <QJsonObject>
#include <QString>

#define AddRow2(x) AddRow<0>(#x, sizeof(x))

namespace Model
{       
    struct Device{
        QString address = "";
        bool connected = false;
        QString name = "";

        QString toString() const;
    };

    template<typename T>
    struct Meta{
        struct Row{
            int offset;
            QString name;
            QMetaType t;

            Row(){};
            Row(void* a, void* b, const QString& _name, const QMetaType& t);
        };

        Meta(T* a);

        T* base;
        QMap<QString, Row> rows;

        template<typename R>
        void AddRow(R* b, const QString& _name);

        template<typename Q>
        void Set(const QString& _name, Q v);

        void Set2(const QString& _name, const QString& v);
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
    };
//    struct Data{
//            //ApiVer apiVer;
//            Device device;
//        };
};

#endif // MODELS_H

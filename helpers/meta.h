#ifndef META_H
#define META_H

//#include "logger.h"
#include <QDateTime>
#include <QMetaType>
#include <QString>

#define AddRow(y,x) __brummabrumma<y>(x, MetaHelper_GetRowName(#x))
QString MetaHelper_GetRowName(const QString &n);

#define META

template<typename T>
struct Meta{
    struct Row{
        int offset;
        QString name;
        QMetaType t;

        Row(){};
        Row(void* a, void* b, const QString &_name, const QMetaType& _t){
            offset = (b>=a)?(char*)b-(char*)a:-1;
            name = _name;
            t = _t;
        }
    };

    Meta(T* a){ base = a;}

    T* base;
    QMap<QString, Row> rows;

    template<typename R>
    void __brummabrumma(R* b, const QString& _name){
        QMetaType t = QMetaType::fromType<R>();
        rows.insert(_name, Row(base, b, _name, t));
    }

    template<typename Q>
    void Set(const QString& _name, Q v){
        if(!rows.contains(_name)) return;
        Row& row = rows[_name];
        if(row.offset<0) return;
        *(Q*)((char*)base+row.offset) = v;
    }

    bool Parse(const QString& _name, const QString& vst){
        if(!rows.contains(_name)) return false;
        Row& row = rows[_name];
        if(row.offset<0) return false;
        char* ptr = (char*)base+row.offset;
        if(row.t.id() == QMetaType::Type::QDateTime)
        {
            QDateTime d = QDateTime::fromString(vst, "yyyy-MM-dd hh:mm:ss.zzz0000");
            *(QDateTime*)ptr = d;
            return d.isValid();
        }
        static QMetaType o0 = QMetaType::fromType<QString>();
        return QMetaType::convert(o0, &vst, row.t, ptr);
    }
};

#endif // META_H

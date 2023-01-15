#ifndef RESPONSEMODEL_H
#define RESPONSEMODEL_H

#include <QPixmap>
#include <QString>
#include <QUuid>

namespace ResponseModel{

    struct Base{

        Base(const QUuid _guid){
            guid = _guid;
        }
        QString message;
        QUuid guid;
    };


    struct FindPi: public Base{
        FindPi(const QUuid _guid):Base(_guid){};
        QStringList iplist;
    };

};
#endif // RESPONSEMODEL_H

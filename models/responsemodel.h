#ifndef RESPONSEMODEL_H
#define RESPONSEMODEL_H

#include <QPixmap>
#include <QString>
#include <QUuid>
#include "model.h"

namespace ResponseModel{

    struct Base{

        Base(const QUuid _guid){
            guid = _guid;
        }
        QString message;
        QUuid guid;
    };


    struct FindPi: public Base{
        enum FindStatus
        {
            Started=0, Finished
        };
        FindPi(const QUuid _guid):Base(_guid){};
        bool ok;
        QStringList apiKey_L;
        QStringList apiKey_R;
        QStringList apiKey_U; // van, de nem L/R
        QStringList apiKey_E; // nincs
        FindStatus status;
    };

    struct PiData: public Base{
        PiData(const QUuid _guid):Base(_guid){};

        //QString msg;
        Model::PhysDirection::Directions direction;
    };
};
#endif // RESPONSEMODEL_H

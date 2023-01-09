#ifndef RESPONSEMODEL_H
#define RESPONSEMODEL_H

#include "model.h"

#include <QPixmap>
#include <QString>
#include <QUuid>

namespace ResponseModel{

    struct Base{

        Base(const QUuid _guid){
            guid = _guid;
        }
        QString msg;
        QUuid guid;
    };


    struct GetApiVer: public Base{
        GetApiVer(const QUuid _guid):Base(_guid){};
        Model::ApiVer apiVer;
    };   

};
#endif // RESPONSEMODEL_H

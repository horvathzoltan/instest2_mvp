#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include "model.h"

#include <QPixmap>
#include <QString>


namespace ViewModel
{    

    struct FindPi{
        //Model::ApiVer apiver;
        QString errors;
        QStringList hosts;
    };  

    struct Device{
        Model::Device device;
    };

};

#endif // VIEWMODEL_H

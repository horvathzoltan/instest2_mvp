#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include "model.h"

#include <QPixmap>
#include <QString>


namespace ViewModel
{    

    struct FindPi{
        QString message;
        QStringList hosts;
    };  

    struct Device{
        Model::Device device;
    };

};

#endif // VIEWMODEL_H

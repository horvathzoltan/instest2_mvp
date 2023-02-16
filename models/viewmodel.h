#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QPixmap>
#include <QString>
#include "model.h"

namespace ViewModel
{    
    struct FindPi{
        QString message_L;
        QString message_R;
    };  

    struct PiData{
        QString message;
        Model::PhysDirection::Directions direction;
    };
};

#endif // VIEWMODEL_H

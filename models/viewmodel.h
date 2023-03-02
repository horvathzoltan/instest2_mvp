#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QImage>
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
        QImage heatmapImage;
        Model::PhysDirection::Directions direction;
    };
};

#endif // VIEWMODEL_H

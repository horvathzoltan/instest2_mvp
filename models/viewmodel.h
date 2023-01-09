#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include "model.h"

#include <QPixmap>
#include <QString>


namespace ViewModel
{    

    struct Apiver{
        Model::ApiVer apiver;
    };  

    struct Device{
        Model::Device device;
    };

};

#endif // VIEWMODEL_H

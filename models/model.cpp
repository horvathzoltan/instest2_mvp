#include "model.h"


QString Model::ApiVer::toString() const
{
    return QString::number(buildnum);
}

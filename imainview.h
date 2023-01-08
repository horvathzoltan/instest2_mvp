#ifndef IMAINVIEW_H
#define IMAINVIEW_H

#include "models/viewmodel.h"


class IMainView
{
public:
// (wiew)action -> (presenter)processAction -> [ (dowork)ResponseAction -> (presenter)onResponseAction -> ] (wiew)set_view
// ------------                                                                                             --------------

    //2//apiver
    virtual void GetApiverActionTriggered(IMainView *sender) = 0;
    virtual void set_ApiverView(const ViewModel::Apiver& m) = 0;
};

#endif // IMAINVIEW_H

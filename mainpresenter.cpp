#include "dowork.h"
#include "dowork.h"
#include "logger.h"
#include "mainpresenter.h"
#include <QDebug>
#include <models/viewmodel.h>
#include <models/responsemodel.h>
#include <models/model.h>

MainPresenter::MainPresenter(QObject *parent) :QObject(parent)
{

}

void MainPresenter::appendView(IMainView *w)
{
    if(_views.contains(w)) return;
    _views.append(w);

    auto *view_obj = dynamic_cast<QObject*>(w);

    //2//apiver
    QObject::connect(view_obj, SIGNAL(GetApiverActionTriggered(IMainView*)),
                     this, SLOT(processGetApiverAction(IMainView*)));

    //refreshView(w);    
}


auto MainPresenter::init(const MainPresenterInit& m) -> bool
{
    _isInited = false;
    _dowork.init({m.settings});
    // (dowork)ResponseAction - (presenter)onResponseAction
    //2//apiver
    connect(&_dowork,SIGNAL(ResponseFindPi(ResponseModel::FindPi)),
            this,SLOT(onResponseFindPi(ResponseModel::FindPi)));
    _isInited = true;
    return true;
}

void MainPresenter::initView(IMainView *w) const {
   // MainViewModel::DoWorkRModel rm{"started",{}};
    //w->set_DoWorkRModel(rm);
};



/*GetApiver*/
void MainPresenter::processGetApiverAction(IMainView *sender){
    ResponseModel::FindPi r = _dowork.FindPi({8080}, 30);
    _senders.insert(r.guid,sender);
    ViewModel::FindPi vm;
    vm.errors = r.message;
    sender->set_ApiverView(vm);
}

void MainPresenter::onResponseFindPi(ResponseModel::FindPi m)
{
    if(_senders.contains(m.guid)){
        ViewModel::FindPi vm = {.errors = m.message, .hosts=m.iplist };
        _senders[m.guid]->set_ApiverView(vm);
        _senders.remove(m.guid);
    }
}



#include "dowork.h"
#include "dowork.h"
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
    connect(&_dowork,SIGNAL(ResponsePiData(ResponseModel::PiData)),
            this,SLOT(onResponsePiData(ResponseModel::PiData)));
    _isInited = true;
    return true;
}

void MainPresenter::initView(IMainView *w) const {
   // MainViewModel::DoWorkRModel rm{"started",{}};
    //w->set_DoWorkRModel(rm);
};




/*GetApiver*/
void MainPresenter::processGetApiverAction(IMainView *sender){
    ResponseModel::FindPi r = _dowork.FindPi({8080}, 8, 4);//8, 4
    _senders.insert(r.guid,sender);
    ViewModel::FindPi vm
    {
        .message_L = r.message,
        .message_R = {}
    };
    sender->set_ApiverView(vm);
}

void MainPresenter::onResponseFindPi(ResponseModel::FindPi m)
{
    if(!_senders.contains(m.guid)) return;
    ViewModel::FindPi vm;
    for(auto&key:m.apiKey_L)
    {
        if(!vm.message_L.isEmpty()) vm.message_L+='\n';
        auto api = _dowork.GetApi(key);
        if(api)
        {
            vm.message_L+=api->toString();
        }
    }
    for(auto&key:m.apiKey_R)
    {
        if(!vm.message_R.isEmpty()) vm.message_R+='\n';
        auto api = _dowork.GetApi(key);
        if(api)
        {
            vm.message_R+=api->toString();
        }
    }

    _senders[m.guid]->set_ApiverView(vm);
}

void MainPresenter::onResponsePiData(ResponseModel::PiData m)
{
    if(!_senders.contains(m.guid)) return;
    ViewModel::PiData vm;

    vm.direction = m.direction;
    vm.message = m.message;

    _senders[m.guid]->set_PiDataView(vm);
    // ha a request ready, azaz nem jön több művelet
    // ha van egy jobb és egy right, készen vagyunk, kezdődhet a kiolvasás
}


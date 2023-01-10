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
    connect(&_dowork,SIGNAL(ResponseGetApiverAction(ResponseModel::GetApiVer)),
            this,SLOT(onResponseGetApiverAction(ResponseModel::GetApiVer)));
    _isInited = true;
    return true;
}

void MainPresenter::initView(IMainView *w) const {
   // MainViewModel::DoWorkRModel rm{"started",{}};
    //w->set_DoWorkRModel(rm);
};



/*GetApiver*/
void MainPresenter::processGetApiverAction(IMainView *sender){
    DoWork::FindPiModelR result = _dowork.FindPi({8080}, 30);

    ViewModel::Apiver m;

    for(auto&key:result.ipAddresses.keys())
    {
        QSet<int> values = result.ipAddresses[key];
        QString str;//ports
        for(auto&v:values){
            if(!str.isEmpty()) str+=',';
            str+=QString::number(v);
        };
        m.hosts.append("ip:" + key + ":" +str);
    }

    m.errors = result.errors.first();
    sender->set_ApiverView(m);
}

//void MainPresenter::onResponseGetApiverAction(ResponseModel::GetApiVer m)
//{
//    if(_senders.contains(m.guid)){
//        //_data.apiVer = m.apiVer;
//        //_dowork.setData(m.apiVer);
//        ViewModel::Apiver rm = {m.apiVer};
//        _senders[m.guid]->set_ApiverView(rm);
//        _senders.remove(m.guid);
//    }
//}



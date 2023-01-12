#ifndef MAINPRESENTER_H
#define MAINPRESENTER_H
#include <QObject>
#include <models/responsemodel.h>
#include "imainview.h"
#include "dowork.h"

class IMainView;

class MainPresenter : public QObject
{
    Q_OBJECT

public:
    explicit MainPresenter(QObject *parent = nullptr);
    void appendView(IMainView *w);
    void initView(IMainView *w) const;
    struct MainPresenterInit{
        Settings settings;
    };

    bool init(const MainPresenterInit& m);
private:
    QList<IMainView*> _views;

private slots:
    // (wiew)action -> (presenter)processAction -> [ (dowork)ResponseAction -> (presenter)onResponseAction -> ] (wiew)set_view
    //                 ------------------------                                ---------------------------
    //2//apiver
    void processGetApiverAction(IMainView *sender);
    void onResponseFindPi(ResponseModel::FindPi m);

private:
    bool _isInited = false;
    DoWork _dowork;
    QMap<QUuid, IMainView*> _senders;

    //Model::Data _data;
};

#endif // MAINPRESENTER_H

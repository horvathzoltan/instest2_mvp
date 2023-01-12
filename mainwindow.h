#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imainview.h"

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public IMainView
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

// (wiew)action -> (presenter)processAction -> [ (dowork)ResponseAction -> (presenter)onResponseAction -> ] (wiew)set_view
// ------------                                                                                             --------------

    void set_ApiverView(const ViewModel::FindPi& m);

signals:
    void GetApiverActionTriggered(IMainView *sender);

private slots:
    void onTimerTimeout();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTimer _timer;
    unsigned long int _tick=0;
};
#endif // MAINWINDOW_H

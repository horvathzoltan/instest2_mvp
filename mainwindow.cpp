#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setText("started");
    //setPage(ViewModel::Page::main);
    _timer.setParent(this);
    QObject::connect(&_timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
    _timer.start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*UserActions*/

void MainWindow::on_pushButton_clicked()
{
    emit GetApiverActionTriggered(this);
}

void MainWindow::set_ApiverView(const ViewModel::Apiver &m)
{
    QString msg2 = m.apiver.toString();
    ui->label->setText(msg2);
}

void MainWindow::onTimerTimeout()
{
    if(_tick==0){

    }
    //   emit GetDataActionTriggered(this);
    _tick++;    
}







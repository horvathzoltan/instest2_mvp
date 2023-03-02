#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include "models/model.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_L->setText("started");
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

void MainWindow::set_ApiverView(const ViewModel::FindPi &m)
{
//    if(m.message_L.isEmpty())
//    {
//        ui->label_L->clear();
//    }
//    else
//    {
//        ui->label_L->setText(m.message_L);
//    }
//    if(m.message_R.isEmpty())
//    {
//        ui->label_R->clear();
//    }
//    else
//    {
//        ui->label_R->setText(m.message_R);
//    }
    ui->label_L->setText(m.message_L);
    ui->label_R->setText(m.message_R);
}

void MainWindow::set_PiDataView(const ViewModel::PiData &m)
{
    QLabel* labelMessage;
    QLabel* labelImage;
    switch(m.direction){
    case Model::PhysDirection::Directions::Left:
        labelMessage=ui->label_2_L;
        labelImage = ui->label_3_L;
        break;
    case Model::PhysDirection::Directions::Right:
        labelMessage=ui->label_2_R;
        labelImage = ui->label_3_R;
        break;
    default:
        labelMessage=nullptr;
        labelImage=nullptr;
        break;
    }
    if(labelMessage) labelMessage->setText(m.message);

    if(labelImage){
        //m.heatmapImage.mirror(false, true);
        auto pixmap = QPixmap::fromImage(
                    m.heatmapImage.mirrored(false, true)
                    );

        /*QImage image;
        QPainter p;
        p.begin(&image);
        p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        p.fillRect(image.rect(), QColor(0, 0, 0, 50));
        p.end();*/

        labelImage->setPixmap(pixmap);
    }
}

void MainWindow::onTimerTimeout()
{
    if(_tick==0){

    }
    //   emit GetDataActionTriggered(this);
    _tick++;    
}







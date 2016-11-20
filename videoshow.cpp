#include "videoshow.h"
#include "ui_videoshow.h"
#include "common.h"
#include <QTimer>
#include <QMessageBox>
#include <QDebug>

videoShow::videoShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::videoShow),
    paused(false)
{
    ui->setupUi(this);
    this->setWindowTitle("Video Show");


    camera.open(VIDEO_NUMBER);


    timer_id = this->startTimer(33);
    if(!camera.isOpened())
    {
        QMessageBox::warning(this, tr("ERROR"), tr("Opening camera %1 error").arg(VIDEO_NUMBER));

    }

}

videoShow::~videoShow()
{
    delete ui;

}

void videoShow::on_pauseStartBtn_clicked()
{
     paused ^= true;
}

void videoShow::on_takePicBtn_clicked()
{
    Mat  img_yuv;
    camera >>  img_yuv;
    if(img_yuv.empty()){
        QMessageBox::warning(this, tr("ERROR"), tr("Takingpicture error"));
        return ;
    }

    cvtColor(img_yuv, img_rgb, CV_BGR2RGB);
    QImage image((uchar*)img_rgb.data, img_rgb.cols, img_rgb.rows,QImage::Format_RGB888);
    ui->pictureLabel->setPixmap(QPixmap::fromImage(image));
}

void videoShow::timerEvent(QTimerEvent*)
{
    if(!paused){
        Mat  img_yuv;
        camera >>  img_yuv;
        if(img_yuv.empty()){
            QMessageBox::warning(this, tr("ERROR"), tr("VideoPlaying error"));
            return ;
        }
        cvtColor(img_yuv,img_rgb,CV_BGR2RGB);
        QImage image((uchar*)img_rgb.data, img_rgb.cols, img_rgb.rows,QImage::Format_RGB888);
        ui->videoLabel->setPixmap(QPixmap::fromImage(image));
    }
}

void videoShow::closeEvent(QCloseEvent *)
{
    paused = true;
    camera.release();
    this->killTimer(timer_id);
#ifdef USE_DEBUG
    qDebug()<<"videoShow has been closed";
#endif
}

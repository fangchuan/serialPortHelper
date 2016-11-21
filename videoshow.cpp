#include "videoshow.h"
#include "ui_videoshow.h"
#include "common.h"
#include <QTime>
#include <QMessageBox>
#include <QDebug>

videoShow::videoShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::videoShow),
    paused(false)
{
    ui->setupUi(this);
    this->setWindowTitle("Video Show");

    videoCilent = new QTcpSocket;
    videoCilent->connectToHost(ui->IPEdit->text(), ui->portEdit->text().toInt(), QIODevice::ReadWrite);
    camera.open(VIDEO_NUMBER);


    timer_id = this->startTimer(33);
    if(!camera.isOpened())
    {
        QMessageBox::warning(this, tr("ERROR"), tr("Opening camera %1 error").arg(VIDEO_NUMBER));

    }

    connect(videoCilent, &QTcpSocket::connected, this, &sendVideoRequest);
    connect(videoCilent, &QTcpSocket::readyRead, this, &readVideoStream);
    connect(videoCilent, &QTcpSocket::error,   this, &socketError);
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
//        QTime time_stamp;
//        time_stamp=time_stamp.currentTime();
        QString str=(QTime::currentTime()).toString();
        putText(img_yuv, str.toLatin1().data(),
                Point(0,30),3,1,Scalar(255,0,0),1,LINE_8,false);
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

void videoShow::sendVideoRequest()
{
    QByteArray req("GET /?action=stream\n\n");
    QDataStream out(&req, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6);
    videoCilent->write(out);
}

void videoShow::readVideoStream()
{

}

void videoShow::socketError()
{

}

#include "videoshow.h"
#include "ui_videoshow.h"
#include "common.h"
#include "core/core.hpp"
#include <QTime>
#include <QFile>
#include <QFileDialog>
#include <QRegExp>
#include <QRegExpValidator>
#include <QScrollArea>
#include <QHostAddress>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>

void imageDrawText(QImage* img,QString str);


videoShow::videoShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::videoShow),
    paused(false),
    videoClient(NULL),
    dataClient(NULL)
{
    ui->setupUi(this);
    this->setWindowTitle("Video Show");

//    QRect rcLabel = ui->videoLabel->geometry();
//    //为标签添加滚动区域，方便浏览大图
//    QScrollArea *pSA = new QScrollArea(this);   //该对象交给主窗体自动管理，不用手动删除
//    //把标签填充到滚动区域里
//    pSA->setWidget(ui->videoLabel);
//    //设置滚动区域占据矩形
//    pSA->setGeometry(rcLabel);

    videoClient = new QTcpSocket;
    dataClient = new QTcpSocket;


    ipAddressValider();

    startCamera();

    connect(videoClient, SIGNAL(connected()), this, SLOT(sendVideoRequest()));
    connect(videoClient, SIGNAL(readyRead()), this, SLOT(readVideoStream()));
    connect(videoClient, SIGNAL(error(QAbstractSocket::SocketError)),     this, SLOT(socketError()));
    connect(dataClient,  SIGNAL(readyRead()), this, SLOT(readDataStream()));
    connect(dataClient,  SIGNAL(error(QAbstractSocket::SocketError)),     this, SLOT(socketError()));
}

videoShow::~videoShow()
{
    delete ui;

}
//
void videoShow::on_pauseStartBtn_clicked()
{
     paused ^= true;
}
//
void videoShow::on_takePicBtn_clicked()
{
    const QPixmap *pixmap = ui->videolabel->pixmap();
    if(!pixmap){
        QMessageBox::warning(this, tr("ERROR"), tr("Takingpicture error"));
        return ;
    }
    ui->picturelabel->setPixmap(*pixmap);
}
//
void videoShow::timerEvent(QTimerEvent*)
{
    if(!paused){
        Mat  img_bgr;
        camera >>  img_bgr;
        if(img_bgr.empty()){
            QMessageBox::warning(this, tr("ERROR"), tr("VideoPlaying error"));
            return ;
        }

        QString str=(QTime::currentTime()).toString();
#ifdef  USE_DEBUG
        qDebug()<<str;
#endif

        cvtColor(img_bgr,img_rgb,CV_BGR2RGB);
        QImage image((uchar*)img_rgb.data, img_rgb.cols, img_rgb.rows,QImage::Format_RGB888);
        imageDrawText(&image, str);
        ui->videolabel->setPixmap(QPixmap::fromImage(image));

        ui->videolabel->trackSelectObject();
    }
}
//
void videoShow::closeEvent(QCloseEvent *)
{
    paused = true;
    if(camera.isOpened()){
        camera.release();
    }
    if(videoClient){
        videoClient->abort();
    }
    if(dataClient){
        dataClient->abort();
    }
    this->killTimer(timer_id);
#ifdef USE_DEBUG
    qDebug()<<"videoShow has been closed";
#endif
}
//
void videoShow::sendVideoRequest()
{
    QByteArray req("GET /?action=stream\n\n");
    QDataStream out(&req, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6);
    videoClient->write((const char*)&req);
}
//
void videoShow::readVideoStream()
{

}
//
void videoShow::readDataStream()
{

}
//
void videoShow::socketError()
{
    QMessageBox::warning(this,
                         tr("Error"),
                         tr("Tcp Connection Fail"));
}
//
void videoShow::on_savePicBtn_clicked()
{
    QString fn = QFileDialog::getSaveFileName(
                this,
                tr("保存图片"),
                ".",
                ("jpeg file(*.jpg);;png file(*.png);;bmp file(*.bmp)"));
    if(fn.isEmpty()){
        return ;
    }

    const QPixmap *pixmap = ui->picturelabel->pixmap();
    if(pixmap){
        if(pixmap->save(fn)){
            QMessageBox::information(this,
                                     tr("Tip"),
                                     tr("Save Pictrue Success"));
        }
        else{
            QMessageBox::warning(this,
                                 tr("Error"),
                                 tr("Save Pictrue Fail"));
        }
    }

//    imwrite(fn,);
}
//
void videoShow::on_connectIPBtn_clicked()
{
    QString ip_str = ui->IPEdit->text();
    quint16 ip_port = ui->portEdit->text().toInt();

    if(ip_str.isEmpty() || ip_port == 0){
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Please Input IP Address And Port"));
        return;
    }
    QHostAddress ip(ip_str);
    //mjpeg stream transform via 8080port
    if(videoClient){
        videoClient->connectToHost(ip, 8080/*ui->portEdit->text().toInt()*/,
                                   QIODevice::ReadWrite);
    }
    if(dataClient){
        dataClient->connectToHost(ip, ip_port,
                                  QIODevice::ReadWrite);
    }
}
//
void videoShow::ipAddressValider()
{
    //定义 IPv4 正则表达式，注意 "\\" 就是一个反斜杠字符
    QRegExp re("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
               "(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    //新建正则表达式验证器
    QRegExpValidator *reVali = new QRegExpValidator(re);
    //设置给 lineEditIP
    ui->IPEdit->setValidator(reVali);
    //新建整数验证器
    QIntValidator *intVali = new QIntValidator(0, 65535);
    //设置给 lineEditPort
    ui->portEdit->setValidator(intVali);
}
//
void videoShow::startCamera()
{
    camera.open(VIDEO_NUMBER);

    timer_id = this->startTimer(33);
    if(!camera.isOpened())
    {
        QMessageBox::warning(this, tr("ERROR"), tr("Opening camera %1 error").arg(VIDEO_NUMBER));
    }
}
//
void imageDrawText(QImage* img,QString str)
{
    QPainter painter(img) ;
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    QPen pen = painter.pen();
    pen.setColor(Qt::red);
    QFont font = painter.font();
    font.setBold(true);//加粗
    font.setPixelSize(20);//改变字体大小
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(img->rect(), Qt::AlignCenter,str);
}

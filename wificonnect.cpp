#include "wificonnect.h"
#include "ui_wificonnect.h"
#include "picturedialog.h"
#include "common.h"
#include "core/core.hpp"
#include <QRegExp>
#include <QRegExpValidator>
#include <QScrollArea>
#include <QHostAddress>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>

//regular express validator
inline void wifiConnect::ipAddressValidator()
{
    //define an IPV4 RegularExpress
    QRegExp re("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
               "(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    QRegExpValidator *reVali = new QRegExpValidator(re);
    ui->IPEdit->setValidator(reVali);
}

wifiConnect::wifiConnect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wifiConnect),
    videoClient(NULL),
    dataClient(NULL),
    vw(NULL),
    isPaused(true),
    isConnected(false)
{
    ui->setupUi(this);
    this->setWindowTitle("WIFI");

    ui->connectIPBtn->setDefault(true);
    ui->connectIPBtn->setFocus();

    ipAddressValidator();

    videoClient = new QTcpSocket;
    dataClient = new QTcpSocket;

    connect(videoClient, &QTcpSocket::connected, this, &wifiConnect::sendVideoRequest);
    connect(videoClient, &QTcpSocket::readyRead, this, &wifiConnect::readVideoStream);
    connect(videoClient, static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            this, &wifiConnect::socketError);
    connect(dataClient,  &QTcpSocket::readyRead, this, &wifiConnect::readDataStream);
    connect(dataClient,  static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            this, &wifiConnect::socketError);

}

wifiConnect::~wifiConnect()
{
    delete ui;
    delete videoClient;
    delete dataClient;
    videoClient = NULL;
    dataClient = NULL;
}

//the slot of connectIP button
void wifiConnect::on_connectIPBtn_clicked()
{

    QString ip_str = ui->IPEdit->text();
    if(ip_str.isEmpty()){
        QMessageBox::warning(this,
                             "Error",
                             "Please Input IP Address");
        return;
    }
    if(isConnected == false){
        QHostAddress ip(ip_str);
        //mjpeg stream transform via 8080port
        if(videoClient){
            videoClient->connectToHost(ip, VIDEO_SERVICE_PORT,
                                       QIODevice::ReadWrite);
        }else{
            QMessageBox::warning(this,
                                 "Error",
                                 "Alloc videoClient fail");
            return ;
        }
        if(dataClient){
            dataClient->connectToHost(ip, DATA_SERVICE_PORT,
                                      QIODevice::ReadWrite);
        }else{
            QMessageBox::warning(this,
                                 "Error",
                                 "Alloc dataClient fail");
            return ;
        }

        isConnected = true;
        emit wifiConnection("Connect established");
    }
    ui->connectIPBtn->setIcon(QIcon(":/images/button_running"));
#ifdef USE_DEBUG
    qDebug()<<"Connect Successfully";
#endif
}
//take picture from the QLabel
void wifiConnect::on_takePicBtn_clicked()
{
    const QPixmap *pixmap = ui->videolabel->pixmap();
    if(!pixmap){
        QMessageBox::warning(this, "ERROR", "Taking picture error");
        return ;
    }

    pictureDialog pd(*pixmap);
    pd.exec();//模态对话框
}
//close the video windows
void wifiConnect::closeEvent(QCloseEvent *)
{
    //only release the ptr and the QBytearray pool
    if(videoClient->isOpen()){
        videoClient->abort();
    }
    if(dataClient->isOpen()){
        dataClient->abort();
    }
    if(!img_stream.isEmpty()){
        img_stream.clear();
    }
    if(!data_stream.isEmpty()){
        data_stream.clear();
    }
    if(vw){
        vw->release();
    }

    emit wifiConnection("Connection distryed");
    //dont need to release the Mat
/*    if(!img_rgb.empty()){
        img_rgb.release();
    }*/
#ifdef USE_DEBUG
    qDebug()<<"videoShow has been closed";
#endif
}
//send video request command
void wifiConnect::sendVideoRequest()
{
    //http://192.168.1.1:8080/?action=stream
    QByteArray req("GET /?action=stream/?action=stream\n\n");
    videoClient->write(req);
}
//read the video stream from port 8080
int wifiConnect::readVideoStream()
{
    if(videoClient->bytesAvailable() < FRAME_MIN_LENGTH )
            return ERROR_VIDEO_STREAM_BYTES_NUM;

    QByteArray tmpBlock;
    static const char SOFData[]={0xff,0xd8};
    static const char EOFData[]={0xff,0xd9};

    QByteArray SOFstr = QByteArray::fromRawData(SOFData,sizeof(SOFData));
    QByteArray EOFstr = QByteArray::fromRawData(EOFData,sizeof(EOFData));

    int SOFpos = 0;
    int EOFpos = 0;

    tmpBlock = videoClient->readAll();
    img_stream.append(tmpBlock);//save the stream data

    if((SOFpos = img_stream.indexOf(SOFstr))!= -1)
    {
        if((EOFpos = img_stream.indexOf(EOFstr))!= -1)
        {
            EOFpos += 2;
            if(EOFpos > SOFpos)
            {
                QByteArray ba;
                ba = img_stream.mid(SOFpos,EOFpos - SOFpos);

                QImage image;
                if( image.loadFromData(ba) ){
                    ui->videolabel->setPixmap(QPixmap::fromImage(image));
                    ui->videolabel->trackSelectObject();
                    saveVideo();
                }else{
                    QMessageBox::warning(this, "Error", "Fail to receive image from router!");
                }

                ba.clear();
            }else{
                return ERROR_VIDEO_STREAM_START_END_POS;
            }
            img_stream.remove(0,EOFpos+1);

            return NO_ERROR_VIDEO_STREAM;
        }else{
            return ERROR_VIDEO_STREAM_END_OF_FRAME;
        }
    }else{
        return ERROR_VIDEO_STREAM_START_OF_FRAME;
    }

}
//read the data stream from the port 2001
int wifiConnect::readDataStream()
{
    QByteArray temp = dataClient->readAll();
    data_stream.append(temp);

    if(data_stream.indexOf(QByteArray(1,FRAME_STR)) != -1)
    if(data_stream.indexOf(QByteArray(1,FRAME_END)) != -1)

    return 0;
}
//handle the connection error
void wifiConnect::socketError()
{
    QMessageBox::warning(this,
                         "Error",
                         "Tcp Connection Fail");
}

//save the received video
void wifiConnect::on_videoSaveBtn_clicked()
{
    if(NULL  == vw){
        vw = new VideoWriter(VIDEO_NAME, -1,
                             VIDEO_FPS, Size(640,480));
    }
    isPaused = !isPaused;
    if(isPaused){
        ui->videoSaveBtn->setIcon(QIcon(":/images/button_stop"));
    }else{
        ui->videoSaveBtn->setIcon(QIcon(":/images/button_running"));
    }
}
//
void wifiConnect::saveVideo()
{
    if(!isPaused ){
    img_mat = ui->videolabel->getImage();
        if(!img_mat.empty()){

            if(!vw->isOpened()){
                vw->open(VIDEO_NAME, -1,
                         VIDEO_FPS, Size(640, 480));
            }
            vw->write(img_mat);
        }
    }
}



void wifiConnect::on_forwardBtn_clicked()
{
    QByteArray cmd;
    cmd += (char)FRAME_STR;
    cmd += (char)1;
    cmd += (char)CMD_FORWARD;
    cmd += (char)FRAME_END;
    qDebug()<<cmd.toHex();
    dataClient->write(cmd);
}

void wifiConnect::on_turnLeftBtn_clicked()
{
    QByteArray cmd;
    cmd += (char)FRAME_STR;
    cmd += (char)1;
    cmd += (char)CMD_LEFT;
    cmd += (char)FRAME_END;
    qDebug()<<cmd.toHex();
    dataClient->write(cmd);
}

void wifiConnect::on_turnRightBtn_clicked()
{
    QByteArray cmd;
    cmd += (char)FRAME_STR;
    cmd += (char)1;
    cmd += (char)CMD_RIGHT;
    cmd += (char)FRAME_END;
    qDebug()<<cmd.toHex();
    dataClient->write(cmd);
}

void wifiConnect::on_backwardBtn_clicked()
{
    QByteArray cmd;
    cmd += (char)FRAME_STR;
    cmd += (char)1;
    cmd += (char)CMD_BACKWARD;
    cmd += (char)FRAME_END;
    qDebug()<<cmd.toHex();
    dataClient->write(cmd);
}

void wifiConnect::on_stopBtn_clicked()
{
    QByteArray cmd;
    cmd += (char)FRAME_STR;
    cmd += (char)1;
    cmd += (char)CMD_STOP;
    cmd += (char)FRAME_END;
    qDebug()<<cmd.toHex();
    dataClient->write(cmd);
}

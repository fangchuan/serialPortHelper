#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

#include <QWidget>
#include <QTcpSocket>
#include <QRegExpValidator>
#include "highgui/highgui.hpp"
#include "imgproc/imgproc.hpp"
using namespace cv;

namespace Ui {
class wifiConnect;
}



class wifiConnect : public QWidget
{
    Q_OBJECT
private:
    Ui::wifiConnect *ui;
//    VideoCapture  camera;
    Mat          img_mat;
    QByteArray   img_stream;
    QByteArray   data_stream;
    QTcpSocket* videoClient;
    QTcpSocket* dataClient;
    VideoWriter*      vw;
    bool        isPaused;
    bool        isConnected;
//    virtual void timerEvent(QTimerEvent*);
    virtual void closeEvent(QCloseEvent*);

public:
    explicit wifiConnect(QWidget *parent = 0);
    ~wifiConnect();
    void saveVideo();
    inline void ipAddressValidator();

signals:
    void wifiConnection(const QString &statu );

private slots:

    void on_takePicBtn_clicked();

    void sendVideoRequest();

    int readVideoStream();

    int readDataStream();

    void socketError();

    void on_connectIPBtn_clicked();

    void on_videoSaveBtn_clicked();


    void on_forwardBtn_clicked();
    void on_turnLeftBtn_clicked();
    void on_turnRightBtn_clicked();
    void on_backwardBtn_clicked();
    void on_stopBtn_clicked();
};

#endif
// WIFI_CONNECT_H

#ifndef VIDEOSHOW_H
#define VIDEOSHOW_H

#include <QWidget>
#include <QTcpSocket>
#include "highgui/highgui.hpp"
#include "imgproc/imgproc.hpp"
using namespace cv;

namespace Ui {
class videoShow;
}

class videoShow : public QWidget
{
    Q_OBJECT

public:
    explicit videoShow(QWidget *parent = 0);
    ~videoShow();
    void startCamera();
    void ipAddressValider();
private slots:
    void on_pauseStartBtn_clicked();

    void on_takePicBtn_clicked();

    void sendVideoRequest();

    void readVideoStream();

    void readDataStream();

    void socketError();

    void on_connectIPBtn_clicked();

private:
    Ui::videoShow *ui;
    VideoCapture  camera;
    Mat          img_rgb;
    QTcpSocket* videoClient;
    QTcpSocket* dataClient;
    bool        paused;
    int       timer_id;

    virtual void timerEvent(QTimerEvent*);
    virtual void closeEvent(QCloseEvent*);
};

#endif // VIDEOSHOW_H

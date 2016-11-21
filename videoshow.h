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

private slots:
    void on_pauseStartBtn_clicked();

    void on_takePicBtn_clicked();

    void sendVideoRequest();

    void readVideoStream();

    void socketError();
private:
    Ui::videoShow *ui;
    VideoCapture  camera;
    Mat          img_rgb;
    QTcpSocket* videoCilent;
    bool        paused;
    int       timer_id;

    virtual void timerEvent(QTimerEvent*);
    virtual void closeEvent(QCloseEvent*);
};

#endif // VIDEOSHOW_H

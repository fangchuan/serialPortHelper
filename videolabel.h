#ifndef VIDEOLABEL_H
#define VIDEOLABEL_H

#include <QWidget>
#include <QLabel>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;


class videoLabel: public QLabel
{
    Q_OBJECT
public:
    explicit videoLabel(QWidget *parent);
    ~videoLabel();
    void trackSelectObject();
    Mat getImage();
private:
    virtual void paintEvent(QPaintEvent* pe);
    virtual void mousePressEvent(QMouseEvent* me);
    virtual void mouseReleaseEvent(QMouseEvent* me);
    virtual void mouseMoveEvent(QMouseEvent *me);


    Mat  image,hist, backproj;
    Point origin;
    Rect selection;
    Rect trackWindow;
    RotatedRect trackBox;
    bool selectObject;
    qint8 trackObject;

};

#endif // VIDEOLABEL_H

#ifndef COMMON_H
#define COMMON_H

#include "qnamespace.h"
#include <QWidget>
#include "opencv.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#define  CH1_CURVE_COLOR  Qt::blue
#define  CH2_CURVE_COLOR  Qt::red
#define  CH3_CURVE_COLOR  Qt::green
#define  CH4_CURVE_COLOR  Qt::black
#define  CH_PEN_WIDTH     2

#define  CH1_VALUE        1
#define  CH2_VALUE        2
#define  CH3_VALUE        3
#define  CH4_VALUE        4

#define  VIDEO_NUMBER     0

//#define  USE_DEBUG
//#define  USE_SYSTRAYICON

extern void qImageToIplImage(const QImage * qImage,IplImage *iplImageBuffer);
extern QImage IplImageToQImage(const IplImage * iplImage, quint8 *qImageBuffer,
                               double mini, double maxi);
#endif // COMMON_H

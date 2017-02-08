#ifndef COMMON_H
#define COMMON_H

#include "qnamespace.h"
#include <QWidget>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;


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
#define  VIDEO_NAME       "0.avi"
#define  VIDEO_FPS        15.0f

#define  VIDEO_SERVICE_PORT     8080
#define  DATA_SERVICE_PORT      2001
#define  DATA_SERVICE_PORT_MIN  1000
#define  DATA_SERVICE_PORT_MAX  65535

#define  HEADER_MAX_LENGTH  100
#define  FRAME_MIN_LENGTH   3000
#define  FRAME_MAX_LENGTH   (4000 + HEADER_MAX_LENGTH )

#define  NO_ERROR_VIDEO_STREAM             0
#define  ERROR_VIDEO_STREAM_BYTES_NUM      -1
#define  ERROR_VIDEO_STREAM_START_OF_FRAME -2
#define  ERROR_VIDEO_STREAM_END_OF_FRAME   -3
#define  ERROR_VIDEO_STREAM_START_END_POS  -4


//#define  USE_DEBUG
//#define  USE_SYSTRAYICON

//protocol
#define  VERSION    0x01
#//一帧数据最长为19bytes
#define  MAX_LEN   19

#define  FRAME_STR   0xAA    //
#define  FRAME_END   0xBB    //

//控制器上各组件ID
#define  MOTOR_ID     0x00
#define  MOTOR_1_ID   0x01
#define  MOTOR_2_ID   0x02

#define  SERVO_ID     0x10
#define  SERVO_1_ID   0x11
#define  SERVO_2_ID   0x12
#define  SERVO_3_ID   0x13
#define  SERVO_4_ID   0x14

#define  LED_ID       0x20
#define  LED_1_ID     0x21
#define  LED_2_ID     0x22
#define  LED_3_ID     0x23
#define  LED_4_ID     0x24

#define  AS_ID        0x30
#define  AS_1_ID      0x31
#define  AS_2_ID      0x32
#define  AS_3_ID      0x33
#define  AS_4_ID      0x34

#define  DS_ID        0x40
#define  DS_1_ID      0x41
#define  DS_2_ID      0x42
#define  DS_3_ID      0x43
#define  DS_4_ID      0x44

#define  ULTRASNIO_ID 0x50

#define  ANGLE_ID     0x60
#define  ANGLE_X_ID   0x61
#define  ANGLE_Y_ID   0x62

#define  ACCEL_ID     0x70
#define  ACCEL_X_ID   0x71
#define  ACCEL_Y_ID   0x72

#define  CMD_FORWARD    0x01
#define  CMD_BACKWARD   0x02
#define  CMD_LEFT       0x03
#define  CMD_RIGHT      0x04
#define  CMD_STOP       0x05
#define  CMD_DECELERATE 0x07
#define  CMD_ACCELERATE 0x08


extern QImage Mat2QImage(const Mat &mat);
extern Mat QImage2Mat(const QImage & image);
#endif // COMMON_H

#include "videolabel.h"
#include "common.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>

videoLabel::videoLabel(QLabel *parent):QLabel(parent)
{

}

videoLabel::~videoLabel()
{

}

void videoLabel::paintEvent(QPaintEvent *pe)
{
    QLabel::paintEvent(pe) ;
    QPainter painter(this) ;
    painter.drawText(QPoint(0,30),"text");
    return ;

}
/*
*********************************************************************************************************
*	函 数 名: mousePressEvent
*	功能说明: 鼠标按下事件函数
*	形    参:
*           event: 指向QMouseEvent的指针
*	返 回 值: 无
*********************************************************************************************************
*/
void videoLabel::mousePressEvent(QMouseEvent *me)
{

        if(me->button() == Qt::LeftButton)
        {
            origin = Point(me->x(), me->y());
            selection = Rect(me->x(), me->y(), 0, 0);
            selectObject = true;
         }

}
/*
*********************************************************************************************************
*	函 数 名: mouseReleaseEvent
*	功能说明: 鼠标释放事件函数
*	形    参:
*           event: 指向QMouseEvent的指针
*	返 回 值: 无
*********************************************************************************************************
*/
void videoLabel::mouseReleaseEvent(QMouseEvent *me)
{
    if(me->button() == Qt::RightButton)
    {
        selectObject = false;
        if( selection.width > 0 && selection.height > 0 )
            trackObject = -1;
     }
}
/*
*********************************************************************************************************
*	函 数 名: mouseMoveEvent
*	功能说明: 鼠标移动事件函数
*	形    参:
*           event: 指向QMouseEvent的指针
*	返 回 值: 无
*********************************************************************************************************
*/
void videoLabel::mouseMoveEvent(QMouseEvent *me)
{
    if( selectObject )
    {
        selection.x = MIN(me->x(), origin.x);
        selection.y = MIN(me->y(), origin.y);
        selection.width = std::abs(me->x() - origin.x);
        selection.height = std::abs(me->y() - origin.y);

        selection &= Rect(0, 0, image.cols, image.rows);
    }
}
/*
*********************************************************************************************************
*	函 数 名: trackSelectObject
*	功能说明: 追踪所选中的目标
*	形    参:
*	返 回 值: 无
*********************************************************************************************************
*/
int videoLabel::trackSelectObject()
{
    Rect trackWindow;
    int hsize = 16;
    float hranges[] = {0,180};
    const float* phranges = hranges;

    const QPixmap *pixmap = this->pixmap();
    const QImage *qImage = &(pixmap->toImage());

    Mat frame, hsv, hue, mask, hist, backproj;
    IplImage *iplframe = &frame.operator IplImage();
    bool paused = false;

    for(;;)
    {

        qImageToIplImage(qImage, iplframe);
        if( frame.empty() )
            break;


        frame.copyTo(image);

        {   //为了减少光照变化对目标跟踪的影响，首先将图像从RGB颜色空间转换到HSV颜色空间
            cvtColor(image, hsv, COLOR_BGR2HSV);

        if( trackObject )
        {
            int _vmin = vmin, _vmax = vmax;

            inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
                    Scalar(180, 256, MAX(_vmin, _vmax)), mask);
            int ch[] = {0, 0};
            hue.create(hsv.size(), hsv.depth());
            //将hsv图片中的第一列复制到hue图片的第一列,即提取亮度
            mixChannels(&hsv, 1, &hue, 1, ch, 1);

            if( trackObject < 0 )
            {   //对H分量进行直方图统计，直方图代表了不同H分量取值出现的概率，或者说可以据此查找出H分量的大小为x时的概率或像素个数，即得到颜色概率查找表；
                Mat roi(hue, selection), maskroi(mask, selection);
                calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
                normalize(hist, hist, 0, 255, CV_MINMAX);

                trackWindow = selection;
                trackObject = 1;
            }
            //计算灰度图的反向投影，得到颜色概率查找表；
            calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
            backproj &= mask;
            //camShift算法
            RotatedRect trackBox = CamShift(backproj, trackWindow,
                                TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
            if( trackWindow.area() <= 1 )
            {
                int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
                trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
                                   trackWindow.x + r, trackWindow.y + r) &
                              Rect(0, 0, cols, rows);
            }

            if( backprojMode )
                cvtColor( backproj, image, COLOR_GRAY2BGR );
            //将trackBox画出来
//            ellipse( image, trackBox, Scalar(0,0,255), 3, CV_AA );
//            int x = trackBox.boundingRect().x ;
//            int y = trackBox.boundingRect().y ;
//            int width = trackBox.boundingRect().width;
//            int height = trackBox.boundingRect().height;
//            QPainter painter;
//            QPen pen = painter.pen();
//            pen.setColor(Qt::red);
//            painter.drawEllipse(QRect(x,y,width,height));
        }
    }
    else if( trackObject < 0 )
        paused = false;

    //如果选择了感兴趣区域，则负片表示选中的区域
    if( selectObject && selection.width > 0 && selection.height > 0 )
    {
        Mat roi(image, selection);
        bitwise_not(roi, roi);
    }

    imshow( "CamShift Demo", image );
    }
}


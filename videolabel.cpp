#include "videolabel.h"
#include "common.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QTime>
#include <QDebug>

#define   VMIN  10
#define   VMAX  256
#define   SMIN  50

videoLabel::videoLabel(QWidget *parent):QLabel(parent),
    selectObject(false),
    trackObject(0)
{

}

videoLabel::~videoLabel()
{
}
/*
*********************************************************************************************************
*	函 数 名: paintEvent
*	功能说明: 重绘事件函数
*	形    参:
*           pe: 指向QPaintEvent的指针
*	返 回 值: 无
*********************************************************************************************************
*/
void videoLabel::paintEvent(QPaintEvent *pe)
{
    QLabel::paintEvent(pe) ;

    QPainter painter(this) ;
    QPen pen = painter.pen();
    pen.setColor(Qt::red);
    QFont font = painter.font();
    font.setBold(true);
    font.setPixelSize(20);
    painter.setPen(pen);
    painter.setFont(font);

    //display current time on the videolabel
    QString str=(QTime::currentTime()).toString();
    painter.drawText(QPoint(0,30), str);

    //如果选择了感兴趣区域，则用红色矩形画出选中区域
    if( selectObject && selection.width > 0 && selection.height > 0 )
    {
        painter.drawRect(selection.x, selection.y,
                         selection.width, selection.height);
    }
    if(trackBox.size.width >0 && trackBox.size.height > 0)
    {
        //将trackBox画出来
//        ellipse( image, trackBox, Scalar(0,0,255), 3, CV_AA );
        int x = trackBox.boundingRect().x ;
        int y = trackBox.boundingRect().y ;
        int width = trackBox.boundingRect().width;
        int height = trackBox.boundingRect().height;
        painter.drawEllipse(QRect(x,y,width,height));
    }
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
#ifdef  USE_DEBUG
            qDebug()<<"x"<<selection.x
                   <<"y"<<selection.y;
#endif
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
    if(me->button() == Qt::LeftButton)
    {
        selectObject = false;
        if( selection.width > 0 && selection.height > 0 )
            trackObject = -1;
#ifdef  USE_DEBUG
            qDebug()<<"mouse Release";
#endif
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

//        selection &= Rect(0, 0, image.cols, image.rows);
#ifdef  USE_DEBUG
        qDebug()<<"width"<<selection.width
               <<"height"<<selection.height;
#endif
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
void videoLabel::trackSelectObject()
{

    int hsize = 16;
    float hranges[] = {0,180};
    const float* phranges = hranges;

    const QPixmap *pixmap = this->pixmap();
    const QImage qImage = pixmap->toImage();
#ifdef USE_DEBUG
    qDebug()<<qImage.format();
#endif
    Mat hsv, hue, mask;

    image = Mat(qImage.height(), qImage.width(), CV_8UC4,
                (void*)qImage.constBits(), qImage.bytesPerLine());

        if( image.empty() ){
            qDebug()<<"frame convertion failure";
            return;
        }
        {   //为了减少光照变化对目标跟踪的影响，首先将图像从RGB颜色空间转换到HSV颜色空间
            cvtColor(image, hsv, COLOR_BGR2HSV);

            if( trackObject )
            {
                int _vmin = VMIN, _vmax = VMAX;

                inRange(hsv, Scalar(0, SMIN, MIN(_vmin,_vmax)),
                        Scalar(180, 256, MAX(_vmin, _vmax)), mask);
                int ch[] = {0, 0};
                hue.create(hsv.size(), hsv.depth());
                //将hsv图片中的第一列复制到hue图片的第一列,即提取亮度
                mixChannels(&hsv, 1, &hue, 1, ch, 1);

                if( trackObject < 0 )
                {   //对H分量进行直方图统计，直方图代表了不同H分量取值出现的概率
                    //或者说可以据此查找出H分量的大小为x时的概率或像素个数，即得到颜色概率查找表；
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
                trackBox = CamShift(backproj, trackWindow,
                                    TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
                if( trackWindow.area() <= 1 )
                {
                    int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
                    trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
                                       trackWindow.x + r, trackWindow.y + r) &
                                  Rect(0, 0, cols, rows);
                }
             }
        }
}
//get the current image(mat format)
Mat videoLabel::getImage()
{
    if(!image.empty())
        return image;
    else
        return Mat();
}
//


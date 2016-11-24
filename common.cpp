#include "common.h"
#include <QImage>
#include <QDebug>
using namespace cv;




/*
*********************************************************************************************************
*	函 数 名: Mat2QImage
*	功能说明: 将一幅图片从Mat数据类型转换成QImage
*	形    参:
*           mat:Mat图片
*	返 回 值: QImage
*********************************************************************************************************
*/
QImage Mat2QImage(const Mat &mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        Mat mat_rgb;
        //convert BGR to RGB
        cvtColor(mat, mat_rgb, CV_BGR2RGB);
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat_rgb.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat_rgb.cols, mat_rgb.rows,
                     mat_rgb.step, QImage::Format_RGB888);
        return image;
    }
    else if(mat.type() == CV_8UC4)
    {
#ifdef USE_DEBUG
        qDebug() << "CV_8UC4";
#endif
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else{
#ifdef USE_DEBUG
        qDebug() << "ERROR: Mat could not be converted to QImage.";
#endif
        return QImage();
    }
}
/*
*********************************************************************************************************
*	函 数 名: QImage2Mat
*	功能说明: 将一幅图片从QImage数据类型转换成Mat
*	形    参:
*           image: QImage 类型图片数据
*	返 回 值: Mat
*********************************************************************************************************
*/
Mat QImage2Mat(const QImage & image)
{
    Mat mat;
#ifdef USE_DEBUG
    qDebug() << image.format();
#endif
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;

}



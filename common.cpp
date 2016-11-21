﻿#include "common.h"
using namespace cv;


inline int align(int size, int align)
{
    return (size + align - 1) & -align;
}
/*
*********************************************************************************************************
*	函 数 名: qImageToIplImage
*	功能说明: 将一幅图片从QImage数据类型转换成IplImage
*	形    参:
*           qImage: 指向QImage的指针
*           iplImageBuffer: 指向IplImage的指针
*	返 回 值: 无
*********************************************************************************************************
*/
void qImageToIplImage(const QImage * qImage,IplImage *iplImageBuffer)
{
    int width = qImage->width();
    int height = qImage->height();

    for (int y = 0; y < height; ++y)
    {
        quint8* charTemp=(quint8*)(iplImageBuffer->imageData + y * iplImageBuffer->widthStep);
        for (int x = 0; x < width; ++x)
        {
            charTemp[3*x+0] = (unsigned char) qBlue(qImage->pixel(x, y));//取B通道数据
            charTemp[3*x+1] = (unsigned char) qGreen(qImage->pixel(x, y));//取G通道数据
            charTemp[3*x+2] = (unsigned char) qRed(qImage->pixel(x, y));//取R通道数据
        }
    }
}
/*
*********************************************************************************************************
*	函 数 名: IplImageToQImage
*	功能说明: 将一幅图片从IplImage数据类型转换成QImage
*	形    参:
*           iplImage: 指向IplImage的指针
*           qimage:   QImage的调色板
*           qImageBuffer: 指向QImage的指针
*           mini:
*           maxi:
*	返 回 值: 无
*********************************************************************************************************
*/
QImage IplImageToQImage(const IplImage * iplImage, quint8 *qImageBuffer,
                        double mini, double maxi)
{

    int width = iplImage->width;

/* Note here that OpenCV image is stored so that each lined is
32-bits aligned thus explaining the necessity to "skip" the few last bytes of each
line of OpenCV image buffer.*/
    int widthStep = iplImage->widthStep;
    int height = iplImage->height;

    switch (iplImage->depth)
    {
     case IPL_DEPTH_8U:
            if (iplImage->nChannels == 1){
                /* OpenCV image is stored with one byte grey pixel. We convert it
                to an 8 bit depth QImage.
                */
                quint8 *qImagePtr = qImageBuffer;
                const quint8 *iplImagePtr = (const quint8*) iplImage->imageData;

                for (int y = 0; y < height; y++)
                {
                    // Copy line by line
                    memcpy(qImagePtr, iplImagePtr, width);
                    qImagePtr += width;
                    iplImagePtr += widthStep;
                }
            }
            else
                if (iplImage->nChannels == 3)
                {
                    /* OpenCV image is stored with 3 byte color pixels (3 channels).
                    We convert it to a 32 bit depth QImage.
                    */
                    quint8 *qImagePtr = qImageBuffer;
                    const quint8 *iplImagePtr = (const quint8 *) iplImage->imageData;
                    for (int y = 0; y < height; y++)
                    {
                        for (int x = 0; x < width; x++)
                        {
                            // We cannot help but copy manually.
                            qImagePtr[0] = iplImagePtr[0];
                            qImagePtr[1] = iplImagePtr[1];
                            qImagePtr[2] = iplImagePtr[2];
                            qImagePtr[3] = 0;

                            qImagePtr += 4;
                            iplImagePtr += 3;
                        }
                        iplImagePtr += widthStep-3*width;
                    }
                }
            else{
                qDebug("IplImageToQImage: image format is not supported : "
                       "depth=8U and %d channels\n", iplImage->nChannels);
            }
      break;

    case IPL_DEPTH_16U:
         if (iplImage->nChannels == 1){
            /* OpenCV image is stored with 2 bytes grey pixel. We convert it
            to an 8 bit depth QImage.
            */
            quint8 *qImagePtr = qImageBuffer;
            const quint16 *iplImagePtr = (const quint16 *)iplImage->imageData;
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                // We take only the highest part of the 16 bit value. It is
                //similar to dividing by 256.
                   *qImagePtr++ = ((*iplImagePtr++) >> 8);
                }
                iplImagePtr += widthStep/sizeof(quint16)-width;
            }
        }
        else{
            qDebug("IplImageToQImage: image format is not supported : "
                   "depth=16U and %d channels\n", iplImage->nChannels);
        }
     break;

   case IPL_DEPTH_32F:
         if (iplImage->nChannels == 1){
            /* OpenCV image is stored with float (4 bytes) grey pixel. We
            convert it to an 8 bit depth QImage.
            */
             quint8 *qImagePtr = qImageBuffer;
             const float *iplImagePtr = (const float *) iplImage->imageData;
             for (int y = 0; y < height; y++)
             {
                 for (int x = 0; x < width; x++)
                 {
                     uchar p;
                     float pf = 255 * ((*iplImagePtr++) - mini) / (maxi - mini);
                     if (pf < 0) p = 0;
                     else if (pf > 255) p = 255;
                     else p = (uchar) pf;

                     *qImagePtr++ = p;
                  }
                iplImagePtr += widthStep/sizeof(float)-width;
             }
         }
         else{
             qDebug("IplImageToQImage: image format is not supported : "
                    "depth=32F and %d channels\n", iplImage->nChannels);
         }
     break;

   case IPL_DEPTH_64F:
         if (iplImage->nChannels == 1){
            /* OpenCV image is stored with double (8 bytes) grey pixel. We
            convert it to an 8 bit depth QImage.
            */
            quint8 *qImagePtr = qImageBuffer;
            const double *iplImagePtr = (const double *) iplImage->imageData;
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    uchar p;
                    double pf = 255 * ((*iplImagePtr++) - mini) / (maxi - mini);

                    if (pf < 0) p = 0;
                    else if (pf > 255) p = 255;
                    else p = (uchar) pf;

                    *qImagePtr++ = p;
                }
                iplImagePtr += widthStep/sizeof(double)-width;
            }
        }
        else{
            qDebug("IplImageToQImage: image format is not supported : "
                   "depth=64F and %d channels\n", iplImage->nChannels);
        }
    break;
  default:
        qDebug("IplImageToQImage: image format is not supported :"
               " depth=%d and %d channels\n", iplImage->depth, iplImage->nChannels);
    }//end of switch

    QImage qImage(qImageBuffer, width, height, QImage::Format_RGB888);
    QVector<QRgb> vcolorTable;
    if (iplImage->nChannels == 1)
    {
        // We should check who is going to destroy this allocation.
        QRgb *colorTable = new QRgb[256];
        for (int i = 0; i < 256; i++)
        {
           colorTable[i] = qRgb(i, i, i);
           vcolorTable[i] = colorTable[i];
        }
        delete colorTable;
        qImage.setColorTable(vcolorTable);
    }

    return qImage;

}
static void onMouse( int event, int x, int y, int, void* )
{
    if( selectObject )
    {
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x);
        selection.height = std::abs(y - origin.y);

        selection &= Rect(0, 0, image.cols, image.rows);
    }

    switch( event )
    {
    case CV_EVENT_LBUTTONDOWN:
        origin = Point(x,y);
        selection = Rect(x,y,0,0);
        selectObject = true;
        break;
    case CV_EVENT_LBUTTONUP:
        selectObject = false;
        if( selection.width > 0 && selection.height > 0 )
            trackObject = -1;
        break;
    }
}

VideoCapture cap;
Rect trackWindow;
int hsize = 16;
float hranges[] = {0,180};
const float* phranges = hranges;
CommandLineParser parser(argc, argv, keys);
int camNum = parser.get<int>("1");

cap.open(camNum);

if( !cap.isOpened() )
{
    help();
    cout << "***Could not initialize capturing...***\n";
    cout << "Current parameter's value: \n";
    parser.printParams();
    return -1;
}

namedWindow( "Histogram", 0 );
namedWindow( "CamShift Demo", 0 );
setMouseCallback( "CamShift Demo", onMouse, 0 );
createTrackbar( "Vmin", "CamShift Demo", &vmin, 256, 0 );
createTrackbar( "Vmax", "CamShift Demo", &vmax, 256, 0 );
createTrackbar( "Smin", "CamShift Demo", &smin, 256, 0 );

Mat frame, hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
bool paused = false;

for(;;)
{
    if( !paused )
    {
        cap >> frame;
        if( frame.empty() )
            break;
    }

    frame.copyTo(image);

    if( !paused )
    {
        cvtColor(image, hsv, COLOR_BGR2HSV);

        if( trackObject )
        {
            int _vmin = vmin, _vmax = vmax;

            inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
                    Scalar(180, 256, MAX(_vmin, _vmax)), mask);
            int ch[] = {0, 0};
            hue.create(hsv.size(), hsv.depth());
            mixChannels(&hsv, 1, &hue, 1, ch, 1);

            if( trackObject < 0 )
            {
                Mat roi(hue, selection), maskroi(mask, selection);
                calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
                normalize(hist, hist, 0, 255, CV_MINMAX);

                trackWindow = selection;
                trackObject = 1;

                histimg = Scalar::all(0);
                int binW = histimg.cols / hsize;
                Mat buf(1, hsize, CV_8UC3);
                for( int i = 0; i < hsize; i++ )
                    buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
                cvtColor(buf, buf, CV_HSV2BGR);

                for( int i = 0; i < hsize; i++ )
                {
                    int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
                    rectangle( histimg, Point(i*binW,histimg.rows),
                               Point((i+1)*binW,histimg.rows - val),
                               Scalar(buf.at<Vec3b>(i)), -1, 8 );
                }
            }

            calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
            backproj &= mask;
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
            ellipse( image, trackBox, Scalar(0,0,255), 3, CV_AA );
        }
    }
    else if( trackObject < 0 )
        paused = false;

    if( selectObject && selection.width > 0 && selection.height > 0 )
    {
        Mat roi(image, selection);
        bitwise_not(roi, roi);
    }

    imshow( "CamShift Demo", image );
    imshow( "Histogram", histimg );

    char c = (char)waitKey(10);
    if( c == 27 )
        break;
    switch(c)
    {
    case 'b':
        backprojMode = !backprojMode;
        break;
    case 'c':
        trackObject = 0;
        histimg = Scalar::all(0);
        break;
    case 'h':
        showHist = !showHist;
        if( !showHist )
            destroyWindow( "Histogram" );
        else
            namedWindow( "Histogram", 1 );
        break;
    case 'p':
        paused = !paused;
        break;
    default:
        ;
    }
}

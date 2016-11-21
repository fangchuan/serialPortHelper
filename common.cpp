#include "common.h"

inline int align(int size, int align)
{
    return (size + align - 1) & -align;
}

void QImageToIplImage(const QImage * qImage,IplImage *charIplImageBuffer)
{
    int width = qImage->width();
    int height = qImage->height();

    for (int y = 0; y < height; ++y)
    {
        unsigned char* charTemp=(unsigned char*)(charIplImageBuffer->imageData+y*charIplImageBuffer->widthStep);
        for (int x = 0; x < width; ++x)
        {
            charTemp[3*x+0] = (unsigned char) qBlue(qImage->pixel(x, y));//取B通道数据
            charTemp[3*x+1] = (unsigned char) qGreen(qImage->pixel(x, y));//取G通道数据
            charTemp[3*x+2] = (unsigned char) qRed(qImage->pixel(x, y));//取R通道数据
        }
    }
}


void IplImageToQImage(const IplImage * iplImage,QImage *qimage,uchar *qImageBuffer,double mini, double maxi)
{
//    uchar *qImageBuffer = NULL;

    int width = iplImage->width;

/* Note here that OpenCV image is stored so that each lined is
32-bits aligned thus
* explaining the necessity to "skip" the few last bytes of each
line of OpenCV image buffer.
*/
    int widthStep = iplImage->widthStep;
    int height = iplImage->height;

    switch (iplImage->depth)
    {
        case IPL_DEPTH_8U:
        if (iplImage->nChannels == 1)
        {
        /* OpenCV image is stored with one byte grey pixel. We convert it
        to an 8 bit depth QImage.
        */

        uchar *QImagePtr = qImageBuffer;
        const uchar *iplImagePtr = (const uchar *) iplImage->imageData;

        for (int y = 0; y < height; y++)
        {
            // Copy line by line
            memcpy(QImagePtr, iplImagePtr, width);
            QImagePtr += width;
            iplImagePtr += widthStep;
        }

        }
        else if (iplImage->nChannels == 3)
        {
            /* OpenCV image is stored with 3 byte color pixels (3 channels).
            We convert it to a 32 bit depth QImage.
            */
            uchar *QImagePtr = qImageBuffer;
            const uchar *iplImagePtr = (const uchar *) iplImage->imageData;
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    // We cannot help but copy manually.
                    QImagePtr[0] = iplImagePtr[0];
                    QImagePtr[1] = iplImagePtr[1];
                    QImagePtr[2] = iplImagePtr[2];
                    QImagePtr[3] = 0;

                    QImagePtr += 4;
                    iplImagePtr += 3;
                }
            iplImagePtr += widthStep-3*width;
            }

        }
        else
        {
            qDebug("IplImageToQImage: image format is not supported : depth=8U and %d channels\n", iplImage->nChannels);
        }
        break;
        case IPL_DEPTH_16U:
        if (iplImage->nChannels == 1)
        {
        /* OpenCV image is stored with 2 bytes grey pixel. We convert it
        to an 8 bit depth QImage.
        */
            uchar *QImagePtr = qImageBuffer;
            const unsigned int *iplImagePtr = (const unsigned int *)iplImage->imageData;
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                // We take only the highest part of the 16 bit value. It is
                //similar to dividing by 256.
                *QImagePtr++ = ((*iplImagePtr++) >> 8);
                }
                iplImagePtr += widthStep/sizeof(unsigned int)-width;
            }
        }
        else
        {
            qDebug("IplImageToQImage: image format is not supported : depth=16U and %d channels\n", iplImage->nChannels);

        }
        break;
        case IPL_DEPTH_32F:
         if (iplImage->nChannels == 1)
         {
        /* OpenCV image is stored with float (4 bytes) grey pixel. We
        convert it to an 8 bit depth QImage.
        */
             uchar *QImagePtr = qImageBuffer;
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

                     *QImagePtr++ = p;
                  }
             iplImagePtr += widthStep/sizeof(float)-width;
             }
         }
         else
         {
             qDebug("IplImageToQImage: image format is not supported : depth=32F and %d channels\n", iplImage->nChannels);
         }
       break;
       case IPL_DEPTH_64F:
         if (iplImage->nChannels == 1)
         {
            /* OpenCV image is stored with double (8 bytes) grey pixel. We
            convert it to an 8 bit depth QImage.
            */
            uchar *QImagePtr = qImageBuffer;
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

                    *QImagePtr++ = p;
                }
                iplImagePtr += widthStep/sizeof(double)-width;
            }
        }
        else
        {
            qDebug("IplImageToQImage: image format is not supported : depth=64F and %d channels\n", iplImage->nChannels);
        }
        break;
        default:
        qDebug("IplImageToQImage: image format is not supported : depth=%d and %d channels\n", iplImage->depth, iplImage->nChannels);
    }//end of switch

    QImage *qImage;
    qImage=qimage;
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
        qImage->setColorTable(vcolorTable);
    }
    else
    {

    }

}

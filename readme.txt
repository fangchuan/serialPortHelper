
需求分析：1、可以接收串口数据并显示成字符串、曲线----serial qwt
          2、可以接收网络摄像头视频，并实现视频存储和照片存储，并实现相关简单控制指令下发----tcpsocket VideoCaptrue  File
          3、实现目标跟踪，上位机可选取跟踪目标，下位机根据接收到的目标坐标实现跟踪----camShift
          4、Google地图指定航点

2016-11-20:这种MJPG格式的摄像头输出的还是RGB，但是Mat类存储的是BGR,所以最终赋给qt显示要进行BGR2RGB
2016-11-21:MJPEGStream使用得是8080端口，其他串口数据传输得另开端口，所以射了两个socket
           拍照和保存功能实现，下一步考虑如何转换图片数据  QImage-->IplImage/Mat-->QImage
           添加camShift

2016-11-23:只需要QImage2IplImage，在Mat上做ObjectTrack工作，同时在videoLabel上对QImage画出追踪的物体,无需再IplImage2QImage

2016-11-24:完成videoLabel的提升和重绘操作
           QImage2IplImage有问题,

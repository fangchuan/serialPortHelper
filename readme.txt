
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
           QImage2IplImage有问题,已替换
           camShift基本功能实现，明天考虑调阈值VMIN VMAX SMIN

2017-1-6: 得修改程序，不能找不到摄像头就一直挂在那

2017-1-12: 修改找不到摄像头就一直跳出warning对话框。
	   完成videoCilent视频流接收。
	   定时器方法读取video_stream方法不可取，会出现JPEG图片帧不完整的问题，仍采用readyRead信号槽方式接收video_stream
	   JPEG data仍然不完整，查看mjpeg_streamer源码

2017-1-13: 使用webView加载了简单的网页，但是还出现LEAK 警告

2017-1-16: 实现离线加载百度地图，但是定位是在北京，
           如果想定位在南京大学，必须重新制作离线地图。
           可以通过百度地图API密钥在线加载百度地图

2017-1-17: 可以保存视频，但是很多格式系统不支持，需要手动选择系统支持的视频编码格式
           经验证，fourcc只能支持Intel IYUV编码和Microsoftcode和全帧不压缩
           明天测试dataCilent

2017-1-20: videoShow 改名为wifiConnect，并修复几个bug,vw指针使用不当.
           录制按钮加上图片，录制/暂停两种状态。
           增加了WiFi连接时主窗口的statuBar显示
修改wifiConnect.ui，既要有视频显示框，又要有数据显示框及不同类型数据的显示控件

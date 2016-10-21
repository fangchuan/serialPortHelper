#include "showwave.h"
#include "ui_showwave.h"
#include <qwt_symbol.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_legend.h>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QMessageBox>
#include <QDebug>



#define  CH1_CURVE_COLOR  Qt::blue
#define  CH2_CURVE_COLOR  Qt::red
#define  CH3_CURVE_COLOR  Qt::green
#define  CH4_CURVE_COLOR  Qt::black
#define  CH_PEN_WIDTH     2

#define  CH1_VALUE        1
#define  CH2_VALUE        2
#define  CH3_VALUE        3
#define  CH4_VALUE        4

//QList<QByteArray> baList;


ShowWave::ShowWave(QWidget *parent) :QMainWindow(parent),ui(new Ui::ShowWave),
    id_timer(0),time(0),
    _flag(FLAG_CLEAR),
    recv_data(new QString()),
    ch1_curve(NULL),
    ch2_curve(NULL),
    ch3_curve(NULL),
    ch4_curve(NULL),
    grid(NULL)
{
    ui->setupUi(this);
    this->setWindowTitle("Show Wave");

    ui->ch1CheckBox->setChecked(true);

    initWavePlot();

    initCurves(CH1_VALUE);

    connect(ui->ch1CheckBox, SIGNAL(clicked()), this, SLOT(showOrHideCurve1()));
    connect(ui->ch2CheckBox, SIGNAL(clicked()), this, SLOT(showOrHideCurve2()));
    connect(ui->ch3CheckBox, SIGNAL(clicked()), this, SLOT(showOrHideCurve3()));
    connect(ui->ch4CheckBox, SIGNAL(clicked()), this, SLOT(showOrHideCurve4()));

//    baList<< "Yaw: 0.01"
//          << "Yaw: -0.98"
//          << "Yaw: 0.76"
//          << "Yaw: 0.87"
//          << "Yaw: 0.45"
//          << "Yaw: 0.99"
//          << "Yaw: 0.95"
//          << "Yaw: 0.55"
//          << "Yaw: 1.01"
//          <<"Yaw:1.20";
}

ShowWave::~ShowWave()
{
    delete recv_data;
//    delete grid;
//    delete ch1_curve;
//    delete ch2_curve;
//    delete ch3_curve;
//    delete ch4_curve;

    delete ui;
    qDebug()<<"The showwave window is destoryed!!!";
}

void ShowWave::on_showWaveBtn_clicked()
{   //如果之前的操作是清除操作,则重新启动一个定时器
    if(_flag == FLAG_CLEAR){
        //启动定时器，100ms响应，用于模拟产生实时数据
        id_timer = this->startTimer(100);

        if(ch1_curve && ui->ch1CheckBox->isChecked())
            ch1_curve->setVisible(true);
        if(ch2_curve && ui->ch2CheckBox->isChecked())
            ch2_curve->setVisible(true);
        if(ch3_curve && ui->ch3CheckBox->isChecked())
            ch3_curve->setVisible(true);
        if(ch4_curve && ui->ch4CheckBox->isChecked())
            ch4_curve->setVisible(true);

        ui->wavePlot->replot();
    }

    _flag = FLAG_SHOW;
//    for(int i=0; i<10;i++)
//        putData(baList[i]);
}

void ShowWave::on_clearWaveBtn_clicked()
{
    ch_x_data.clear();
    ch1_y_data.clear();
    ch2_y_data.clear();
    ch3_y_data.clear();
    ch4_y_data.clear();

    if(ch1_curve && ch1_curve->isVisible())
        ch1_curve->setVisible(false);
    if(ch2_curve && ch2_curve->isVisible())
        ch2_curve->setVisible(false);
    if(ch3_curve && ch3_curve->isVisible())
        ch3_curve->setVisible(false);
    if(ch4_curve && ch4_curve->isVisible())
        ch4_curve->setVisible(false);

    ui->wavePlot->replot();
    ui->ch1CheckBox->setChecked(false);
    ui->ch2CheckBox->setChecked(false);
    ui->ch3CheckBox->setChecked(false);
    ui->ch4CheckBox->setChecked(false);
    //如果前一个操作不是清零操作,则停掉定时器并清零计数
    if(_flag != FLAG_CLEAR){
        this->killTimer(id_timer);
        time = 0;
    }

    _flag = FLAG_CLEAR;
}

void ShowWave::on_drawGridBtn_clicked()
{
    if(!grid){
        grid = new QwtPlotGrid();
        grid->setPen(QColor(0,0,0),1, Qt::DotLine);
        grid->attach(ui->wavePlot);

        ui->wavePlot->replot();
    }
    else
        return;
}

void ShowWave::on_pauseWaveBtn_clicked()
{
    _flag = FLAG_PAUSE;
}

void ShowWave::on_frame_ch1_editingFinished()
{
    frame_ch1_str = ui->frame_ch1->text();
    if(frame_ch1_str.isEmpty())
    {
        QMessageBox::critical(this, "Error",
                              tr("帧首不能为空"));
    }
    qDebug()<<"frame_1"<<frame_ch1_str;
}

void ShowWave::on_frame_ch2_editingFinished()
{
    frame_ch2_str = ui->frame_ch2->text();
    if(frame_ch2_str.isEmpty())
    {
        QMessageBox::critical(this, "Error",
                              tr("帧首不能为空"));
    }
    qDebug()<<"frame_2"<<frame_ch2_str;
}

void ShowWave::on_frame_ch3_editingFinished()
{
    frame_ch3_str = ui->frame_ch3->text();
    if(frame_ch3_str.isEmpty())
    {
        QMessageBox::critical(this, "Error",
                              tr("帧首不能为空"));
    }
    qDebug()<<"frame_3"<<frame_ch3_str;
}

void ShowWave::on_frame_ch4_editingFinished()
{
    frame_ch4_str = ui->frame_ch4->text();
    if(frame_ch4_str.isEmpty())
    {
        QMessageBox::critical(this, "Error",
                              tr("帧首不能为空"));
    }
    qDebug()<<"frame_4"<<frame_ch4_str;
}

void ShowWave::showOrHideCurve1()
{
    if(ch1_curve){
        if(ui->ch1CheckBox->isChecked())
            ch1_curve->setVisible(true);
        else
            ch1_curve->setVisible(false);

        ui->wavePlot->replot();
    }
    else{
        initCurves(CH1_VALUE);
     }
}

void ShowWave::showOrHideCurve2()
{
    if(ch2_curve){
        if(ui->ch2CheckBox->isChecked())
            ch2_curve->setVisible(true);
        else
            ch2_curve->setVisible(false);

        ui->wavePlot->replot();
    }
    else{
        initCurves(CH2_VALUE);
     }
}

void ShowWave::showOrHideCurve3()
{
    if(ch3_curve){
        if(ui->ch3CheckBox->isChecked())
            ch3_curve->setVisible(true);
        else
            ch3_curve->setVisible(false);

        ui->wavePlot->replot();
    }
    else{
       initCurves(CH3_VALUE);
    }
}

void ShowWave::showOrHideCurve4()
{
    if(ch4_curve){
        if(ui->ch4CheckBox->isChecked())
            ch4_curve->setVisible(true);
        else
            ch4_curve->setVisible(false);

        ui->wavePlot->replot();
    }
    else{
        initCurves(CH4_VALUE);
    }
}
//时间更新事件
void ShowWave::timerEvent(QTimerEvent *)
{
    if(_flag == FLAG_SHOW){
        time ++;
        ch_x_data.append(time);

//        ch1_y_data.append(qrand()%10);
//        ch2_y_data.append(qrand()%10);
//        ch3_y_data.append(qrand()%10);
//        ch4_y_data.append(qrand()%10);

        //重新加载数据
        if(ch1_curve)
            ch1_curve->setSamples(ch_x_data, ch1_y_data);
        if(ch2_curve)
            ch2_curve->setSamples(ch_x_data, ch2_y_data);
        if(ch3_curve)
            ch3_curve->setSamples(ch_x_data, ch3_y_data);
        if(ch4_curve)
            ch4_curve->setSamples(ch_x_data, ch4_y_data);

        ui->wavePlot->replot();
    }
//    qDebug()<<"Enter timer event";
}

void ShowWave::closeEvent(QCloseEvent *)
{
    //reset all configuration
    this->killTimer(id_timer);
    time = 0;
    _flag = FLAG_CLEAR;


    ch_x_data.clear();
    ch1_y_data.clear();
    ch2_y_data.clear();
    ch3_y_data.clear();
    ch4_y_data.clear();

    ui->ch1CheckBox->setChecked(false);
    ui->ch2CheckBox->setChecked(false);
    ui->ch3CheckBox->setChecked(false);
    ui->ch4CheckBox->setChecked(false);
    qDebug()<<"The waveshow window is closed.";
}

//初始化plot
void ShowWave::initWavePlot(){
    ui->wavePlot->setTitle("MultiChannels Plot");
    ui->wavePlot->setCanvasBackground( Qt::white );
//    ui->wavePlot->setAutoDelete(true);
    //设置坐标轴的名称
    ui->wavePlot->setAxisTitle(QwtPlot::xBottom, "X->");
    ui->wavePlot->setAxisTitle(QwtPlot::yLeft, "Y->");
    //设置X轴不自动调节，Y轴自动调节
    ui->wavePlot->setAxisAutoScale(QwtPlot::xBottom,true);
    ui->wavePlot->setAxisAutoScale(QwtPlot::yLeft, true);
    //设置图例在右边
    ui->wavePlot->insertLegend(new QwtLegend, QwtPlot::RightLegend);
    ui->wavePlot->insertLegend(new QwtLegend, QwtPlot::RightLegend);
    ui->wavePlot->insertLegend(new QwtLegend, QwtPlot::RightLegend);
    ui->wavePlot->insertLegend(new QwtLegend, QwtPlot::RightLegend);
    //使用滚轮放大/缩小
    (void) new QwtPlotMagnifier( ui->wavePlot->canvas());
     //使用鼠标左键平移
    (void) new QwtPlotPanner( ui->wavePlot->canvas());
}
//构造曲线数据
void ShowWave::initCurves(quint8 chx){

    switch(chx)
    {
    case CH1_VALUE:
        ch1_curve = new QwtPlotCurve("CH1");
        ch1_curve->setStyle(QwtPlotCurve::Lines);//直线形式
        ch1_curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        ch1_curve->setCurveAttribute(QwtPlotCurve::Fitted, true);//是曲线更光滑
        ch1_curve->setPen(QPen(CH1_CURVE_COLOR, CH_PEN_WIDTH));//设置画笔
        ch1_curve->setSamples(ch_x_data, ch1_y_data);
        ch1_curve->attach(ui->wavePlot);//把曲线附加到plot上
        break;
    case CH2_VALUE:
        ch2_curve = new QwtPlotCurve("CH2");
        ch2_curve->setStyle(QwtPlotCurve::Lines);//直线形式
        ch2_curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        ch2_curve->setCurveAttribute(QwtPlotCurve::Fitted, true);//是曲线更光滑
        ch2_curve->setPen(QPen(CH2_CURVE_COLOR, CH_PEN_WIDTH));//设置画笔
        ch2_curve->setSamples(ch_x_data, ch2_y_data);
        ch2_curve->attach(ui->wavePlot);//把曲线附加到plot上
        break;
    case CH3_VALUE:
        ch3_curve = new QwtPlotCurve("CH3");
        ch3_curve->setStyle(QwtPlotCurve::Lines);//直线形式
        ch3_curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        ch3_curve->setCurveAttribute(QwtPlotCurve::Fitted, true);//是曲线更光滑
        ch3_curve->setPen(QPen(CH3_CURVE_COLOR, CH_PEN_WIDTH));//设置画笔
        ch3_curve->setSamples(ch_x_data, ch3_y_data);
        ch3_curve->attach(ui->wavePlot);//把曲线附加到plot上
        break;
    case CH4_VALUE:
        ch4_curve = new QwtPlotCurve("CH4");
        ch4_curve->setStyle(QwtPlotCurve::Lines);//直线形式
        ch4_curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        ch4_curve->setCurveAttribute(QwtPlotCurve::Fitted, true);//是曲线更光滑
        ch4_curve->setPen(QPen(CH4_CURVE_COLOR, CH_PEN_WIDTH));//设置画笔
        ch4_curve->setSamples(ch_x_data, ch4_y_data);
        ch4_curve->attach(ui->wavePlot);//把曲线附加到plot上
        break;
    }
}
//接收从串口得到的一行数据
void ShowWave::putData(const QByteArray &data)
{

    *recv_data = QString(data).simplified();

    qDebug()<<*recv_data;

    if(!frame_ch1_str.isEmpty() && recv_data->startsWith(frame_ch1_str, Qt::CaseInsensitive))
    {
        recv_data->remove(frame_ch1_str, Qt::CaseInsensitive);
        qDebug()<<*recv_data;
        ch1_y_data.append(recv_data->toDouble());
    }
    else{
        if(!frame_ch2_str.isEmpty()&& recv_data->startsWith(frame_ch2_str, Qt::CaseInsensitive))
        {
            recv_data->remove(frame_ch2_str, Qt::CaseInsensitive);
            ch2_y_data.append(recv_data->toDouble());
        }
        else{
            if(!frame_ch3_str.isEmpty()&& recv_data->startsWith(frame_ch3_str, Qt::CaseInsensitive))
            {
                recv_data->remove(frame_ch3_str, Qt::CaseInsensitive);
                ch3_y_data.append(recv_data->toDouble());
            }
            else{
                if(!frame_ch4_str.isEmpty() && recv_data->startsWith(frame_ch4_str, Qt::CaseInsensitive))
                {
                    recv_data->remove(frame_ch4_str, Qt::CaseInsensitive);
                    ch4_y_data.append(recv_data->toDouble());
                }
            }
        }
    }
}

void ShowWave::on_loadFileBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("打开文件"),
                ".",
                ("Text File(*.txt)"));
    if(fileName.isEmpty()){
        return ;
    }
    else{
        QFile fileOut(fileName);
        if(!fileOut.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(this,"Error",tr("打开文件失败!"));
        }
        else{
            QTextStream ts(&fileOut);
            QString  strLine;
            while(!ts.atEnd()){
                ts>>strLine;//读取打头的单词
                if(strLine == "")
                {
                    ts.skipWhiteSpace();
                    continue;
                }
                if(strLine.startsWith(frame_ch1_str, Qt::CaseInsensitive))
                {
                    strLine.remove(frame_ch1_str,Qt::CaseInsensitive);
                    ch1_y_data.append(strLine.toDouble());

                    ts.readLine();
                    continue;
                }
                if(strLine.startsWith(frame_ch2_str, Qt::CaseInsensitive))
                {
                    strLine.remove(frame_ch2_str,Qt::CaseInsensitive);
                    ch2_y_data.append(strLine.toDouble());

                    ts.readLine();
                    continue;
                }
                if(strLine.startsWith(frame_ch3_str, Qt::CaseInsensitive))
                {
                    strLine.remove(frame_ch3_str,Qt::CaseInsensitive);
                    ch3_y_data.append(strLine.toDouble());

                    ts.readLine();
                    continue;
                }
                if(strLine.startsWith(frame_ch4_str, Qt::CaseInsensitive))
                {
                    strLine.remove(frame_ch4_str,Qt::CaseInsensitive);
                    ch4_y_data.append(strLine.toDouble());

                    ts.readLine();
                    continue;
                }
            }
            fileOut.close();

            QMessageBox::information(this,
                                     "Information",
                                     tr("加载成功"));
        }
    }
}

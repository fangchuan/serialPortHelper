#ifndef SHOWWAVE_H
#define SHOWWAVE_H

#include <QMainWindow>
#include <qwt_plot.h>
#include <qwt_point_data.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>

namespace Ui {
class ShowWave;
}

enum _SHOWFLAG{
    FLAG_SHOW = 0,
    FLAG_CLEAR,
    FLAG_PAUSE
};

class ShowWave : public QMainWindow
{
    Q_OBJECT


public:
    explicit ShowWave(QWidget *parent = 0);
    ~ShowWave();

    void initWavePlot();
    void initCurves(quint8 chx);
    void putData(const QByteArray &data);
private slots:
    void on_showWaveBtn_clicked();

    void on_drawGridBtn_clicked();

    void on_pauseWaveBtn_clicked();

    void on_clearWaveBtn_clicked();

    void showOrHideCurve1();
    void showOrHideCurve2();
    void showOrHideCurve3();
    void showOrHideCurve4();

    void on_frame_ch1_editingFinished();

    void on_frame_ch2_editingFinished();

    void on_frame_ch3_editingFinished();

    void on_frame_ch4_editingFinished();

private:
    Ui::ShowWave *ui;

    int id_timer;//定时器ID
    quint32 time;//计数器，用于更新plot时间轴
    _SHOWFLAG _flag;//plot的操作flag
    //4个帧首字符串
    QString frame_ch1_str;
    QString frame_ch2_str;
    QString frame_ch3_str;
    QString frame_ch4_str;
    QString *recv_data;

    QVector<double>  ch_x_data;
    QVector<double> ch1_y_data;
    QVector<double> ch2_y_data;
    QVector<double> ch3_y_data;
    QVector<double> ch4_y_data;

    QwtPlotCurve * ch1_curve;
    QwtPlotCurve * ch2_curve;
    QwtPlotCurve * ch3_curve;
    QwtPlotCurve * ch4_curve;


    QwtPlotGrid * grid;



    virtual void timerEvent(QTimerEvent* );
    virtual void closeEvent(QCloseEvent*);
};

#endif // SHOWWAVE_H

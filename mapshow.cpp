#include "mapshow.h"
#include "ui_mapshow.h"
#include <QWebFrame>
//#include <QDir>
#include <QDebug>

mapShow::mapShow(QWidget *parent) :QWidget(parent),
    ui(new Ui::mapShow)
{
    ui->setupUi(this);

    QString strMapPath="file:///";
//    QDir mapDir;
//    qDebug()<<mapDir.exists("map.html");
//    qDebug()<<mapDir.absoluteFilePath("map.html");
    strMapPath += QCoreApplication::applicationDirPath();
//    qDebug()<<strMapPath;
    strMapPath += "/map/map.html";
//    qDebug()<<strMapPath;
    QUrl url(strMapPath);
    ui->mapView->load(url);

}

mapShow::~mapShow()
{
    delete ui;
}

void mapShow::on_mapView_loadFinished(bool)
{
    QWebFrame *frame  = ui->mapView->page()->mainFrame();
    frame->evaluateJavaScript(NULL);
}

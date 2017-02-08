#ifndef MAPSHOW_H
#define MAPSHOW_H

#include <QWidget>

namespace Ui {
class mapShow;
}

class mapShow : public QWidget
{
    Q_OBJECT

public:
    explicit mapShow(QWidget *parent = 0);
    ~mapShow();

private slots:
    void on_mapView_loadFinished(bool);

private:
    Ui::mapShow *ui;
};

#endif // MAPSHOW_H

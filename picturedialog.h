#ifndef PICTUREDIALOG_H
#define PICTUREDIALOG_H

#include <QDialog>

namespace Ui {
class pictureDialog;
}

class pictureDialog : public QDialog
{
    Q_OBJECT

public:
    pictureDialog(const QPixmap &pixmap);
    ~pictureDialog();

private slots:
    void on_savePictureBtn_clicked();

private:
    Ui::pictureDialog *ui;

};

#endif // PICTUREDIALOG_H

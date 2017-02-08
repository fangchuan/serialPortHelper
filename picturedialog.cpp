#include "picturedialog.h"
#include "ui_picturedialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileDialog>

pictureDialog::pictureDialog(const QPixmap &pixmap) :
    ui(new Ui::pictureDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("save picture");
    ui->picturelabel->setPixmap(pixmap);

}

pictureDialog::~pictureDialog()
{
    delete ui;
}

void pictureDialog::on_savePictureBtn_clicked()
{
    QString fn = QFileDialog::getSaveFileName(
                this,
                tr("保存图片"),
                ".",
                ("jpeg file(*.jpg);;png file(*.png);;bmp file(*.bmp)"));
    if(fn.isEmpty()){
        return ;
    }

    const QPixmap *pixmap = ui->picturelabel->pixmap();
    if(pixmap){
        if(pixmap->save(fn)){
            QMessageBox::information(this,
                                     tr("Tip"),
                                     tr("Save Pictrue Success"));
        }
        else{
            QMessageBox::warning(this,
                                 tr("Error"),
                                 tr("Save Pictrue Fail"));
        }
    }

//    imwrite(fn,);
}

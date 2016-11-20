/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "settingsdialog.h"
#include "showwave.h"
#include "common.h"
#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QFileDialog>
#include <QFile>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    console = new Console;
    console->setEnabled(false);
    setCentralWidget(console);
    //Instantiation of serial port and
    //setting dialog ,show wave dialog
    serial = new QSerialPort(this);
    settings = new SettingsDialog;
    showWave = new ShowWave;
#ifdef  USE_SYSTRAYICON
    //initialize system tray icon
    initSysTrayIcon();
#endif
    //initialize signal and slot
    initActionsConnections();

    status = new QLabel;
    ui->statusBar->addWidget(status);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);
    ui->actionShowWave->setEnabled(true);
    ui->actionSave->setEnabled(true);
    //serial port open error handler
    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MainWindow::handleError);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(console, &Console::getData, this, &MainWindow::writeData);
}


MainWindow::~MainWindow()
{
    delete settings;
    delete showWave;
    delete ui;
#ifdef USE_DEBUG
    qDebug()<<"The manwindow is destoryed!!!";
#endif
}

//open the setting serial port
void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        console->setEnabled(true);
        console->setLocalEchoEnabled(p.localEchoEnabled);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        showStatusMessage(tr("Open error"));
    }
}


//
void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
    {
        serial->close();
    }
//    console->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    showStatusMessage("Disconnected");
}
//

void MainWindow::about()
{
    QMessageBox::about(this, "About This Terminal",
                       "The <b>Simple Terminal</b> example demonstrates how to "
                       "use the Qt Serial Port module in modern GUI applications "
                       "using Qt, with a menu bar, toolbars, and a status bar.");
}

//串口发送数据
void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}
//

//the paintedit show data recieved
void MainWindow::readData()
{
//    QByteArray data = serial->readAll();
    while(!serial->atEnd()){
        QByteArray data = serial->readLine();
        console->putData(data);
        showWave->putData(data);
#ifdef USE_DEBUG
//        qDebug()<<data;
#endif
    }

}


//serial port open error handler
void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}
//save the recieved data to local file
void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("保存文件"),
                                                    ".",
                                                    tr("text file(*.txt)"));
    if(fileName.isEmpty()){

        return ;
    }
    else{
        QFile fileIn(fileName);
        if(!fileIn.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QMessageBox::warning(this,"Error",tr("打开文件失败"));
            return ;
        }
        else{
            QTextStream tsIn(&fileIn);
            do{
                tsIn<<console->toPlainText();
            }while(!tsIn.atEnd());
            fileIn.close();

            QMessageBox::information(this,
                                     "Information",
                                     tr("保存成功"));
        }
    }
}

void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionConfigure, &QAction::triggered, settings, &MainWindow::show);
    connect(ui->actionClear, &QAction::triggered, console, &Console::clear);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(ui->actionShowWave, &QAction::triggered, showWave, &MainWindow::show);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveFile);
}

//设置程序系统托盘图标
void MainWindow::initSysTrayIcon()
{
    sysIcon=new QSystemTrayIcon();
    QIcon icon=QIcon(":\\images\\sysicon");
//    qDebug()<<icon;
    sysIcon->setIcon(icon);
    sysIcon->setToolTip("Apollo_Qt_Software!");
    sysIcon->show();
    sysIcon->showMessage("Serial Port Helper",
                         "Qt software is runnning!",
                         QSystemTrayIcon::Information,5000);
}
//show message on statu bar
void MainWindow::showStatusMessage(const QString &message)
{
    status->setText(message);
}

/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
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
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QMessageBox>
#include <QLineEdit>
#include <QDateTime>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "settingsdialog.h"

#include "myserialport.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  //  x(0), y(0), z(0), r(0)
{
    ui->setupUi(this);
    console = new Console;
    console->setEnabled(false);

    serial = new MySerialPort(this);
    settings = new SettingsDialog(this);

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);
    ui->actionBluetooth->setEnabled(true);
    ui->actionAbout->setEnabled(true);

    initActionsConnections();
    initSerialConnections();

    widget = new QWidget(this);
    QGridLayout* centralLayout = new QGridLayout(widget);

    createTimeGroupBox();
    createStatusGroupBox();
    createCommandGroupBox();
    createComTestGroupBox();

    centralLayout->addWidget(timeGroupBox,0,0);
    centralLayout->addWidget(statusGroupBox,0,1);
    centralLayout->addWidget(commandGroupBox,1,0);
    centralLayout->addWidget(comTestGroupBox,1,1);

    initUpdateConnections();
    initCommandConnections();

    widget->setLayout(centralLayout);
    setCentralWidget(widget);

    resize(600,200);
    setWindowTitle(QApplication::translate("toplevel", "Smart House Control Panel"));

    startTimer(1000);
}

MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent * event){
    switch(event->isAccepted()){default:break;}
    timeLabel->setText(QTime::currentTime().toString("hh:mm:ss"));
}

void MainWindow::createTimeGroupBox(){

    timeGroupBox = new QGroupBox(tr("Time"),this);
    QHBoxLayout* layout  = new QHBoxLayout(timeGroupBox);

    timeLabel = new QLabel(this);//time since boot(ms)
    timeLabel->setBaseSize(80,30);
    timeLabel->setText(" ");

    layout->addWidget(timeLabel);
    timeGroupBox->setLayout(layout);
}

void MainWindow::createStatusGroupBox(){

    statusGroupBox = new QGroupBox(tr("House Status"),this);
    QGridLayout* layout  = new QGridLayout(statusGroupBox);

    temperatureLabel = new QLabel(this);
    temperatureLabel->setBaseSize(50,30);
    temperatureLabel->setText("Current Temperature: Unknown");

    doorConditionLabel = new QLabel(this);
    doorConditionLabel->setBaseSize(50,30);
    doorConditionLabel->setText("Door Status: Unknown");

    layout->addWidget(temperatureLabel,0,0);
    layout->addWidget(doorConditionLabel,1,0);

    statusGroupBox->setLayout(layout);
}

void MainWindow::createCommandGroupBox(){

    commandGroupBox = new QGroupBox(tr("Commands"),this);
    QVBoxLayout* layout = new QVBoxLayout(commandGroupBox);

    // Air conditioning commander
    AcGroupBox = new QGroupBox(tr("Air-Conditioner"),this);
    AcLayout = new QHBoxLayout(AcGroupBox);
    turnOnAcButton = new QRadioButton("On",this);
    turnOnAcButton->setChecked(false);
    turnOffAcButton = new QRadioButton("Off",this);
    turnOffAcButton->setChecked(true);

    AcLayout->addWidget(turnOnAcButton);
    AcLayout->addWidget(turnOffAcButton);
    AcGroupBox->setLayout(AcLayout);

    // Door commander
    doorGroupBox = new QGroupBox(tr("Open Door"),this);
    doorLayout = new QVBoxLayout(doorGroupBox);
    uname = new QLabel(tr("Username"), this);
    username = new QLineEdit(this);
    pw = new QLabel(tr("Password"), this);
    password = new QLineEdit(this);
    password->setEchoMode(QLineEdit::Password);
    password->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    openDoor = new QPushButton(tr("Login"), this);

    doorLayout->addWidget(uname);
    doorLayout->addWidget(username);
    doorLayout->addWidget(pw);
    doorLayout->addWidget(password);
    doorLayout->addWidget(openDoor);
    doorGroupBox->setLayout(doorLayout);

    layout->addWidget(AcGroupBox);
    layout->addWidget(doorGroupBox);

    // connect slots
    connect(turnOnAcButton,SIGNAL(clicked()),this,SLOT(onSetAc()));
    connect(turnOffAcButton,SIGNAL(clicked()),this,SLOT(onSetAc()));
    connect(openDoor,SIGNAL(clicked()),this,SLOT(onLogin()));

    commandGroupBox->setLayout(layout);
    commandGroupBox->setAlignment(Qt::AlignHCenter);
}

void MainWindow::createComTestGroupBox(){

    comTestGroupBox = new QGroupBox(tr("Communication Test"),this);
    QVBoxLayout* layout = new QVBoxLayout(comTestGroupBox);

    Rxmsg = new QLabel(tr("Received message:"), this);
    Txmsg = new QLabel(tr("Sent message:"), this);
    Tx = new QLineEdit(this);
    send = new QPushButton(tr("Send"), this);

    layout->addWidget(Rxmsg);
    layout->addWidget(Txmsg);
    layout->addWidget(Tx);
    layout->addWidget(send);

    connect(send,SIGNAL(clicked()),this,SLOT(onSend()));
    connect(Tx,SIGNAL(returnPressed()),this,SLOT(onSend()));
    connect(serial,SIGNAL(readyCollect()), this,SLOT(onUpdateRx()));

    comTestGroupBox->setLayout(layout);
    comTestGroupBox->setAlignment(Qt::AlignHCenter);


}

void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    // linux uses absolute address
    // windows uses relative address
    // serial->setPortName("/dev/cu."+p.name);  (for mac)
    serial->setPortName("/dev/"+p.name);  // for linux
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    serial->m_timer->start(200);

    if (serial->open(QIODevice::ReadWrite)) {
            console->setEnabled(true);
            console->setLocalEchoEnabled(p.localEchoEnabled);
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->actionConfigure->setEnabled(false);
            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                       .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
        serial->m_timer->stop();
        ui->statusBar->showMessage(tr("Open error"));
    }
}


void MainWindow::readData()
{
    QByteArray data = serial->readAll();
    //serial->uartRead(&data);
    Rxmsg->setText("Received Message:"+data);
}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    console->setEnabled(false);
    serial->m_timer->stop();
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    ui->statusBar->showMessage(tr("Disconnected"));
}


void MainWindow::about()
{
    QMessageBox::about(this, tr("About ELEC3300-UI"),
                       tr("\tThis UI is designed for \n"
                          "\ta 2015 Fall semester ELEC3300\n"
                          "\tsmart house project"));
}


void MainWindow::writeData(const QByteArray &data){

    serial->write(data);
}

void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
    connect(ui->actionClear, SIGNAL(triggered()), console, SLOT(clear()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionBluetooth, SIGNAL(triggered()), settings, SLOT(show()));
    connect(ui->actionLogo, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::initSerialConnections(){

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
}

void MainWindow::initUpdateConnections(){

}

void MainWindow::initCommandConnections(){

}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::onUpdateTime(){}
void MainWindow::onUpdateTemp(){}
void MainWindow::onUpdateDoor(){}

void MainWindow::onSetAc(){}
void MainWindow::onLogin(){}

void MainWindow::onSend(){
    QString str = Tx->text();
    str+='\n';
    QByteArray data(str.toStdString().c_str(),str.size());
    serial->writeFoo(data);
   // writeData(data);
    Txmsg->setText("Sent message:\n"+str);
    Tx->clear();
}

void MainWindow::onUpdateRx(){
    //QByteArray data = serial->readAll();
    QByteArray data=serial->m_readData;//serial->m_readData.append(serial->read(64));
    if (!data.isEmpty()){
        Rxmsg->setText("Received message:\n"+data);
    }
    //if(data.size())
    //  serial->m_readData.clear();
}



void MainWindow::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_Up:
        break;
    }
}

 void MainWindow::keyReleaseEvent(QKeyEvent* event){
     switch (event->key()) {
     case Qt::Key_Up:

     default:
         break;
     }
 }



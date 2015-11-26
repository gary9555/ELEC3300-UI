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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>

#include "mavserialport.h"
#include "myserialport.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class Console;
class SettingsDialog;
class MavSerialPort;
class MySerialPOrt;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    enum FLIGHT_MODE {RETURN = 0, MANUAL = 1, ALTCTL = 2, POSCTL = 3, MISSION = 4, LOITER = 5, DELIVERY = 6} flight_mode;
    enum ARM_STATE {DISARMED = 0, ARMED = 1} arm_state;
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void timerEvent(QTimerEvent *event);

signals:



private slots:
    /** Serial port dialog */
    void openSerialPort();
    void closeSerialPort();
    void about();
    void writeData(const QByteArray &data);
    //void writeFlightLog();
    void readData();

    void handleError(QSerialPort::SerialPortError error);

    /** Update Info */
    void onUpdateTime();
    void onUpdateTemp();
    void onUpdateDoor();
    void onUpdateRx();
    //void onUpdateTx();

    // Setting commands
    void onSetAc();
    void onCloseAc();
    void onAdd();
    void onVerify();
    void onClear();
    void onOpenCurtain();
    void onCloseCurtain();
    void onLogin();
    void onLogOff();

    void onSend();

private:
    /** communication */
    void initActionsConnections();
    void initSerialConnections();
    void initUpdateConnections();
    void initCommandConnections();

    /** inside MainWindow */
    void createTimeGroupBox();
    void createStatusGroupBox();
    void createCommandGroupBox();
    void createComTestGroupBox();

private:
    Ui::MainWindow *ui;

    /** Console */
    Console* console;
    SettingsDialog* settings;
    MySerialPort* serial;

    /** Smart House Control main window */
    QWidget* widget;

    QGroupBox* timeGroupBox;
    QGroupBox* statusGroupBox;
    QGroupBox* commandGroupBox;


    /** timeGroupBox */
    QLabel* timeLabel;  // real time display

    /** statusGroupBox */
    QLabel* sysState;
    QLabel* temperatureLabel;
    QLabel* curtainLabel;
    QLabel* doorConditionLabel;

    /** commandGroupBox */
    QGroupBox* AcGroupBox;
        QHBoxLayout* AcLayout;
            QRadioButton* turnOnAcButton;
            QRadioButton* turnOffAcButton;
    QGroupBox* FPGroupBox;
        QHBoxLayout* FPLayout;
            QRadioButton* addUser;
            QRadioButton* verifyUser;
            QRadioButton* clearUser;
    QGroupBox* CurtainGroupBox;
        QHBoxLayout* CurtainLayout;
            QRadioButton* openCurtain;
            QRadioButton* closeCurtain;
    QGroupBox* doorGroupBox;
        QVBoxLayout* doorLayout;
            QLineEdit* username;
            QLabel*    uname;
            QLineEdit* password;
            QLabel*    pw;
            QPushButton* openDoor;
            QPushButton* LogOff;

    /** Communication Test Box */
    QGroupBox* comTestGroupBox;
        QLabel* Rxmsg;
        QLabel* Txmsg;
        QLineEdit* Tx;
        QPushButton* send;

    int status; // system status

};

#endif // MAINWINDOW_H

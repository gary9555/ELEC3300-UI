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


/******************************/
/*** Credit to Ms. Eva Liu ****/
/******************************/

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QTextStream>
#include <QFile>
#include <QStringList>


#include "mainwindow.h"

#include "serialport.h"
#include "myserialport.h"
//#define DEBUG_SERIAL
//#define SERIAL_TEST
#define MAIN
QT_USE_NAMESPACE

int main(int argc, char *argv[])
{
#ifdef MAIN
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
#endif

#ifdef DEBUG_SERIAL
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer()<<endl;
        //qDebug() << "Std Baudrates: " << info.standardBaudRates()<<endl;

        // Example use QSerialPort
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
            serial.close();
    }
#endif
    return a.exec();
#ifdef SERIAL_TEST
        QCoreApplication coreApplication(argc, argv);
        //int argumentCount = QCoreApplication::arguments().size();
        QStringList argumentList = QCoreApplication::arguments();

        QTextStream standardOutput(stdout);
/*
        if (argumentCount == 1) {
            standardOutput << QObject::tr("Usage: %1 <serialportname> [baudrate]").arg(argumentList.first()) << endl;
            return 1;
        }
*/

        QSerialPort serialPort;
        QString serialPortName = "/dev/ttyUSB0";//argumentList.at(1);
        serialPort.setPortName(serialPortName);

        int serialPortBaudRate = 57600;//(argumentCount > 2) ? argumentList.at(2).toInt() : QSerialPort::Baud9600;
        serialPort.setBaudRate(serialPortBaudRate);

        if (!serialPort.open(QIODevice::ReadOnly)) {
            standardOutput << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
            return 1;
        }

        SerialPortReader serialPortReader(&serialPort);

        return coreApplication.exec();
#endif

#ifdef MY_SERIAL
        QCoreApplication coreApplication(argc, argv);
        //int argumentCount = QCoreApplication::arguments().size();
        QStringList argumentList = QCoreApplication::arguments();

        QTextStream standardOutput(stdout);
/*
        if (argumentCount == 1) {
            standardOutput << QObject::tr("Usage: %1 <serialportname> [baudrate]").arg(argumentList.first()) << endl;
            return 1;
        }
*/

        MySerialPort serialPort;
        QString serialPortName = "/dev/ttyUSB0";//argumentList.at(1);
        serialPort.setPortName(serialPortName);

        int serialPortBaudRate = 57600;//(argumentCount > 2) ? argumentList.at(2).toInt() : QSerialPort::Baud9600;
        serialPort.setBaudRate(serialPortBaudRate);

        if (!serialPort.open(QIODevice::ReadOnly)) {
            standardOutput << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serialPort.errorString()) << endl;
            return 1;
        }



        return coreApplication.exec();
#endif
}

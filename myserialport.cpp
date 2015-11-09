#include "myserialport.h"

#include <QCoreApplication>

QT_USE_NAMESPACE


MySerialPort::MySerialPort(QObject *parent)
    : QSerialPort(parent)
    , m_standardOutput(stdout)
    , m_bytesWritten(0)
{
    m_timer = new QTimer(this);
   // connect(m_serialPort, SIGNAL(bytesWritten(qint64)), SLOT(handleBytesWritten(qint64)));
    //connect(m_serialPort, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(handleError(QSerialPort::SerialPortError)));
    connect(m_timer, SIGNAL(timeout()), SLOT(handleTimeout()));
    connect(this, SIGNAL(readyRead()), SLOT(handleReadyRead()));
    connect(this, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(handleError(QSerialPort::SerialPortError)));
}

MySerialPort::~MySerialPort()
{
}

void MySerialPort::handleBytesWritten(qint64 bytes)
{
    m_bytesWritten += bytes;
    if (m_bytesWritten == m_writeData.size()) {
        m_bytesWritten = 0;
        m_standardOutput << QObject::tr("Data successfully sent to port %1").arg(this->portName()) << endl;
        QCoreApplication::quit();
    }
}

void MySerialPort::handleTimeout()
{
   // m_standardOutput << QObject::tr("Operation timed out for port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
    //QCoreApplication::exit(1);
}

void MySerialPort::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::WriteError) {
        m_standardOutput << QObject::tr("An I/O error occurred while writing the data to port %1, error: %2")\
                            .arg(this->portName()).arg(this->errorString()) << endl;
        QCoreApplication::exit(1);
    }
}

void MySerialPort::writeFoo(const QByteArray &writeData)
{
    m_writeData = writeData;

    qint64 bytesWritten = this->write(writeData);

    if (bytesWritten == -1) {
        m_standardOutput << QObject::tr("Failed to write the data to port %1, error: %2")\
                            .arg(this->portName()).arg(this->errorString()) << endl;
        QCoreApplication::exit(1);
    } else if (bytesWritten != m_writeData.size()) {
        m_standardOutput << QObject::tr("Failed to write all the data to port %1, error: %2")\
                            .arg(this->portName()).arg(this->errorString()) << endl;
        QCoreApplication::exit(1);
    }

    //m_timer->start(5000);
}

void MySerialPort::handleReadyRead()
{
    //m_readData.clear();
    while(canReadLine())
        m_readData = readLine(64);
        //m_readData.append(this->read(64));
    if (!m_readData.isEmpty()){
        //m_standardOutput << m_readData<< endl;
        // m_standardOutput << m_readData ;

    }
    emit readyCollect();

    //m_standardOutput << endl;
  //  if (!m_timer.isActive())
    //    m_timer.start(5000);

}


void MySerialPort::writeData(){
    if (!m_readData.isEmpty())
        m_standardOutput << m_readData << endl;

}


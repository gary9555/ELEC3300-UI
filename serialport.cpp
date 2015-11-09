#include "serialport.h"

#include <QCoreApplication>

QT_USE_NAMESPACE

SerialPortWriter::SerialPortWriter(QSerialPort *serialPort, QObject *parent)
    : QObject(parent)
    , m_serialPort(serialPort)
    , m_standardOutput(stdout)
    , m_bytesWritten(0)
{
    m_timer.setSingleShot(true);
    connect(m_serialPort, SIGNAL(bytesWritten(qint64)), SLOT(handleBytesWritten(qint64)));
    connect(m_serialPort, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(handleError(QSerialPort::SerialPortError)));
    connect(&m_timer, SIGNAL(timeout()), SLOT(handleTimeout()));
}

SerialPortWriter::~SerialPortWriter()
{
}

void SerialPortWriter::handleBytesWritten(qint64 bytes)
{
    m_bytesWritten += bytes;
    if (m_bytesWritten == m_writeData.size()) {
        m_bytesWritten = 0;
        m_standardOutput << QObject::tr("Data successfully sent to port %1").arg(m_serialPort->portName()) << endl;
        QCoreApplication::quit();
    }
}

void SerialPortWriter::handleTimeout()
{
    m_standardOutput << QObject::tr("Operation timed out for port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
    QCoreApplication::exit(1);
}

void SerialPortWriter::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::WriteError) {
        m_standardOutput << QObject::tr("An I/O error occurred while writing the data to port %1, error: %2")\
                            .arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
        QCoreApplication::exit(1);
    }
}

void SerialPortWriter::write(const QByteArray &writeData)
{
    m_writeData = writeData;

    qint64 bytesWritten = m_serialPort->write(writeData);

    if (bytesWritten == -1) {
        m_standardOutput << QObject::tr("Failed to write the data to port %1, error: %2")\
                            .arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
        QCoreApplication::exit(1);
    } else if (bytesWritten != m_writeData.size()) {
        m_standardOutput << QObject::tr("Failed to write all the data to port %1, error: %2")\
                            .arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
        QCoreApplication::exit(1);
    }

    m_timer.start(5000);
}


//////////////////////////////////////////////////////////////////////////////




SerialPortReader::SerialPortReader(QSerialPort *serialPort, QObject *parent)
    : QObject(parent)
    , m_serialPort(serialPort)
    , m_standardOutput(stdout)
{
    connect(m_serialPort, SIGNAL(readyRead()), SLOT(handleReadyRead()));
    connect(m_serialPort, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(handleError(QSerialPort::SerialPortError)));
    //connect(&m_timer, SIGNAL(timeout()), SLOT(handleTimeout()));
    //connect(m_serialPort, SIGNAL(getData(QByteArray)), this, SLOT(writeData()));

    m_timer.start(5000);
}

SerialPortReader::~SerialPortReader()
{
}

void SerialPortReader::handleReadyRead()
{
    m_readData.append(m_serialPort->read(64));
    if (!m_readData.isEmpty()){
        m_standardOutput << m_readData<< endl;
        // m_standardOutput << m_readData ;
    }
    if(m_readData.size())
        m_readData.clear();
    //m_standardOutput << endl;
  //  if (!m_timer.isActive())
    //    m_timer.start(5000);
}

void SerialPortReader::handleTimeout()
{
    if (m_readData.isEmpty()) {
        m_standardOutput << QObject::tr("No data was currently available for reading from port %1").arg(m_serialPort->portName()) << endl;
    } else {
        m_standardOutput << QObject::tr("Data successfully received from port %1").arg(m_serialPort->portName()) << endl;
        m_standardOutput << m_readData << endl;
    }

    QCoreApplication::quit();
}

void SerialPortReader::writeData(){
    if (!m_readData.isEmpty())
        m_standardOutput << m_readData << endl;

}

void SerialPortReader::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        m_standardOutput << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2")\
                            .arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
        QCoreApplication::exit(1);
    }
}

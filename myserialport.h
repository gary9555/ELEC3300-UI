#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QSerialPort>
#include <QByteArray>
#include <QString>
#include <QDebug>
#include <QTimer>


class MySerialPort: public QSerialPort {
    Q_OBJECT

public:
    MySerialPort(QObject *parent = 0);
    ~MySerialPort();

    void writeFoo(const QByteArray &writeData);

signals:
    void readyCollect();

private slots:
    void handleBytesWritten(qint64 bytes);
    void handleTimeout();
    void handleReadyRead();
    void writeData();
    void handleError(QSerialPort::SerialPortError error);

signals:
    //void readyRead();

public:
   // QSerialPort     *m_serialPort;
    QByteArray      m_writeData;
    QByteArray      m_readData;
    QTextStream     m_standardOutput;
    qint64          m_bytesWritten;
    QTimer          *m_timer;



};
#endif // MYSERIALPORT_H


#ifndef SERIAL_H
#define SERIAL_H

#include <iostream>
#include <QSerialPort>
#include <Windows.h>
#include <QRegularExpression>
#include <QDebug>


class Serial: public QSerialPort{
public:
    QSerialPort serial;
    void setSerialParams();
    QString receivedMessages();
    void writeMessages(QString textMsg);
    QString street_name;
};

#endif // SERIAL_H

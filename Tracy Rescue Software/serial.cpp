#include "serial.h"
#include "db2.h"

void Serial::setSerialParams()
{
    serial.setPortName("COM5");
    serial.open(QIODevice::ReadWrite);
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

}

QString Serial::receivedMessages()
{
    QRegularExpression regex("<PADDING>(.*)</PADDING>");
    QRegularExpression street("<STREET>(.*)</STREET>");

    using namespace std;
    setSerialParams();
    while(!serial.isOpen()) serial.open(QIODevice::ReadWrite);
    if(serial.isOpen()){
        QByteArray input;
        input = serial.readAll();

        QRegularExpressionMatch match = regex.match(QString::fromStdString(input.toStdString()));
        QRegularExpressionMatch match_street = street.match(QString::fromStdString(input.toStdString()));

        street_name = match_street.captured(1);
        QString textYouWant = match.captured(1);

        makeEntry *e = new makeEntry();


        qDebug() << input;
        qDebug() << textYouWant;
        if(textYouWant != ""){
        e->setParams("trialName","777777777",street_name,"205","",textYouWant); // currently incomplete
        makeEntry *e2 = e;
        return textYouWant;
        }
    }
    return "";
}

void Serial::writeMessages(QString textMsg)
{
    setSerialParams();
    QByteArray output = textMsg.toUtf8();
    while(!serial.isOpen()) serial.open(QIODevice::ReadWrite);
    serial.write(output);
    serial.flush();
    qDebug() << output;
}


#pragma once


#include <QObject>
#include"instructgenerator.hpp"

class QSerialPort;
class SerialManager : public QObject
{
    Q_OBJECT
public:
    explicit SerialManager(QObject *parent = nullptr);



    static void printSerials();

    void sendStatusCheck();
    void sendRequireArg();

    void setStatusCallback(std::function<void(bool,bool,bool)>);

signals:
    void sendTemperature(float);
    void sendBatteryVal(int);
    void sendArgs(float,float);

private:
    QSerialPort* rs485=nullptr;

    InstructGenerator generator;

    void recvData();
    void conductBuffer();
    QByteArray buffer;

    int expireTime=4000;
};


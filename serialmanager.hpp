
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

    bool checkSerialPort();

signals:
    void sendTemperature(float);
    void sendBatteryVal(int);
    void sendArgs(float,float);
    void sendlongitude(float);
    void sendlatitude(float);
    void sendPosition(unsigned char,unsigned char);

private:
    QSerialPort* rs485=nullptr;

    InstructGenerator generator;

    void recvData();
    void conductBuffer();
    QByteArray buffer;

    int expireTime=4000;
};


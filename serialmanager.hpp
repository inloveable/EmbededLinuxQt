
#pragma once


#include <QObject>


class QSerialPort;
class SerialManager : public QObject
{
    Q_OBJECT
public:
    explicit SerialManager(QObject *parent = nullptr);



    static void printSerials() ;

signals:

};


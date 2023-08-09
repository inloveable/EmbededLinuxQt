
#include "serialmanager.hpp"
#include "qdebug.h"
#include "qserialportinfo.h"

#include<QSerialPortInfo>
#include<glog/logging.h>
#include<QSerialPort>
SerialManager::SerialManager(QObject *parent)
    : QObject{parent}
{



}


void SerialManager::printSerials(){
    auto list=QSerialPortInfo::availablePorts();
    for(auto&& l:list){
        qDebug()<<"serial:"<<l.portName();
        if(l.portName()=="ttyS2"){
            LOG(INFO)<<"serial ttyS2(rs485) detected writing 0x01";
            QSerialPort port{l};
            port.setBaudRate(9600);
            port.open(QIODevice::WriteOnly);
            if(!port.isOpen()){
                LOG(INFO)<<"serial ttys2 openfailed";
            }
            QByteArray arr;
            arr.push_back(0x01);

            port.write(arr);
            port.flush();
            port.close();;
        }
    }

}

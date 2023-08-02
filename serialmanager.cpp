
#include "serialmanager.hpp"
#include "qdebug.h"
#include "qserialportinfo.h"

#include<QSerialPortInfo>
SerialManager::SerialManager(QObject *parent)
    : QObject{parent}
{



}


void SerialManager::printSerials(){
    auto list=QSerialPortInfo::availablePorts();
    for(auto&& l:list){
        qDebug()<<"serial:"<<l.portName();
    }
}

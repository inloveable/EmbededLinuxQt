
#include "serialmanager.hpp"
#include "qdebug.h"
#include "qserialportinfo.h"

#include<QSerialPortInfo>
#include<glog/logging.h>
#include<QSerialPort>
SerialManager::SerialManager(QObject *parent)
    : QObject{parent},rs485{nullptr}
{

    auto list=QSerialPortInfo::availablePorts();
    for(auto&& l:list){
        qDebug()<<"serial:"<<l.portName();
        if(l.portName()=="ttyS2"){
            LOG(INFO)<<"serial ttyS2(rs485) detected writing 0x01";
            rs485=new QSerialPort(l,this);
            rs485->open(QIODevice::ReadWrite);
            connect(rs485,&QSerialPort::readyRead,this,&SerialManager::recvData);
            break;
        }
    }

    auto tempCb=[obj=this](float temp){
        emit obj->sendTemperature(temp);
    };
    auto batterCb=[obj=this](int battery){
        emit obj->sendBatteryVal(battery);
    };
    auto longitudeCb=[obj=this](float lon){
        emit obj->sendlongitude(lon);
    };

    auto latitudeCb=[obj=this](float la){
        emit obj->sendlatitude(la);
    };
    auto positionCb=[obj=this](unsigned char lo,unsigned char la){
        emit obj->sendPosition(lo,la);
    };


    generator.batteryCb=batterCb;
    generator.temperatureCb=tempCb;
    generator.latitudeCb=latitudeCb;
    generator.positionCb=positionCb;
    generator.longitudeCb=longitudeCb;



}

void SerialManager::recvData(){
    LOG(INFO)<<"rs485 receiving data:"<<rs485->bytesAvailable();
    buffer.push_back(rs485->readAll());
    conductBuffer();
}

void SerialManager::conductBuffer(){
    if(buffer.size()<8){
        return;
    }
    while(buffer.size()>=8){
        std::array<unsigned char,8> packet;
        for(int i=0;i<8;++i){
            packet[i]=buffer[i];
        }
        buffer.remove(0,8);
        generator.parseData(packet);
    }

}

void SerialManager::sendRequireArg(){
    if(rs485==nullptr){
        return;
    }
    auto instruct=generator.getInstruction<ReadData>();
    for(auto&& c:instruct){
        rs485->putChar(c);
    }
}

void SerialManager::sendStatusCheck(){
    if(rs485==nullptr){
        LOG(WARNING)<<"rs485 not connected return";
        return;
    }
    auto instruct=generator.getInstruction<ReadStatus>();
    for(auto&& c:instruct){
        rs485->putChar(c);
    }
}

void SerialManager::setStatusCallback(std::function<void(bool,bool,bool)> cb){
    generator.statusCb=cb;
}


void SerialManager::printSerials(){
    auto list=QSerialPortInfo::availablePorts();
    for(auto&& l:list){
        qDebug()<<"serial:"<<l.portName();       
    }

}

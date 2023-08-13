
#include "devicemanager.hpp"
#include "PublicDefs.hpp"

#include "qdebug.h"
#include "qdir.h"
#include "qfilesystemwatcher.h"
#include "qprocess.h"
#include "serialmanager.hpp"
#include<glog/logging.h>
DeviceManager::DeviceManager(QObject *parent)
    : QObject{parent}
{
    moniter=new QFileSystemWatcher(this);
    moniter->addPath("/dev/");
    connect(moniter,&QFileSystemWatcher::directoryChanged,this,[=](const QString& path){

        LOG(INFO)<<"detected directory changed:"<<path.toStdString();

        QDir dir("/dev");
        QStringList udisks = dir.entryList(QStringList() << "sd*", QDir::System);

        if (udisks.isEmpty()) {
            qDebug() << "No SD Udisk detected.";
            if(usbDetected==true){
                usbDetected=false;//TODO:potentially multi usb device bug here;
                emit usbUnPluged();
            }
        } else {
            if(usbDetected){
                return;//usb has different areas,invokes this func multiple times;
            }
            LOG(INFO) << "Detected SD Udisk(s):";
            auto first=udisks.first();

            emit detectedUsb("/dev/"+first);
            usbDetected=true;

            this->usb="/dev/"+first;
        }
    });

    serials=new SerialManager(this);
    serials->setStatusCallback([obj=this](bool inde,bool battery,bool tempe){
        obj->impedence=inde;
        obj->batterry=battery;
        obj->temperature=tempe;
        obj->statusReady();
    });
    SerialManager::printSerials();

    connect(serials,&SerialManager::sendArgs,this,&DeviceManager::onArgsSent);
    connect(serials,&SerialManager::sendBatteryVal,this,&DeviceManager::onBatterySent);
    connect(serials,&SerialManager::sendTemperature,this,&DeviceManager::onTemperature);
}

void DeviceManager::mountUsb(const QString& usb,const QString& dst){

    QDir dir;
    dir.mkpath(dst);
    QProcess process;
    QStringList args;
    args<<usb<<dst;
    process.start("mount",args);
    process.waitForFinished();

    // 检查挂载是否成功
    if (process.exitCode() == 0) {
        qDebug() << "USB device mounted successfully.";
    } else {
        qDebug() << "Failed to mount USB device.";
    }
}

void DeviceManager::unmountUsb(const QString& ){

    // 执行卸载命令
    QProcess process;
    QStringList args;
    args<<this->usb;
    qDebug()<<"unmounting usb:args:"<<args;
    process.start("umount",args);
    process.waitForFinished();

    // 检查卸载是否成功
    if (process.exitCode() == 0) {
        qDebug() << "USB device unmounted successfully.";
    } else {
        qDebug() << "Failed to unmount USB device.";
    }
}

void DeviceManager::checkStatus(){
    serials->sendStatusCheck();
}

void DeviceManager::checkAllArgs(){
    serials->sendRequireArg();
}

DeviceManager::~DeviceManager(){

}




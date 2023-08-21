
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
    connect(serials,&SerialManager::sendPosition,this,[obj=this](unsigned char lo,unsigned char la){
        obj->longitudeSign=lo;
        obj->latitudeSign=la;
    });
    connect(serials,&SerialManager::sendlatitude,this,[obj=this](float val){
        obj->latitudeVal=val ;
    });
    connect(serials,&SerialManager::sendlongitude,this,[obj=this](float val){
        obj->longtitudeVal=val;
    });
}

void DeviceManager::mountUsb(const QString& usb1,const QString& dst){



    QDir dir;
    dir.mkpath(dst);
    QProcess process;
    QStringList args;
    //dst="/mnt";
    auto dst1="/mnt";
    args<<usb1<<dst1;
    process.start("mount",args);
    process.waitForFinished();

    // 检查挂载是否成功
    usbDetected=true;
    if (process.exitCode() == 0) {
        qDebug() << "USB device mounted successfully.";
    } else {
        qDebug() << "Failed to mount USB device.";
    }

    this->usb=usb1;
}

void DeviceManager::unmountUsb(const QString& ){



    // 执行卸载命令
    QProcess process;
    QStringList args;
    args<<"/mnt";
    qDebug()<<"unmounting usb:args:"<<args;
    process.start("umount",args);
    process.waitForFinished();

    // 检查卸载是否成功
    if (process.exitCode() == 0) {
        qDebug() << "USB device unmounted successfully.";

    } else {
        qDebug() << "Failed to unmount USB device.";
    }
    usbDetected=false;
}

void DeviceManager::checkStatus(){
    serials->sendStatusCheck();
}

void DeviceManager::checkAllArgs(){
    serials->sendRequireArg();
}

DeviceManager::~DeviceManager(){

}

QString DeviceManager::getGps() const {
    QString format{tr("经度: %1 %2  纬度: %3 %4")};

    auto gps=format.arg(longitudeSign).arg(longtitudeVal).arg(latitudeSign).arg(latitudeVal);

    LOG(INFO)<<gps.toStdString();
    return gps;
};

void DeviceManager::exportFileToUsb(const QString& file){
    QString usbPath ="/mnt";

    // 检查USB设备是否已挂载
    QDir usbDir(usbPath);
    if (!usbDir.exists()) {
        qDebug() << "USB设备未挂载";
        return;
    }

    // 拷贝文件到USB设备
    QString destinationPath = usbPath + "/" + file;
    if (QFile::copy(file, destinationPath)) {
        qDebug() << "sucess"<<destinationPath;
    } else {
        qDebug() << "fail";
    }
    QFile::remove("/root/"+file);
}


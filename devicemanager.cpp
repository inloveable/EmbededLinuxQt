
#include "devicemanager.hpp"
#include "qdebug.h"
#include "qdir.h"
#include "qfilesystemwatcher.h"
#include "qprocess.h"
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
            LOG(INFO) << "Detected SD Udisk(s):";
            auto first=udisks.first();

            emit detectedUsb("/dev/"+first);

           // for (const QString& udisk : udisks) {
            //    qDebug().noquote() << udisk;
             //   emit detectedUsb("/dev/"+udisk);
            //    this->usbDetected=true;
           //     this->usb=udisk;
           // }
        }
    });
}

void DeviceManager::mountUsb(const QString& usb,const QString& dst){
    // 构造挂载命令

    // 执行挂载命令
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

void DeviceManager::unmountUsb(const QString& usb){
    QString command = "umount " + usb;

    // 执行卸载命令
    QProcess process;
    QStringList args;
    args<<usb;
    process.start("umount",args);
    process.waitForFinished();

    // 检查卸载是否成功
    if (process.exitCode() == 0) {
        qDebug() << "USB device unmounted successfully.";
    } else {
        qDebug() << "Failed to unmount USB device.";
    }
}

DeviceManager::~DeviceManager(){

}


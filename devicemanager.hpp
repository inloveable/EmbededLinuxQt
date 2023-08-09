
#pragma once


#include <QObject>
#include<QFileSystemWatcher>

class QProcess;
class SerialManager;
class DeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit DeviceManager(QObject *parent = nullptr);
    ~DeviceManager();

    void mountUsb(const QString& usbPath,const QString& dstPath);
    void unmountUsb(const QString& usbPath);

    void exportFileToUsb(const QString& file);

    void checkSerialIsOnline(std::function<void(bool,void*)> func);

    void checkStatus();


signals:
    void detectedUsb(QString usb);
    void statusResult();
    void usbUnPluged();
private:


    QFileSystemWatcher* moniter;
    QProcess*           linuxCmdOperator=nullptr;


    bool    usbDetected=false;
    QString usb;

    SerialManager* serials=nullptr;



};


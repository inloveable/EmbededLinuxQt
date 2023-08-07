
#pragma once


#include <QObject>
#include<QFileSystemWatcher>

class QProcess;
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

signals:
    void detectedUsb(QString usb);
    void usbUnPluged();
private:


    QFileSystemWatcher* moniter;
    QProcess*           linuxCmdOperator=nullptr;


    bool    usbDetected=false;
    QString usb;



};

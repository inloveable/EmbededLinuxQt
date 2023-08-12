
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
    void checkAllArgs();

    //status
    bool impedence=false;
    bool batterry=false;
    bool temperature=false;

    QString getGps() const {
        return gps;
    };


signals:
    void detectedUsb(QString usb);
    void statusReady();
    void usbUnPluged();

    void onTemperature(float temp);
    void onBatterySent(int battery);

    void onArgsSent(float amp,float phaseAngle);


private:


    QFileSystemWatcher* moniter;
    QProcess*           linuxCmdOperator=nullptr;


    bool    usbDetected=false;
    QString usb;

    SerialManager* serials=nullptr;


    QString gps="--";
};




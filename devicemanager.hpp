
#pragma once


#include <QObject>
#include<QFileSystemWatcher>
#include <utility>
#include <variant>

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

    bool hasUsb() const{
        return usbDetected;
    }

    void exportFileToUsb(const QString& file);

    void checkSerialIsOnline(std::function<void(bool,void*)> func);

    void checkStatus();
    void checkAllArgs();

    //status
    bool impedence=false;
    bool batterry=false;
    bool temperature=false;

    QString getGps() const ;
        SerialManager* serials=nullptr;

signals:
    void detectedUsb(QString usb);
    void statusReady();
    //void gpsReady();
    void usbUnPluged();

    void onTemperature(float temp);
    void onBatterySent(int battery);

    void onArgsSent(float amp,float phaseAngle);


private:


    QFileSystemWatcher* moniter;
    QProcess*           linuxCmdOperator=nullptr;


    bool    usbDetected=false;
    QString usb;



    QString longitudeSign{"--"};
    QString latitudeSign{"--"};
    float   longtitudeVal=0.0f;
    float   latitudeVal=0.0f;

    bool hasGps=false;



};




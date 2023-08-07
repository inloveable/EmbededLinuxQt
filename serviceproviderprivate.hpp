
#pragma once


#include "qnetworkaccessmanager.h"
#include "qobjectdefs.h"
#include <QObject>
#include <mutex>

class TestPointModel;
class DeviceManager;
class ServiceProviderPrivate : public QObject
{
    Q_OBJECT
public:
    explicit ServiceProviderPrivate(QObject *parent = nullptr);

    Q_INVOKABLE void init();


    Q_INVOKABLE void getTime();

    Q_INVOKABLE void generateModel(QString name);
    TestPointModel*  getPreparedModel();


    Q_INVOKABLE void requestProjectInfo();

    Q_INVOKABLE void exportData();

signals:

    void sendTime(QString,bool);
    void modelReady();


    void sendProjectInfo(QList<QObject*> list);
    Q_INVOKABLE void projectInfoNeedsUpdate();


    void usbLoaded();
    void usbUnloaded();
private:
    QNetworkAccessManager* manager;

    std::mutex modelMutex;
    TestPointModel* prepared=nullptr;

    QList<QObject*> projectInfoBuffer;

    DeviceManager* devices=nullptr;


};



#pragma once


#include "qnetworkaccessmanager.h"
#include "qobjectdefs.h"
#include <QObject>
#include <mutex>

class TestPointModel;
class ServiceProviderPrivate : public QObject
{
    Q_OBJECT
public:
    explicit ServiceProviderPrivate(QObject *parent = nullptr);

    Q_INVOKABLE void init();


    Q_INVOKABLE void getTime();

    Q_INVOKABLE void generateModel(QString name);
    TestPointModel*  getPreparedModel();

signals:

    void sendTime(QString,bool);
    void modelReady();
private:
    QNetworkAccessManager* manager;

    std::mutex modelMutex;
    TestPointModel* prepared=nullptr;


};


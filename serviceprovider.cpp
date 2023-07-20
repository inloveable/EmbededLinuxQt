
#include "serviceprovider.hpp"
#include "qthread.h"
#include "serviceproviderprivate.hpp"
#include<QMessageBox>
ServiceProvider::ServiceProvider(QObject *parent)
    : QObject{parent}
{
    d=new ServiceProviderPrivate();
    this->serviceThread=new QThread();

    serviceThread->start();

    connect(d,&ServiceProviderPrivate::sendTime,this,&ServiceProvider::sendTime);

    d->moveToThread(serviceThread);

    QMetaObject::invokeMethod(d,"init");
}

ServiceProvider::~ServiceProvider(){
    connect(serviceThread,&QThread::finished,this,[&](){
        this->d->deleteLater();
        this->serviceThread->deleteLater();
    });

    serviceThread->quit();
}

void ServiceProvider::getTime(){
    QMetaObject::invokeMethod(d,"getTime");
}

void ServiceProvider::messageBox(QString text){
    QMessageBox::information(nullptr,"提示",text);
}


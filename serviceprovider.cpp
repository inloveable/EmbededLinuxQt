
#include "serviceprovider.hpp"
#include "qthread.h"
#include "serviceproviderprivate.hpp"
#include"serialmanager.hpp"
#include "testpointmodel.hpp"

#include <boost/math/statistics/linear_regression.hpp>

#include<QMessageBox>
#include<QFontDatabase>
ServiceProvider::ServiceProvider(QObject *parent)
    : QObject{parent}
{
    d=new ServiceProviderPrivate();
    this->serviceThread=new QThread();

    serviceThread->start();

    connect(d,&ServiceProviderPrivate::sendTime,this,&ServiceProvider::sendTime);
    //connect(d,&ServiceProviderPrivate::modelReady,this,&ServiceProvider::onModelReady);

    d->moveToThread(serviceThread);

    QMetaObject::invokeMethod(d,"init");

    QFontDatabase data;
    QStringList fontFamilies = data.families();

    qDebug()<<"fonts:"<<fontFamilies;

    SerialManager::printSerials();

    tModel=new TestPointModel(this);

}

TestPointModel* ServiceProvider::getTestPointModel(){
    return  tModel;
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

TestPointModel* ServiceProvider::getTestPointModel_Ex(const QString& name){

    //load model from database
   // auto model=
    return nullptr;
}


std::tuple<double,double,double> ServiceProvider::linearRegression(
                                const std::vector<double>& x,
                                const std::vector<double>& y)
{
    using boost::math::statistics::simple_ordinary_least_squares_with_R_squared;
    return simple_ordinary_least_squares_with_R_squared(x, y);
}

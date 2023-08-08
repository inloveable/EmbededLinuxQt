
#include "serviceprovider.hpp"
#include "modelinfo.hpp"
#include "qthread.h"
#include "serviceproviderprivate.hpp"
#include"serialmanager.hpp"
#include "testpointmodel.hpp"

#include <boost/math/statistics/linear_regression.hpp>

#include<QMessageBox>
#include<QFontDatabase>
#include"datamanager.hpp"
#include<glog/logging.h>
#include <memory>
ServiceProvider::ServiceProvider(QObject *parent)
    : QObject{parent}
{
    d=new ServiceProviderPrivate();
    this->serviceThread=new QThread();

    serviceThread->start();

    connect(d,&ServiceProviderPrivate::sendTime,this,&ServiceProvider::sendTime);
    connect(d,&ServiceProviderPrivate::sendProjectInfo,this,&ServiceProvider::sendProjectInfo);
    connect(d,&ServiceProviderPrivate::usbLoaded,this,[this](){
        this->hasUsb=true;
        emit usbOnlineChanged();
    });
    connect(d,&ServiceProviderPrivate::usbUnloaded,this,[this](){
        this->hasUsb=false;
        emit usbOnlineChanged();
    });
    //connect(d,&ServiceProviderPrivate::modelReady,this,&ServiceProvider::onModelReady);

    d->moveToThread(serviceThread);
    DataManager::getInstance().moveToThread(serviceThread);



    QMetaObject::invokeMethod(d,"init");
    QMetaObject::invokeMethod(&DataManager::getInstance(),"init");

    QFontDatabase data;
    QStringList fontFamilies = data.families();

    qDebug()<<"fonts:"<<fontFamilies;

    SerialManager::printSerials();

    //tModel=new TestPointModel(this);

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
    callBackend("getTime");
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

void  ServiceProvider::requestProjectInfo(){
    callBackend("requestProjectInfo");
}

void ServiceProvider::callBackend(const QString& funct){
    QMetaObject::invokeMethod(d,funct.toLatin1());
}

void ServiceProvider::selfCheck(){
    LOG(INFO)<<"self checking";
}

void ServiceProvider::prepareCreateNewModel(){
    LOG(INFO)<<"preparing create new model";
    if(modelInfo==nullptr){
        modelInfo=std::make_unique<ModelInfo>();

        modelInfo->addTestPoint(TestPointInfo::generate(modelInfo->testPointSize(),5.2,22,8000,32,36.3,false));
        modelInfo->addTestPoint(TestPointInfo::generate(modelInfo->testPointSize(),6.2,48,7000,12,35.3,true));
        modelInfo->addTestPoint(TestPointInfo::generate(modelInfo->testPointSize(),6.2,92,9000,73,48.3,true));

        if(tModel!=nullptr){
            tModel->deleteLater();
        }
        tModel=new TestPointModel(this);
        for(int i=0;i<modelInfo->testPointSize();++i){
            tModel->add(modelInfo->getTestPoint(i));
        }
    }
}

void ServiceProvider::deleteNewModelTestPoint(int index){
    if(modelInfo->testPointSize()==0){
        LOG(INFO)<<"no more to delete";
        return;
    }
    tModel->removePoint(index);
    modelInfo->deleteTestPoint(index);
}
void ServiceProvider::addNewModelTestPoint(){
    auto ptr=TestPointInfo::generate(modelInfo->testPointSize(),0,0,0,0,0,false);
    tModel->add(ptr);
    modelInfo->addTestPoint(ptr);
}


void  ServiceProvider::saveNewModel(){
    if(modelInfo==nullptr)return;
    LOG(INFO)<<"saving new model:"<<modelInfo->modelName.toStdString();
    modelSaved=true;
}
void  ServiceProvider::createNewModelExit(){
    if(modelSaved){
        LOG(INFO)<<"modelSaved safe exit";
    }

    LOG(INFO)<<"model unsaved,unsafe exit";

    modelInfo=nullptr;
    tModel->deleteLater();
    tModel=nullptr;
}

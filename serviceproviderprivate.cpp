
#include "serviceproviderprivate.hpp"
#include "PublicDefs.hpp"
#include "devicemanager.hpp"
#include "modelinfo.hpp"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qnetworkaccessmanager.h"
#include "qnetworkreply.h"
#include "qnetworkrequest.h"
#include "testpointmodel.hpp"
#include<QNetworkAccessManager>
#include<QFile>
#include <memory>
#include <mutex>
#include"serialmanager.hpp"
#include"glog/logging.h"
#include"datamanager.hpp"

ServiceProviderPrivate::ServiceProviderPrivate(QObject *parent)
    : QObject{parent}
{

}

void ServiceProviderPrivate::getTime(){



    //    QFile file{"/root/config.json"};

    //    file.open(QIODevice::ReadOnly);
    //    QJsonDocument doc;
    //    QString website;
    //    if(!file.isOpen()){
    //        //emit sendTime(QString(tr("config file not found")),false);

    //        website="http://quan.suning.com/getSysTime.do";
    //    }else{
    //        doc =QJsonDocument::fromJson(file.readAll());
    //        file.close();
    //        auto obj=doc.object();
    //        qDebug()<<obj;
    //        website=obj["timeApi"].toString();
    //    }



    //    qDebug()<<"website:"<<website;

    //    QNetworkRequest request;
    //    request.setUrl(website);


    //    auto reply=manager->get(request);
    //    connect(reply,&QNetworkReply::readyRead,this,[=](){
    //        auto json=QJsonDocument::fromJson(reply->readAll());
    //        if(json.isNull()){
    //            emit sendTime(tr("errorRespond"),false);
    //            reply->deleteLater();
    //            return;
    //        }
    //        auto str=json.object()["sysTime2"].toString();

    //        emit sendTime(str,true);
    //        reply->deleteLater();
    //    });
    //    connect(reply,&QNetworkReply::errorOccurred,this,[=]
    //            (QNetworkReply::NetworkError error){
    //        emit sendTime(reply->errorString(),false);
    //                reply->deleteLater();
    //        Q_UNUSED(error)
    //    });


    emit sendTime(DataManager::getInstance().getTime(),true);
}

void ServiceProviderPrivate::init(){
    manager=new QNetworkAccessManager(this);

    //QNetworkRequest request;
    devices=new DeviceManager(this);
    connect(devices,&DeviceManager::detectedUsb,this,[=](const QString& usb){
        LOG(INFO)<<"usb :"<<usb.toStdString();


        usbNum++;
        devices->mountUsb(usb,QString("/mnt/usb%1/").arg(usbNum));

        emit usbLoaded();
    });
    connect(devices,&DeviceManager::usbUnPluged,this,[=](){
        devices->unmountUsb(QString("/mnt/usb%1/").arg(usbNum));
        usbNum--;
        emit this->usbUnloaded();
    });
    connect(devices,&DeviceManager::onArgsSent,this,&ServiceProviderPrivate::onArgsSent);
    connect(devices,&DeviceManager::onBatterySent,this,&ServiceProviderPrivate::onBatterySent);
    connect(devices,&DeviceManager::onTemperature,this,&ServiceProviderPrivate::onTemperature);
    connect(devices,&DeviceManager::statusReady,this,[obj=this](){
        emit obj->pointParingComplete(obj->devices->impedence
                                      &&obj->devices->temperature
                                          &&obj->devices->batterry,obj->currentRequestPointIndex
                                      ,obj->devices->getGps());
    });
    connect(&DataManager::getInstance(),&DataManager::isUsbOnline,devices,&DeviceManager::hasUsb);
    connect(&DataManager::getInstance(),&DataManager::exportToUsb,devices,&DeviceManager::exportFileToUsb);
}

TestPointModel* ServiceProviderPrivate::getPreparedModel(){
    std::lock_guard<std::mutex> lock{modelMutex};
    return this->prepared;
}

void ServiceProviderPrivate::generateModel(QString name){

    TestPointModel* model=new TestPointModel;

    //load from database;

    std::lock_guard<std::mutex> lock{modelMutex};
    this->prepared=model;
    emit modelReady();
}

void ServiceProviderPrivate::requestProjectInfo(){
    if(projectInfoBuffer.size()>0){
        for(auto&& p:projectInfoBuffer){
            p->deleteLater();
        }
    }
    projectInfoBuffer=DataManager::getInstance().getAllProjectInfo();
    emit sendProjectInfo(projectInfoBuffer);
}

void ServiceProviderPrivate::exportData(){

}

void ServiceProviderPrivate::saveModelInfo(std::shared_ptr<ModelInfo> in){

    LOG(INFO)<<"saving model";
    auto info=in;
    auto& data=DataManager::getInstance();
    in->gps=devices->getGps();
    data.saveModelInfo(in);
}

void ServiceProviderPrivate::onArgsSent(float amp,float angle){
    static std::unique_ptr<DataField> amps=nullptr;
    if(!amps){
        amps=std::make_unique<DataField>(currentRequestPointIndex);
    }
    static std::unique_ptr<DataField> angles=nullptr;
    if(!angles){
        angles=std::make_unique<DataField>(currentRequestPointIndex);
    }
    //put it here because those two values are which the actual important ones.
    //this signal aims to acknowledge ui of the coming of data,to update(needs twice in total)
    emit temperatureValueRecv(currentRequestPointIndex);

    angles->setValue(angle);
    amps->setValue(amp);
    if(angles->isReady()&&amps->isReady()){
        emit argsValueReady(amps->getValue(),angles->getValue(),currentRequestPointIndex);
        angles->reset();
        amps->reset();
        argsDone=true;
        if(argsDone&&temperatureDone){
            currentRequestPointIndex=-1;
        }
        LOG(INFO)<<"args value ready sending";
    }


}
void ServiceProviderPrivate::onTemperature(float tem){
    static std::unique_ptr<DataField> temperatures=nullptr;
    if(!temperatures){
        temperatures=std::make_unique<DataField>(currentRequestPointIndex);
    }

    temperatures->setValue(tem);
    if(temperatures->isReady()){
        temperatures->reset();
        LOG(INFO)<<"temperature value ready sending";
        temperatureDone=true;
        if(argsDone&&temperatureDone){
            currentRequestPointIndex=-1;
        }
        emit temperatureValueReady(temperatures->getValue(),currentRequestPointIndex);
    }
}
void ServiceProviderPrivate::onBatterySent(int battery){
    LOG(INFO)<<"battery value updated:"<<battery;
    emit batteryValueUpdated(battery);

}

void ServiceProviderPrivate::requestPointInfoUpdate(int index){
    LOG(INFO)<<"backend paring";
    currentRequestPointIndex=index;
    this->devices->checkStatus();
}

void ServiceProviderPrivate::requestPointTest(){
    if(currentRequestPointIndex<0){
        LOG(INFO)<<"not paired return";
    }

    argsDone=false;
    temperatureDone=false;
    devices->checkAllArgs();
}

void ServiceProviderPrivate::requestPointTest(int index){

    currentRequestPointIndex=index;
    argsDone=false;
    temperatureDone=false;
    devices->checkAllArgs();
}

void ServiceProviderPrivate::addProject(QString project,qreal dryness){
    DataManager::getInstance().addProject(project,
                                          DataManager::getInstance().getTime()
                                          ,devices->getGps(),dryness);
    emit this->projectInfoNeedsUpdate();
}

void ServiceProviderPrivate::removeProject(int index){
    DataManager::getInstance().removeProject(index);

    emit this->projectInfoNeedsUpdate();
}

void ServiceProviderPrivate::onSentProjectInfo(ProjectInfo* project,
                                               TestPointModel* projectModel){
    //sender guarantees project has correct index(projectId)

    LOG(INFO)<<"backend project info init";
    auto& data=DataManager::getInstance();
    auto [name,createTime,gps,dryness]=data.getProjectInfo(project->projectId);
    project->projectName=name;
    project->createTime=createTime;
    project->gps=gps;
    if(dryness!=""){
        project->dryness=dryness.toDouble();
    }else{
        project->dryness=0.0f;
    }



    const auto pointIds=data.getPoints(project->projectId,DataManager::BelongType::Project);

    for(auto&& id:pointIds){
        auto ptr=data.getPointWithId(id);
        project->points.push_back(ptr);
        projectModel->add(ptr);
    }

    LOG(INFO)<<"project total poing count:"<<project->points.size();

}

void ServiceProviderPrivate::onSaveProjectInfo(ProjectInfo* info){
    LOG(INFO)<<"backend saving project info";
    auto& data=DataManager::getInstance();


    for(auto&& i:info->points){
        data.saveTestPoint(i,DataManager::BelongType::Project,info->projectId);
    }

}
void ServiceProviderPrivate::getModelInfoFromDb(){
    auto data=DataManager::getInstance().getModelInfoFromDb();
    emit this->sendModelListForTestPoint(data);
}

QString ServiceProviderPrivate::onRequestGps(){
    return devices->getGps();
}

void ServiceProviderPrivate::selfCheck(){
    devices->serials->checkSerialPort();
}

void ServiceProviderPrivate::umountUsb(){
    devices->unmountUsb("/mnt");
}





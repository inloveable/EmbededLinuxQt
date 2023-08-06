
#include "serviceproviderprivate.hpp"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qnetworkaccessmanager.h"
#include "qnetworkreply.h"
#include "qnetworkrequest.h"
#include "testpointmodel.hpp"
#include<QNetworkAccessManager>
#include<QFile>
#include <mutex>

#include"datamanager.hpp"

ServiceProviderPrivate::ServiceProviderPrivate(QObject *parent)
    : QObject{parent}
{

}

void ServiceProviderPrivate::getTime(){



    QFile file{"/root/config.json"};

    file.open(QIODevice::ReadOnly);
    QJsonDocument doc;
    QString website;
    if(!file.isOpen()){
        //emit sendTime(QString(tr("config file not found")),false);

        website="http://quan.suning.com/getSysTime.do";
    }else{
        doc =QJsonDocument::fromJson(file.readAll());
        file.close();
        auto obj=doc.object();
        qDebug()<<obj;
        website=obj["timeApi"].toString();
    }



    qDebug()<<"website:"<<website;

    QNetworkRequest request;
    request.setUrl(website);


    auto reply=manager->get(request);
    connect(reply,&QNetworkReply::readyRead,this,[=](){
        auto json=QJsonDocument::fromJson(reply->readAll());
        if(json.isNull()){
            emit sendTime(tr("errorRespond"),false);
            reply->deleteLater();
            return;
        }
        auto str=json.object()["sysTime2"].toString();

        emit sendTime(str,true);
        reply->deleteLater();
    });
    connect(reply,&QNetworkReply::errorOccurred,this,[=]
            (QNetworkReply::NetworkError error){
        emit sendTime(reply->errorString(),false);
                reply->deleteLater();
    });

}

void ServiceProviderPrivate::init(){
    manager=new QNetworkAccessManager(this);

    //QNetworkRequest request;

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



#include "serviceproviderprivate.hpp"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qnetworkaccessmanager.h"
#include "qnetworkreply.h"
#include "qnetworkrequest.h"
#include<QNetworkAccessManager>
#include<QFile>



ServiceProviderPrivate::ServiceProviderPrivate(QObject *parent)
    : QObject{parent}
{

}

void ServiceProviderPrivate::getTime(){



    QFile file{"config.json"};

    file.open(QIODevice::ReadOnly);
    if(!file.isOpen()){
        emit sendTime(QString(tr("config file not found")),false);
    }

    QJsonDocument doc =QJsonDocument::fromJson(file.readAll());
    file.close();
    auto obj=doc.object();
    qDebug()<<obj;
    auto website=obj["timeApi"].toString();
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



#include "serviceprovider.hpp"
#include "modelinfo.hpp"

#include "qmetaobject.h"
#include "qnamespace.h"
#include "qobjectdefs.h"
#include "qthread.h"
#include "serviceproviderprivate.hpp"
#include "testpointmodel.hpp"

#include <boost/math/statistics/linear_regression.hpp>
#include"datamanager.hpp"
#include<QMessageBox>
#include<QFontDatabase>
#include"datamanager.hpp"
#include<glog/logging.h>
#include <memory>

Q_DECLARE_METATYPE(std::shared_ptr<ModelInfo>);
Q_DECLARE_METATYPE(std::shared_ptr<ProjectInfo>);
Q_DECLARE_METATYPE(std::shared_ptr<TestPointInfo>);

ServiceProvider::ServiceProvider(QObject *parent)
    : QObject{parent},modelInfo{nullptr},projectInfo{nullptr},tModel{nullptr}
{
    d=new ServiceProviderPrivate();
    this->serviceThread=new QThread();

    serviceThread->start();
    initPrivateSignals();
    d->moveToThread(serviceThread);
    DataManager::getInstance().moveToThread(serviceThread);



    QMetaObject::invokeMethod(d,"init");
    QMetaObject::invokeMethod(&DataManager::getInstance(),"init");

    QFontDatabase data;
    QStringList fontFamilies = data.families();

    qDebug()<<"fonts:"<<fontFamilies;


    qRegisterMetaType<ModelInfo>();
    qRegisterMetaType<std::shared_ptr<ModelInfo>>();
    qRegisterMetaType<ProjectInfo>();
    qRegisterMetaType<std::shared_ptr<ProjectInfo>>();
    qRegisterMetaType<QList<QPair<QString,int>>>();
    qRegisterMetaType<std::shared_ptr<TestPointInfo>>();


    //tModel=new TestPointModel(this);

}



int ServiceProvider::getCurrentBattery(){
    return batteryValue;
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

TestPointModel* ServiceProvider::getTestPointModel_Project(){

    if(this->projectTestPointModel!=nullptr){
        return projectTestPointModel;
    }

    return nullptr;
}

void ServiceProvider::prepareProject(int index){
    isProject=true;
    if(!projectInfo){
        projectInfo=std::make_unique<ProjectInfo>();
    }
    if(!projectTestPointModel){
        projectTestPointModel=new TestPointModel(this);
    }

    projectInfo->projectId=index;
    emit this->sendProjectInfoToInit(projectInfo.get(),projectTestPointModel);

}


std::tuple<double,double,double> ServiceProvider::linearRegression(
                                const std::vector<double>& x,
                                const std::vector<double>& y)
{
    try{
        using boost::math::statistics::simple_ordinary_least_squares_with_R_squared;
        return simple_ordinary_least_squares_with_R_squared(x, y);
    }catch(...){//the divider might be zero;
        return {};
    }

}

void  ServiceProvider::requestProjectInfo(){
    callBackend("requestProjectInfo");
}

void ServiceProvider::callBackend(const QString& funct){
    QMetaObject::invokeMethod(d,funct.toLatin1());
}

void ServiceProvider::selfCheck(){
    LOG(INFO)<<"self checking";
    callBackend("selfCheck");
}

void ServiceProvider::prepareCreateNewModel(){
    isProject=false;
    LOG(INFO)<<"preparing create new model";
    if(modelInfo==nullptr){
        modelInfo=std::make_unique<ModelInfo>();
        if(tModel!=nullptr){
            tModel->deleteLater();
        }
        tModel=new TestPointModel(this);
        connect(tModel,&TestPointModel::sendFitArgs,this,[&](qreal a,qreal b,
                                                                qreal r2,int type){//what if here uses [=]? interesting

            if(type==0){
                modelInfo->argA=a;
                modelInfo->argB=b;
                modelInfo->argR2=r2;
            }else if(type==1){
                modelInfo->argAWater=a;
                modelInfo->argBWater=b;
                modelInfo->argR2Water=r2;
            }
        });
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
    if(modelSaved){
        this->messageBox(tr("模型已保存,请勿重复保存"));
        return;
    }
    if(modelInfo==nullptr)return;
    LOG(INFO)<<"saving new model:"<<modelInfo->modelName.toStdString();
    emit saveModelInfo(std::shared_ptr<ModelInfo>(std::move(modelInfo)));
    modelSaved=true;
}
void  ServiceProvider::createNewModelExit(){
    if(modelSaved){
        LOG(INFO)<<"modelSaved safe exit";
    }else{
         LOG(INFO)<<"model unsaved,unsafe exit";
    }

    modelInfo=nullptr;
    tModel->deleteLater();
    tModel=nullptr;
    modelSaved=false;
}

void ServiceProvider::requestPointInfoUpdate(int index){

    QMetaObject::invokeMethod(d,"requestPointInfoUpdate",Q_ARG(int,index));
}

void ServiceProvider::requestPointTest(){
    callBackend("requestPointTest");
}

void ServiceProvider::requestPointTest(int index){
    QMetaObject::invokeMethod(d,"requestPointTest",Q_ARG(int,index));
}

void ServiceProvider::setModelName(QString name){
    if(modelInfo!=nullptr){
         LOG(INFO)<<"setting model name:"<<name.toStdString();
         modelInfo->modelName=name;
    }
}

void ServiceProvider::initPrivateSignals(){
    connect(d,&ServiceProviderPrivate::sendTime,this,&ServiceProvider::sendTime);
    connect(d,&ServiceProviderPrivate::sendProjectInfo,this,[obj=this](QList<QObject*> info){
        emit obj->sendProjectInfo(info);
    });
    connect(d,&ServiceProviderPrivate::projectInfoNeedsUpdate,this,&ServiceProvider::projectInfoNeedsUpdate);
    connect(d,&ServiceProviderPrivate::usbLoaded,this,[this](){
        this->hasUsb=true;
        emit usbOnlineChanged();
    });
    connect(d,&ServiceProviderPrivate::usbUnloaded,this,[this](){
        this->hasUsb=false;
        emit usbOnlineChanged();
    });
    connect(d,&ServiceProviderPrivate::batteryValueUpdated,this,[this](int val){
        this->batteryValue=val;
        emit this->batteryChanged();
    });
    connect(this,&ServiceProvider::saveModelInfo,d,&ServiceProviderPrivate::saveModelInfo);
    //connect(d,&ServiceProviderPrivate::modelReady,this,&ServiceProvider::onModelReady);
    connect(d,&ServiceProviderPrivate::pointParingComplete,this,[obj=this](bool sucess,int index,QString gpsSt){
        auto point=obj->modelInfo->getTestPoint(index);
        if(point!=nullptr){
            point->gps=true;
            point->gpsStr=gpsSt;
        }
        emit obj->requestParingComplete(sucess,index);
    });
    connect(d,&ServiceProviderPrivate::argsValueReady,this,[obj=this](float amp,float phase,int index){


        if(!obj->isProject){
            if(obj->tModel==nullptr){
                return;
            }
            auto modelIndex=obj->tModel->index(index);
            obj->tModel->setData(modelIndex,amp,TestPointModel::AmpitudeRole);
            obj->tModel->setData(modelIndex,phase,TestPointModel::PhaseAngleRole);
        }else{

            auto& pointModel=obj->projectTestPointModel;
            if(pointModel==nullptr){
                return;
            }
            auto modelIndex=pointModel->index(index);
            pointModel->setData(modelIndex,amp,TestPointModel::AmpitudeRole);
            pointModel->setData(modelIndex,phase,TestPointModel::PhaseAngleRole);

            auto [aD,bD,r2D,aW,bW,r2W]=emit obj->getModelArgWithId(obj->projectInfo->projectId);

            auto density=obj->getFitValue(aD,bD,amp);
            auto waterR=obj->getFitValue(aW,bW,phase);

            pointModel->setData(modelIndex,density,TestPointModel::DensityRole);
            pointModel->setData(modelIndex,waterR,TestPointModel::WaterRateRole);
            auto dryness=obj->projectInfo->dryness;
            pointModel->setData(modelIndex,density/dryness,TestPointModel::Solidity);
        }
    });
    connect(d,&ServiceProviderPrivate::temperatureValueReady,this,[obj=this](float temp,int index){
        if(obj->tModel==nullptr){
            return;
        }
        auto modelIndex=obj->tModel->index(index);
        obj->tModel->setData(modelIndex,temp,TestPointModel::TemperatureRole);
    });
    connect(this,&ServiceProvider::sendProjectInfoToInit,d,&ServiceProviderPrivate::onSentProjectInfo
            ,Qt::BlockingQueuedConnection);//for sync
    connect(this,&ServiceProvider::saveProjectInfo,d,&ServiceProviderPrivate::onSaveProjectInfo);
    connect(d,&ServiceProviderPrivate::sendModelListForTestPoint,this,[obj=this, this](QList<QPair<QString,int>> info){
        if(tinyModelInfos.size()>0){
            for(auto&& t:tinyModelInfos){
                t->deleteLater();
            }
        }
        QList<QObject*> in;
        for(auto&& i:info){
            QObject* m=new TinyModelInfo(obj);
            static_cast<TinyModelInfo*>(m)->setModelIndex(i.second);
            static_cast<TinyModelInfo*>(m)->setModelName(i.first);
            in.push_back(m);
        }
        this->tinyModelInfos=in;

        //after prepare project model
        if(this->projectInfo!=nullptr){
            pointIndexToModelIMap.clear();
            for(auto&& p:projectInfo->points){
                if(tinyModelInfos.size()>0)
                pointIndexToModelIMap[p->index]=0;
                else
                pointIndexToModelIMap[p->index]=-1;
            }
        }


        emit this->tinyModelInfoUpdated();

    });
    connect(this,&ServiceProvider::requestGps,
            d,&ServiceProviderPrivate::onRequestGps,Qt::BlockingQueuedConnection);

}

void ServiceProvider::addProject(QString name,qreal dryness){
    QMetaObject::invokeMethod(d,"addProject",Q_ARG(QString,name),Q_ARG(qreal,dryness));
}

void ServiceProvider::removeProject(int index){
     QMetaObject::invokeMethod(d,"removeProject",Q_ARG(int,index));
}

void ServiceProvider::projectInfoExit(){
     LOG(INFO)<<"project info exiting";
     if(projectInfo){
         projectInfo=nullptr;
     }
     if(this->projectTestPointModel){
         projectTestPointModel->deleteLater();
         projectTestPointModel=nullptr;
     }
}

void ServiceProvider::projectInfoSave(){
     LOG(INFO)<<"saving project info";
     emit this->saveProjectInfo(projectInfo.get());
}

void ServiceProvider::addProjectTestPoint()
{
     if(projectInfo==nullptr){
         return;
     }
     auto ptr=TestPointInfo::generate(projectInfo->points.size(),0,0,0,0,0,false);
     ptr->soildity=0;
     ptr->modelIndex=0;
     ptr->dryDesity=0;
     ptr->gps="--";


     projectTestPointModel->add(ptr);
     projectInfo->points.push_back(ptr);

}
void ServiceProvider::removeProjectTestPoint(int index)
{
     if(projectInfo==nullptr){
         return;
     }
     if(projectInfo->points.size()==0){
         LOG(INFO)<<"no more to delete";
         return;
     }
     std::shared_ptr<TestPointInfo> ptr;

     for(auto&& p:projectInfo->points){
         if(p->index==index){
            ptr=p;
            break;
         }
     }

     QMetaObject::invokeMethod(&DataManager::getInstance(),"removePoint",
                               Q_ARG(std::shared_ptr<TestPointInfo>,ptr));

     projectTestPointModel->removePoint(index);
     projectInfo->points.removeAt(index);
}

void ServiceProvider::requestTinyModelInfo(){
     callBackend("getModelInfoFromDb");
}

void ServiceProvider::setPointToModel(int pointI,int modelI){
     auto iter=pointIndexToModelIMap.find(pointI);
     if(iter!=pointIndexToModelIMap.end()){
         LOG(INFO)<<"setting point to model:"<<pointI<<" model:"<<modelI;
         pointIndexToModelIMap[pointI]=modelI;
         auto ptr=this->projectInfo->points.value(pointI);
         if(ptr!=nullptr){
            ptr->modelIndex=modelI;//this might work
         }
     }

}

void ServiceProvider::refreshTestPointDataWithModel(){
     if(this->projectTestPointModel==nullptr){
         return;
     }

     auto& pointModel=this->projectTestPointModel;
     connect(this,&ServiceProvider::getModelArgWithId,
             &DataManager::getInstance(),&DataManager::getModelArgWithId,
             Qt::BlockingQueuedConnection);
     for(auto&& iter=this->pointIndexToModelIMap.begin();
         iter!=pointIndexToModelIMap.end();++iter){
         const int pIndex=iter.key();
         const int mIndex=iter.value();

         auto ptr=pointModel->getSequence().value(pIndex);
         if(ptr==nullptr){
            LOG(INFO)<<"logic error point pointer with index:"<<pIndex<<" is NULL";
            continue;
         }


         auto [aD,bD,r2D,aW,bW,r2W]=emit getModelArgWithId(mIndex);

         LOG(INFO)<<"density:y="<<aD<<"*x+"<<bD;
         LOG(INFO)<<"water:y="<<aW<<"*x+"<<bW;
         auto density=this->getFitValue(aD,bD,ptr->ampitude);
         auto waterR=this->getFitValue(aW,bW,ptr->phaseAngle);
         auto modelIndex=pointModel->index(pIndex);
         pointModel->setData(modelIndex,density,TestPointModel::DensityRole);
         pointModel->setData(modelIndex,waterR,TestPointModel::WaterRateRole);

         auto dryness=this->projectInfo->dryness;
         pointModel->setData(modelIndex,density/dryness,TestPointModel::Solidity);
     }

     disconnect(this,&ServiceProvider::getModelArgWithId,
             &DataManager::getInstance(),&DataManager::getModelArgWithId
             );
}

QList<QObject*> ServiceProvider::getExportInfos(){

     connect(this,&ServiceProvider::requestExportData,
             &DataManager::getInstance(),&DataManager::getExportData,
             Qt::BlockingQueuedConnection);

     auto list=emit this->requestExportData();

     disconnect(this,&ServiceProvider::requestExportData,
                &DataManager::getInstance(),&DataManager::getExportData
                );
     return list;
}

void ServiceProvider::exportData(int index,int type){
     QMetaObject::invokeMethod(&DataManager::getInstance(),
                               "exportDataToUsb",Q_ARG(int,index),Q_ARG(int,type));
}

QString ServiceProvider::getGps(){
     return emit requestGps();
}


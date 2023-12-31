﻿
#include "datamanager.hpp"
#include "ProjectInfoObject.hpp"
#include "dataexporter.hpp"
#include "modelinfo.hpp"
#include "modelmanagemodel.hpp"
#include "qdebug.h"
#include <memory>
#include <mutex>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlRecord>
#include<QSqlError>
#include<glog/logging.h>
#include"PublicDefs.hpp"

#include<QDateTime>
#include <tuple>
#include <vector>
DataManager::DataManager(QObject *parent)
    : QObject{parent}
{

}

void DataManager::initializeDatabase(){
    std::lock_guard<std::mutex> lock{sqlmu};
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydatabase.db");


    // 打开数据库
    if (!db.open()) {
        qDebug() << "Failed to open database, error:" << db.lastError().text();
        return;
    }

    if(db.tables().size()>0){
        LOG(INFO)<<"database has been inited,returning";
        return;
    }
    // 创建一个表
    QSqlQuery query;
    if (!query.exec("CREATE TABLE projectInfo ("
                    "id INTEGER PRIMARY KEY,"
                    "createTime VARCHAR,"
                    "name VARCHAR,"
                    "gps VARCHAR,"
                    "dryness REAL"
                    ")"))
    {
        qDebug() << "Failed to create table, error:" << query.lastError().text();
    // 关闭数据库连接

    }

    if (!query.exec("CREATE TABLE soilModel ("
                    "id INTEGER PRIMARY KEY,"
                    "modelName TEXT,"
                    "createTime TEXT,"
                    "GPS TEXT,"
                    "rD REAL,"
                    "aD REAL,"
                    "bD REAL,"//拟合系数
                    "rW REAL,"
                    "aW REAL,"
                    "bW REAL"//拟合系数
                    ")")) {
        qDebug() << "Failed to create table, error:" << query.lastError().text();

    }
    if (!query.exec("CREATE TABLE measurementPoint ("
                    "pointId INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "pointName varchar,"
                    "belongId INTEGER,"
                    "belongType INTEGER,"//0:model 1:project
                    "density REAL,"
                    "waterContent REAL,"
                    "amplitude REAL,"
                    "phase REAL,"
                    "temperature REAL,"
                    "pointIndex INTEGER,"
                    "gps varchar,"
                    "soildity REAL,"
                    "dryDensity REAL,"
                    "modelId INTEGER"//only useful when belongType==1
                    ")")) {
        qDebug() << "Failed to create table, error:" << query.lastError().text();
    }
    db.close();

}
int DataManager::getProjectId(QString name)
{
    QSqlDatabase db=QSqlDatabase::database();
    db.open();
    QSqlQuery query;
    query.prepare("SELECT id FROM projectInfo WHERE projectName = :name");
    query.bindValue(":name", name);
    if (!query.exec()) {
        qDebug() << "Failed to execute query, error:" << query.lastError().text();
        return -1;
    }
    if (query.first()) {
        return query.value(0).toInt();
    }

    return -1;
}

int DataManager::getSoilModelId(QString name)
{
    QSqlDatabase db=QSqlDatabase::database();
    db.open();
    QSqlQuery query;
    query.prepare("SELECT id FROM soilModel WHERE modelName = :name");
    query.bindValue(":name", name);
    if (!query.exec()) {
        qDebug() << "Failed to execute query, error:" << query.lastError().text();
        return -1;
    }
    if (query.first()) {
        return query.value(0).toInt();
    }
    db.close();
    return -1;
}



DataManager::BelongType DataManager::getBelongType(int id)
{
    QSqlDatabase db=QSqlDatabase::database();
    db.open();
    QSqlQuery query;
    query.prepare("SELECT belongType FROM measurementPoint WHERE pointId = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Failed to execute query, error:" << query.lastError().text();
        return BelongType::Model; // 默认返回模型类型
    }
    if (query.first()) {
        int belongType = query.value(0).toInt();
        if (belongType == (int)BelongType::Model || belongType == (int)BelongType::Project) {
            return static_cast<DataManager::BelongType>(belongType);
        }
    }
    db.close();
    return BelongType::Model; // 默认返回模型类型
}

std::vector<int> DataManager::getPoints(int belongId,BelongType belongType){
    QSqlDatabase db=QSqlDatabase::database();
    db.open();
    std::vector<int> rec;

    LOG(INFO)<<"getting point ids with:"<<belongId<<" belongType:"<<(int)belongType;

    QSqlQuery query;
    query.prepare("SELECT pointId FROM measurementPoint WHERE belongId=:id AND belongType=:belongType");
    query.bindValue(":id", belongId);
    query.bindValue(":belongType",(int)belongType);
    if (!query.exec()) {
        qDebug() << "Failed to execute query, error:" << query.lastError().text();
        return rec; // 默认返回模型类型
    }

    while(query.next()){
        rec.push_back(query.value("pointId").toInt());
    }
    db.close();

    return rec;
}

std::shared_ptr<TestPointInfo> DataManager::getPointWithId(int id)
{
    QSqlDatabase db=QSqlDatabase::database();
    db.open();
    QSqlQuery query;
    query.prepare("SELECT * FROM measurementPoint WHERE pointId=:id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Failed to execute query, error:" << query.lastError().text();
        return nullptr;
    }
    std::shared_ptr<TestPointInfo> rec{nullptr};
    while (query.next()) {      
        rec = std::make_shared<TestPointInfo>();
        rec->pointId = query.value("pointId").toInt();
        rec->modelIndex = query.value("modelId").toInt();
        rec->index = query.value("pointIndex").toInt();
        rec->density = query.value("density").toReal();
        rec->waterRate = query.value("waterContent").toReal();
        rec->ampitude = query.value("amplitude").toReal();
        rec->phaseAngle = query.value("phase").toReal();
        rec->temperature = query.value("temperature").toReal();
    }

    db.close();
    return rec;
}

void DataManager::init(){
     initializeDatabase();
//DataExporter().exportModel(4);
    //DataExporter().exportProject(1);
}

void DataManager::Destory(){

}

QList<QObject*> DataManager::getAllProjectInfo(){

    QSqlDatabase db=QSqlDatabase::database();
    db.open();

    if(!db.isOpen()){
        LOG(WARNING)<<db.lastError().text().toStdString();
        LOG(WARNING)<<"database connection failed";
        return QList<QObject*>{};
    }

    QSqlQuery query{db};
    query.exec("select id, createTime, name from ProjectInfo ");
    QList<QObject*> objs;
    while(query.next()){
        int             id=query.value("id").toInt();
        QString createTime=query.value("createTime").toString();
        QString   name    =query.value("name").toString();
        QObject* obj=new ProjectInfoObject;

        auto p=static_cast<ProjectInfoObject*>(obj);
        p->setIndex(id);
        p->setName(name);
        p->setTime(createTime);
        objs<<obj;
    };

    db.close();
    return objs;

}

void DataManager::saveModelInfo(const std::shared_ptr<ModelInfo>& model)
{
    std::lock_guard<std::mutex> lock{sqlmu};
    QSqlDatabase database = QSqlDatabase::database();
    //database.setDatabaseName("mydatabase.db");


    // 打开数据库
    if (!database.open()) {
        qWarning() << "Failed to open database"<<database.lastError();
        return;
    }


    QSqlQuery query;

    query.exec("SELECT MAX(id) FROM soilModel;");
    query.next();
    int index=query.value(0).toInt()+1;
    qDebug()<<"model index:"<<index;

    query.prepare("INSERT INTO soilModel (id,modelName,createTime,GPS,rD,aD,bD,rW,aW,bW) "
                  "VALUES (:modelName,:maximumDryness,:bestWaterRate,:gps,:argR2,:argA,:argB,:rW,:aW,:bW)");

    query.bindValue(":modelName",index);
    query.bindValue(":maximumDryness", model->modelName);
    query.bindValue(":bestWaterRate", this->getTime());
    query.bindValue(":gps",model->gps);
    query.bindValue(":argA", model->argA);
    query.bindValue(":argB", model->argB);
    query.bindValue(":argR2", model->argR2);
    query.bindValue(":rW", model->argR2Water);
    query.bindValue(":aW", model->argAWater);
    query.bindValue(":bW", model->argBWater);

    if (!query.exec()) {
        qWarning() << "Failed to insert data, error:" << query.lastError().text();
        return;
    }

    for(int i=0;i<model->testPointSize();++i){
        saveTestPoint(model->getTestPoint(i),BelongType::Model,index);
    }
    database.close();

}

void DataManager::saveTestPoint(const std::shared_ptr<TestPointInfo>& point, BelongType belong, int belongId) {
    // 建立数据库连接
    QSqlDatabase database = QSqlDatabase::database();

    // 打开数据库
    if (!database.open()) {
        qWarning() << "Failed to open database";
        return;
    }

    // 准备 SQL 查询语句
    QSqlQuery query;

    query.exec(QString("select * from measurementPoint where pointId='%1'").arg(point->pointId));
    if(query.next()){
        LOG(INFO)<<"point exists,updating";
        query.prepare("UPDATE measurementPoint SET pointIndex=:pointIndex, density=:density, waterContent=:waterContent, "
                      "amplitude=:amplitude, phase=:phase, temperature=:temperature, gps=:gps, soildity=:soildity, "
                      "dryDensity=:dryDensity, belongType=:belongType, belongId=:belongId WHERE pointId=:pointId");
        query.bindValue(":pointIndex", point->index);
        query.bindValue(":density", point->density);
        query.bindValue(":waterContent", point->waterRate);
        query.bindValue(":amplitude", point->ampitude);
        query.bindValue(":phase", point->phaseAngle);
        query.bindValue(":temperature", point->temperature);
        query.bindValue(":gps", point->gps);
        query.bindValue(":soildity", point->soildity);
        query.bindValue(":dryDensity", point->dryDesity);
        query.bindValue(":belongType", (int)belong);
        query.bindValue(":belongId", belongId);
        query.bindValue(":pointId", point->pointId);
        if(!query.exec()){
            LOG(WARNING)<<"sql error:"<<query.lastError().text().toStdString();
        }


        return;

    }


    query.prepare("INSERT INTO measurementPoint (pointIndex, density, waterContent, amplitude, phase,"
                  "temperature, gps, soildity, dryDensity, belongType, belongId) "
                  "VALUES (:pointIndex, :density, :waterContent, :amplitude, :phase, "
                  ":temperature, :gps, :soildity, :dryDensity, :belongType, :belongId)");

    // 绑定参数
    query.bindValue(":pointIndex", point->index);
    query.bindValue(":density", point->density);
    query.bindValue(":waterContent", point->waterRate);
    query.bindValue(":amplitude", point->ampitude);
    query.bindValue(":phase", point->phaseAngle);
    query.bindValue(":temperature", point->temperature);
    query.bindValue(":gps", point->gps);
    query.bindValue(":soildity", point->soildity);
    query.bindValue(":dryDensity", point->dryDesity);
    query.bindValue(":belongType", static_cast<int>(belong));
    query.bindValue(":belongId", belongId);


    // 执行查询
    if (!query.exec()) {
        qWarning() << "Failed to insert test point, error:" << query.lastError().text();
        return;
    }


    query.exec("SELECT MAX(pointId) FROM measurementPoint;");
    query.next();
    int index=query.value(0).toInt();
    point->pointId=index;
    database.close();
}


void DataManager::addProject(QString project,QString createTime,QString gps,qreal dryness){
    auto database=QSqlDatabase::database();
    QSqlQuery query;

    query.exec("SELECT MAX(id) FROM projectInfo;");
    query.next();
    int index=query.value(0).toInt()+1;
    qDebug()<<"model index:"<<index;
    // 构建插入语句

    QString insertQuery = QString("INSERT INTO projectInfo (id,name, createTime, gps,dryness) "
                                  "VALUES ('%1', '%2', '%3','%4','%5')")
                              .arg(index)
                              .arg(project)
                              .arg(createTime)
                              .arg(gps)
                              .arg(dryness);
    // 执行插入语句
    if (!query.exec(insertQuery)) {
        qDebug() << "Failed to add project, error:" << query.lastError().text();
        // 关闭数据库连接或进行其他错误处理
        return;
    }

    // 添加项目成功
    qDebug() << "Project added successfully";

    database.close();
    // 可以进行其他操作或返回适当的成功标识

}

void DataManager::removeModel(int index){
    auto database=QSqlDatabase::database();
    QSqlQuery query;
    QString deleteQuery = QString("DELETE FROM soilModel WHERE id = %1").arg(index);


    if (!query.exec(deleteQuery)) {
        return;
    }

    // 检查是否有行受到影响
    if (query.numRowsAffected() > 0) {
        // 删除成功
        qDebug() << "Project removed successfully";
    } else {
        // 未找到匹配的项目
        qDebug() << "No project found with index:" << index;
    }
    database.close();
}

void DataManager::removeProject(int index){
    auto db=QSqlDatabase::database();

    QSqlQuery query;
    QString deleteQuery = QString("DELETE FROM projectInfo WHERE id = %1").arg(index);


    if (!query.exec(deleteQuery)) {
        return;
    }

    // 检查是否有行受到影响
    if (query.numRowsAffected() > 0) {
        // 删除成功
        qDebug() << "Project removed successfully";
    } else {
        // 未找到匹配的项目
        qDebug() << "No project found with index:" << index;
    }

    db.close();
}

QList<QPair<QString,int>> DataManager::getModelInfoFromDb(){
    QList<QPair<QString, int>> modelInfoList;

    // 创建数据库连接
    QSqlDatabase db = QSqlDatabase::database();


    if (db.open()) {
        // 执行查询语句
        QString queryStr = "SELECT id, modelName FROM soilModel";
        QSqlQuery query(queryStr);

        // 处理查询结果
        while (query.next()) {
            QString modelName = query.value(1).toString();
            int id = query.value(0).toInt();
            modelInfoList.append(qMakePair(modelName, id));

        }

        // 关闭数据库连接
    } else {
        // 连接数据库失败处理逻辑
        LOG(INFO)<<"Failed to connect to the database";
    }
    db.close();

    return modelInfoList;
}

std::tuple<QString,QString,QString,QString> DataManager::getProjectInfo(int index){
    auto db=QSqlDatabase::database();
    if(!db.open()){
        LOG(INFO)<<"db not open in:getProjectInfo";
    }
    QSqlQuery query{db};
    query.exec(QString("select name,createTime,gps,dryness from projectInfo where id='%1'").arg(index));

    std::tuple<QString,QString,QString,QString> rec;
    while(query.next()){
        std::get<0>(rec)=query.value("name").toString();
        std::get<1>(rec)=query.value("createTime").toString();
        std::get<2>(rec)=query.value("gps").toString();
        std::get<3>(rec)=query.value("dryness").toString();
    }
    db.close();
    return rec;
}

std::shared_ptr<ModelInfo> DataManager:: getModelInfoWithId(int id){
    std::shared_ptr<ModelInfo> info {nullptr};

    std::lock_guard<std::mutex> lock{sqlmu};
    QSqlDatabase database = QSqlDatabase::database();

    // 打开数据库
    if (!database.open()) {
        qWarning() << "Failed to open database" << database.lastError();
        return info;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM soilModel WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Failed to execute query" << query.lastError().text();
        return info;
    }

    if (query.next()) {
        info= std::make_shared<ModelInfo>();
        info->id = query.value("id").toInt();
        info->modelName = query.value("modelName").toString();
        info->gps = query.value("GPS").toString();
        info->argA = query.value("aD").toDouble();
        info->argB = query.value("bD").toDouble();
        info->argR2 = query.value("rD").toDouble();
        info->argR2Water = query.value("rW").toDouble();
        info->argAWater = query.value("aW").toDouble();
        info->argBWater = query.value("bW").toDouble();
        info->createTime=query.value("createTime").toString();
    }

    const auto pointIds=this->getPoints(info->id,DataManager::BelongType::Model);

    for(auto&& id:pointIds){
        auto ptr=this->getPointWithId(id);
        info->addTestPoint(ptr);
    }
    database.close();
    return info;
}


void DataManager::removePoint(std::shared_ptr<TestPointInfo> info){
    QSqlDatabase database = QSqlDatabase::database();

    // 打开数据库
    if (!database.open()) {
        qWarning() << "Failed to open database" << database.lastError();
        return;
    }

    QSqlQuery query;

    query.prepare("DELETE FROM measurementPoint WHERE pointId = :id");
    query.bindValue(":id", info->pointId);

    if (!query.exec()) {
        qWarning() << "Failed to execute query" << query.lastError().text();
        return;
    }

}

std::tuple<float,float,float,float,float,float> DataManager::getModelArgWithId(int index){
    QSqlDatabase database = QSqlDatabase::database();

    // 打开数据库
    if (!database.open()) {
        qWarning() << "Failed to open database" << database.lastError();
        return {};
    }

    QSqlQuery query;

    query.prepare("select a,b,r,aW,bW,rW FROM soilModel WHERE id = :id");
    query.bindValue(":id", index);
    if (!query.exec()) {
        qWarning() << "Failed to execute query" << query.lastError().text();
        return {};
    }

    if(query.next()){
        LOG(INFO)<<"return arg";
        database.close();
        return {query.value("a").toFloat(),query.value("b").toFloat(),
                query.value("r").toFloat(),query.value("aW").toFloat(),
                query.value("bW").toFloat(),query.value("rW").toFloat()};
    }
   database.close();
    return {};
}

QList<QObject*> DataManager::getExportData(){
    if(exportData.size()!=0){
        for(auto&& e:exportData){
            e->deleteLater();
        }
        exportData.clear();
    }
    auto models=getModelInfoFromDb();
    if(models.size()!=0){
        for(auto&& info:models){
            QObject* m=new ExportData{};
            static_cast<ExportData*>(m)->index=info.second;
            static_cast<ExportData*>(m)->type=0;
            static_cast<ExportData*>(m)->label=QString{"name: %1 -%2"}.arg(info.first).arg(QString(tr("model")));
            exportData.push_back(m);
        }
    }

    auto projects=getAllProjectInfo();
    if(projects.size()!=0){
        for(auto&& info:projects){
            QObject* m=new ExportData{};
            auto p=static_cast<ProjectInfoObject*>(info);
            static_cast<ExportData*>(m)->index=p->index();
            static_cast<ExportData*>(m)->type=1;
            static_cast<ExportData*>(m)->label=QString{"name: %1 -%2"}.arg(p->name()).arg(QString("project"));
            exportData.push_back(m);
        }
    }


    return exportData;
}


void DataManager::exportDataToUsb(int index,int type){


    QString fileName;
    if(type==0){
        fileName=DataExporter().exportModel(index);
    }else if(type==1){
        fileName=DataExporter().exportProject(index);
    }

    if(fileName==""){
        LOG(WARNING)<<"export failed index:"<<index<<" type:"<<type;
        return;
    }

    if(emit this->isUsbOnline()==true){

        LOG(INFO)<<"usb online moving to usb:"<<fileName.toStdString();
        emit this->exportToUsb(fileName);
    }else{
        LOG(INFO)<<"usb offline,not moving return";
        return;
    }

}

void DataManager::onSentModelManageModelToInit(ModelManageModel* model){
    QSqlDatabase database = QSqlDatabase::database();

    // 打开数据库
    if (!database.open()) {
        qWarning() << "Failed to open database" << database.lastError();
        return;
    }

    auto indices=this->getModels();
    for(auto& index:indices){
        auto ptr=this->getModelInfoWithId(index);
        model->addModel(index,ptr);
    }

}

std::vector<int> DataManager::getModels(){
    std::vector<int> rec;



    QSqlQuery query;
    query.prepare("SELECT id FROM soilModel");

    if (!query.exec()) {
        qDebug() << "Failed to execute query, error:" << query.lastError().text();
        return rec; // 默认返回模型类型
    }

    while(query.next()){
        rec.push_back(query.value("id").toInt());
    }

    return rec;
}


QString DataManager::getTime() const{
    QString format{"yyyy-MM-dd hh:mm:ss"};

    return QDateTime::currentDateTime().toString(format);
}



#include "datamanager.hpp"
#include "ProjectInfoObject.hpp"
#include "dataexporter.hpp"
#include "modelinfo.hpp"
#include "qdebug.h"
#include <memory>
#include <mutex>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlRecord>
#include<QSqlError>
#include<glog/logging.h>

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
                    "gps VARCHAR"
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

}
int DataManager::getProjectId(QString name)
{
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
    return -1;
}



DataManager::BelongType DataManager::getBelongType(int id)
{
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
    return BelongType::Model; // 默认返回模型类型
}

std::vector<int> DataManager::getPoints(int belongId,BelongType belongType){
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

    return rec;
}

std::shared_ptr<TestPointInfo> DataManager::getPointWithId(int id)
{

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

    return rec;
}

void DataManager::init(){
     initializeDatabase();
    DataExporter().exportModel(4,"./testModel.txt");
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

    query.prepare("INSERT INTO soilModel (id,modelName,createTime,GPS,r,a,b,rW,aW,bW) "
                  "VALUES (:modelName,:maximumDryness,:bestWaterRate,:gps,:argR2,:argA,:argB,:rW,:aW,bW)");

    query.bindValue(":modelName",index);
    query.bindValue(":maximumDryness", model->modelName);
    query.bindValue(":bestWaterRate", QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
    query.bindValue(":gps",QVariant{QVariant::String});
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
}


void DataManager::addProject(QString project,QString createTime,QString gps){
    QSqlQuery query;

    query.exec("SELECT MAX(id) FROM projectInfo;");
    query.next();
    int index=query.value(0).toInt()+1;
    qDebug()<<"model index:"<<index;
    // 构建插入语句

    QString insertQuery = QString("INSERT INTO projectInfo (id,name, createTime, gps) "
                                  "VALUES ('%1', '%2', '%3','%4')")
                              .arg(index)
                              .arg(project)
                              .arg(createTime)
                              .arg(gps);
    // 执行插入语句
    if (!query.exec(insertQuery)) {
        qDebug() << "Failed to add project, error:" << query.lastError().text();
        // 关闭数据库连接或进行其他错误处理
        return;
    }

    // 添加项目成功
    qDebug() << "Project added successfully";
    // 可以进行其他操作或返回适当的成功标识

}

void DataManager::removeProject(int index){

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

    return modelInfoList;
}

std::tuple<QString,QString,QString> DataManager::getProjectInfo(int index){
    auto db=QSqlDatabase::database();
    if(!db.open()){
        LOG(INFO)<<"db not open in:getProjectInfo";
    }
    QSqlQuery query{db};
    query.exec(QString("select name,createTime,gps from projectInfo where id='%1'").arg(index));

    std::tuple<QString,QString,QString> rec;
    while(query.next()){
        std::get<0>(rec)=query.value("name").toString();
        std::get<1>(rec)=query.value("createTime").toString();
        std::get<2>(rec)=query.value("gps").toString();
    }
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
        info->argA = query.value("a").toDouble();
        info->argB = query.value("b").toDouble();
        info->argR2 = query.value("r").toDouble();
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
        return {query.value("a").toFloat(),query.value("b").toFloat(),
                query.value("r").toFloat(),query.value("aW").toFloat(),
                query.value("bW").toFloat(),query.value("rW").toFloat()};
    }
    return {};
}



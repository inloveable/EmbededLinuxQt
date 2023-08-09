
#include "datamanager.hpp"
#include "ProjectInfoObject.hpp"
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
                    "r REAL,"
                    "a REAL,"
                    "b REAL"//拟合系数
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
                    "dryDensity REAL"
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

void DataManager::getPointsWithId(int id, BelongType type)
{

    QSqlQuery query;
    query.prepare("SELECT * FROM measurementPoint WHERE belongType=:type");
    query.bindValue(":id", id);
    query.bindValue(":type",int(type));

    if (!query.exec()) {
        qDebug() << "Failed to execute query, error:" << query.lastError().text();
        return;
    }
    while (query.next()) {
        // 处理查询结果
        int pointId = query.value("pointId").toInt();
        QString pointName = query.value("pointName").toString();
        int belongId = query.value("belongId").toInt();
        int belongType = query.value("belongType").toInt();
        int buildModel = query.value("buildModel").toInt();
        int pointIndex = query.value("pointIndex").toInt();
        qreal density = query.value("density").toReal();
        qreal waterContent = query.value("waterContent").toReal();
        qreal amplitude = query.value("amplitude").toReal();
        qreal phase = query.value("phase").toReal();
        qreal temperature = query.value("temperature").toReal();
        // 处理测点记录
        // ...
    }
}

void DataManager::init(){
     initializeDatabase();
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

    query.prepare("INSERT INTO soilModel (id,modelName,createTime,GPS,r,a,b) "
                  "VALUES (:modelName,:maximumDryness,:bestWaterRate,:gps,:argR2,:argA,:argB)");

    query.bindValue(":modelName",index);
    query.bindValue(":maximumDryness", model->modelName);
    query.bindValue(":bestWaterRate", QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
    query.bindValue(":gps",QVariant{QVariant::String});
    query.bindValue(":argA", model->argA);
    query.bindValue(":argB", model->argB);
    query.bindValue(":argR2", model->argR2);

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
    query.prepare("INSERT INTO measurementPoint (pointIndex, density, waterContent, amplitude, phase,"
                  " temperature, gps, soildity, dryDensity, belongType, belongId) "
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
}


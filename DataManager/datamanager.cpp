
#include "datamanager.hpp"
#include "qdebug.h"
#include <mutex>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlRecord>
#include<QSqlError>
#include<glog/logging.h>



DataManager::DataManager(QObject *parent)
    : QObject{parent}
{
    initializeDatabase();
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
                    "density REAL,"
                    "waterContent REAL,"
                    "amplitude REAL,"
                    "phase REAL,"
                    "temperature REAL,"
                    "r REAL,"
                    "a REAL"
                    ")")) {
        qDebug() << "Failed to create table, error:" << query.lastError().text();

    }
    if (!query.exec("CREATE TABLE measurementPoint ("
                    "pointId INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "pointName TEXT,"
                    "belongId INTEGER,"
                    "belongType INTEGER,"
                    "density REAL,"
                    "waterContent REAL,"
                    "amplitude REAL,"
                    "phase REAL,"
                    "temperature REAL,"
                    "pointIndex INTEGER"
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

void DataManager::Destory(){

}

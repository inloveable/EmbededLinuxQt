
#include "dataexporter.hpp"
#include"datamanager.hpp"
#include"boost/format.hpp"
#include "modelinfo.hpp"
#include "qdebug.h"
#include "qstringliteral.h"
#include<iostream>
#include<fstream>

#if QT_VERSION_MAJOR<6
   #pragma execution_character_set("utf-8")
#endif

DataExporter::DataExporter(QObject *parent)
    : QObject{parent}
{

}



#include <QFile>
#include <QTextStream>
#include <QDebug>

void DataExporter::exportModel(int index, std::string fileName) {
    auto& data = DataManager::getInstance();
    auto model = data.getModelInfoWithId(index);
    auto& modelInfo = *model;
    if (model == nullptr) {
        qDebug() << "nullptr model when export";
        return;
    }

    QFile file(QString::fromStdString(fileName));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file: " << &fileName;
        return;
    }

    if(modelInfo.gps==""){
        modelInfo.gps="";
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    out << QString("\t土壤建模:\n");
    out << QString("\t编号: %1\n").arg(modelInfo.id);
    out << QString("\t模型名称: %1\n").arg(modelInfo.modelName);
    out << QString("\t建立时间: %1\n").arg(modelInfo.createTime);
    out << QString("\tGPS: %1\n").arg(modelInfo.gps);
    out << QString("\t           湿密度         含水率");
    out<<"\n";
    out << QString("\t相关系数r2:  %1         %2\n").arg(modelInfo.argR2, 0, 'f', 2).arg(modelInfo.argR2Water, 0, 'f', 2);
    out << QString("\t拟合系数a,b: %1,%2   %3,%4\n").arg(modelInfo.argA).arg(modelInfo.argAWater).arg(modelInfo.argB).arg(modelInfo.argBWater);
    out << QString("                \t数据点数据");
    out<<"\n";


    qDebug()<<"total point size:"<<modelInfo.testPointSize();
    for(auto i=0;i<modelInfo.testPointSize();++i){
        auto ptr=modelInfo.getTestPoint(i);
        out<<QString("\t测点 %1 湿密度:%2 含水率:%3 幅值:%4 相角:%5 温度:%6 \n")
                   .arg(ptr->index).arg(ptr->density).arg(ptr->waterRate)
                   .arg(ptr->ampitude).arg(ptr->phaseAngle).arg(ptr->temperature);
    }

    file.close();
}

void DataExporter::exportProject(int index,std::string fileName){
    auto& data = DataManager::getInstance();
    auto [name,createTime,gps,dryness]=data.getProjectInfo(index);



    QFile file(QString::fromStdString(fileName));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file: " << &fileName;
        return;
    }



    QTextStream out(&file);
    out.setCodec("UTF-8");

    out << QString("\t工程测试:\n");
    out << QString("\t编号: %1\n").arg(index);
    out << QString("\t工程名称: %1\n").arg(name);
    out << QString("\t建立时间: %1\n").arg(createTime);
    out << QString("\tGPS: %1\n").arg(gps);
    out << QString("\t干密度:%1\n").arg(dryness);


    out << QString("                \t数据点数据");
    out<<"\n";

    const auto pointIds=data.getPoints(index,DataManager::BelongType::Project);

    out << QString("数据点数据:\t测点编号\t模型编号\t湿密度\t含水率\t压实度\t幅值\t相角\tGPS位置\n");
    for (auto&& id : pointIds) {
        auto ptr = data.getPointWithId(id);
        if(ptr==nullptr){
            return;
        }

        out << QString("数据点数据:\t") << ptr->index << "\t" << ptr->modelIndex << "\t"
            << ptr->density << "\t" << ptr->waterRate << "\t"
            << ptr->soildity << "\t"
            << ptr->ampitude << "\t" << ptr->phaseAngle << "\t"
            << ptr->gps << "\n";
    }

    file.close();
}

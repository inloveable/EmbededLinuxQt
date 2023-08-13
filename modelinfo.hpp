
#pragma once


#include"PublicDefs.hpp"
#include "qobject.h"
#include<array>
#include<vector>
#include<memory>
#include<QString>
#include<QMetaType>

class ModelInfo{
public:
    QString modelName;
    float   maximumDryness;
    float   bestWaterRate;

    float   argA=0.0f;
    float   argB=0.0f;
    float   argR2=0.0f;

    float   argBWater=0.0f;
    float   argAWater=0.0f;
    float   argR2Water=0.0f;

    QString createTime;

    int id=-1;

    QString gps;

    std::size_t testPointSize();

    void addTestPoint(const std::shared_ptr<TestPointInfo>& info);
    void deleteTestPoint(int index);

    std::shared_ptr<TestPointInfo> getTestPoint(int index);
private:
    std::vector<std::shared_ptr<TestPointInfo>> testPoints;
};

class TinyModelInfo:public QObject{
    Q_OBJECT
public:
    TinyModelInfo(QObject* parent=nullptr):QObject{parent}{

    }

    Q_PROPERTY(QString modelName READ getModelName WRITE setModelName NOTIFY modelNameChanged)
    Q_PROPERTY(int modelIndex READ getModelIndex WRITE setModelIndex NOTIFY modelIndexChanged)

    Q_INVOKABLE QString getModelName(){
        return modelName;
    };
    Q_INVOKABLE int     getModelIndex(){
        return modelIndex;
    };

    void setModelIndex(int index){
        modelIndex=index;
    }
    void setModelName(QString name){
        modelName=name;
    }
signals:

    void modelNameChanged();
    void modelIndexChanged();
private:
    QString modelName;
    int modelIndex;


};

Q_DECLARE_METATYPE(ModelInfo);

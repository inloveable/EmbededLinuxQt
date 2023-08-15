
#pragma once

#include<QString>
#include <memory>
#include<array>
#include <variant>
#include<vector>
#include<QList>
#include<QMetaType>
class TestPointInfo{
public:


    static std::shared_ptr<TestPointInfo> generate(int index,
                                                  double density,
                                                  double waterRate,
                                                  double ampitude,
                                                  double phaseAngle,
                                                  double temperature,
                                                  bool gps,
                                                  bool isPairing = false)
    {
        auto ptr = std::make_shared<TestPointInfo>();
        ptr->index = index;
        ptr->density = density;
        ptr->waterRate = waterRate;
        ptr->ampitude = ampitude;
        ptr->phaseAngle = phaseAngle;
        ptr->temperature = temperature;
        ptr->gps = gps;
        ptr->isPairing = isPairing;
        return ptr;
    }

    int index;
    float  density;
    float  waterRate;
    float  ampitude;
    float  phaseAngle;
    float  temperature;
    bool    gps;
    bool    isPairing;
    int    pointId=-1;//to updateInfo in database;


    int modelIndex=0;
    float soildity;
    float dryDesity;

    QString gpsStr;

    //for optimization
    bool    isSelected=true;

private:
};




//for template deduction(should nerver be instantiated)
struct ReadPacket{};
struct ReadStatus:public ReadPacket{};
struct ReadData:public ReadPacket{};

class ProjectInfo{
public:
    QString projectName;
    int     projectId;
    QString gps;
    QString createTime;
    qreal   dryness;
    QList<std::shared_ptr<TestPointInfo>> points;
};
Q_DECLARE_METATYPE(ProjectInfo);









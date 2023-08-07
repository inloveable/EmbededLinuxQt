
#pragma once

#include<QString>
#include <memory>
#include<array>
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

    float soildity;
    float dryDesity;



    //for optimization
    bool    isSelected=true;

private:
};

//for template deduction(should nerver be instantiated)
struct ReadPacket{};
struct ReadStatus:public ReadPacket{};
struct ReadData:public ReadPacket{};











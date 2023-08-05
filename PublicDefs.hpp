
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


    //for optimization
    bool    isSelected=true;
private:
};

//0xAA 0x55 framebody 0xAA 0x56 framehead

class SendPacket{
public:
    static std::array<unsigned char,5> readStatus(){
        return {0xAA,0x55,0x01,0x01,(unsigned char)(0xAA+0x55+0x01+0x01)};
    };
    static std::array<unsigned char,5> readArgs(){
        return {0xAA,0x55,0x01,0x02,(unsigned char)(0xAA+0x55+0x01+0x02)};
    };

    enum class HeadType{};
};






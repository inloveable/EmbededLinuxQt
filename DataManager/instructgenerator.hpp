
#pragma once

#include "qmath.h"
#include<array>
#include <complex>
#include <exception>
#include <functional>
#include <type_traits>


#include<PublicDefs.hpp>

#include<glog/logging.h>
class InstructGenerator
{
public:
    InstructGenerator();



    template<typename T>
    inline std::enable_if_t<std::is_base_of_v<ReadPacket,T>,std::array<unsigned char,8>>
    getInstruction();


    void parseData(std::array<unsigned char,8>& data);


    std::function<void(int)>                  batteryCb;
    std::function<void(float)>                temperatureCb;
    std::function<void(float,float,float)>    argCb;
    std::function<void(bool,bool,bool)>       statusCb;

private:

    template<unsigned char N>
    inline void    read(std::array<unsigned char,8>& data);




};

template<typename T>
inline std::enable_if_t<std::is_base_of_v<ReadPacket,T>,std::array<unsigned char,8>>
InstructGenerator::getInstruction(){
    std::array<unsigned char,8> data{0x00};
    data[0]=0xAA;data[1]=0x56;
    if constexpr(std::is_same_v<T,ReadStatus>){
        data[2]=0x01;
    }else if constexpr(std::is_same_v<T,ReadData>){
        data[2]=0x02;
    }
    data[7]=data[0]+data[1]+data[2];
    return data;
}

template<>
inline void InstructGenerator::read<0x01>(std::array<unsigned char, 8>& data) {
    try{
        statusCb(data[3],data[4],data[5]);
    }catch(...){

    }

}

template<>
inline void InstructGenerator::read<0x02>(std::array<unsigned char, 8>& data) {
    // 读取阻抗
    unsigned char A31to24 = data[3];  // 假设 A31~24 位在第4个字节
    unsigned char A23to16 = data[4];  // 假设 A23~16 位在第5个字节
    unsigned char A15to8 = data[5];   // 假设 A15~8 位在第6个字节
    unsigned char A7to0 = data[6];    // 假设 A7~0 位在第7个字节
    // 根据获取到的数据执行针对阻抗的操作

    float real_data = (A31to24<<8)| A23to16;
    float img_data = (A15to8<<8)| A7to0;

   // std::complex<float> val;//conflict between compliers
    //val._Val[0]=real_data;
    ///val._Val[1]=img_data;
    ///
    auto mag=sqrt(std::pow(real_data,2)+std::pow(img_data,2));
    auto alpha =atan(img_data/ real_data);

    if(real_data<0 &&img_data>0){
        alpha = atan((float)img_data/(float)real_data)/M_PI*180+180;
    }
    if (real_data<0 &&img_data<0){
        alpha = atan((float)img_data/(float)real_data)/M_PI*180+180;
    }
    if (real_data>0 &&img_data<0){
        alpha = atan((float)img_data/(float)real_data)/M_PI*180+360;
    }

    auto Amp =1/ mag* 10000;

    try{
        argCb(mag,alpha,Amp);
    }catch(std::exception& ex){
        LOG(WARNING)<<"arg cb not set";
    }
}

template<>
inline void InstructGenerator::read<0x03>(std::array<unsigned char, 8>& data) {
    // 读取温度
    unsigned char A31to24 = data[3];  // 假设 A31~24 位在第4个字节
    unsigned char A23to16 = data[4];  // 假设 A23~16 位在第5个字节
    // 根据获取到的数据执行针对温度的操作

    int data1 = (A31to24<<8)| A23to16;
    float part_fraction= ((data1 & 0x00FF) >> 4) * 0.0625;
    int part_int = (data1 >> 8) & 0X7F;
    float tj;
    if(data1 & 0x8000){
        tj = (127-part_int)+(1-part_fraction);
    }else{
        tj = part_int+part_fraction;
    }
    LOG(INFO)<<"recv temperature:"<<tj;

    try{
        temperatureCb(tj);
    }catch(std::exception& ex){
        LOG(WARNING)<<"temperature cb not set";
    }
}

template<>
inline void InstructGenerator::read<0x04>(std::array<unsigned char, 8>& data) {
    // 读取电池电量
    unsigned char A31to24 = data[3];  // 假设 A31~24 位在第4个字节
    // 根据获取到的数据执行针对电池电量的操作
    int val=A31to24;
    LOG(INFO)<<"recv battery:"<<val<<"%";



    try{
        batteryCb(val);
    }catch(std::exception& ex){
        LOG(WARNING)<<"battery cb not set";
    }
}


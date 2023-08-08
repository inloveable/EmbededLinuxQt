
#include "modelinfo.hpp"
#include <memory>



std::size_t ModelInfo::testPointSize(){
    return this->testPoints.size();
}

void ModelInfo::addTestPoint(const std::shared_ptr<TestPointInfo>& info){
    this->testPoints.push_back(info);
}

std::shared_ptr<TestPointInfo> ModelInfo::getTestPoint(int index){
    if(index<testPoints.size()){
        return testPoints[index];
    }
    return nullptr;
}

void ModelInfo::deleteTestPoint(int index){
    auto iter=testPoints.begin()+index;
    testPoints.erase(iter);
}


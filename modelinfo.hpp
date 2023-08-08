
#pragma once


#include"PublicDefs.hpp"
#include<array>
#include<vector>
#include<memory>
#include<QString>

class ModelInfo{
public:
    QString modelName;
    float   maximumDryness;
    float   bestWaterRate;


    std::size_t testPointSize();

    void addTestPoint(const std::shared_ptr<TestPointInfo>& info);
    void deleteTestPoint(int index);

    std::shared_ptr<TestPointInfo> getTestPoint(int index);
private:
    std::vector<std::shared_ptr<TestPointInfo>> testPoints;
};

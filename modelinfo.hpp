
#pragma once


#include"PublicDefs.hpp"
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

    std::size_t testPointSize();

    void addTestPoint(const std::shared_ptr<TestPointInfo>& info);
    void deleteTestPoint(int index);

    std::shared_ptr<TestPointInfo> getTestPoint(int index);
private:
    std::vector<std::shared_ptr<TestPointInfo>> testPoints;
};

Q_DECLARE_METATYPE(ModelInfo);

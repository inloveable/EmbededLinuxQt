
#include "testpointmodel.hpp"

#include"PublicDefs.hpp"
#include "qnamespace.h"
#include <algorithm>
#include <memory>
#include<glog/logging.h>
#include<QDebug>
TestPointModel::TestPointModel(QObject *parent)
    : QAbstractListModel(parent)
{
    auto index=getAvailableIndex();
    sequence[index]=TestPointInfo::generate(index,5.2,0.4,8000,72,36.3,false);

    index=getAvailableIndex();
    sequence[index]=TestPointInfo::generate(index,6.2,24,7000,72,35.3,true);


}

Qt::ItemFlags TestPointModel::flags(const QModelIndex &index) const{
    return Qt::ItemIsEditable|Qt::ItemIsSelectable;
}

int TestPointModel::getAvailableIndex(){

    if(sequence.size()==0){
        return 0;
    }

    auto keys=sequence.keys();
    auto max=std::max_element(keys.begin(),keys.end());
    return (*max)+1;
}

void TestPointModel::resetIndex(){


    const int count=sequence.size();

    QMap<int,std::shared_ptr<TestPointInfo>> map;
    beginResetModel();
    auto iter=sequence.begin();
    for(int i=0;i<count;++i){
        map[i]=iter.value();
        iter++;
    }
    sequence=map;
    endResetModel();


}

bool TestPointModel::add(const std::shared_ptr<TestPointInfo>& inf){
    if(sequence.constFind(inf->index)!=sequence.cend()){

        LOG(INFO)<<"error when add model data,duplicate index";
        return false;
    }
    sequence[inf->index]=inf;
    return true;
}


bool TestPointModel::setData(const QModelIndex& index,const QVariant& data,int role){

    qDebug()<<"setting data:"<<role;
    if (role == WaterRateRole) {
        // Set data in model here. It can also be a good idea to check whether
        // the new value actually differs from the current value
           sequence[index.row()]->waterRate=data.toDouble();

       emit dataChanged(index, index, { Qt::EditRole, WaterRateRole });
            return true;

    }
    return false;
}

QVariant TestPointModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    return QVariant{};
}

int TestPointModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!

    return sequence.size();
}

QVariant TestPointModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid())
        return QVariant();

    auto row=index.row();
    if(sequence.constFind(row)==sequence.cend()){
        return QVariant();
    }
    const auto& ptr=sequence[row];

    switch (role) {
    case IndexRole:
        return ptr->index;
    case DensityRole:
        return ptr->density;
    case WaterRateRole:
        return ptr->waterRate;
    case AmpitudeRole:
        return ptr->ampitude;
    case PhaseAngleRole:
        return ptr->phaseAngle;
    case TemperatureRole:
        return ptr->temperature;
    case GpsRole:
        return ptr->gps;
    case IsPairingRole:
        return ptr->isPairing;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> TestPointModel::roleNames() const{


    QHash<int, QByteArray> roles;
    roles[IndexRole] = "index";
    roles[DensityRole] = "density";
    roles[WaterRateRole] = "waterRate";
    roles[AmpitudeRole] = "ampitude";
    roles[PhaseAngleRole] = "phaseAngle";
    roles[TemperatureRole] = "temperature";
    roles[GpsRole] = "gps";
    roles[IsPairingRole] = "isPairing";
    return roles;
}


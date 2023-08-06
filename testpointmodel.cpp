
#include "testpointmodel.hpp"

#include"PublicDefs.hpp"
#include "qabstractseries.h"
#include "qnamespace.h"
#include "qxyseries.h"
#include "seriespointswaper.hpp"
#include "serviceprovider.hpp"
#include "serviceproviderprivate.hpp"
#include <algorithm>
#include <memory>
#include<glog/logging.h>
#include<QDebug>
#include <vector>
TestPointModel::TestPointModel(QObject *parent)
    : QAbstractListModel(parent)
{
    auto index=getAvailableIndex();
    sequence[index]=TestPointInfo::generate(index,5.2,22,8000,32,36.3,false);

    index=getAvailableIndex();
    sequence[index]=TestPointInfo::generate(index,6.2,48,7000,12,35.3,true);
    index=getAvailableIndex();
    sequence[index]=TestPointInfo::generate(index,6.2,92,9000,73,48.3,true);


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



    if (role == WaterRateRole) {
        // Set data in model here. It can also be a good idea to check whether
        // the new value actually differs from the current value
           sequence[index.row()]->waterRate=data.toDouble();
       emit dataChanged(index, index, { Qt::EditRole, WaterRateRole });
            return true;
    }
    if(role==isSelectedRole){

            if(data.toBool()==sequence[index.row()]->isSelected){
                return false;
            }
            sequence[index.row()]->isSelected=data.toBool();
            emit dataChanged(index, index, { Qt::EditRole, isSelectedRole });
            //switch direction
            bool isReverse=sequence[index.row()]->isSelected?true:false;


            emit requireSeiresSwap(index.row(),isReverse);

            return true;
    }
    return false;
}

//from and to are decided by qml codes
void TestPointModel::swapSeriesPoint(QtCharts::QXYSeries* from,QtCharts::QXYSeries* to,int pointIndex,int type){

    const auto& info=sequence.constFind(pointIndex);
    SeriesPointSwaper swaper;
    auto pointer=info.value();
    if(type==0){//density
        swaper.swapPoint(pointer->phaseAngle,pointer->density,from,to);
    }else if(type==1){
        swaper.swapPoint(pointer->ampitude,pointer->waterRate,from,to);
    }
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
    {
        auto t=ptr->density;
        QString result = QString::number(t, 'f', 4);
        return result;
    }
    case WaterRateRole:
        return ptr->waterRate;
    case AmpitudeRole:
        return ptr->ampitude;
    case PhaseAngleRole:
        {
                auto t=ptr->phaseAngle;
                QString result = QString::number(t, 'f', 4);
                return result;
        }

    case TemperatureRole:
    {
        auto t=ptr->temperature;
        QString result = QString::number(t, 'f', 4);
        return result;
    }
    case GpsRole:
        return ptr->gps;
    case IsPairingRole:
        return ptr->isPairing;
    case isSelectedRole:
        return ptr->isSelected;
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
    roles[isSelectedRole]="isSelected";
    return roles;
}

void TestPointModel::setChecked(QPointF point,bool val,int type){

    qDebug()<<"set chekced:"<<point;

    QPointF& clickedPoint = point;

    qreal distance(INT_MAX);

    auto closeIter=sequence.end();

    float x;
    float y;
    for (auto i=sequence.begin();i!=sequence.end();++i) {

        auto pointer=i.value();
        if(type==0){
                x=pointer->phaseAngle;
                y=pointer->density;
        }else if(type==1){
                x=pointer->ampitude;
                y=pointer->waterRate;
        }
        const auto currentPoint=QPointF{x,y};
        qreal currentDistance = qSqrt((currentPoint.x() - clickedPoint.x())
                                          * (currentPoint.x() - clickedPoint.x())
                                      + (currentPoint.y() - clickedPoint.y())
                                            * (currentPoint.y() - clickedPoint.y()));
        if (currentDistance < distance) {
                distance = currentDistance;
                closeIter=i;
        }
    }

    auto modelIndex=this->index(closeIter.key(),0);

    this->setData(modelIndex,QVariant{val},isSelectedRole);

}


void TestPointModel::updateSeries(QtCharts::QXYSeries* from ,
                                  QtCharts::QXYSeries* mask,int type){
    QList<QPointF> selected;
    QList<QPointF> notSeleted;

    notSeleted.push_back(QPointF{10000,0});//dummy point to avoid crush when click on chart with qt5.15
    selected.push_back(QPointF{10000,0});

    qreal xmin=0;
    qreal xmax=0;
    qreal ymin=0;
    qreal ymax=0;

    for(auto&& p:sequence){
        if(p->isSelected){
            if(type==0){//density
                selected.push_back(QPointF{p->phaseAngle,p->density});
                if(p->phaseAngle>xmax)xmax=p->phaseAngle;
                if(p->density>ymax)ymax=p->density;
                if(p->phaseAngle<xmin)xmin=p->phaseAngle;
                if(p->density<ymin)ymin=p->density;
            }
            else{//waterRate
                selected.push_back(QPointF{p->ampitude,p->waterRate});
                if(p->ampitude>xmax)xmax=p->ampitude;
                if(p->waterRate>ymax)ymax=p->waterRate;
                if(p->ampitude<xmin)xmin=p->ampitude;
                if(p->waterRate<ymin)ymin=p->waterRate;
            }
        }else{
            if(type==0){
                notSeleted.push_back(QPointF{p->phaseAngle,p->density});
                if(p->phaseAngle>xmax)xmax=p->phaseAngle;
                if(p->density>ymax)ymax=p->density;
                if(p->phaseAngle<xmin)xmin=p->phaseAngle;
                if(p->density<ymin)ymin=p->density;
            }else{
                notSeleted.push_back(QPointF{p->ampitude,p->waterRate});
                if(p->ampitude>xmax)xmax=p->ampitude;
                if(p->waterRate>ymax)ymax=p->waterRate;
                if(p->ampitude<xmin)xmin=p->ampitude;
                if(p->waterRate<ymin)ymin=p->waterRate;
            }
        }

    }
    auto predicate=[](QPointF p1,QPointF p2){return p1.x()<p2.x();};
    std::sort(selected.begin(),selected.end(),predicate);
    std::sort(notSeleted.begin(),notSeleted.end(),predicate);

    fillSeries(from,selected);
    fillSeries(mask,notSeleted);

    emit updateAxis(xmin-100,xmax+100,ymin-10,ymax+10);
}

void TestPointModel::fillSeries(QtCharts::QXYSeries* series,
                                QList<QPointF>& points){
    series->replace(points);
}

void TestPointModel::getFitSequence(QtCharts::QXYSeries* series,int type){
    std::vector<double> x;
    std::vector<double> y;
    for(auto&& p:sequence){
        if(p->isSelected){
            if(type==0){//density
                x.push_back(p->phaseAngle);
                y.push_back(p->density);
            }
            else if(type==1){//waterRate
                x.push_back(p->ampitude);
                y.push_back(p->waterRate);
            }
        }
    }
    if(x.size()<2||y.size()<2){
        LOG(INFO)<<"fit sequence point size <2";
        return;
    }
    auto predicate=[](double p1,double p2){return p1<p2;};
    std::sort(x.begin(),x.end(),predicate);
    std::sort(y.begin(),y.end(),predicate);


    //get fit args
    auto [c0,c1,r2]=ServiceProvider::linearRegression(x,y);
    if(type==0){
        this->densityR_Squrare=r2;
    }else if(type==1){
        this->waterRateR_Square=r2;
    }

    //a=sqrt(r2)

   // currentFitA=c1;
    //currentFitB=c0;
    //currentFitR=std::sqrt(r2);

    //generate chart line
    QList<QPointF> fitSequence;
    double y1;
    for(int i=0;i<x.size();++i){
        y1=c1*x[i]+c0;
        fitSequence.push_back(QPointF{x[i],y1});
    }
    LOG(INFO)<<"fit result:y="<<c1<<"*x + ("<<c0<<"); r2:"<<r2;
    fillSeries(series,fitSequence);
}

void TestPointModel::findPointAndCheck(QPointF point,bool check,int type){

}



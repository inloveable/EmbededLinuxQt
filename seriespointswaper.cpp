
#include "seriespointswaper.hpp"
#include "qabstractseries.h"
#include "qscatterseries.h"
#include <exception>
#include <iostream>

SeriesPointSwaper::SeriesPointSwaper(QObject *parent)
    : QObject{parent}
{

}


void SeriesPointSwaper::swapPoint(double x,double y,QtCharts::QXYSeries* from,QtCharts::QXYSeries* to){

    QPointF clickedPoint = QPointF{x,y};
    QPointF closest(INT_MAX, INT_MAX);
    qreal distance(INT_MAX);
     auto points = from->points();

    auto toPoints=to->points();

     for (int i=0;i<points.size();++i) {
        const auto& currentPoint=points[i];
        qreal currentDistance = qSqrt((currentPoint.x() - clickedPoint.x())
                                          * (currentPoint.x() - clickedPoint.x())
                                      + (currentPoint.y() - clickedPoint.y())
                                            * (currentPoint.y() - clickedPoint.y()));
        if (currentDistance < distance) {
            distance = currentDistance;
            closest = currentPoint;
        }
    }
    toPoints.append(closest);
    points.removeOne(closest);

    std::sort(toPoints.begin(),toPoints.end(),[](QPointF& p1,QPointF& p2){
        return p1.x()<p2.x();
    });//不排序会崩溃

    std::sort(points.begin(),points.end(),[](QPointF& p1,QPointF& p2){
        return p1.x()<p2.x();
    });//不排序会崩溃
    from->replace(points);
    to->replace(toPoints);
}




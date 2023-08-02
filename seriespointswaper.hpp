
#pragma once




#include <QObject>
#include<QtQml>
#include<QScatterSeries>
class SeriesPointSwaper : public QObject
{
    Q_OBJECT
    QML_ELEMENT
        public:
   explicit SeriesPointSwaper(QObject *parent = nullptr);

  Q_INVOKABLE     void swapPoint(double x,double y,QtCharts::QXYSeries* from,QtCharts::QXYSeries* to);

signals:

private:



};



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

   Q_INVOKABLE void swapPoint(float x,float y,QtCharts::QXYSeries* from,QtCharts::QXYSeries* to);

signals:

private:



};


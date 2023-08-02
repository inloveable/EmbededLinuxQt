
#pragma once


#include <QObject>
#include<QtQuick>

class TimeProvider : public QObject
{
    Q_OBJECT

    QML_ELEMENT
public:
    explicit TimeProvider(QObject *parent = nullptr);


    QString getTime() const;
signals:
    void sendTime(QString);
private:

    QString readFromConfig();

};


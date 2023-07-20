
#pragma once


#include "qnetworkaccessmanager.h"
#include "qobjectdefs.h"
#include <QObject>


class ServiceProviderPrivate : public QObject
{
    Q_OBJECT
public:
    explicit ServiceProviderPrivate(QObject *parent = nullptr);

    Q_INVOKABLE void init();


    Q_INVOKABLE void getTime();
signals:

    void sendTime(QString,bool);
private:
    QNetworkAccessManager* manager;

};


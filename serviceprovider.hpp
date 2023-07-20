
#pragma once


#include "qobjectdefs.h"
#include "qqml.h"
#include <QObject>

class ServiceProviderPrivate;
class ServiceProvider : public QObject
{
    Q_OBJECT
    QML_ELEMENT//D:/QtCodes/TouchScreen0/qtquicktest/qtquicktest/config.json
public:
    explicit ServiceProvider(QObject *parent = nullptr);
    ~ServiceProvider();


    Q_INVOKABLE void getTime();

    Q_INVOKABLE void messageBox(QString text);



signals:

    void sendTime(QString,bool);
private:
    QThread* serviceThread;
    ServiceProviderPrivate* d;


};


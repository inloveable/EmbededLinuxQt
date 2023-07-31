
#pragma once


#include "qobjectdefs.h"
#include "qqml.h"
#include <QObject>

#include<complex>
class TestPointModel;
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


    Q_INVOKABLE TestPointModel* getTestPointModel();

    Q_INVOKABLE TestPointModel* getTestPointModel_Ex(const QString& name);




signals:

    void sendTime(QString,bool);
private:
    QThread* serviceThread;
    ServiceProviderPrivate* d;

    TestPointModel* tModel=nullptr;

    void onModelReady();
    QMap<QString,TestPointModel*> modelMap;

    static std::pair<double,double> linearRegression(
        const std::vector<double>& x,
        const std::vector<double>& y);

};


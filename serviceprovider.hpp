
#pragma once


#include "qobjectdefs.h"
#include "qqml.h"
#include <QObject>


class TestPointModel;
class ServiceProviderPrivate;
class ServiceProvider : public QObject
{
    Q_OBJECT
    QML_ELEMENT//D:/QtCodes/TouchScreen0/qtquicktest/qtquicktest/config.json

    Q_PROPERTY(bool usbOnline READ usbOnline NOTIFY usbOnlineChanged)
        public:
                 explicit ServiceProvider(QObject *parent = nullptr);
    ~ServiceProvider();


    Q_INVOKABLE void getTime();

    Q_INVOKABLE void messageBox(QString text);


    Q_INVOKABLE TestPointModel* getTestPointModel();

    Q_INVOKABLE TestPointModel* getTestPointModel_Ex(const QString& name);
    Q_INVOKABLE void            requestProjectInfo();

    Q_INVOKABLE void selfCheck();


    bool  usbOnline()const{return hasUsb;};

    static std::tuple<double,double,double> linearRegression(
        const std::vector<double>& x,
        const std::vector<double>& y);

    static qreal absDistance(QPointF& p,QPointF p1);

signals:

    void sendTime(QString,bool);


    void sendProjectInfo(QList<QObject*> infos);
    Q_INVOKABLE void projectInfoNeedsUpdate();

    void usbOnlineChanged();

    void selfCheckFinished(bool);
private:
    QThread* serviceThread;
    ServiceProviderPrivate* d;

    TestPointModel* tModel=nullptr;

    void onModelReady();
    QMap<QString,TestPointModel*> modelMap;


    void callBackend(const QString& func);


    qint64 selfCheckExpireTime=100000;//10s;

    bool hasUsb=false;
};


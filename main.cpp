#include <QApplication>
#include <QQmlApplicationEngine>
#include "qqml.h"
#include "qscopedpointer.h"

#include<QFile>

#include<QThread>
#include"serviceprovider.hpp"
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);


    QScopedPointer<ServiceProvider> privider{new ServiceProvider};

    qmlRegisterSingletonInstance("CppCore",1,0,"Service",privider.get());



    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    //engine.addImageProvider(QLatin1String("Uis"), new UIImageProvider);
    engine.load(url);

    return app.exec();
}

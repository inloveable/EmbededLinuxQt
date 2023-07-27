#include <QApplication>
#include <QQmlApplicationEngine>
#include "qdir.h"
#include "qqml.h"
#include "qscopedpointer.h"

#include<QFile>

#include<QThread>
#include "seriespointswaper.hpp"
#include"serviceprovider.hpp"
#include "testpointmodel.hpp"

#include<glog/logging.h>
#include <iostream>
void initGlog(int argc,char** argv);
int main(int argc, char *argv[])
{

#ifdef __linux__
    // Linux-specific code

#elif _WIN32
    // Windows-specific code
     //qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
#else
    // Unsupported platform
#endif


    initGlog(argc,argv);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);


    QScopedPointer<ServiceProvider> privider{new ServiceProvider};

    qmlRegisterSingletonInstance("CppCore",1,0,"Service",privider.get());
    qmlRegisterType<TestPointModel>("CppCore",1,0,"TestPointModel");
    qmlRegisterType<SeriesPointSwaper>("CppCore",1,0,"SeriesPointSwaper");




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
void SinalHandle(const char* message,std::size_t len)
{
    std::cerr<<"glog error:";
    std::cerr.write(message,len);
    std::cerr<<"\n";
}

void initGlog(int argc,char** argv){

    QDir dir{"/usr"};
    dir.mkdir("log");
    google::InitGoogleLogging(argv[0]);

    google::EnableLogCleaner(3);
    google::SetLogFilenameExtension(".txt");


    FLAGS_alsologtostderr = 1;
    FLAGS_minloglevel = google::GLOG_INFO;
    FLAGS_log_dir = "./log";
    FLAGS_colorlogtostderr = true;//是否启用不同颜色显示(如果终端支持)
    google::InstallFailureSignalHandler();
    google::InstallFailureWriter(&SinalHandle);
}

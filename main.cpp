#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDir>
#include <QtQml>
#include <QScopedPointer>
#include<QFile>

#include<glog/logging.h>
#include <iostream>
#include<QThread>


#include "seriespointswaper.hpp"
#include"serviceprovider.hpp"
#include "testpointmodel.hpp"
#include"modelmanagemodel.hpp"
#include"DataManager/datamanager.hpp"


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


    LOG(INFO)<<"app running dir:"<<QApplication::applicationFilePath().toStdString();





    QTranslator transZH;
    QTranslator transEN;




    transEN.load(":/En_Us.qm");
    transZH.load(":/Zh_CN.qm");

    QFile file{"/root/config.json"};
    if(file.open(QIODevice::ReadOnly)){
        qDebug()<<"config file exists";
        QJsonDocument doc=QJsonDocument::fromJson(file.readAll());
        auto obj=doc.object();
        auto lang=obj["language"].toString();
        if(lang!=""){
            if(lang=="ZH"){
                app.installTranslator(&transZH);
            }else if(lang=="EN"){
                app.installTranslator(&transEN);
            }
        }
    }else{
        app.installTranslator(&transZH);
    }




    qRegisterMetaType<QList<QObject*>>();
    qmlRegisterType<TestPointModel>("CppCore",1,0,"TestPointModel");
    qmlRegisterType<ModelManageModel>("CppCore",1,0,"ManageModel");

    qmlRegisterType<SeriesPointSwaper>("CppCore",1,0,"SeriesPointSwaper");

    DataManager::getInstance();//init datamanager
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

    QObject::connect( privider.get(),&ServiceProvider::requsetTranslate,&transZH,[&transZH,&engine,&app,&transEN](QString lan){

        qDebug()<<"translating:"<<lan;
        if(lan=="ZH"){
            qApp->removeTranslator(&transEN);
           qDebug()<<qApp->installTranslator(&transZH);

        }else if(lan=="EN"){
           qApp->removeTranslator(&transZH);
           qDebug()<<qApp->installTranslator(&transEN);
        }


        engine.retranslate();

    });

    auto rec=app.exec();

    DataManager::getInstance().Destory();
    return rec;
}
void SinalHandle(const char* message,std::size_t len)
{
    std::cerr<<"glog error:";
    std::cerr.write(message,len);
    std::cerr<<"\n";
}

void initGlog(int argc,char** argv){

    Q_UNUSED(argc)
    QDir dir{"/usr"};
    dir.mkdir("log");
    google::InitGoogleLogging(argv[0]);

    //google::EnableLogCleaner(3);
    //google::SetLogFilenameExtension(".txt");


    FLAGS_alsologtostderr = 1;
    FLAGS_minloglevel = google::GLOG_INFO;
    FLAGS_log_dir = "/usr/log";
    FLAGS_colorlogtostderr = true;//是否启用不同颜色显示(如果终端支持)
    //google::InstallFailureSignalHandler();
    //google::InstallFailureWriter(&SinalHandle);
}

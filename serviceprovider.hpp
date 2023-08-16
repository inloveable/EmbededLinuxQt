
#pragma once


#include "qobjectdefs.h"
#include "qqml.h"
#include <QObject>
#include <memory>


class TestPointModel;
class ServiceProviderPrivate;
class ModelInfo;
class ProjectInfo;
class TinyModelInfo;
class ServiceProvider : public QObject
{
    Q_OBJECT
    QML_ELEMENT//D:/QtCodes/TouchScreen0/qtquicktest/qtquicktest/config.json

    Q_PROPERTY(bool usbOnline READ usbOnline NOTIFY usbOnlineChanged)
    Q_PROPERTY(int batteryVal READ getCurrentBattery NOTIFY batteryChanged);

        public:
                 explicit ServiceProvider(QObject *parent = nullptr);
    ~ServiceProvider();


    Q_INVOKABLE void getTime();
    Q_INVOKABLE QString getGps();
    Q_INVOKABLE void messageBox(QString text);
    Q_INVOKABLE TestPointModel* getTestPointModel();

    Q_INVOKABLE void selfCheck();


    //create new model apis
    Q_INVOKABLE void prepareCreateNewModel();
    Q_INVOKABLE void deleteNewModelTestPoint(int index);
    Q_INVOKABLE void addNewModelTestPoint();
    Q_INVOKABLE void saveNewModel();
    Q_INVOKABLE void createNewModelExit();
    Q_INVOKABLE void setModelName(QString name);



    Q_INVOKABLE int getCurrentBattery();
    Q_INVOKABLE void requestPointInfoUpdate(int index);//配对
    Q_INVOKABLE void requestPointTest();//测试
    Q_INVOKABLE void requestPointTest(int index);//测试(project)

    //projectInfoApis
    Q_INVOKABLE void            requestProjectInfo();
    Q_INVOKABLE void            addProject(QString name,qreal dryness);
    Q_INVOKABLE void            removeProject(int index);

    Q_INVOKABLE void            prepareProject(int index);
    Q_INVOKABLE TestPointModel* getTestPointModel_Project();
    Q_INVOKABLE void            projectInfoSave();
    Q_INVOKABLE void            projectInfoExit();
    Q_INVOKABLE void            addProjectTestPoint();
    Q_INVOKABLE void            removeProjectTestPoint(int index);

    Q_INVOKABLE void            requestTinyModelInfo();
    Q_INVOKABLE QList<QObject*> getTinyModelInfos(){
        return tinyModelInfos;
    }

    Q_INVOKABLE void            refreshTestPointDataWithModel();

    Q_INVOKABLE void            setPointToModel(int pointI,int modelI);

    Q_INVOKABLE QList<QObject*> getExportInfos();
    Q_INVOKABLE void exportData(int index,int type);


    /*------------------------------------------------------------*/

    bool  usbOnline()const{return hasUsb;};

    static std::tuple<double,double,double> linearRegression(
        const std::vector<double>& x,
        const std::vector<double>& y);
    static float getFitValue(qreal a,qreal b,qreal x){
        return a*x+b;
    };

    static qreal absDistance(QPointF& p,QPointF p1);

signals:

    void sendTime(QString,bool);

    void exportInfoReady();



    //projectInfo apis
    void sendProjectInfo(QList<QObject*> infos);
    Q_INVOKABLE void projectInfoNeedsUpdate();
    void tinyModelInfoUpdated();
    //---------------------------------------------

    void usbOnlineChanged();

    void selfCheckFinished(bool);

    //backend signals;
    void saveModelInfo(std::shared_ptr<ModelInfo> info);
    void batteryChanged();


    void requestParingComplete(bool,int);

    void sendProjectInfoToInit(ProjectInfo*,TestPointModel*);
    void saveProjectInfo(ProjectInfo* info);

    std::tuple<float,float,float,float,float,float> getModelArgWithId(int index);


    QList<QObject*>  requestExportData();



    QString requestGps();



private:
    QThread* serviceThread;
    ServiceProviderPrivate* d;
    void initPrivateSignals();



    void onModelReady();
    QMap<QString,TestPointModel*> modelMap;


    void callBackend(const QString& func);


    qint64 selfCheckExpireTime=100000;//10s;

    bool hasUsb=false;
    bool modelSaved=false;


    std::unique_ptr<ModelInfo> modelInfo;
    std::unique_ptr<ProjectInfo> projectInfo;
    TestPointModel* tModel;
    TestPointModel* projectTestPointModel=nullptr;

    QMap<int,int> pointIndexToModelIMap;

    int batteryValue=100;

    QList<QObject*> tinyModelInfos;




    bool isProject;



};



#pragma once


#include "qnetworkaccessmanager.h"
#include "qobjectdefs.h"
#include <QObject>
#include <mutex>
#include<memory>
class TestPointModel;
class DeviceManager;
class ModelInfo;
class ServiceProviderPrivate : public QObject
{
    Q_OBJECT
public:
    explicit ServiceProviderPrivate(QObject *parent = nullptr);

    Q_INVOKABLE void init();


    Q_INVOKABLE void getTime();


    //obsolete
    Q_INVOKABLE void generateModel(QString name);
    TestPointModel*  getPreparedModel();


    Q_INVOKABLE void removeProject(int index);
    Q_INVOKABLE void requestProjectInfo();
    Q_INVOKABLE void addProject(QString project);

    Q_INVOKABLE void exportData();


    void saveModelInfo(std::shared_ptr<ModelInfo>);




    //data conduct
    void onTemperature(float temp);
    void onBatterySent(int battery);
    void onArgsSent(float amp,float phaseAngle);

    class DataField{
    public:
        DataField(int ointIndex):pointIndex{ointIndex}{}
        bool isReady() {
            return count==2;
        };

        void reset() {
            count=0;
        }

        bool setValue(float val){
            if(count==2){
                return false;
            }
            temperatures[count++]=val;
            return true;
        }
        float getValue(){
            float tmp=0.0f;
            for(int i=0;i<2;++i){
                tmp+=temperatures[i];
            }
            return tmp/2;
        }
        int getPointIndex(){return pointIndex;};

    private:
        float temperatures[2]{0.0f};
        int count=0;
        int pointIndex;
    };

    Q_INVOKABLE void requestPointInfoUpdate(int index);//配对
    Q_INVOKABLE void requestPointTest();//测试


signals:

    void sendTime(QString,bool);
    void modelReady();




    void sendProjectInfo(QList<QObject*> list);
    void projectInfoNeedsUpdate();





    void usbLoaded();
    void usbUnloaded();

    void batteryValueUpdated(int);

    void temperatureValueRecv(int pointIndex);//first time//
    void temperatureValueReady(float val,int pointIndex);//second time;

    void argsValueReady(float amp,float angle,int pointIndex);

    void pointParingComplete(bool,int index);

    void sendStatus(bool ,bool ,bool);
private:
    QNetworkAccessManager* manager;

    std::mutex modelMutex;
    TestPointModel* prepared=nullptr;

    QList<QObject*> projectInfoBuffer;



    DeviceManager* devices=nullptr;

    int usbNum=0;

    int currentRequestPointIndex=-1;


    bool temperatureDone=false;
    bool argsDone=false;


};



#pragma once


#include <QObject>

#include<mutex>
#include<memory>
class TestPointInfo;
class ModelInfo;
class DataManager : public QObject
{
    Q_OBJECT
public:

//call datamanager in service thread
    static DataManager& getInstance(){
        static DataManager* manager=nullptr;
        if(manager==nullptr){
            manager=new DataManager;
        }
        return *manager;
    }
    ~DataManager(){

    };
    void Destory();

    Q_INVOKABLE void init();

    void saveModelInfo(const std::shared_ptr<ModelInfo>& info);

    enum class BelongType{Model=0,Project=1};
    void saveTestPoint(const std::shared_ptr<TestPointInfo>& point,
                       BelongType belong,int belongId);
    std::vector<int> getPoints(int belongId,BelongType belongType);
    std::shared_ptr<TestPointInfo> getPointWithId(int id);



    Q_INVOKABLE QList<QPair<QString,int>> getModelInfoFromDb();


    //ProjectInfoObject*
    QList<QObject*> getAllProjectInfo();
    void            addProject(QString project,QString createTime,QString gps);
    void            removeProject(int index);


    std::tuple<QString,QString,QString> getProjectInfo(int index);


signals:
private:
    explicit DataManager(QObject *parent = nullptr);
    void initializeDatabase();

    std::mutex sqlmu;



    int  getProjectId(QString name);
    int  getSoilModelId(QString name);




    BelongType getBelongType(int id);

};


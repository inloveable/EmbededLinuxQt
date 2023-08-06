
#pragma once


#include <QObject>

#include<mutex>
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


    //ProjectInfoObject*
   QList<QObject*> getAllProjectInfo();


signals:
private:
    explicit DataManager(QObject *parent = nullptr);
    void initializeDatabase();

    std::mutex sqlmu;

    enum class BelongType{Model=0,Project=1};

    int  getProjectId(QString name);
    int  getSoilModelId(QString name);
    void getPointsWithId(int id,BelongType type);



    BelongType getBelongType(int id);

};



#pragma once
#include<QObject>
class ProjectInfoObject:public QObject{
    Q_OBJECT
public:
    ProjectInfoObject(QObject* obj=nullptr):QObject{obj}{

    }

    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString createTime READ getTime WRITE setTime NOTIFY timeChanged)


    Q_INVOKABLE int index() const{return m_index;};
    Q_INVOKABLE QString name() const{return m_name;};
    Q_INVOKABLE QString getTime() const{return m_createTime;};

    Q_INVOKABLE void setTime(const QString& time){m_createTime=time;};
    Q_INVOKABLE void setIndex(const int& time){m_index=time;};
    Q_INVOKABLE void setName(const QString& time){m_name=time;};

signals:
    void indexChanged();
    void nameChanged();
    void timeChanged();

private:
    int m_index;
    QString m_name;
    QString m_createTime;
};

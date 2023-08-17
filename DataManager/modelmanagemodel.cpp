
#include "modelmanagemodel.hpp"
#include "modelinfo.hpp"
#include "qdebug.h"
#include "qstringliteral.h"
#include<glog/logging.h>
#include"datamanager.hpp"
ModelManageModel::ModelManageModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QVariant ModelManageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    return QVariant{};
}

int ModelManageModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return sequence.size();
}

QVariant ModelManageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if(!(sequence.size()>index.row())){
        return QVariant();
    }
    auto row=index.row();
    if(sequence.constFind(row+1)==sequence.cend()){
        qDebug()<<"model not found"<<"row:"<<row;
        return QVariant();
    }
    qDebug()<<"sequence size:"<<sequence.size();

    // FIXME: Implement me!
    if(role==IndexRole){
        return sequence[index.row()+1]->id;
    }
    if(role==NameRole){
        return sequence[index.row()+1]->modelName;
    }
    if(role==TimeRole){
        return sequence[index.row()+1]->createTime;
    }
    if(role==SourceRole){
        return QString{tr("手动")};
    }
    if(role==relateRole){
        return QString{"r2D:%1 r2W:%2"}.
            arg(sequence[index.row()+1]->argR2,0,'g',2)
                .arg(sequence[index.row()+1]->argR2Water,0,'g',2);
    }
    return QVariant();
}


QHash<int, QByteArray> ModelManageModel::roleNames() const{


    QHash<int, QByteArray> roles;
    roles[IndexRole] = "index";
    roles[NameRole] = "name";
    roles[relateRole] = "rate";
    roles[TimeRole] = "time";
    roles[SourceRole] = "source";
    return roles;
}


void ModelManageModel::removeModel(int index){



    auto iter=sequence.find(index);
    if(iter!=sequence.end()){

        LOG(INFO)<<"removing model";
        QMetaObject::invokeMethod(&DataManager::getInstance(),"removeModel",Q_ARG(int,index));
        beginResetModel();
        sequence.remove(index);
        endResetModel();
    }else{
        LOG(WARNING)<<"can't find model with index:"<<index;
    }
}

void ModelManageModel::addModel(int i,std::shared_ptr<ModelInfo> model){
    beginInsertRows(QModelIndex(),i-1,i-1);
    sequence[i]=model;
    //sequence.insert(i,model);
    endInsertRows();
}


#pragma once

#include <QAbstractListModel>
#include <memory>



class ModelInfo;
class ModelManageModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ModelManageModel(QObject *parent = nullptr);

    enum ModelRoles {
        IndexRole = Qt::UserRole + 1,
        NameRole,
        TimeRole,
        relateRole,
        SourceRole,

    };
    Q_ENUM(ModelRoles)
    QHash<int, QByteArray> roleNames() const override;

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Remove data:


    void removeModel(int index);
    void addModel(int,std::shared_ptr<ModelInfo>);


private:

    QMap<int,std::shared_ptr<ModelInfo>> sequence;
};


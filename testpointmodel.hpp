
#pragma once


#include "qabstractseries.h"
#include <QAbstractListModel>
#include <memory>



class TestPointInfo;
class TestPointModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TestPointModel(QObject *parent = nullptr);

    enum TestPointRoles {
        IndexRole = Qt::UserRole + 1,
        DensityRole,
        WaterRateRole,
        AmpitudeRole,
        PhaseAngleRole,
        TemperatureRole,
        GpsRole,
        IsPairingRole,
        isSelectedRole
    };
    Q_ENUM(TestPointRoles)

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:

    void linearRegression();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QHash<int, QByteArray> roleNames() const override;


    bool add(const std::shared_ptr<TestPointInfo>& info);
    void remove(int index);


    void getFitSequenence(QtCharts::QAbstractSeries* series);
signals:
    void linearRegressionReady();
private:

    QMap<int,std::shared_ptr<TestPointInfo>> sequence;

    int getAvailableIndex();
    void resetIndex();


};


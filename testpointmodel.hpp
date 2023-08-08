
#pragma once



#include <QAbstractListModel>
#include <memory>

#include<QPointF>
namespace QtCharts{
class QXYSeries;
}
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





    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QHash<int, QByteArray> roleNames() const override;


    bool add(const std::shared_ptr<TestPointInfo>& info);



    //chart click api
    Q_INVOKABLE void setChecked(QPointF point, bool,int type);


    //初步构思，通过check作为统一步骤，设置uncheck来控制series
    //或许可以缓存一下点？
    Q_INVOKABLE void findPointAndCheck(QPointF point,bool check,int type);

    //type 0: density 1:waterRate
    Q_INVOKABLE void getFitSequence(QtCharts::QXYSeries* series,int type);
    Q_INVOKABLE void updateSeries(QtCharts::QXYSeries* actual,
                      QtCharts::QXYSeries* mask,int type);

    Q_INVOKABLE void swapSeriesPoint(QtCharts::QXYSeries* from,QtCharts::QXYSeries* to,int pointIndex,int type);

    void removePoint(int index);

signals:
    void linearRegressionReady();
    void updateAxis(qreal xmin,qreal xmax,qreal ymin,qreal ymax);
    void requireSeiresSwap(int pointIndex,bool reverse=false);

    void chartNeedsUpdate();

private:

    QMap<int,std::shared_ptr<TestPointInfo>> sequence;

    int getAvailableIndex();
    void resetIndex();

    void fillSeries(QtCharts::QXYSeries* series,QList<QPointF>& points);

    qreal waterRateR_Square=0;
    qreal densityR_Squrare=0;

    double currentFitA;
    double currentFitB;
    double currentFitR;


};


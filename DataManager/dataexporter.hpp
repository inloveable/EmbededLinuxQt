
#pragma once


#include <QObject>


class DataExporter : public QObject
{
    Q_OBJECT
public:
    explicit DataExporter(QObject *parent = nullptr);

    QString exportProject(int index);
    QString exportModel(int index);

signals:

};


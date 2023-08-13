
#pragma once


#include <QObject>


class DataExporter : public QObject
{
    Q_OBJECT
public:
    explicit DataExporter(QObject *parent = nullptr);

    void exportProject(int index,std::string fileName);
    void exportModel(int index,std::string fileName);

signals:

};


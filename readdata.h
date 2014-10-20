#ifndef READDATA_H
#define READDATA_H
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QObject>
class ReadData
{
public:
    ReadData();
    void ReadInfo(QString filename);
     QVector<QString>  ReadLoop(QString filename);
};

#endif // READDATA_H

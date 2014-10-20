#include "readdata.h"
#include <QtDebug>
#include <QObject>
ReadData::ReadData()
{
}
void ReadData::ReadInfo(QString filename){
    QFile file(filename);
    QTextStream out(stdout);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        while (!file.atEnd()) {
            QString data = QString(file.readLine());
            QStringList v=data.split(QRegExp("\\s+"));
            for(int i=1;i<v.size()-1;++i){
                QString temp=v.at(i);
                out<<temp<<"  ";
                QFile wfile("Line"+QObject::tr("%1").arg(i)+".txt");
                if(wfile.exists()) wfile.close();
                QTextStream in(&wfile);
                if(wfile.open(QIODevice::ReadWrite |QIODevice::Append| QIODevice::Text))
                {
                    in<<temp.toFloat()<<" ";
                }
//                in<<endl<<endl;
                wfile.close();
            }
        }
    }else{

        qDebug()<<"Read Error"<<endl;
    }
}
QVector<QString>  ReadData::ReadLoop(QString filename){
    QFile file(filename);
     QTextStream out(stdout);
    QVector<QString> vLoop;
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        while (!file.atEnd()) {
            QString data = QString(file.readLine());
            if(!data.isEmpty()&&data.contains(QRegExp("[0-9]")))
                out<<data<<endl;
                vLoop.append(data);
        }
    }else{
        qDebug()<<"Read Error ReadLoop"<<endl;
    }
    file.close();
    return vLoop;

}

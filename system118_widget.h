#ifndef SYSTEM118_WIDGET_H
#define SYSTEM118_WIDGET_H
#include <QObject>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QPolygon>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QSlider>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QRect>
#include <QTime>
#include <QCoreApplication>
#include <QtDebug>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFrame>
#include <QMap>
#include <QVector>
#include <QMediaPlayer>
#include <QComboBox>
#include <QTextStream>
#include <QMouseEvent>
#include <QStatusBar>
#include "systemwidget.h"
#include "lineitem.h"
#include "genitem.h"
#include "busitem.h"
#include "readdata.h"
#include "logindialog.h"
#include "loaditem.h"

class System118_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit System118_Widget(QWidget *parent = 0);

    void createControlFrame(int busnum=30,int linenum=40,int gennum=6);
    void ReadInfo(QString filename,int length);
    void ReadSet(QString filename);
    void setTimer(int timer);
    void setTitle(QString title);
    int getTimer();
    QString getTitle();

    void mousePressEvent(QMouseEvent *event);


private:
    QGraphicsView *view;
    QGraphicsScene *scene;
    QFrame *ctrlFrame;

    void initScene_bus();
    void createBus(QPointF *Bus);
    void createLine(QVector<QPointF> &points);
    void createGenerator(QPointF &point,bool up=false);
    void createLoad(QPointF &point);
    void startGenerator(QPointF & point,bool up=false);
    void delay( int millisecondsToWait );
signals:

public slots:
    void slotNew();              			//新建一个显示窗体
    void slotClear();           			//清除场景中所有的图元
    void slotStart();
    void slotScale(int);
    void slotPause();
    void slotStrategy();
    void slotStrategyMenu(int from=0);
    void slotstate();
private:
    qreal x1;  //Point in the topleft
    qreal y1;
    qreal Vspace;  //vertical space between two bus
    qreal Hspace;  //horizontal space between two bus
    qreal Height;  // height of bus
    qreal Width;   //width of bus
    qreal Hbase;
    qreal Vbase;
    qreal shortY;
    qreal shortX;
    qreal Radius;
    qreal X1;
    qreal X2;
    qreal X3;
    qreal X4;
    qreal Y1;
    qreal Y2;
    qreal Y3;
    qreal Y4;
    const static qreal X5=850;
    const static qreal Y5=600;
    const static qreal hUnit=4;
    qreal wUnit;
    qreal yUnit;
    qreal xUnit;

    int _loop;
    bool _auto;

    QLabel* label;
    QLabel *linelabel;
    QLabel *genlabel;
    QLabel *buslabel;
    QLabel *syslabel;
    QLabel *stralabel;
    QLabel *warninglabel;
    QLabel *breakLabel;
    QLabel *stateLabel;
    bool pause;

    QVector<BusItem*> busItems;
    QVector<LineItem*> lineItems;
    QVector<GenItem*> genItems;
    QVector<LineItem*> gLineItems;
    QVector<Loaditem*> loadItems;
    QVector<QPointF*> *_pload;
    FlashItem * flashboom;

    qreal angle;
    qreal scaleValue;
    qreal shearValue;
    qreal translateValue;
    ReadData *read;

    QComboBox *strategyCom;
    QComboBox *stateCom;
    QComboBox *warnCom;
    QComboBox *breakCom;
    QString _Title;
    int _Timer;

};

#endif // SYSTEM118_WIDGET_H

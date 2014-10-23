#ifndef SYSTEMWIDGET_H
#define SYSTEMWIDGET_H
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QTimer>
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
#include "systemwidget.h"
#include "lineitem.h"
#include "genitem.h"
#include "busitem.h"
#include "readdata.h"
#include "logindialog.h"
#include "flashitem.h"
class SystemWidget : public QWidget
{
    Q_OBJECT
public:
    SystemWidget(QWidget *parent = 0);

    void createControlFrame(int busnum=30,int linenum=40,int gennum=6);
    void ReadInfo(QString filename,int length);
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
    void slotStrategyMenu(int straIndex);
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

    QLabel *linelabel;
    QLabel *genlabel;
    QLabel *buslabel;
    QLabel *syslabel;
    QLabel *stralabel;
    QLabel *warninglabel;
    QLabel *breakLabel;
    QLabel *stateLabel;
    QLabel* label;
    QLabel* label2;
    bool pause;

    QVector<BusItem*> busItems;
    QVector<LineItem*> lineItems;
    QVector<GenItem*> genItems;
    QVector<LineItem*> gLineItems;
    FlashItem * flashboom;
    FlashItem * flashboom_1;
    FlashItem * flashboom_2;
    FlashItem * flashboom_3;



    QPointF *G1;
    QPointF *G2;
    QPointF *G13;
    QPointF *G22;
    QPointF *G23;
    QPointF *G27;
    QVector<QPointF> *gBus1;
    QVector<QPointF> *gBus2;
    QVector<QPointF> *gBus13;
    QVector<QPointF> *gBus22;
    QVector<QPointF> *gBus23;
    QVector<QPointF> *gBus27;

    QPointF *L2;
    QPointF *L3;
    QPointF *L4;
    QPointF *L7;
    QPointF *L8;
    QPointF *L10;
    QPointF *L12;
    QPointF *L14;
    QPointF *L15;
    QPointF *L16;
    QPointF *L17;
    QPointF *L18;
    QPointF *L19;
    QPointF *L20;
    QPointF *L21;
    QPointF *L23;
    QPointF *L24;
    QPointF *L26;
    QPointF *L29;
    QPointF *L30;

    qreal angle;
    qreal scaleValue;
    qreal shearValue;
    qreal translateValue;
    ReadData *read;

    QComboBox *strategyCom;
    QComboBox *stateCom;
    QComboBox *warnCom;
    QComboBox *breakCom;

    bool _auto;
    int _loop;


};

#endif // SYSTEMWIDGET_H

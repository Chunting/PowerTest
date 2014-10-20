#ifndef LINEITEM_H
#define LINEITEM_H

#include <QObject>
#include <QPainter>
#include <QGraphicsItem>
#include <QPointF>
#include <QPolygon>
#include <QTextStream>

class LineItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit LineItem(QObject *parent = 0);
    LineItem(QPointF lpoint1,QPointF lpoint2,QPointF lpoint3,QPointF lpoint4,int n=4,QObject *parent = 0);
    LineItem(QPointF lpoint1,QPointF lpoint2,int n=2,QObject *parent = 0);
    LineItem(QPointF lpoint1,QPointF lpoint2,QPointF lpoint3,int n=3,QObject *parent = 0);
    LineItem(QPointF lpoint1,QPointF lpoint2,QPointF lpoint3,QPointF lpoint4,QPointF lpoint5,QPointF lpoint6,int n=6,QObject *parent = 0);
    LineItem(QPointF lpoint1,QPointF lpoint2,QPointF lpoint3,QPointF lpoint4,QPointF lpoint5,int n=5,QObject *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void timerEvent(QTimerEvent *);
    void setStart(bool start);
    void setStop(bool stop);
    void setrunstate(bool runstate);
    void setreset(bool reset);
    float getLineCap();
    void setLineCap(float linecap);
    float getupLineCap();
    void setupLineCap(float uplinecap);
    float getratioline();
signals:

public slots:
private:
    QPointF _lpoint1;
    QPointF _lpoint2;
    QPointF _lpoint3;
    QPointF _lpoint4;
    QPointF _lpoint5;
    QPointF _lpoint6;
    int number;

    bool _start;
    bool _stop;
    bool _runstate;
    bool _flash;
    bool _reset;
    float _linecap;
    float _uplinecap;
    QTimer *timer;



};

#endif // LINEITEM_H

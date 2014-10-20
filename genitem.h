#ifndef GENITEM_H
#define GENITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QTextItem>
#include <QPalette>
#include "lineitem.h"
class GenItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit GenItem(QObject *parent = 0);
    GenItem(qreal radius,QPointF gpoint,bool up=false, QObject *parent=0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void timerEvent(QTimerEvent *);
    void setStart(bool start);
    void setStop(bool stop);
    void setrunstate(bool runstate);
    void setgenCap(float gencap);
    float getgenCap();
    void setRadius(qreal radius);

signals:

public slots:
private:
    bool _start;
    bool _stop;
    bool _runstate;
    bool _flash;
    QTimer *timer;

    QPointF _gpoint;
    bool _up;
    qreal _radius;
    LineItem* _gline;
    float _genCap;

};

#endif // GENITEM_H

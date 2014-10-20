#ifndef NEWGENITEM_H
#define NEWGENITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QTextItem>
#include <QPalette>
class Newgenitem : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Newgenitem(QObject *parent = 0);
    Newgenitem (qreal radius,QPointF gpoint,bool up=false, bool left=false,QObject *parent=0);

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
    bool _left;
    qreal _radius;
    float _genCap;
    qreal x1;
    qreal y1;
    qreal x2;
    qreal y2;
    QPointF *endline;

};

#endif // NEWGENITEM_H

#ifndef BUSITEM_H
#define BUSITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QBrush>
#include <QStyleOptionGraphicsItem>
#include <QTextStream>
#include <QPen>
class BusItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit BusItem(QObject *parent = 0);
    BusItem(qreal x,qreal y,qreal height,qreal width,QObject *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void timerEvent(QTimerEvent *);
    void setStart(bool start);
    void setStop(bool stop);
    void setrunstate(bool runstate);
    void setColor(int color);
    int getColor();
    void setBusnum(int busnum);
    int getBusnum();
    void setBusquar(int quartier);
    int getBusquar();

signals:

public slots:

private:
    qreal _x;
    qreal _y;
    qreal _height;
    qreal _width;

    bool _start;
    bool _stop;
    bool _runstate;
    bool _flash;
    QTimer *timer;
    int _color;
    int _busnum;
    int _quartier;
};

#endif // BUSITEM_H

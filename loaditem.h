#ifndef LOADITEM_H
#define LOADITEM_H

#include <QObject>
#include <QPainter>
#include <QGraphicsItem>
#include <QPointF>
#include <QPolygon>
#include <QTextStream>

class Loaditem : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Loaditem(QObject *parent = 0);
    Loaditem(QPointF loadpoint,int direct=0,qreal x=6,qreal y=12,int hUnit=4,QObject *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:

public slots:
private:
    QPointF _loadpoint;
    int _direct;
    qreal _x;
    qreal _y;
    qreal _hUnit;

    bool _start;
    bool _stop;
    bool _runstate;
    bool _flash;
    bool _reset;
    float _linecap;
    float _uplinecap;
    QTimer *timer;



};

#endif // LOADITEM_H

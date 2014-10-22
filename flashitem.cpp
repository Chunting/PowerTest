#include "flashitem.h"

FlashItem::FlashItem(QObject *parent) :
    QObject(parent)
{
    flash=true;
    setFlag(ItemIsMovable);
    startTimer(700);
}

QRectF FlashItem::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10-adjust,-10-adjust,35+adjust,35+adjust);
}

void FlashItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    pen.setWidth(3);
    pen.setColor(flash?(Qt::red):(QColor(200,200,200)));

    QPolygonF polyline;
    polyline.append(QPointF(0, 0));
    polyline.append(QPointF(20, 20));
//    polyline.append(QPointF(25, 0));
//    polyline.append(QPointF(0, 25));
    QPainterPath* path = new QPainterPath();
    path->addPolygon(polyline);
    painter->setPen(pen);
    painter->drawPath(*path);

    QPolygonF polyline1;
    polyline1.append(QPointF(20, 0));
    polyline1.append(QPointF(0, 20));
//    polyline1.append(QPointF(25, 0));
//    polyline1.append(QPointF(0, 25));
    QPainterPath* path1 = new QPainterPath();
    path1->addPolygon(polyline1);
    painter->setPen(pen);
    painter->drawPath(*path1);

}

void FlashItem::timerEvent(QTimerEvent *)
{
    flash=!flash;
    update();
}

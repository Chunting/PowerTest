#include "loaditem.h"

Loaditem::Loaditem(QObject *parent) :
    QObject(parent)
{
}
Loaditem::Loaditem(QPointF loadpoint,int direct,qreal x,qreal y, int hUnit,QObject *parent) :
    _loadpoint(loadpoint),_direct(direct),_x(x),_y(y),_hUnit(hUnit),QObject(parent)
{
    _start=false;
    _flash=true;
    _runstate=false;
     _stop=false;
//    setPos(_loadpoint);
}

QRectF Loaditem::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10-adjust,-10-adjust,450+adjust,450+adjust);
}
/**
 * @brief gLoad
 * _direct=0, down;
 * _direct=1, up;
 * _direct=2, left;
 * _direct =3, right;
 */
void Loaditem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QVector<QPointF>* gLoad=new QVector<QPointF>();
    if(_direct==0){
    gLoad->append(QPointF(_loadpoint.rx(),_loadpoint.ry()+_hUnit));
    gLoad->append(QPointF(_loadpoint.rx(),_loadpoint.ry()+_y));
    gLoad->append(QPointF(_loadpoint.rx()-0.4*_x,_loadpoint.ry()+_y));
    gLoad->append(QPointF(_loadpoint.rx(),_loadpoint.ry()+1.3*_y));
    gLoad->append(QPointF(_loadpoint.rx()+0.4*_x,_loadpoint.ry()+_y));
    gLoad->append(QPointF(_loadpoint.rx(),_loadpoint.ry()+_y));
    }else if (_direct==1){
        gLoad->append(QPointF(_loadpoint.rx(),_loadpoint.ry()));
        gLoad->append(QPointF(_loadpoint.rx(),_loadpoint.ry()-_y));
        gLoad->append(QPointF(_loadpoint.rx()-0.4*_x,_loadpoint.ry()-_y));
        gLoad->append(QPointF(_loadpoint.rx(),_loadpoint.ry()-1.3*_y));
        gLoad->append(QPointF(_loadpoint.rx()+0.4*_x,_loadpoint.ry()-_y));
        gLoad->append(QPointF(_loadpoint.rx(),_loadpoint.ry()-_y));
    }else if(_direct==2){
        gLoad->append(QPointF(_loadpoint.rx(),_loadpoint.ry()));
        gLoad->append(QPointF(_loadpoint.rx()-_y,_loadpoint.ry()));
        gLoad->append(QPointF(_loadpoint.rx()-_y,_loadpoint.ry()-0.4*_x));
        gLoad->append(QPointF(_loadpoint.rx()-1.3*_y,_loadpoint.ry()));
        gLoad->append(QPointF(_loadpoint.rx()-_y,_loadpoint.ry()+0.4*_x));
        gLoad->append(QPointF(_loadpoint.rx()-_y,_loadpoint.ry()));
    }else if(_direct==3){
        gLoad->append(QPointF(_loadpoint.rx(),_loadpoint.ry()));
        gLoad->append(QPointF(_loadpoint.rx()+_y,_loadpoint.ry()));
        gLoad->append(QPointF(_loadpoint.rx()+_y,_loadpoint.ry()-0.4*_x));
        gLoad->append(QPointF(_loadpoint.rx()+1.3*_y,_loadpoint.ry()));
        gLoad->append(QPointF(_loadpoint.rx()+_y,_loadpoint.ry()+0.4*_x));
        gLoad->append(QPointF(_loadpoint.rx()+_y,_loadpoint.ry()));
    }

    QPolygonF* polyline=new QPolygonF(* gLoad);
    QPen Lpen;
    Lpen.setWidth(2);
    Lpen.setColor(QColor(0,0,0));
    QPainterPath* path = new QPainterPath();
    path->addPolygon(*polyline);
    painter->setPen(Lpen);
    painter->setBrush(Qt::black);
    painter->drawPath(*path);


}
//void Loaditem::timerEvent(QTimerEvent *)
//{
//    if(_start){
//    _flash=!_flash;
//    update();
//    }
//}

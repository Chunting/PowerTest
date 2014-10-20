#include "newgenitem.h"
#include "QTextStream"

Newgenitem::Newgenitem(QObject *parent) :
    QObject(parent)
{
}

Newgenitem::Newgenitem(qreal radius,QPointF gpoint,bool up,bool left, QObject *parent) :
    _radius(radius),_gpoint(gpoint),_up(up),_left(left),QObject(parent)
{
    _start=false;
    _flash=true;
    _runstate=false;
    _stop=false;
    _genCap=0;
    startTimer(1000);
    QTextStream out(stdout);
    if(_up==false&&_left==false){
        x1=_gpoint.x();
        y1=_gpoint.y()+_radius;
        x2=x1-_radius;
        y2=y1;

        out<<"false,false: "<<x1<<" "<<y1<<" : "<<x2<<" "<<y2<<endl;
    }else if(_up==true&&_left==false)
    {
        x1=_gpoint.x();
        y1=_gpoint.y()-_radius;
        x2=x1;
        y2=y1-2*_radius;
       endline=new QPointF(x1,y1);


    }else if(_up==false&&_left==true){
        x1=_gpoint.x()-_radius;
        y1=_gpoint.y();
        x2=x1-2*_radius;
        y2=y1-_radius;
    }
    setPos(QPointF(x2,y2));
}

QRectF Newgenitem::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10-adjust,-10-adjust,450+adjust,450+adjust);
}
void Newgenitem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{ QPen Lpen;
    Lpen.setWidth(2);
    Lpen.setColor(QColor(0,0,0));
    painter->setPen(Lpen);

    QVector<QPointF> *Line=new QVector<QPointF>();
    Line->append(_gpoint);
    Line->append(*endline);
    QPolygonF* polyline=new QPolygonF(*Line);

    QPainterPath* path = new QPainterPath();
    path->addPolygon(*polyline);
    painter->drawPath(*path);
    painter->setBrush(Qt::black);
    painter->setRenderHint((QPainter::Antialiasing));
    QFont font;
    font.setPointSize(14);
    font.setBold(true);

    if(_runstate&&!_stop)
        painter->setBrush(Qt::green);
    if(_stop){
        painter->setBrush(_flash?(Qt::yellow):(Qt::red));
        painter->drawEllipse(QRectF(0,0,2*_radius, 2*_radius));
        painter->setBrush(Qt::red);
    }
    painter->drawEllipse(QRectF(0,0,2*_radius, 2*_radius));
    painter->setFont(font);
    painter->drawText(QRectF(0,0,2*_radius, 2*_radius),Qt::AlignCenter, "G");




}
void Newgenitem::timerEvent(QTimerEvent *)
{
    if(_start){
        _flash=!_flash;
        update();
    }
}
void  Newgenitem::setStart(bool start){
    _start=start;
}
void  Newgenitem::setStop(bool stop){
    _stop=stop;
}
void  Newgenitem::setrunstate(bool runstate){
    if(!_stop)
        _runstate=runstate;
}
void Newgenitem::setgenCap(float gencap){
    _genCap=gencap;
}
float Newgenitem::getgenCap(){
    return _genCap;
}
void Newgenitem::setRadius(qreal radius){
    _radius=radius;
}

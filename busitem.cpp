#include "busitem.h"
#include <QTextStream>

BusItem::BusItem(QObject *parent) :
    QObject(parent)
{
}
BusItem::BusItem(qreal x,qreal y,qreal width,qreal height,QObject *parent):
    _x(x),_y(y),_width(width), _height(height),QObject(parent)
{
    _start=false;
    _flash=true;
    _runstate=false;
    _stop=false;
    _busnum=0;
    _color=255;
    startTimer(1000);
    setPos(_x,_y);
}
QRectF BusItem::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10-adjust,-10-adjust,43+adjust,43+adjust);
}

void BusItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QFont font;
    font.setPointSize(6);
//    font.setBold(true);
    painter->setFont(font);
    painter->setBrush(Qt::black);
    QTextStream out(stdout);
    if(_runstate&&!_stop)
        painter->setBrush(QColor(0.3*_color,255-_color,_color));
    if(_stop){
        painter->setPen(Qt::NoPen);
        if(_quartier==0){
            painter->setBrush(QColor(138,43,226));  //Light Cyan
        }else if(_quartier==1){
            painter->setBrush(QColor(255,255,0));  //Yellow
        }else if(_quartier==2){
            painter->setBrush(QColor(238,130,238));  //Violet
        }else if(_quartier==3){
            painter->setBrush(QColor(65,105,225));  //light blue
        }else if(_quartier==4){
            painter->setBrush(QColor(240,128,128));  //Light coral
        }else if(_quartier==5){
            out<<"quartier: "<<_quartier<<endl;
            painter->setBrush(QColor(0,238,144));  //light green
        }else{
            painter->setBrush(QColor((_color+80)%255,(_color+80)%255,(_color+80)%255));
        }

        painter->drawText(QRectF(-12,-12,_width,_height), Qt::AlignLeft,QString::number(_busnum));
//        if(_width>=_height){
//        painter->drawRect(-0.3*_width,-0.5*_height,1.6*_width,2*_height);
//        }else{
//            painter->drawRect(-0.3*_width,-0.25*_height,1.6*_width,1.5*_height);
//        }
        painter->setBrush(Qt::red);
    }
    painter->drawRect(0,0,_width,_height);
    QPen Lpen;
    Lpen.setWidth(3);
    Lpen.setColor(QColor(0,0,0));
    painter->setPen(Lpen);
    painter->drawText(QRectF(-12,-12,5+_width,3+_height), Qt::AlignLeft,QString::number(_busnum));
}
void BusItem::timerEvent(QTimerEvent *)
{
//    if(_runstate&&!_stop){
////        _flash=!_flash;
//        update();
//    }
}
void  BusItem::setStart(bool start){
    _start=start;
}
void  BusItem::setStop(bool stop){
    _stop=stop;
}
void  BusItem::setrunstate(bool runstate){
    if(!_stop)
        _runstate=runstate;
}
void BusItem::setColor(int color){
    if(color!=0)
        _color=color;
}
int BusItem::getColor(){
    return _color;
}
void BusItem::setBusnum(int busnum){
    _busnum=busnum;
}
void BusItem::setBusquar(int quartier){
    _quartier=quartier;
}

int BusItem::getBusquar(){
    return _quartier;
}

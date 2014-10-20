#include "lineitem.h"
#include <QTextStream>

LineItem::LineItem(QObject *parent) :
    QObject(parent)
{
}
LineItem::LineItem(QPointF lpoint1,QPointF lpoint2,QPointF lpoint3,QPointF lpoint4,QPointF lpoint5,QPointF lpoint6, int n,QObject*parent) :
    _lpoint1(lpoint1),_lpoint2(lpoint2), _lpoint3(lpoint3),_lpoint4(lpoint4),_lpoint5(lpoint5),_lpoint6(lpoint6),number(n),QObject(parent)
{
    _start=false;
    _flash=true;
    _runstate=false;
    _stop=false;
    _reset=false;
    _linecap=0;
    _uplinecap=100;
    startTimer(1000);
}
LineItem::LineItem(QPointF lpoint1,QPointF lpoint2,QPointF lpoint3,QPointF lpoint4,QPointF lpoint5, int n,QObject*parent) :
    _lpoint1(lpoint1),_lpoint2(lpoint2), _lpoint3(lpoint3),_lpoint4(lpoint4),_lpoint5(lpoint5),number(n),QObject(parent)
{
    _start=false;
    _flash=true;
    _runstate=false;
    _stop=false;
    _reset=false;
    _linecap=0;
    _uplinecap=100;
    startTimer(1000);
}
LineItem::LineItem(QPointF lpoint1,QPointF lpoint2,QPointF lpoint3,QPointF lpoint4, int n,QObject*parent) :
    _lpoint1(lpoint1),_lpoint2(lpoint2), _lpoint3(lpoint3),_lpoint4(lpoint4),number(n),QObject(parent)
{
    _start=false;
    _flash=true;
    _runstate=false;
    _stop=false;
    _reset=false;
    _linecap=0;
    _uplinecap=100;
    startTimer(1000);
}
LineItem::LineItem(QPointF lpoint1,QPointF lpoint2,QPointF lpoint3, int n,QObject*parent) :
    _lpoint1(lpoint1),_lpoint2(lpoint2), _lpoint3(lpoint3),number(n),QObject(parent)
{
    _start=false;
    _flash=true;
    _runstate=false;
    _stop=false;
    _reset=false;
    _linecap=0;
    _uplinecap=100;
    startTimer(1000);
}
LineItem::LineItem(QPointF lpoint1,QPointF lpoint2,int n,QObject*parent) :
    _lpoint1(lpoint1),_lpoint2(lpoint2),number(n),QObject(parent)
{
    _start=false;
    _flash=true;
    _runstate=false;
    _stop=false;
    _reset=false;
    _linecap=0;
    _uplinecap=100;
    startTimer(1000);
}
QRectF LineItem::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10-adjust,-10-adjust,2000+adjust,2000+adjust);
}

void LineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QVector<QPointF> *Line=new QVector<QPointF>();
    if(number==2){
        Line->append(_lpoint1);
        Line->append(_lpoint2);
    }else if(number==3){
        Line->append(_lpoint1);
        Line->append(_lpoint2);
        Line->append(_lpoint3);
    }else if(number==4){
        Line->append(_lpoint1);
        Line->append(_lpoint2);
        Line->append(_lpoint3);
        Line->append(_lpoint4);
    }else if(number==5){
        Line->append(_lpoint1);
        Line->append(_lpoint2);
        Line->append(_lpoint3);
        Line->append(_lpoint4);
        Line->append(_lpoint5);
    }else if(number==6){
        Line->append(_lpoint1);
        Line->append(_lpoint2);
        Line->append(_lpoint3);
        Line->append(_lpoint4);
        Line->append(_lpoint5);
        Line->append(_lpoint6);
    }
    QTextStream out(stdout);
    QPolygonF* polyline=new QPolygonF(*Line);
    QPen Lpen;
    Lpen.setWidth(2);
    QPainterPath* path = new QPainterPath();
    path->addPolygon(*polyline);
    int col=(int)abs((255*getratioline()))%255;
    Lpen.setColor(QColor(0,0,0));
    if(_start){
        Lpen.setColor(QColor(0,255,0));
    }
    if(_runstate){
        if(col<100) col=1.7*col;
        Lpen.setColor(QColor(col,255-col,0));
        if(col>255 ||col<0)
            out<<"col: "<<col<<"  255-col: "<<255-col<<endl;

    }
    if(_reset)
        Lpen.setColor(Qt::black);
    if(_stop){
        Lpen.setStyle(Qt::DashDotLine);
        Lpen.setWidth(2);
        Lpen.setColor(QColor(255,0,0));
    }
//    if(_uplinecap==24.82){
//        float x=(_lpoint2.x()+_lpoint3.x())/2;
//        float y=_lpoint1.y();
//        QVector<QPointF> *Line1=new QVector<QPointF>();
//        Line1->append(QPoint(x-10,y+10));
//        Line1->append(QPoint(x+10,y+10));

//        QPolygonF* polyline1=new QPolygonF(*Line1);
//        QPen Lpen1;
//        Lpen1.setWidth(2);
//        QPainterPath* path1 = new QPainterPath();
//        path1->addPolygon(*polyline1);
//        painter->setPen(Lpen1);
//        painter->drawPath(*path1);
//   // }

    painter->setPen(Lpen);
    painter->drawPath(*path);
}
void LineItem::timerEvent(QTimerEvent *)
{
    if(_start||_runstate||_reset){
        _flash=!_flash;
        update();
    }
}
void  LineItem::setStart(bool start){
    _start=start;
}
void  LineItem::setStop(bool stop){
    _stop=stop;
}
void  LineItem::setrunstate(bool runstate){
    if(!_stop)
        _runstate=runstate;
}
void LineItem::setreset(bool reset){
    if(_runstate)
        _reset=reset;

}
void LineItem::setLineCap(float linecap){
    _linecap=linecap;
}
float LineItem::getLineCap(){
    return _linecap;
}
void LineItem::setupLineCap(float uplinecap){
    _uplinecap=uplinecap;
}
float LineItem::getupLineCap(){
    return _uplinecap;
}
float LineItem::getratioline(){
    QTextStream out(stdout);
    if(_uplinecap!=0&&_linecap<_uplinecap)
        return (_linecap)/(_uplinecap);
    else return 1;
}

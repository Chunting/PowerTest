#include "genitem.h"

GenItem::GenItem(QObject *parent) :
    QObject(parent)
{
}
GenItem::GenItem(qreal radius,QPointF gpoint,bool up, QObject *parent) :
    _radius(radius),_gpoint(gpoint),_up(up),QObject(parent)
{
    _start=false;
    _flash=true;
    _runstate=false;
     _stop=false;
     _genCap=100;
    //_color=255;
    _chuli = 0;
    startTimer(1000);
    setPos(_gpoint);
}

QRectF GenItem::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10-adjust,-10-adjust,450+adjust,450+adjust);
}
void GenItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
     painter->setBrush(Qt::black);
     painter->setRenderHint((QPainter::Antialiasing));
     QFont font;
     font.setPointSize(14);
         font.setBold(true);
     int col=(int)abs((255*getratio()))%255;
      QTextStream out(stdout);
 //   if(_runstate&&!_stop)
 //        painter->setBrush(Qt::green);
    if(_runstate&&!_stop) {
        if(col <= 255 && col >= 0) {
          //  out<<"col: "<<col<<"  255-col: "<<255-col<<endl;
            painter->setBrush(QColor(col,255-col,0));
        }
        else
            painter->setBrush(Qt::green);
    }
    if(_stop){
//        painter->setBrush(_flash?(Qt::yellow):(Qt::red));
        painter->drawEllipse(QRectF(0,0,2*_radius, 2*_radius));
        painter->setBrush(Qt::red);
    }
    painter->drawEllipse(QRectF(0,0,2*_radius, 2*_radius));
    painter->setFont(font);
    painter->drawText(QRectF(0,0,2*_radius, 2*_radius),Qt::AlignCenter, "");


}
void GenItem::timerEvent(QTimerEvent *)
{
//    if(_start){
////    _flash=!_flash;
//    update();
//    }
}
void  GenItem::setStart(bool start){
    _start=start;
}
void  GenItem::setStop(bool stop){
    _stop=stop;
}
void  GenItem::setrunstate(bool runstate){
    if(!_stop)
    _runstate=runstate;
}
void GenItem::setgenCap(float gencap){
    _genCap=gencap;
}
float GenItem::getgenCap(){
    return _genCap;
}
void GenItem::setRadius(qreal radius){
    _radius=radius;
}

float GenItem::getratio(){
     QTextStream out(stdout);
    // out << "Chuli: " << _chuli <<" genCap: " << _genCap << endl;
    if(_chuli != 0 && _chuli < _genCap && _genCap != 0) {
        return (_chuli)/(_genCap);
    }
    return 0;
}
void GenItem::setChuli(float chuli) {
    _chuli = chuli;
}
float GenItem:: getChuli() {
    return _chuli;
}

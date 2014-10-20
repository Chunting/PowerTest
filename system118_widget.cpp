#include "System118_Widget.h"
#include "mainwindow.h"
#include "flashitem.h"
#include "startitem.h"
#include "busitem.h"
#include "lineitem.h"
#include "math.h"
#include <qtextstream.h>
#include <qtextcodec.h>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QPolygon>
#include <QHBoxLayout>
#include <QSlider>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QRect>
#include <QTime>
#include <QCoreApplication>
#include <QtDebug>
#include <QObjectList>

System118_Widget::System118_Widget(QWidget *parent) :
    QWidget(parent)
{
    pause=false;
    _loop=0;
    _auto=true;
    _Timer=3000;
    ReadSet(":/IEEE118_data/IEEE118_data/set.txt");

    xUnit=X5/50;
    yUnit=Y5/50;
    wUnit=2*xUnit;
    X1=10*xUnit;
    X2=20*xUnit;
    X3=30*xUnit;
    X4=40*xUnit;
    Y1=10*yUnit;
    Y2=20*yUnit;
    Y3=30*yUnit;
    Y4=40*yUnit;
    Radius=yUnit;
    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,60*xUnit,60*yUnit);
    scene->setBackgroundBrush(QColor(200,200,200));
    initScene_bus();
    view = new QGraphicsView();
    view->setScene(scene);

    view->setMinimumSize(6*xUnit,40*yUnit);
    ctrlFrame = new QFrame;
    ctrlFrame->setMaximumWidth(9*xUnit);
    createControlFrame(118,186,54);
    //主窗口布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setObjectName("mainlayout");
    mainLayout->setMargin(10);
    mainLayout->setSpacing(20);
    mainLayout->addWidget(ctrlFrame);
    mainLayout->addWidget(view);

    QVBoxLayout *vLayout = new QVBoxLayout;
    QFont font;
    font.setPointSize(28);
    font.setBold(true);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    label = new QLabel;

    label->setObjectName(QStringLiteral("label"));
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);
    label->setFrameShape(QFrame::StyledPanel);
    if(_Title.isNull()){
        _Title="电网异常状态传播仿真系统";
    }
    label->setText(_Title);
    label->setPalette(pa);
    vLayout->addWidget(label);
    vLayout->addLayout(mainLayout);
    setLayout(vLayout);
    setWindowTitle(tr("电网异常状态传播仿真系统"));   //设置主窗体的标题
}

void System118_Widget::createControlFrame(int busnum,int linenum,int gennum)
{
    scaleValue = 5;
    setObjectName("controlFrame");
    //Start button
    QPushButton *startbutton=new QPushButton;
    startbutton->setText("开始");
    QVBoxLayout *startLayout = new QVBoxLayout;
    startLayout->addWidget(startbutton);
    QGroupBox *startGroup = new QGroupBox(tr("控制"));
    startGroup->setAlignment(Qt::AlignCenter);
    connect(startbutton, SIGNAL(released()), this, SLOT(slotStart()));
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::blue);

    QLabel *strategLabel =new QLabel(tr("策略选择："));
    strategyCom=new QComboBox;
    strategyCom->addItem(tr("裕度"));
    strategyCom->addItem(tr("重载"));
    strategyCom->addItem(tr("随机"));
    QHBoxLayout *straLayout = new QHBoxLayout;
    straLayout->addWidget(strategLabel);
    straLayout->addWidget(strategyCom);
    connect(strategyCom,SIGNAL(activated(int)),this,SLOT(slotStrategy()));
    QGroupBox *straGroup = new QGroupBox(tr("策略选择"));
    straGroup->setAlignment(Qt::AlignCenter);
    straGroup->setLayout(straLayout);
    syslabel=new QLabel;
    syslabel->setObjectName("syslabel");
    syslabel->adjustSize();
    syslabel->setFont(font);
    syslabel->setPalette(pa);
    syslabel->setText(QObject::tr("118节点系统 "));
    syslabel->setWordWrap(true);
    syslabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout *infoLayout = new QVBoxLayout;
    infoLayout->addWidget(syslabel);

    stralabel=new QLabel;
    stralabel->setObjectName("syslabel");
    stralabel->adjustSize();
    stralabel->setFont(font);
    stralabel->setPalette(pa);
    stralabel->setText(QObject::tr("选择策略 "));
    stralabel->setWordWrap(true);
    stralabel->setAlignment(Qt::AlignCenter);
    infoLayout->addWidget(stralabel);


    buslabel=new QLabel(ctrlFrame);
    buslabel->setObjectName("buslabel");
    buslabel->adjustSize();
    buslabel->setFont(font);
    buslabel->setPalette(pa);
    buslabel->setText(QObject::tr("节点： ")+QString::number(busnum));
    buslabel->setWordWrap(true);
    buslabel->setAlignment(Qt::AlignLeft);
    infoLayout->addWidget(buslabel);
    infoLayout->setObjectName("infoLayout");


    linelabel=new QLabel(ctrlFrame);
    linelabel->setObjectName("linelabel");
    linelabel->adjustSize();
    linelabel->setFont(font);
    linelabel->setPalette(pa);
    linelabel->setText(QObject::tr("线路： ")+QString::number(linenum));
    linelabel->setWordWrap(true);
    linelabel->setAlignment(Qt::AlignLeft);
    infoLayout->addWidget(linelabel);

    genlabel=new QLabel(ctrlFrame);
    genlabel->setObjectName("genlabel");
    genlabel->adjustSize();
    genlabel->setFont(font);
    genlabel->setPalette(pa);
    genlabel->setText(QObject::tr("机组： ")+QString::number(gennum));
    genlabel->setWordWrap(true);
    genlabel->setAlignment(Qt::AlignLeft);
    infoLayout->addWidget(genlabel);

    QGroupBox *infoGroup = new QGroupBox(tr("系统信息"));
    infoGroup->setAlignment(Qt::AlignCenter);
    infoGroup->setLayout(infoLayout);
    infoGroup->setObjectName("infoGroup");

    warnCom=new QComboBox;
    warnCom->setMinimumContentsLength(12);
    QLabel *warnLabel =new QLabel(tr("线路预警"));
    warnLabel->setObjectName("warnLabel");
    pa.setColor(QPalette::WindowText,QColor(255,97,0));
    warnLabel->setPalette(pa);
    warnCom->addItem(tr(" "));

    QHBoxLayout *warnLayout = new QHBoxLayout;
    warnLayout->addWidget(warnLabel);
    warnLayout->addWidget(warnCom);

    QGroupBox *warnGroup = new QGroupBox(tr("实时预警"));
    warnGroup->setAlignment(Qt::AlignCenter);
    warnGroup->setLayout(warnLayout);

    breakCom=new QComboBox;
    breakCom->setMinimumContentsLength(12);
    QLabel *breakLabel =new QLabel(tr("线路故障"));
    breakLabel->setObjectName("breakLabel");
    pa.setColor(QPalette::WindowText,Qt::red);
    breakLabel->setPalette(pa);
    breakCom->addItem(tr(" "));
    QHBoxLayout *breakLayout = new QHBoxLayout;
    breakLayout->addWidget(breakLabel);
    breakLayout->addWidget(breakCom);
    // connect(breaktegyCom,SIGNAL(activated(int)),this,SLOT(slotbreaktegy()));
    QGroupBox *breakGroup = new QGroupBox(tr("实时故障"));
    breakGroup->setAlignment(Qt::AlignCenter);
    breakGroup->setLayout(breakLayout);

    stateLabel=new QLabel(ctrlFrame);
    stateLabel->setObjectName("stateLabel");
    stateLabel->adjustSize();
    stateLabel->setFont(font);
    pa.setColor(QPalette::WindowText,Qt::blue);
    stateLabel->setPalette(pa);
    stateLabel->setText(QObject::tr("状态： ")+QString::number(0));
    stateLabel->setWordWrap(true);
    stateLabel->setAlignment(Qt::AlignLeft);

    QLabel *stateLabel2 =new QLabel(tr("复查："));
    stateCom=new QComboBox;
    for(int i=0;i<11;++i){
        stateCom->addItem(QObject::tr("状态 ")+QString::number(i));
    }

    QHBoxLayout *stateLayout2 = new QHBoxLayout;
    stateLayout2->addWidget(stateLabel2);
    stateLayout2->addWidget(stateCom);
    connect(stateCom,SIGNAL(activated(int)),this,SLOT(slotstate()));
    QVBoxLayout *stateLayout = new QVBoxLayout;
    stateLayout->addWidget(stateLabel);
    stateLayout->addLayout(stateLayout2);
    QGroupBox *stateGroup = new QGroupBox(tr("状态"));
    stateGroup->setAlignment(Qt::AlignCenter);
    stateGroup->setLayout(stateLayout);
    stateGroup->setObjectName("stateGroup");

    //缩放控制
    QSlider *scaleSlider = new QSlider;
    scaleSlider->setOrientation(Qt::Horizontal);
    scaleSlider->setRange(2,2*scaleValue);
    scaleSlider->setValue(scaleValue);
    scaleSlider->setObjectName("scaleSlider");

    QHBoxLayout *scaleLayout = new QHBoxLayout;
    scaleLayout->addWidget(scaleSlider);

    QGroupBox *scaleGroup = new QGroupBox(tr("缩放"));
    scaleGroup->setAlignment(Qt::AlignCenter);
    scaleGroup->setLayout(scaleLayout);
    scaleGroup->setObjectName("scaleGroup");

    connect(scaleSlider,SIGNAL(valueChanged(int)),this,SLOT(slotScale(int)));
    //控制面板布局

    QLabel *label_1 =new QLabel;
    font.setPointSize(12);
    label_1->setScaledContents(true);
    label_1 = new QLabel;
    label_1->setObjectName(QStringLiteral("label_1"));
    label_1->setFont(font);
    label_1->setAlignment(Qt::AlignCenter);
    label_1->setText("西安交通大学");
    QLabel *label_2 = new QLabel;
    label_2->setObjectName(QStringLiteral("label_2"));

    label_2->setText("智能网络与网络安全教育部重点实验室");
    QVBoxLayout *frameLayout = new QVBoxLayout;
    frameLayout->setObjectName("framelayout");
    frameLayout->setMargin(0);
    frameLayout->setSpacing(10);
    frameLayout->addLayout(startLayout);
    frameLayout->addWidget(straGroup);
    frameLayout->addWidget(infoGroup);
    frameLayout->addWidget(warnGroup);
    frameLayout->addWidget(breakGroup);
    frameLayout->addWidget(stateGroup);
    frameLayout->addWidget(scaleGroup);

    ctrlFrame->setLayout(frameLayout);
}
void System118_Widget::slotNew()        		//新建一个显示窗体
{
    slotClear();
    initScene_bus();

    MainWindow *newWin = new MainWindow;
    newWin->show();
}
void System118_Widget::slotPause(){
    if(!_auto)
        _loop++;
}

void System118_Widget::slotStart()
{
    QTimeLine *timer = new QTimeLine(5000);
    timer->setFrameRange(0, 100);
    timer->start();
    ReadInfo(":/IEEE118_data/IEEE118_data/init.txt",lineItems.size());
    ReadInfo(":/IEEE118_data/IEEE118_data/loop0.txt",lineItems.size());
    QMessageBox::information(this,tr("策略选择"),tr("请进行策略选择！"));
}

void System118_Widget::slotClear()        	//清除场景中所有的图元
{
    QList<QGraphicsItem*> listItem = scene->items();

    while(!listItem.empty())
    {
        scene->removeItem(listItem.at(0));
        listItem.removeAt(0);
    }
    initScene_bus();
}
void System118_Widget::slotScale(int value)
{
    qreal s;
    if(value>scaleValue)
        s=pow(1.1,(value-scaleValue));
    else
        s=pow(1/1.1,(scaleValue-value));

    view->scale(s,s);
    scaleValue=value;
}
void System118_Widget::slotStrategy(){
    int straIndex=strategyCom->currentIndex()+1;
    slotStrategyMenu(straIndex);
}
void System118_Widget::slotStrategyMenu(int straIndex){

    if(straIndex==1){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("策略选择"));
        msgBox.setText("确认使用裕度优先攻击策略？");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Ok:
            strategyCom->setCurrentIndex(0);
            stralabel->setText(QObject::tr("裕度"));

            for(int i=0;i<11;++i){
                if(_auto){
                    stateLabel->setText(QObject::tr("状态： ")+QString::number(_loop));
                    QString filename=":/IEEE118_data/IEEE118_data/loop"+QObject::tr("%1").arg(_loop++)+".txt";
                    ReadInfo(filename,lineItems.size());

                    delay(_Timer);
                }else{
                    while (!_auto) {
                        delay(6000);
                    }
                }
            }
            QMessageBox::information(this,tr("Evaluation"),tr("影响评估： 76.19% ！"),QMessageBox::NoButton);
            break;

        case QMessageBox::Cancel:
            // Cancel was clicked
            break;
        default:
            // should never be reached
            break;
        }
        // QMessageBox::information(this,tr("策略选择"),tr("是否确认使用裕度优先攻击策略？"),QMessageBox::Ok);


    }
}
void System118_Widget::slotstate(){
    int stateIndex=stateCom->currentIndex();
    _auto=false;
    if(stateIndex==0){
        flashboom->hide();
    }else{
        flashboom->show();
    }
    QVectorIterator<BusItem*> itbus(busItems);
    while(itbus.hasNext()){
        BusItem* temp=itbus.next();
        temp->setStop(false);
    }
    QVectorIterator<LineItem*> itline(lineItems);
    while(itline.hasNext()){
        LineItem* temp=itline.next();
        temp->setStop(false);
    }

    QVectorIterator<GenItem*> itgen(genItems);
    while(itgen.hasNext()){
        GenItem* temp=itgen.next();
        temp->setStop(false);
    }
    stateLabel->setText(QObject::tr("状态： ")+QString::number(stateIndex));
    QString filename=":/IEEE118_data/IEEE118_data/loop"+QObject::tr("%1").arg(stateIndex)+".txt";

    ReadInfo(filename,lineItems.size());
}
void System118_Widget::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}
void System118_Widget::ReadInfo(QString filename,int length){
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::green);
    linelabel->setFont(font);
    linelabel->setPalette(pa);
    buslabel->setFont(font);
    buslabel->setPalette(pa);
    genlabel->setFont(font);
    genlabel->setPalette(pa);
    QFile file(filename);
    QTextStream out(stdout);
    int linenum=1;
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        out<<endl<<"-----"<<filename<<endl;
        while (!file.atEnd()) {
            QString data = QString(file.readLine());
            if(linenum==1){
                QStringList v=data.split(QRegExp("\\s+"));
                int size=v.size()-1;
                if(size!=length){
                    out<<endl<<"*********"<<size<<"*************"<<endl;
                    out<<"Length error: "<<filename<<endl;
                    out<<"length: "<<length<<endl;
                    out<<endl<<"**********************"<<endl;
                }
                int count=size;  //count is shown in left control banel;
                for(int i=0;i<size;++i){
                    LineItem* linetemp=lineItems[i];
                    QString temp=v.at(i);
                    if(filename!=":/IEEE118_data/IEEE118_data/init.txt"){
                        linetemp->setLineCap(temp.toFloat());
                        float linef=temp.toFloat();
                        if(linef<9999&&linef!=0){
                            linetemp->setStart(true);
                            linetemp->setrunstate(true);
                        }
                        else if(linef==10000){
                            if(filename==":/IEEE118_data/IEEE118_data/loop1.txt"){
                                int index=strategyCom->currentIndex();
                                out<<"index :"<<index<<endl;
                                flashboom=new FlashItem;
                                flashboom->setPos(3.5*xUnit,Y2+5*yUnit);
                                scene->addItem(flashboom);
                            }
                            linetemp->setStop(true);
                            --count;
                        }else{
                            linetemp->setreset(true);
                            --count;
                        }
                    }else{
                        linetemp->setupLineCap(temp.toFloat());
                    }
                }
                out<<endl<<"count: "<<count<<endl;
                linelabel->setText(QObject::tr("线路： ")+QString::number(count));
                ++linenum;
            }
            else if(linenum==2){
                QStringList v=data.split(QRegExp("\\s+"));
                int size=v.size()-1;
                int count=size;
                for(int i=0;i<size;++i){

                    GenItem* gentemp=genItems[i];
                    QString temp=v.at(i);
                    if(filename!="init.txt"){
                        gentemp->setgenCap(temp.toFloat());
                        float geni=temp.toFloat();

                        if(geni==0){
                            gentemp->setStop(true);
                            --count;
                        }
                        else{
                            gentemp->setStart(true);
                            gentemp->setrunstate(true);
                        }
                    }else{
                        gentemp->setgenCap(temp.toFloat());
                    }
                }
                genlabel->setText(QObject::tr("机组： ")+QString::number(count));
                ++linenum;
            }
            else if(linenum==3){
                QStringList v=data.split(QRegExp(";"));
                int size=v.size();
                QVector<int> mark(118,1);
                out<<"quartier size: "<<size<<endl;
                for(int q=0;q<size;++q){

                    QString quatier=v.at(q);
                    QStringList qtemp=quatier.split(QRegExp("\\s+"));
                    int qsize=qtemp.size()-1;
                    out<<"qar: "<<q<<"  bus number: "<<qsize<<endl;
                    for(int i=0;i<qsize;++i){
                        QString stemp=qtemp.at(i);
                        int index=stemp.toInt()-1;
                        BusItem* bustemp=busItems[index];
                        bustemp->setBusquar(q);
                        mark[index]=0;
                        bustemp->setrunstate(true);
                        bustemp->setColor(40*q+40);
                    }
                }
                int count=118;
                for(int i=0;i<118;++i){
                    int bustemp=mark[i];
                    if(bustemp!=0){
                        --count;
                        busItems[i]->setStop(true);
                    }
                }
                buslabel->setText(QObject::tr("节点： ")+QString::number(count));
                linenum++;
            }
            else if(linenum==4){
                linenum++;
                QStringList v=data.split(QRegExp("\\s+"));
                int size=v.size();
                warnCom->clear();
                for(int i=0;i<size-1;++i){
                    QString temp=v.at(i);
                    warnCom->addItem(tr("Bus")+temp);
                }
            }
            else if(linenum==5){
                QStringList v=data.split(QRegExp("\\s+"));
                int size=v.size();
                breakCom->clear();
                for(int i=0;i<size;++i){
                    QString temp=v.at(i);
                    breakCom->addItem(tr("Bus")+temp);
                }
            }
        }
    }else{
        qDebug()<<"Read Error System118_Widget::ReadInfo"<<endl;
    }
}
void System118_Widget::ReadSet(QString filename)
{
    QFile file(filename);
    QTextStream out(stdout);
    int linenum=1;
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream floStream(&file);
        QTextCodec *codec=QTextCodec::codecForName("UTF-8");
        floStream.setCodec(codec);
        while (!file.atEnd()) {
            QString data = codec->fromUnicode(floStream.readLine());
            if(linenum==1){
                if(!data.isEmpty()){
                    setTitle(data);
                }
                ++linenum;
            }
            else if(linenum==2){
                int temp=data.toInt();
                if(temp>0){
                    setTimer(temp);
                }
            }
        }
    }else{
        qDebug()<<"Read Error "<<filename<<endl;
    }
}

void System118_Widget::setTimer(int timer)
{
    _Timer=timer;
}
int System118_Widget::getTimer()
{
    return _Timer;
}
void System118_Widget::setTitle(QString title){
    _Title=title;
}
QString System118_Widget::getTitle()
{
    return _Title;
}
void System118_Widget::mousePressEvent(QMouseEvent *e)
{
    QTextStream out(stdout);
    QString str="("+QString::number(e->x())+","+QString::number(e->y())+")";
    if(e->button()==Qt::LeftButton)
    {
        int straIndex=strategyCom->currentIndex()+1;
        if(straIndex==1&&_loop<11&&_loop>0){
            _auto=false;
            out<<"Here is left: "<<_loop<<endl;
            stateLabel->setText(QObject::tr("状态： ")+QString::number(_loop%10));
            QString filename=":/IEEE118_data/IEEE118_data/loop"+QObject::tr("%1").arg(_loop++)+".txt";
            ReadInfo(filename,lineItems.size());
        }
    }
    else if(e->button()==Qt::RightButton)
    {
        out<<"Here is right: "<<str<<endl;
        _auto=true;
    }

}
void System118_Widget::initScene_bus()
{
    QTextStream out(stdout);
    /* 1-10 Bus */
    busItems.append(new BusItem(xUnit,2*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(4*xUnit,2*yUnit,1.5*wUnit,hUnit));
    busItems.append(new BusItem(xUnit,5*yUnit,2*wUnit,hUnit));
    busItems.append(new BusItem(2.5*xUnit,7*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(xUnit,Y1+2*yUnit,2.5*wUnit,hUnit));

    busItems.append(new BusItem(6.2*xUnit,Y1+2*yUnit,0.75*wUnit,hUnit));
    busItems.append(new BusItem(8.5*xUnit,Y1+2*yUnit,0.75*wUnit,hUnit));
    busItems.append(new BusItem(3*xUnit,Y2,2*wUnit,hUnit));
    busItems.append(new BusItem(3*xUnit,Y2+4*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(3*xUnit,Y2+7*yUnit,wUnit,hUnit));
    /* 11-20 Bus */
    busItems.append(new BusItem(5.5*xUnit,7*yUnit,1.5*wUnit,hUnit));
    busItems.append(new BusItem(9*xUnit,5.5*yUnit,2*wUnit,hUnit));
    busItems.append(new BusItem(X1+3*xUnit,Y1+yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X1+5*xUnit,8*yUnit,1.5*wUnit,hUnit));
    busItems.append(new BusItem(X1+6*xUnit,Y1+yUnit,1.5*wUnit,hUnit));

    busItems.append(new BusItem(X1+0.5*xUnit,Y1+3.5*yUnit,hUnit,wUnit));
    busItems.append(new BusItem(X1+3*xUnit,Y1+7*yUnit,2*wUnit,hUnit));
    busItems.append(new BusItem(X1+8*xUnit,Y1+7*yUnit,1.3*wUnit,hUnit));
    busItems.append(new BusItem(X2-0.15*xUnit,Y1+5*yUnit,2*wUnit,hUnit));
    busItems.append(new BusItem(X2,Y2+yUnit,hUnit,wUnit));
    /* 21-30 Bus */
    busItems.append(new BusItem(X2,Y2+5*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X2+1.5*xUnit,Y2+6*yUnit,hUnit,wUnit));
    busItems.append(new BusItem(X2,Y3,1.5*wUnit,hUnit));
    busItems.append(new BusItem(X2+7*xUnit,Y2+8*yUnit,2*wUnit,hUnit));
    busItems.append(new BusItem(X1+5*xUnit,Y3+4*yUnit,4*wUnit,hUnit));

    busItems.append(new BusItem(X1+5*xUnit,Y3+yUnit,1.5*wUnit,hUnit));
    busItems.append(new BusItem(5*xUnit,Y2+9*yUnit,2.2*wUnit,hUnit));
    busItems.append(new BusItem(5.5*xUnit,Y2+6*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(5.5*xUnit,Y2+3*yUnit,2*wUnit,hUnit));
    busItems.append(new BusItem(X1+4.5*xUnit,Y2,1.5*wUnit,hUnit));
    /* 31-40 Bus */
    busItems.append(new BusItem(X1-0.2*xUnit,Y2+1.8*yUnit,hUnit,1.2*wUnit));
    busItems.append(new BusItem(X1,Y2+6*yUnit,2*wUnit,hUnit));
    busItems.append(new BusItem(X2+9*xUnit,6*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X3+xUnit,Y1+yUnit,1.5*wUnit,hUnit));
    busItems.append(new BusItem(X2+8*xUnit,Y1+4*yUnit,hUnit,wUnit));

    busItems.append(new BusItem(X3+2*xUnit,Y1+5*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X3+3*xUnit,9*yUnit,1.6*wUnit,hUnit));
    busItems.append(new BusItem(X3+4*xUnit,Y2+4*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X3+3*xUnit,4*yUnit,hUnit,wUnit));
    busItems.append(new BusItem(X3+2*xUnit,3*yUnit,2*wUnit,hUnit));

    /* 41-50 Bus */
    busItems.append(new BusItem(X3+7*xUnit,3*yUnit,0.75*wUnit,hUnit));
    busItems.append(new BusItem(X3+9*xUnit,3*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X3+6.7*xUnit,Y1+yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X3+8*xUnit,9*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X3+8.3*xUnit,Y1+4*yUnit,wUnit,hUnit));

    busItems.append(new BusItem(X3+7.8*xUnit,Y1+5*yUnit,hUnit,wUnit));
    busItems.append(new BusItem(X4,Y1+6.5*yUnit,hUnit,wUnit));
    busItems.append(new BusItem(X4+0.5*xUnit,Y1+3*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X4+1.5*xUnit,Y1+8*yUnit,3*wUnit,hUnit));
    busItems.append(new BusItem(X4+6.5*xUnit,Y1,hUnit,0.75*wUnit));
    /* 51-60 Bus */
    busItems.append(new BusItem(X4+9*xUnit,8.8*yUnit,hUnit,wUnit));
    busItems.append(new BusItem(X4+xUnit,6*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X4+1.5*xUnit,3*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X4+4*xUnit,3*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X5,3*yUnit,wUnit,hUnit));

    busItems.append(new BusItem(X4+7*xUnit,3*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X4+6.5*xUnit,6.5*yUnit,hUnit,0.75*wUnit));
    busItems.append(new BusItem(X4+9*xUnit,6*yUnit,hUnit,0.75*wUnit));
    busItems.append(new BusItem(X5+3*xUnit,yUnit,hUnit,3*wUnit));
    busItems.append(new BusItem(X5+3*xUnit,Y1,hUnit,wUnit));
    /* 61-70 Bus */
    busItems.append(new BusItem(X5+3*xUnit,Y1+4*yUnit,hUnit,2*wUnit));
    busItems.append(new BusItem(X5+xUnit,Y2+6*yUnit,2.5*wUnit,hUnit));
    busItems.append(new BusItem(X5+1.5*xUnit,6*yUnit,hUnit,wUnit));
    busItems.append(new BusItem(X5+xUnit,Y1+3*yUnit,hUnit,wUnit));
    busItems.append(new BusItem(X4+6*xUnit,Y2+9*yUnit,2*wUnit,hUnit));

    busItems.append(new BusItem(X4+5.5*xUnit,Y2+6*yUnit,1.5*wUnit,hUnit));
    busItems.append(new BusItem(X4+7.5*xUnit,Y2+4*yUnit,0.75*wUnit,hUnit));
    busItems.append(new BusItem(X4+3*xUnit,Y2+5*yUnit,hUnit,1.5*wUnit));
    busItems.append(new BusItem(X4+xUnit,Y2+4*yUnit,hUnit,2*wUnit));
    busItems.append(new BusItem(X3+2*xUnit,Y2+8*yUnit,1.5*wUnit,hUnit));
    /* 71-80 Bus */
    busItems.append(new BusItem(X3+xUnit,Y3+yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X3,Y3,wUnit,hUnit));
    busItems.append(new BusItem(X3+xUnit,Y3+3*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X3+1.7*xUnit,Y3+5.5*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X3+2*xUnit,Y3+7*yUnit,2*wUnit,hUnit));

    busItems.append(new BusItem(X3+8*xUnit,Y3+4.5*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X3+7.5*xUnit,Y3+7*yUnit,2.5*wUnit,hUnit));
    busItems.append(new BusItem(X4+xUnit,Y3+3*yUnit,0.75*wUnit,hUnit));
    busItems.append(new BusItem(X4+xUnit,Y3+1.5*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X4+2*xUnit,Y3+4.5*yUnit,2*wUnit,hUnit));
    /* 81-90 Bus */
    busItems.append(new BusItem(X4+3.2*xUnit,Y3+3*yUnit,0.75*wUnit,hUnit));
    busItems.append(new BusItem(X3+7.5*xUnit,Y3+9.5*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X3+2.5*xUnit,Y4+yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X3+1.5*xUnit,Y4+2.5*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X3+xUnit,Y4+5*yUnit,2*wUnit,hUnit));

    busItems.append(new BusItem(X3+2*xUnit,Y4+7*yUnit,0.75*wUnit,hUnit));
    busItems.append(new BusItem(X3+3.5*xUnit,Y4+8*yUnit,hUnit,wUnit));
    busItems.append(new BusItem(X3+5.5*xUnit,Y4+5*yUnit,0.75*wUnit,hUnit));
    busItems.append(new BusItem(X3+7.5*xUnit,Y4+5*yUnit,1.5*wUnit,hUnit));
    busItems.append(new BusItem(X3+6*xUnit,Y4+8*yUnit,2*wUnit,hUnit));
    /* 91-100 Bus */
    busItems.append(new BusItem(X4+xUnit,Y4+8*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X4+1.5*xUnit,Y4+5*yUnit,3*wUnit,hUnit));
    busItems.append(new BusItem(X4+3*xUnit,Y4+3.5*yUnit,0.75*wUnit,hUnit));
    busItems.append(new BusItem(X4+4*xUnit,Y4+yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X4+2*xUnit,Y4+yUnit,0.75*wUnit,hUnit));

    busItems.append(new BusItem(X4+2*xUnit,Y3+9.5*yUnit,1.5*wUnit,hUnit));
    busItems.append(new BusItem(X4+2.3*xUnit,Y3+8*yUnit,0.75*wUnit,hUnit));
    busItems.append(new BusItem(X4+5*xUnit,Y3+6*yUnit,1.5*wUnit,hUnit));
    busItems.append(new BusItem(X4+7*xUnit,Y3+4.5*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X4+6.5*xUnit,Y4+yUnit,1.5*wUnit,hUnit));
    /* 101-110 Bus */
    busItems.append(new BusItem(X4+6.5*xUnit,Y4+7*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X4+3*xUnit,Y4+7*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X4+9*xUnit,Y4+7*yUnit,1.5*wUnit,hUnit));
    busItems.append(new BusItem(X5,Y4+yUnit,0.75*wUnit,hUnit));
    busItems.append(new BusItem(X5+2*xUnit,Y4+yUnit,wUnit,hUnit));

    busItems.append(new BusItem(X5,Y3+8*yUnit,3*wUnit,hUnit));
    busItems.append(new BusItem(X5+4.5*xUnit,Y4+yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X5+4.5*xUnit,Y4+2.5*yUnit,hUnit,0.75*wUnit));
    busItems.append(new BusItem(X5+4.5*xUnit,Y4+5.2*yUnit,hUnit,0.75*wUnit));
    busItems.append(new BusItem(X5+1.5*xUnit,Y4+8.5*yUnit,1.5*wUnit,hUnit));

    /* 111-118 Bus */
    busItems.append(new BusItem(X5,Y4+9*yUnit,hUnit,1.5*wUnit));
    busItems.append(new BusItem(X5+5.5*xUnit,Y4+8.5*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X1+2*xUnit,Y1+7.5*yUnit,hUnit,wUnit));
    busItems.append(new BusItem(X1,Y2+8.5*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X1+3*xUnit,Y2+8.5*yUnit,wUnit,hUnit));

    busItems.append(new BusItem(X4+5*xUnit,Y2+7*yUnit,hUnit,0.75*wUnit));
    busItems.append(new BusItem(X1+5*xUnit,5.5*yUnit,wUnit,hUnit));
    busItems.append(new BusItem(X3+5*xUnit,Y3+4.5*yUnit,wUnit,hUnit));


    QVectorIterator<BusItem*> itbus(busItems);
    int busnum=0;
    while(itbus.hasNext()){
        BusItem* temp=itbus.next();
        temp->setBusnum(++busnum);
        scene->addItem(temp);
    }
    /* Line 1-5 */
    lineItems.append(new LineItem(QPointF(2*xUnit,2.5*yUnit),QPointF(2*xUnit,3*yUnit),
                                  QPointF(4.5*xUnit,3*yUnit),QPointF(4.5*xUnit,2.5*yUnit)));
    lineItems.append(new LineItem(QPointF(1.5*xUnit,2.5*yUnit),QPointF(1.5*xUnit,5*yUnit)));
    lineItems.append(new LineItem(QPointF(3*xUnit,7*yUnit),QPointF(3*xUnit,Y1+2*yUnit)));
    lineItems.append(new LineItem(QPointF(2*xUnit,5*yUnit),QPointF(2*xUnit,Y1+2*yUnit)
                                  ));
    lineItems.append(new LineItem(QPointF(5*xUnit,Y1+2*yUnit),QPointF(5*xUnit,Y1+3*yUnit),
                                  QPointF(6.3*xUnit,Y1+3*yUnit),QPointF(6.3*xUnit,Y1+2*yUnit)
                                  ));
    /* Line 6-10 */
    lineItems.append(new LineItem(QPointF(7.5*xUnit,Y1+2*yUnit),QPointF(7.5*xUnit,Y1+3*yUnit),
                                  QPointF(8.8*xUnit,Y1+3*yUnit),QPointF(8.8*xUnit,Y1+2*yUnit)));
    lineItems.append(new LineItem(QPointF(4*xUnit,Y2),QPointF(4*xUnit,Y2+4*yUnit)));
    lineItems.append(new LineItem(QPointF(4*xUnit,Y2),QPointF(4*xUnit,Y1+2*yUnit)));
    lineItems.append(new LineItem(QPointF(4*xUnit,Y2+4*yUnit),QPointF(4*xUnit,Y2+7*yUnit)));
    lineItems.append(new LineItem(QPointF(4*xUnit,7*yUnit),QPointF(4*xUnit,8*yUnit),
                                  QPointF(6*xUnit,8*yUnit), QPointF(6*xUnit,7*yUnit)));
    /* Line 11-15 */
    lineItems.append(new LineItem(QPointF(5*xUnit,Y1+2*yUnit),QPointF(5*xUnit,9*yUnit),
                                  QPointF(6.5*xUnit,9*yUnit),QPointF(6.5*xUnit,7*yUnit)));
    lineItems.append(new LineItem(QPointF(8*xUnit,7*yUnit),QPointF(8*xUnit,8*yUnit),
                                  QPointF(9.5*xUnit,8*yUnit),QPointF(9.5*xUnit,6*yUnit)));
    lineItems.append(new LineItem(QPointF(5.5*xUnit,2*yUnit),QPointF(5.5*xUnit,3*yUnit),
                                  QPointF(X1+0.5*xUnit,3*yUnit),QPointF(X1+0.5*xUnit,5.5*yUnit)));
    lineItems.append(new LineItem(QPointF(4*xUnit,5*yUnit),QPointF(4*xUnit,4*yUnit),
                                  QPointF(X1,4*yUnit),QPointF(X1,5.5*yUnit)));

    lineItems.append(new LineItem(QPointF(X1-0.5*xUnit,Y1+2*yUnit),QPointF(X1-0.5*xUnit,Y1+3*yUnit),
                                  QPointF(X1+0.5*xUnit,Y1+3*yUnit),QPointF(X1+0.5*xUnit,5.5*yUnit)));
    /* Line 16-20 */
    lineItems.append(new LineItem(QPointF(7.5*xUnit,7*yUnit),QPointF(7.5*xUnit,9*yUnit),
                                  QPointF(X1+3.5*xUnit,9*yUnit),QPointF(X1+3.5*xUnit,Y1+yUnit)));
    lineItems.append(new LineItem(QPointF(X1+2*xUnit,5.5*yUnit),QPointF(X1+2*xUnit,7*yUnit),
                                  QPointF(X1+5.5*xUnit,7*yUnit),QPointF(X1+5.5*xUnit,8.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X1+4*xUnit,Y1+yUnit),QPointF(X1+4*xUnit,Y1),
                                  QPointF(X1+6.5*xUnit,Y1),QPointF(X1+6.5*xUnit,Y1+yUnit)));
    lineItems.append(new LineItem(QPointF(X1+7*xUnit,8.5*yUnit),QPointF(X1+7*xUnit,Y1+yUnit)));

    lineItems.append(new LineItem(QPointF(X1+1.5*xUnit,5.5*yUnit),QPointF(X1+1.5*xUnit,Y1+4*yUnit),
                                  QPointF(X1+0.5*xUnit,Y1+4*yUnit)));
    /* Line 21-25 */
    lineItems.append(new LineItem(QPointF(X1+6.5*xUnit,Y1+yUnit),QPointF(X1+6.5*xUnit,Y1+7*yUnit)));
    lineItems.append(new LineItem(QPointF(X1+0.5*xUnit,Y1+5*yUnit),QPointF(X1+5.5*xUnit,Y1+5*yUnit),
                                  QPointF(X1+5.5*xUnit,Y1+7*yUnit)));
    lineItems.append(new LineItem(QPointF(X1+6.5*xUnit,Y1+7*yUnit),QPointF(X1+6.5*xUnit,Y1+8*yUnit),
                                  QPointF(X1+9*xUnit,Y1+8*yUnit),QPointF(X1+9*xUnit,Y1+7*yUnit)));
    lineItems.append(new LineItem(QPointF(X2,Y1+7*yUnit),QPointF(X2,Y1+5*yUnit)));

    lineItems.append(new LineItem(QPointF(X2+xUnit,Y1+5*yUnit),QPointF(X2+xUnit,Y2+1.5*yUnit),
                                  QPointF(X2,Y2+1.5*yUnit)));
    /* Line 26-30 */
    lineItems.append(new LineItem(QPointF(X1+8.5*xUnit,Y1+yUnit),QPointF(X1+8.5*xUnit,Y1+2.5*yUnit),
                                  QPointF(X2+2*xUnit,Y1+2.5*yUnit),QPointF(X2+2*xUnit,Y1+5*yUnit)));
    lineItems.append(new LineItem(QPointF(X2,Y2+3*yUnit),QPointF(X2+xUnit,Y2+3*yUnit),
                                  QPointF(X2+xUnit,Y2+5*yUnit)));
    lineItems.append(new LineItem(QPointF(X2+xUnit,Y2+5*yUnit),QPointF(X2+xUnit,Y2+7*yUnit),
                                  QPointF(X2+1.5*xUnit,Y2+7*yUnit)));
    lineItems.append(new LineItem(QPointF(X2+1.5*xUnit,Y2+8.5*yUnit),QPointF(X2+xUnit,Y2+8.5*yUnit),
                                  QPointF(X2+xUnit,Y3)));

    lineItems.append(new LineItem(QPointF(X2+2.5*xUnit,Y3),QPointF(X2+2.5*xUnit,Y2+9*yUnit),
                                  QPointF(X2+7.5*xUnit,Y2+9*yUnit),QPointF(X2+7.5*xUnit,Y2+8*yUnit)));
    /* Line 31-35 */
    lineItems.append(new LineItem(QPointF(X2+2*xUnit,Y3),QPointF(X2+2*xUnit,Y3+4*yUnit)));
    lineItems.append(new LineItem(QPointF(X1+6.5*xUnit,Y3+yUnit),QPointF(X1+6.5*xUnit,Y3+4*yUnit)));
    lineItems.append(new LineItem(QPointF(X1+5.5*xUnit,Y3+3*xUnit),QPointF(X1+5.5*xUnit,Y3+2.5*xUnit),
                                  QPointF(7.5*xUnit,Y3+2.5*xUnit),QPointF(7.5*xUnit,Y3-0.5*yUnit)));
    lineItems.append(new LineItem(QPointF(6.5*xUnit,Y2+9.5*yUnit),QPointF(6.5*xUnit,Y2+6.5*yUnit)));
    lineItems.append(new LineItem(QPointF(6.5*xUnit,Y2+6.5*yUnit),QPointF(6.5*xUnit,Y2+3.5*yUnit)));
    /* Line 36-40 */
    lineItems.append(new LineItem(QPointF(X1+6*xUnit,Y2),QPointF(X1+6*xUnit,Y1+7*yUnit)));
    lineItems.append(new LineItem(QPointF(5*xUnit,Y2),QPointF(5*xUnit,Y2+yUnit),
                                  QPointF(X1+5*xUnit,Y2+yUnit),QPointF(X1+5*xUnit,Y2)));
    lineItems.append(new LineItem(QPointF(X1+5.5*xUnit,Y3+yUnit), QPointF(X1+5.5*xUnit,Y2)));
    lineItems.append(new LineItem(QPointF(X1+5*xUnit,Y1+7*yUnit),QPointF(X1+5*xUnit,Y1+6*yUnit),
                                  QPointF(X1+xUnit,Y1+6*yUnit), QPointF(X1+xUnit,Y2+2*yUnit),
                                  QPointF(X1,Y2+2*yUnit)));
    lineItems.append(new LineItem(QPointF(7*xUnit,Y2+3*yUnit),QPointF(7*xUnit,Y2+2*yUnit),
                                  QPointF(X1,Y2+2*yUnit)));
    /* Line 41-45 */
    lineItems.append(new LineItem(QPointF(X2+0.5*xUnit,Y3),QPointF(X2+0.5*xUnit,Y2+8*yUnit),
                                  QPointF(X1+8.5*xUnit,Y2+8*yUnit),QPointF(X1+8.5*xUnit,Y2+4.5*yUnit),
                                  QPointF(X1+3.5*xUnit,Y2+4.5*yUnit),
                                  QPointF(X1+3.5*xUnit,Y2+6*yUnit)));
    lineItems.append(new LineItem(QPointF(X1,Y2+4*yUnit), QPointF(X1+xUnit,Y2+4*yUnit),
                                  QPointF(X1+xUnit,Y2+6*yUnit)));
    lineItems.append(new LineItem(QPointF(8*xUnit,Y2+9.4*yUnit),QPointF(8*xUnit,Y2+7*yUnit),
                                  QPointF(X1+0.5*xUnit,Y2+7*yUnit),QPointF(X1+0.5*xUnit,Y2+6*yUnit)));
    lineItems.append(new LineItem(QPointF(X1+8.5*xUnit,Y1+yUnit),QPointF(X1+8.5*xUnit,7.5*yUnit),
                                  QPointF(X2+9.5*xUnit,7.5*yUnit),QPointF(X2+9.5*xUnit,6.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X2+3*xUnit,Y1+5*yUnit),QPointF(X2+3*xUnit,Y1+2*yUnit),
                                  QPointF(X3+1.3*xUnit,Y1+2*yUnit),QPointF(X3+1.3*xUnit,Y1+yUnit)));
    /* Line 46-50 */
    lineItems.append(new LineItem(QPointF(X2+8*xUnit,Y1+6*yUnit), QPointF(X3+2.5*xUnit,Y1+6*yUnit),
                                  QPointF(X3+2.5*xUnit,Y1+5*yUnit)));
    lineItems.append(new LineItem(QPointF(X2+8*xUnit,Y1+4.5*yUnit),QPointF(X3,Y1+4.5*yUnit),
                                  QPointF(X3,8*yUnit),QPointF(X3+3.5*xUnit,8*yUnit),
                                  QPointF(X3+3.5*xUnit,9*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+0.5*xUnit,6*yUnit),QPointF(X3+0.5*xUnit,7.3*yUnit),
                                  QPointF(X3+4*xUnit,7.3*yUnit),QPointF(X3+4*xUnit,9*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+2.5*xUnit,Y1+yUnit),QPointF(X3+2.5*xUnit,Y1+5*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+3.5*xUnit,Y1+yUnit),QPointF(X3+3.5*xUnit,9*yUnit)));
    /* Line 51-55 */
    lineItems.append(new LineItem(QPointF(X3+5*xUnit,Y2+4*yUnit),QPointF(X3+5*xUnit,9*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+4.5*xUnit,9*yUnit),QPointF(X3+4.5*xUnit,5.5*yUnit),
                                  QPointF(X3+3*xUnit,5.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+5*xUnit,9*yUnit),QPointF(X3+5*xUnit,3*yUnit)));
    lineItems.append(new LineItem(QPointF(X1+7*xUnit,Y2),QPointF(X1+7*xUnit,Y1+9*yUnit),
                                  QPointF(X3+4.5*xUnit,Y1+9*yUnit),QPointF(X3+4.5*xUnit,Y2+4*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+3*xUnit,4.5*yUnit),QPointF(X3+4*xUnit,4.5*yUnit),
                                  QPointF(X3+4*xUnit,3*yUnit)));
    /* Line 56-60 */
    lineItems.append(new LineItem(QPointF(X3+5.5*xUnit,3*yUnit),QPointF(X3+5.5*xUnit,4*yUnit),
                                  QPointF(X3+7.5*xUnit,4*yUnit), QPointF(X3+7.5*xUnit,3*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+5.3*xUnit,3*yUnit),QPointF(X3+5.3*xUnit,5*yUnit),
                                  QPointF(X3+9.8*xUnit,5*yUnit),QPointF(X3+9.8*xUnit,3*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+8.5*xUnit,3*yUnit),QPointF(X3+8.5*xUnit,4*yUnit),
                                  QPointF(X3+9.3*xUnit,4*yUnit),QPointF(X3+9.3*xUnit,3*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+8.3*xUnit,Y1+yUnit),QPointF(X3+8.3*xUnit,9*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+3*xUnit,Y1+yUnit),QPointF(X3+3*xUnit,Y1+2*yUnit),
                                  QPointF(X3+7.5*xUnit,Y1+2*yUnit),QPointF(X3+7.5*xUnit,Y1+yUnit)));
    /* Line 61-65 */
    lineItems.append(new LineItem(QPointF(X3+9*xUnit,9*yUnit),QPointF(X3+9*xUnit,Y1+4*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+8.7*xUnit,Y1+4*yUnit),QPointF(X3+8.7*xUnit,Y1+5.3*yUnit),
                                  QPointF(X3+7.8*xUnit,Y1+5.3*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+7.8*xUnit,Y1+7*yUnit),QPointF(X4,Y1+7*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+7.8*xUnit,Y1+6*yUnit),QPointF(X4+xUnit,Y1+6*yUnit),
                                  QPointF(X4+xUnit,Y1+3*yUnit)));
    lineItems.append(new LineItem(QPointF(X4,Y1+7*yUnit),QPointF(X4+1.8*xUnit,Y1+7*yUnit),
                                  QPointF(X4+1.8*xUnit,Y1+8*yUnit)));
    /* Line 66-70 */
    lineItems.append(new LineItem(QPointF(X4+0.3*xUnit,3*yUnit),QPointF(X4+0.3*xUnit,Y1)
                                  ,QPointF(X4+3.2*xUnit,Y1),QPointF(X4+3.2*xUnit,Y1+8*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+0.3*xUnit,3*yUnit),QPointF(X4+0.3*xUnit,Y1)
                                  ,QPointF(X4+3.2*xUnit,Y1),QPointF(X4+3.2*xUnit,Y1+8*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+0.2*xUnit,Y1+4*yUnit),QPointF(X4+0.2*xUnit,Y1+5*yUnit)
                                  ,QPointF(X4+2*xUnit,Y1+5*yUnit),QPointF(X4+2*xUnit,Y1+8*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+2.5*xUnit,Y1+3*yUnit),QPointF(X4+2.5*xUnit,Y1+8*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+5.5*xUnit,Y1+8*yUnit),QPointF(X4+5.5*xUnit,Y1+yUnit),
                                  QPointF(X4+6.5*xUnit,Y1+yUnit)));
    /* Line 71-75 */
    lineItems.append(new LineItem(QPointF(X4+6.5*xUnit,Y1+8*yUnit),QPointF(X4+6.5*xUnit,Y1+3*yUnit)
                                  ,QPointF(X4+7*xUnit,Y1+3*yUnit),QPointF(X4+7*xUnit,Y1+0.5*yUnit)
                                  ,QPointF(X4+9*xUnit,Y1+0.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+9*xUnit,9.5*yUnit),QPointF(X4+2*xUnit,9.5*yUnit)
                                  ,QPointF(X4+2*xUnit,6*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+2*xUnit,6*yUnit) ,QPointF(X4+2*xUnit,3*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+2.5*xUnit,3*yUnit),QPointF(X4+2.5*xUnit,4*yUnit)
                                  ,QPointF(X4+4.5*xUnit,4*yUnit),QPointF(X4+4.5*xUnit,3*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+5*xUnit,3*yUnit) ,QPointF(X4+5*xUnit,Y1+8*yUnit)));
    /* Line 76-80 */
    lineItems.append(new LineItem(QPointF(X4+5*xUnit,8*yUnit) ,QPointF(X4+5*xUnit,3*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+5.5*xUnit,3*yUnit) ,QPointF(X4+5.5*xUnit,yUnit)
                                  ,QPointF(X5+0.5*xUnit,yUnit),QPointF(X5+0.5*xUnit,3*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+5.5*xUnit,3*yUnit) ,QPointF(X4+5.5*xUnit,4*yUnit)
                                  ,QPointF(X4+7.5*xUnit,4*yUnit),QPointF(X4+7.5*xUnit,3*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+0.5*xUnit,3*yUnit),QPointF(X5+0.5*xUnit,4*yUnit)
                                  ,QPointF(X4+8.5*xUnit,4*yUnit),QPointF(X4+8.5*xUnit,3*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+7.2*xUnit,3*yUnit) ,QPointF(X4+7.2*xUnit,7*yUnit)
                                  ,QPointF(X4+6.5*xUnit,7*yUnit)));
    /* Line 81-85 */
    lineItems.append(new LineItem(QPointF(X4+6.5*xUnit,Y1+0.2*yUnit) ,QPointF(X4+8*xUnit,Y1+0.2*yUnit)
                                  ,QPointF(X4+8*xUnit,8*yUnit) ,QPointF(X4+6.5*xUnit,8*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+8*xUnit,3*yUnit),QPointF(X4+8*xUnit,6.5*yUnit)
                                  ,QPointF(X4+9*xUnit,6.5*yUnit)));	 //bus 58 move to X4+9*xUnit
    lineItems.append(new LineItem(QPointF(X4+9*xUnit,9.2*yUnit) ,QPointF(X4+8.3*xUnit,9.2*yUnit)
                                  ,QPointF(X4+8.3*xUnit,7.3*yUnit) ,QPointF(X4+9*xUnit,7.3*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+5.8*xUnit,3*yUnit) ,QPointF(X4+5.8*xUnit,5.5*yUnit)
                                  ,QPointF(X5+3*xUnit,5.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+8.3*xUnit,3*yUnit),QPointF(X4+8.3*xUnit,5*yUnit)
                                  ,QPointF(X5+3*xUnit,5*yUnit)));
    /* Line 86-90 */
    lineItems.append(new LineItem(QPointF(X4+8.3*xUnit,3*yUnit) ,QPointF(X4+8.3*xUnit,5*yUnit)
                                  ,QPointF(X5+3*xUnit,5*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+xUnit,3*yUnit),QPointF(X5+xUnit,4.5*yUnit)
                                  ,QPointF(X5+3*xUnit,4.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+3*xUnit,6*yUnit),QPointF(X5+4*xUnit,6*yUnit)
                                  ,QPointF(X5+4*xUnit,Y1+0.5*yUnit),QPointF(X5+3*xUnit,Y1+0.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+3*xUnit,5*yUnit),QPointF(X5+5*xUnit,5*yUnit)
                                  ,QPointF(X5+5*xUnit,Y1+6*yUnit) ,QPointF(X5+3*xUnit,Y1+6*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+3*xUnit,Y1+1.5*yUnit),QPointF(X5+3.7*xUnit,Y1+1.5*yUnit)
                                  ,QPointF(X5+3.7*xUnit,Y1+5*yUnit),QPointF(X5+3*xUnit,Y1+5*yUnit)));
    /* Line 91-95 */
    lineItems.append(new LineItem(QPointF(X5+3*xUnit,Y1+1*yUnit),QPointF(X5+4.5*xUnit,Y1+1*yUnit)
                                  ,QPointF(X5+4.5*xUnit,Y2+6*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+3*xUnit,Y1+7*yUnit),QPointF(X5+4*xUnit,Y1+7*yUnit)
                                  ,QPointF(X5+4*xUnit,Y2+6*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+1.5*xUnit,6.5*yUnit),QPointF(X5+3*xUnit,6.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+1.5*xUnit,7*yUnit) ,QPointF(X5+0.5*xUnit,7*yUnit)
                                  ,QPointF(X5+0.5*xUnit,Y1+3.5*yUnit) ,QPointF(X5+xUnit,Y1+3.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+xUnit,Y1+5*yUnit) ,QPointF(X5+3*xUnit,Y1+5*yUnit)));
    /* Line 96-100 */
    lineItems.append(new LineItem(QPointF(X3+5.5*xUnit,Y2+4*yUnit) ,QPointF(X3+5.5*xUnit,Y3)
                                  ,QPointF(X4+7*xUnit,Y3),QPointF(X4+7*xUnit,Y2+9*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+xUnit,Y1+4.5*yUnit) ,QPointF(X4+9.5*xUnit,Y1+4.5*yUnit)
                                  ,QPointF(X4+9.5*xUnit,Y2+9*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+5*xUnit,Y1+8*yUnit),QPointF(X4+5*xUnit,Y2+yUnit)
                                  ,QPointF(X4+7*xUnit,Y2+yUnit) ,QPointF(X4+7*xUnit,Y2+9*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+5*xUnit,Y1+8*yUnit),QPointF(X4+5*xUnit,Y2+yUnit)
                                  ,QPointF(X4+7*xUnit,Y2+yUnit) ,QPointF(X4+7*xUnit,Y2+9*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+2*xUnit,Y2+6*yUnit),QPointF(X5+2*xUnit,Y2+5.3*yUnit)
                                  ,QPointF(X4+8.2*xUnit,Y2+5.3*yUnit),QPointF(X4+8.2*xUnit,Y2+6*yUnit)));
    /* Line 101-105 */
    lineItems.append(new LineItem(QPointF(X5+3*xUnit,Y2+6*yUnit),QPointF(X5+3*xUnit,Y2+4.8*yUnit)
                                  ,QPointF(X4+8.5*xUnit,Y2+4.8*yUnit),QPointF(X4+8.5*xUnit,Y2+4*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+7*xUnit,Y2+9*yUnit),QPointF(X4+7*xUnit,Y2+6*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+7.8*xUnit,Y2+6*yUnit) ,QPointF(X4+7.8*xUnit,Y2+4*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+7.3*xUnit,Y2+9*yUnit),QPointF(X4+7.3*xUnit,Y2+6.7*yUnit)
                                  ,QPointF(X4+3*xUnit,Y2+6.7*yUnit)));
    lineItems.append(new LineItem(QPointF(X4,Y1+8*yUnit),QPointF(X4+0.5*xUnit,Y1+8*yUnit)
                                  ,QPointF(X4+0.5*xUnit,Y2+6.5*yUnit) ,QPointF(X4+xUnit,Y2+6.5*yUnit)));
    /* Line 106-110 */
    lineItems.append(new LineItem(QPointF(X4+2.5*xUnit,Y1+8*yUnit),QPointF(X4+2.5*xUnit,Y1+9*yUnit)
                                  ,QPointF(X4+0.8*xUnit,Y1+9*yUnit),QPointF(X4+0.8*xUnit,Y2+5*yUnit)
                                  ,QPointF(X4+xUnit,Y2+5*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+3*xUnit,Y2+7.5*yUnit),QPointF(X4+xUnit,Y2+7.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+xUnit,Y2+7.5*yUnit) ,QPointF(X3+3.5*xUnit,Y2+7.5*yUnit)
                                  ,QPointF(X3+3.5*xUnit,Y2+8*yUnit)));
    lineItems.append(new LineItem(QPointF(X2+8*xUnit,Y2+8*yUnit),QPointF(X2+8*xUnit,Y2+7*yUnit)
                                  ,QPointF(X3+2.5*xUnit,Y2+7*yUnit),QPointF(X3+2.5*xUnit,Y2+8*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+2.2*xUnit,Y2+8*yUnit)
                                  ,QPointF(X3+2.2*xUnit,Y3+yUnit)));
    /* Line 111-115 */
    lineItems.append(new LineItem(QPointF(X3+0.5*xUnit,Y2+8*yUnit) ,QPointF(X3+0.5*xUnit,Y3)));
    lineItems.append(new LineItem(QPointF(X3+1.5*xUnit,Y3+yUnit) ,QPointF(X3+1.5*xUnit,Y3)));
    lineItems.append(new LineItem(QPointF(X3+2*xUnit,Y3+yUnit),QPointF(X3+2*xUnit,Y3+3*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+3.3*xUnit,Y2+8*yUnit),QPointF(X3+3.3*xUnit,Y3+5.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+4*xUnit,Y2+8*yUnit),QPointF(X3+4*xUnit,Y3+7*yUnit)));
    /* Line 116-120 */
    lineItems.append(new LineItem(QPointF(X4+xUnit,Y2+7*yUnit),QPointF(X3+8.5*xUnit,Y2+7*yUnit)
                                  ,QPointF(X3+8.5*xUnit,Y3+yUnit),QPointF(X3+4.5*xUnit,Y3+yUnit)
                                  ,QPointF(X3+4.5*xUnit,Y3+7*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+3*xUnit,Y3+5.5*yUnit),QPointF(X3+3*xUnit,Y3+7*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+9.5*xUnit,Y3+4.5*yUnit),QPointF(X3+9.5*xUnit,Y3+7*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+xUnit,Y2+9.5*yUnit),QPointF(X4+0.5*xUnit,Y2+9.5*yUnit)
                                  ,QPointF(X4+0.5*xUnit,Y3+7*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+5.5*xUnit,Y3+7*yUnit),QPointF(X3+5.5*xUnit,Y3+8*yUnit)
                                  ,QPointF(X3+8*xUnit,Y3+8*yUnit),QPointF(X3+8*xUnit,Y3+7*yUnit)));
    /* Line 120-125 */
    lineItems.append(new LineItem(QPointF(X4+1.5*xUnit,Y3+7*yUnit),QPointF(X4+1.5*xUnit,Y3+3*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+1.5*xUnit,Y3+3*yUnit) ,QPointF(X4+1.5*xUnit,Y3+1.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+2.3*xUnit,Y3+7*yUnit) ,QPointF(X4+2.3*xUnit,Y3+4.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+2.3*xUnit,Y3+7*yUnit),QPointF(X4+2.3*xUnit,Y3+4.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+2.8*xUnit,Y3+1.5*yUnit),QPointF(X4+2.8*xUnit,Y3+4.5*yUnit)));
    /* Line 126-130 */
    lineItems.append(new LineItem(QPointF(X4+3*xUnit,Y2+8*yUnit) ,QPointF(X4+3.5*xUnit,Y2+8*yUnit)
                                  ,QPointF(X4+3.5*xUnit,Y3+3*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+3.5*xUnit,Y3+4.5*yUnit) ,QPointF(X4+3.5*xUnit,Y3+3*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+8.5*xUnit,Y3+7*yUnit) ,QPointF(X3+8.5*xUnit,Y3+9.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+8*xUnit,Y3+9.5*yUnit) ,QPointF(X3+8*xUnit,Y4+0.3*yUnit)
                                  ,QPointF(X3+4*xUnit,Y4+0.3*yUnit),QPointF(X3+4*xUnit,Y4+yUnit)));
    lineItems.append(new LineItem(QPointF(X3+3*xUnit,Y4+yUnit) ,QPointF(X3+3*xUnit,Y4+2.5*yUnit)));
    /* Line 131-135 */
    lineItems.append(new LineItem(QPointF(X3+4*xUnit,Y4+yUnit),QPointF(X3+4*xUnit,Y4+5*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+2.5*xUnit,Y4+2.5*yUnit),QPointF(X3+2.5*xUnit,Y4+5*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+2.5*xUnit,Y4+5*yUnit),QPointF(X3+2.5*xUnit,Y4+7*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+2.9*xUnit,Y4+7*yUnit) ,QPointF(X3+2.9*xUnit,Y4+9*yUnit)
                                  ,QPointF(X3+3.5*xUnit,Y4+9*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+4.5*xUnit,Y4+5*yUnit),QPointF(X3+4.5*xUnit,Y4+6*yUnit)
                                  ,QPointF(X3+6*xUnit,Y4+6*yUnit),QPointF(X3+6*xUnit,Y4+5*yUnit)));
    /* Line 136-140 */
    lineItems.append(new LineItem(QPointF(X3+4*xUnit,Y4+5*yUnit),QPointF(X3+4*xUnit,Y4+6.5*yUnit)
                                  ,QPointF(X3+6.5*xUnit,Y4+6.5*yUnit),QPointF(X3+6.5*xUnit,Y4+5*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+6.5*xUnit,Y4+5*yUnit) ,QPointF(X3+6.5*xUnit,Y4+4*yUnit)
                                  ,QPointF(X3+8*xUnit,Y4+4*yUnit) ,QPointF(X3+8*xUnit,Y4+5*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+6.8*xUnit,Y4+5*yUnit),QPointF(X3+6.8*xUnit,Y4+8*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+6.8*xUnit,Y4+5*yUnit),QPointF(X3+6.8*xUnit,Y4+8*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+9*xUnit,Y4+8*yUnit),QPointF(X3+9*xUnit,Y4+9*yUnit)
                                  ,QPointF(X4+1.5*xUnit,Y4+9*yUnit),QPointF(X4+1.5*xUnit,Y4+8*yUnit)));
    /* Line 141-145 */
    lineItems.append(new LineItem(QPointF(X3+9.5*xUnit,Y4+5*yUnit),QPointF(X3+9.5*xUnit,Y4+6*yUnit)
                                  ,QPointF(X4+2*xUnit,Y4+6*yUnit) ,QPointF(X4+2*xUnit,Y4+5*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+9.5*xUnit,Y4+5*yUnit) ,QPointF(X3+9.5*xUnit,Y4+6*yUnit)
                                  ,QPointF(X4+2*xUnit,Y4+6*yUnit) ,QPointF(X4+2*xUnit,Y4+5*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+2.5*xUnit,Y4+8*yUnit),QPointF(X4+2.5*xUnit,Y4+5*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+3.5*xUnit,Y4+3.5*yUnit) ,QPointF(X4+3.5*xUnit,Y4+5*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+5*xUnit,Y4+5*yUnit),QPointF(X4+5*xUnit,Y4+yUnit)));
    /* Line 146-150 */
    lineItems.append(new LineItem(QPointF(X4+4.3*xUnit,Y4+3.5*yUnit) ,QPointF(X4+4.3*xUnit,Y4+yUnit)));
    lineItems.append(new LineItem(QPointF(X4+4.3*xUnit,Y4+yUnit),QPointF(X4+4.3*xUnit,Y4+0.5*yUnit)
                                  ,QPointF(X4+3*xUnit,Y4+0.5*yUnit),QPointF(X4+3*xUnit,Y4+yUnit)));
    lineItems.append(new LineItem(QPointF(X4+4.5*xUnit,Y3+4.5*yUnit),QPointF(X4+4.5*xUnit,Y3+9.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+9*xUnit,Y3+9.5*yUnit) ,QPointF(X3+9*xUnit,Y4+0.5*yUnit)
                                  ,QPointF(X4+2.5*xUnit,Y4+0.5*yUnit),QPointF(X4+2.5*xUnit,Y3+9.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+4.8*xUnit,Y4+yUnit),QPointF(X4+4.8*xUnit,Y3+9.5*yUnit)));
    /* Line 151-155 */
    lineItems.append(new LineItem(QPointF(X4+3.3*xUnit,Y3+4.5*yUnit),QPointF(X4+3.3*xUnit,Y3+8*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+5.3*xUnit,Y3+4.5*yUnit) ,QPointF(X4+5.3*xUnit,Y3+6*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+5.7*xUnit,Y3+4.5*yUnit),QPointF(X4+5.7*xUnit,Y3+5.5*yUnit)
                                  ,QPointF(X4+7.5*xUnit,Y3+5.5*yUnit) ,QPointF(X4+7.5*xUnit,Y3+4.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+7*xUnit,Y4+5*yUnit),QPointF(X4+7*xUnit,Y4+yUnit)));
    lineItems.append(new LineItem(QPointF(X4+5.5*xUnit,Y4+yUnit),QPointF(X4+5.5*xUnit,Y4)
                                  ,QPointF(X4+7*xUnit,Y4),QPointF(X4+7*xUnit,Y4+yUnit)));
    /* Line 156-160 */
    lineItems.append(new LineItem(QPointF(X4+2.8*xUnit,Y4+yUnit),QPointF(X4+2.8*xUnit,Y3+9.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+2.8*xUnit,Y3+9.5*yUnit),QPointF(X4+2.8*xUnit,Y3+8.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+7.5*xUnit,Y3+6*yUnit) ,QPointF(X4+7.5*xUnit,Y4+yUnit)));
    lineItems.append(new LineItem(QPointF(X4+8.5*xUnit,Y3+4.5*yUnit) ,QPointF(X4+8.5*xUnit,Y4+yUnit)));
    lineItems.append(new LineItem(QPointF(X4+8*xUnit,Y4+yUnit),QPointF(X4+8*xUnit,Y4+7*yUnit)));
    /* Line 161-165 */
    lineItems.append(new LineItem(QPointF(X4+3.5*xUnit,Y4+5*yUnit) ,QPointF(X4+3.5*xUnit,Y4+7*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+6.5*xUnit,Y4+7*yUnit),QPointF(X4+6.5*xUnit,Y4+6.5*yUnit)
                                  ,QPointF(X4+4.5*xUnit,Y4+6.5*yUnit),QPointF(X4+4.5*xUnit,Y4+7*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+8.5*xUnit,Y4+yUnit),QPointF(X4+8.5*xUnit,Y4+5*yUnit)
                                  ,QPointF(X4+9.5*xUnit,Y4+5*yUnit),QPointF(X4+9.5*xUnit,Y4+7*yUnit)));
    lineItems.append(new LineItem(QPointF(X4+9*xUnit,Y4+yUnit) ,QPointF(X4+9*xUnit,Y4+2*yUnit)
                                  ,QPointF(X5+0.3*xUnit,Y4+2*yUnit),QPointF(X5+0.3*xUnit,Y4+yUnit)));
    lineItems.append(new LineItem(QPointF(X5+0.5*xUnit,Y4+yUnit) ,QPointF(X5+0.5*xUnit,Y4+7*yUnit)));
    /* Line 166-170 */
    lineItems.append(new LineItem(QPointF(X5+xUnit,Y4+7*yUnit),QPointF(X5+xUnit,Y4+4*yUnit)
                                  ,QPointF(X5+3*xUnit,Y4+4*yUnit),QPointF(X5+3*xUnit,Y4+yUnit)));
    lineItems.append(new LineItem(QPointF(X4+9*xUnit,Y4+yUnit),QPointF(X4+9*xUnit,Y3+9*yUnit)
                                  ,QPointF(X5+0.5*xUnit,Y3+9*yUnit),QPointF(X5+0.5*xUnit,Y3+8*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+xUnit,Y4+yUnit),QPointF(X5+xUnit,Y4+2*yUnit)
                                  ,QPointF(X5+2.5*xUnit,Y4+2*yUnit) ,QPointF(X5+2.5*xUnit,Y4+yUnit)));
    lineItems.append(new LineItem(QPointF(X5+2.5*xUnit,Y4+yUnit),QPointF(X5+2.5*xUnit,Y3+8*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+3.5*xUnit,Y4+yUnit),QPointF(X5+3.5*xUnit,Y4+2*yUnit)
                                  ,QPointF(X5+5*xUnit,Y4+2*yUnit) ,QPointF(X5+5*xUnit,Y4+yUnit)));
    /* Line 171-175 */
    lineItems.append(new LineItem(QPointF(X5+3.25*xUnit,Y4+yUnit),QPointF(X5+3.25*xUnit,Y4+2.8*yUnit)
                                  ,QPointF(X5+4.5*xUnit,Y4+2.8*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+5.5*xUnit,Y4+yUnit),QPointF(X5+5.5*xUnit,Y3+8*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+4.5*xUnit,Y4+3.25*yUnit),QPointF(X5+4*xUnit,Y4+3.25*yUnit)
                                  ,QPointF(X5+4*xUnit,Y4+5.5*yUnit) ,QPointF(X5+4.5*xUnit,Y4+5.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+1.75*xUnit,Y4+7*yUnit),QPointF(X5+1.75*xUnit,Y4+8.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X5+4.5*xUnit,Y4+6.5*yUnit) ,QPointF(X5+4*xUnit,Y4+6.5*yUnit)
                                  ,QPointF(X5+4*xUnit,Y4+8.5*yUnit)));
    /* Line 176-180 */
    lineItems.append(new LineItem(QPointF(X5+2*xUnit,Y4+8.5*yUnit),QPointF(X5+2*xUnit,Y5),QPointF(X5,Y5)));
    lineItems.append(new LineItem(QPointF(X5+4*xUnit,Y4+8.5*yUnit),QPointF(X5+4*xUnit,Y4+9.5*yUnit)
                                  ,QPointF(X5+6*xUnit,Y4+9.5*yUnit) ,QPointF(X5+6*xUnit,Y4+8.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X1+3.5*xUnit,Y1+7*yUnit),QPointF(X1+3.5*xUnit,Y1+8*yUnit)
                                  ,QPointF(X1+2*xUnit,Y1+8*yUnit)));
    lineItems.append(new LineItem(QPointF(X1+1.5*xUnit,Y2+6*yUnit),QPointF(X1+1.5*xUnit,Y1+8*yUnit)
                                  ,QPointF(X1+2*xUnit,Y1+8*yUnit)));
    lineItems.append(new LineItem(QPointF(X1+xUnit,Y2+6*yUnit) ,QPointF(X1+xUnit,Y2+8.5*yUnit)));
    /* Line 181-186 */
    lineItems.append(new LineItem(QPointF(8.5*xUnit,Y2+6.5*xUnit),QPointF(8.5*xUnit,Y3),
                                  QPointF(X1+4*xUnit,Y3),QPointF(X1+4*xUnit,Y2+8.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X1+1.5*xUnit,Y2+8.5*yUnit),QPointF(X1+1.5*xUnit,Y2+9.5*yUnit),
                                  QPointF(X1+3.5*xUnit,Y2+9.5*yUnit),QPointF(X1+3.5*xUnit,Y2+8.5*yUnit)));

    lineItems.append(new LineItem(QPointF(X4+3*xUnit,Y2+7.5*yUnit),QPointF(X4+5*xUnit,Y2+7.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X1+2.5*xUnit,5.5*yUnit),QPointF(X1+2.5*xUnit,6.5*yUnit),
                                  QPointF(X1+5.5*xUnit,6.5*yUnit),QPointF(X1+5.5*xUnit,5.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+5.5*xUnit,Y3+7*yUnit),QPointF(X3+5.5*xUnit,Y3+4.5*yUnit)));
    lineItems.append(new LineItem(QPointF(X3+8.5*xUnit,Y3+4.5*yUnit),QPointF(X3+8.5*xUnit,Y3+5.5*yUnit)
                                  ,QPointF(X3+6.5*xUnit,Y3+5.5*yUnit),QPointF(X3+6.5*xUnit,Y3+4.5*yUnit)));

    /* Generator 1-5 */
    gLineItems.append(new LineItem(QPointF(3.5*xUnit,7*yUnit),QPointF(3.5*xUnit,7*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(3.5*xUnit-hUnit,7*yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(6.5*xUnit,Y1+2*yUnit), QPointF(6.5*xUnit,Y1+2*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(6.5*xUnit-hUnit,Y1+2*yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(3.5*xUnit,Y2),QPointF(3.5*xUnit,Y2-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(3.5*xUnit-hUnit,Y2-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(4*xUnit,Y2+7*yUnit+hUnit),QPointF(4*xUnit,Y2+7*yUnit+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(4*xUnit-hUnit,Y2+7*yUnit+2*hUnit)));

    gLineItems.append(new LineItem(QPointF(X1+2*xUnit,5.5*yUnit),QPointF(X1+2*xUnit,5.5*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X1+2*xUnit-hUnit,5.5*yUnit-3*hUnit)));
    /* Generator 6-10 */
    gLineItems.append(new LineItem(QPointF(X1+7.5*xUnit,Y1+yUnit+hUnit),QPointF(X1+7.5*xUnit,Y1+yUnit+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X1+7.5*xUnit-hUnit,Y1+yUnit+2*hUnit)));

    gLineItems.append(new LineItem(QPointF(X1+8.5*xUnit,Y1+7*yUnit),QPointF(X1+8.5*xUnit,Y1+7*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X1+8.5*xUnit-hUnit,Y1+7*yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X2+0.5*xUnit,Y1+5*yUnit),QPointF(X2+0.5*xUnit,Y1+5*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X2+0.5*xUnit-hUnit,Y1+5*yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X2+9*xUnit,Y2+8*yUnit+hUnit),QPointF(X2+9*xUnit,Y2+8*yUnit+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X2+9*xUnit-hUnit,Y2+8*yUnit+2*hUnit)));

    gLineItems.append(new LineItem(QPointF(X1+9*xUnit,Y3+4*yUnit),QPointF(X1+9*xUnit,Y3+4*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X1+9*xUnit-hUnit,Y3+4*yUnit-3*hUnit)));
    /* Generator 11-15 */
    gLineItems.append(new LineItem(QPointF(X1+7*xUnit,Y3+yUnit),QPointF(X1+7*xUnit,Y3+yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X1+7*xUnit-hUnit,Y3+yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(5.5*xUnit,Y2+9*yUnit+hUnit),QPointF(5.5*xUnit,Y2+9*yUnit+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(5.5*xUnit-hUnit,Y2+9*yUnit+2*hUnit)));

    gLineItems.append(new LineItem(QPointF(X1-0.2*xUnit,Y2+4*yUnit),QPointF(X1-0.2*xUnit-hUnit,Y2+4*yUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X1-0.2*xUnit-3*hUnit,Y2+4*yUnit-hUnit)));

    gLineItems.append(new LineItem(QPointF(X1+3*xUnit,Y2+6*yUnit),QPointF(X1+3*xUnit,Y2+6*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X1+3*xUnit-hUnit,Y2+6*yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X3+2.5*xUnit,Y1+yUnit),QPointF(X3+2.5*xUnit,Y1+yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X3+2.5*xUnit-hUnit,Y1+yUnit-3*hUnit)));

    /* Generator 16-20 */
    gLineItems.append(new LineItem(QPointF(X3+3.5*xUnit,Y1+5*yUnit),QPointF(X3+3.5*xUnit,Y1+5*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X3+3.5*xUnit-hUnit,Y1+5*yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X3+4*xUnit,3*yUnit),QPointF(X3+4*xUnit,3*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X3+4*xUnit-hUnit,3*yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X3+9.5*xUnit,3*yUnit),QPointF(X3+9.5*xUnit,3*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X3+9.5*xUnit-hUnit,3*yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X3+7.8*xUnit,Y1+5.5*yUnit),QPointF(X3+7.8*xUnit-hUnit,Y1+5.5*yUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X3+7.8*xUnit-3*hUnit,Y1+5.5*yUnit-hUnit)));

    gLineItems.append(new LineItem(QPointF(X4+7*xUnit,Y1+8*yUnit),QPointF(X4+7*xUnit,Y1+8*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X4+7*xUnit-hUnit,Y1+8*yUnit-3*hUnit)));

    /* Generator 21-25 */
    gLineItems.append(new LineItem(QPointF(X4+4.5*xUnit,3*yUnit),QPointF(X4+4.5*xUnit,3*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X4+4.5*xUnit-hUnit,3*yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X5+1.5*xUnit,3*yUnit),QPointF(X5+1.5*xUnit,3*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X5+1.5*xUnit-hUnit,3*yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X4+8.5*xUnit,3*yUnit),QPointF(X4+8.5*xUnit,3*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X4+8.5*xUnit-hUnit,3*yUnit-3*hUnit)));


    gLineItems.append(new LineItem(QPointF(X5+3*xUnit+hUnit,1.5*yUnit),QPointF(X5+3*xUnit+2*hUnit,1.5*yUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X5+3*xUnit+2*hUnit,1.5*yUnit-hUnit)));

    gLineItems.append(new LineItem(QPointF(X5+3*xUnit,Y1+7*yUnit),QPointF(X5+3*xUnit-hUnit,Y1+7*yUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X5+3*xUnit-3*hUnit,Y1+7*yUnit-hUnit)));

    /* Generator 26-30 */
    gLineItems.append(new LineItem(QPointF(X5+5*xUnit,Y2+6*yUnit+hUnit),QPointF(X5+5*xUnit,Y2+6*yUnit+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X5+5*xUnit-hUnit,Y2+6*yUnit+2*hUnit)));

    gLineItems.append(new LineItem(QPointF(X4+8*xUnit,Y2+9*yUnit+hUnit),QPointF(X4+8*xUnit,Y2+9*yUnit+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X4+8*xUnit-hUnit,Y2+9*yUnit+2*hUnit)));

    gLineItems.append(new LineItem(QPointF(X4+8*xUnit,Y2+6*yUnit+hUnit),QPointF(X4+8*xUnit,Y2+6*yUnit+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X4+8*xUnit-hUnit,Y2+6*yUnit+2*hUnit)));

    gLineItems.append(new LineItem(QPointF(X4+xUnit+hUnit,Y2+4.5*yUnit),QPointF(X4+xUnit+2*hUnit,Y2+4.5*yUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X4+xUnit+2*hUnit,Y2+4.5*yUnit-hUnit)));

    gLineItems.append(new LineItem(QPointF(X3+4.5*xUnit,Y2+8*yUnit+hUnit),QPointF(X3+4.5*xUnit,Y2+8*yUnit+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X3+4.5*xUnit-hUnit,Y2+8*yUnit+2*hUnit)));

    /* Generator 31-35 */
    gLineItems.append(new LineItem(QPointF(X3+0.5*xUnit,Y3+hUnit),QPointF(X3+0.5*xUnit,Y3+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X3+0.5*xUnit-hUnit,Y3+2*hUnit)));

    gLineItems.append(new LineItem(QPointF(X3+1.5*xUnit,Y3+yUnit+hUnit),QPointF(X3+1.5*xUnit,Y3+yUnit+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X3+1.5*xUnit-hUnit,Y3+yUnit+2*hUnit)));

    gLineItems.append(new LineItem(QPointF(X3+2.2*xUnit,Y3+5.5*yUnit),QPointF(X3+2.2*xUnit,Y3+5.5*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X3+2.2*xUnit-hUnit,Y3+5.5*yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X3+8.5*xUnit,Y3+4.5*yUnit),QPointF(X3+8.5*xUnit,Y3+4.5*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X3+8.5*xUnit-hUnit,Y3+4.5*yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X4+xUnit,Y3+7*yUnit+hUnit),QPointF(X4+xUnit,Y3+7*yUnit+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X4+xUnit-hUnit,Y3+7*yUnit+2*hUnit)));

    /* Generator 36-40 */
    gLineItems.append(new LineItem(QPointF(X4+4*xUnit,Y3+4.5*yUnit+hUnit),QPointF(X4+4*xUnit,Y3+4.5*yUnit+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X4+4*xUnit-hUnit,Y3+4.5*yUnit+2*hUnit)));

    gLineItems.append(new LineItem(QPointF(X3+8.5*xUnit,Y3+9.5*yUnit+hUnit),QPointF(X3+8.5*xUnit,Y3+9.5*yUnit+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X3+8.5*xUnit-hUnit,Y3+9.5*yUnit+2*hUnit)));

    gLineItems.append(new LineItem(QPointF(X3+1.5*xUnit,Y4+5*yUnit+hUnit),QPointF(X3+1.5*xUnit,Y4+5*yUnit+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X3+1.5*xUnit-hUnit,Y4+5*yUnit+2*hUnit)));

    gLineItems.append(new LineItem(QPointF(X3+3.5*xUnit+hUnit,Y4+9*yUnit),QPointF(X3+3.5*xUnit+2*hUnit,Y4+9*yUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X3+3.5*xUnit+2*hUnit,Y4+9*yUnit-hUnit)));

    gLineItems.append(new LineItem(QPointF(X3+9*xUnit,Y4+5*yUnit),QPointF(X3+9*xUnit,Y4+5*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X3+9*xUnit-hUnit,Y4+5*yUnit-3*hUnit)));

    /* Generator 41-45 */
    gLineItems.append(new LineItem(QPointF(X3+9.5*xUnit,Y4+8*yUnit),QPointF(X3+9.5*xUnit,Y4+8*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X3+9.5*xUnit-hUnit,Y4+8*yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X4+2.5*xUnit,Y4+8*yUnit+hUnit),QPointF(X4+2.5*xUnit,Y4+8*yUnit+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X4+2.5*xUnit-hUnit,Y4+8*yUnit+2*hUnit)));

    gLineItems.append(new LineItem(QPointF(X4+2*xUnit,Y4+5*yUnit),QPointF(X4+2*xUnit,Y4+5*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X4+2*xUnit-hUnit,Y4+5*yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X4+8*xUnit,Y3+4.5*yUnit),QPointF(X4+8*xUnit,Y3+4.5*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X4+8*xUnit-hUnit,Y3+4.5*yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X4+7.5*xUnit,Y4+yUnit+hUnit),QPointF(X4+7.5*xUnit,Y4+yUnit+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X4+7.5*xUnit-hUnit,Y4+yUnit+2*hUnit)));

    /* Generator 46-50 */
    gLineItems.append(new LineItem(QPointF(X4+9.5*xUnit,Y4+7*yUnit+hUnit),QPointF(X4+9.5*xUnit,Y4+7*yUnit+2*hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X4+9.5*xUnit-hUnit,Y4+7*yUnit+2*hUnit)));

    gLineItems.append(new LineItem(QPointF(X5+0.5*xUnit,Y4+yUnit),QPointF(X5+0.5*xUnit,Y4+yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X5+0.5*xUnit-hUnit,Y4+yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X5+3.5*xUnit,Y4+yUnit),QPointF(X5+3.5*xUnit,Y4+yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X5+3.5*xUnit-hUnit,Y4+yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X5+6*xUnit,Y4+yUnit),QPointF(X5+6*xUnit,Y4+yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X5+6*xUnit-hUnit,Y4+yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X5+3*xUnit,Y4+8.5*yUnit),QPointF(X5+3*xUnit,Y4+8.5*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X5+3*xUnit-hUnit,Y4+8.5*yUnit-3*hUnit)));

    /* Generator 51-54 */
    gLineItems.append(new LineItem(QPointF(X5,Y5+yUnit),QPointF(X5-hUnit,Y5+yUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X5-3*hUnit,Y5+yUnit-hUnit)));

    gLineItems.append(new LineItem(QPointF(X5+6.5*xUnit,Y4+8.5*yUnit),QPointF(X5+6.5*xUnit,Y4+8.5*yUnit-hUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X5+6.5*xUnit-hUnit,Y4+8.5*yUnit-3*hUnit)));

    gLineItems.append(new LineItem(QPointF(X1+2*xUnit+hUnit,Y1+8.5*yUnit),QPointF(X1+2*xUnit+2*hUnit,Y1+8.5*yUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X1+2*xUnit+2*hUnit,Y1+8.5*yUnit-hUnit)));

    gLineItems.append(new LineItem(QPointF(X4+5*xUnit+hUnit,Y2+7.4*yUnit),QPointF(X4+5*xUnit+2*hUnit,Y2+7.4*yUnit)));
    genItems.append(new GenItem(hUnit,QPointF(X4+5*xUnit+2*hUnit,Y2+7.4*yUnit-hUnit)));

    QVectorIterator<LineItem*> itline(lineItems);
    while(itline.hasNext()){
        LineItem* temp=itline.next();
        scene->addItem(temp);
    }
    QVectorIterator<LineItem*> itgline(gLineItems);
    while(itgline.hasNext()){
        LineItem* temp=itgline.next();
        scene->addItem(temp);
    }
    QVectorIterator<GenItem*> itgen(genItems);
    while(itgen.hasNext()){
        GenItem* temp=itgen.next();
        scene->addItem(temp);
    }
    /* Load 1-10 */
    loadItems.append(new Loaditem(QPointF(1.2*xUnit,2*yUnit)));
    loadItems.append(new Loaditem(QPointF(5.5*xUnit,2*yUnit),1));
    loadItems.append(new Loaditem(QPointF(1.3*xUnit,5*yUnit)));
    loadItems.append(new Loaditem(QPointF(3.5*xUnit,7*yUnit)));
    loadItems.append(new Loaditem(QPointF(6.5*xUnit,Y1+2*yUnit)));

    loadItems.append(new Loaditem(QPointF(9*xUnit,Y1+2*yUnit)));
    loadItems.append(new Loaditem(QPointF(7*xUnit,7*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X1+xUnit,5.5*yUnit)));
    loadItems.append(new Loaditem(QPointF(X1+4*xUnit,Y1+yUnit)));
    loadItems.append(new Loaditem(QPointF(X1+6.5*xUnit,8*yUnit)));

    /* Load 11-20 */
    loadItems.append(new Loaditem(QPointF(X1+8*xUnit,Y1+yUnit),1));
    loadItems.append(new Loaditem(QPointF(X1+0.5*xUnit,Y1+4.5*yUnit),2));
    loadItems.append(new Loaditem(QPointF(X1+5*xUnit,Y1+7*yUnit)));
    loadItems.append(new Loaditem(QPointF(X1+9.8*xUnit,Y1+7*yUnit)));
    loadItems.append(new Loaditem(QPointF(X2+3*xUnit,Y1+5*yUnit)));

    loadItems.append(new Loaditem(QPointF(X2,Y2+2*yUnit),2));
    loadItems.append(new Loaditem(QPointF(X2+1.5*xUnit,Y2+5*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X2+1.5*xUnit,Y2+7*yUnit),3));
    loadItems.append(new Loaditem(QPointF(X2+xUnit,Y3)));
    loadItems.append(new Loaditem(QPointF(8*xUnit,Y2+9*yUnit)));

    /* Load 21-30 */
    loadItems.append(new Loaditem(QPointF(7*xUnit,Y2+6*yUnit)));
    loadItems.append(new Loaditem(QPointF(8*xUnit,Y2+3*yUnit)));
    loadItems.append(new Loaditem(QPointF(X1-0.2*xUnit,Y1+3*yUnit),3));
    loadItems.append(new Loaditem(QPointF(X1+3*xUnit,Y2+6*yUnit)));
    loadItems.append(new Loaditem(QPointF(X3,6*yUnit),1));

    loadItems.append(new Loaditem(QPointF(X3+2*xUnit,Y1+yUnit)));
    loadItems.append(new Loaditem(QPointF(X2+8*xUnit,Y1+5*yUnit),2));
    loadItems.append(new Loaditem(QPointF(X3+3.5*xUnit,Y1+5*yUnit)));
    loadItems.append(new Loaditem(QPointF(X3+3*xUnit,5*yUnit),2));
    loadItems.append(new Loaditem(QPointF(X3+5*xUnit,3*yUnit),1));
    /* Load 31-40 */
    loadItems.append(new Loaditem(QPointF(X3+7.5*xUnit,3*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X4+0.5*xUnit,3*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X3+7.7*xUnit,Y1+yUnit),1));
    loadItems.append(new Loaditem(QPointF(X3+9*xUnit,9*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X3+9.8*xUnit,Y1+4*yUnit),1));

    loadItems.append(new Loaditem(QPointF(X3+7.8*xUnit,Y1+6.5*yUnit),2));
    loadItems.append(new Loaditem(QPointF(X4,Y1+8*yUnit),2));
    loadItems.append(new Loaditem(QPointF(X4+1.5*xUnit,Y1+3*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X4+3*xUnit,Y1+8*yUnit)));
    loadItems.append(new Loaditem(QPointF(X4+6.5*xUnit,Y1+0.5*yUnit),2));
    /* Load 41-50 */
    loadItems.append(new Loaditem(QPointF(X4+9*xUnit,9.8*yUnit),3));
    loadItems.append(new Loaditem(QPointF(X4+2.5*xUnit,6*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X4+2.5*xUnit,3*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X4+5.2*xUnit,3*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X5+xUnit,3*yUnit),1));

    loadItems.append(new Loaditem(QPointF(X4+7.5*xUnit,3*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X4+6.5*xUnit,7*yUnit),2));
    loadItems.append(new Loaditem(QPointF(X4+9*xUnit,6.5*yUnit),3));
    loadItems.append(new Loaditem(QPointF(X5+3*xUnit,3*yUnit),3));
    loadItems.append(new Loaditem(QPointF(X5+3*xUnit,Y1+yUnit),2));
    /* Load 51-60 */
    loadItems.append(new Loaditem(QPointF(X5+1.5*xUnit,Y2+6*yUnit)));
    loadItems.append(new Loaditem(QPointF(X4+6*xUnit,Y2+6*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X4+8*xUnit,Y2+4*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X3+3*xUnit,Y2+8*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X3+2*xUnit,Y3+5.5*yUnit)));

    loadItems.append(new Loaditem(QPointF(X3+2.5*xUnit,Y3+7*yUnit)));
    loadItems.append(new Loaditem(QPointF(X3+9.5*xUnit,Y3+4.5*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X4,Y3+7*yUnit)));
    loadItems.append(new Loaditem(QPointF(X4+1.8*xUnit,Y3+3*yUnit)));
    loadItems.append(new Loaditem(QPointF(X4+2*xUnit,Y3+1.5*yUnit),1));
    /* Load 61-70 */
    loadItems.append(new Loaditem(QPointF(X4+5.5*xUnit,Y3+4.5*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X3+8*xUnit,Y3+9.5*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X3+4*xUnit,Y4+yUnit)));
    loadItems.append(new Loaditem(QPointF(X3+2*xUnit,Y4+2.5*yUnit)));
    loadItems.append(new Loaditem(QPointF(X3+1.5*xUnit,Y4+5*yUnit),1));

    loadItems.append(new Loaditem(QPointF(X3+2.5*xUnit,Y4+7*yUnit)));
    loadItems.append(new Loaditem(QPointF(X3+6*xUnit,Y4+5*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X3+6.5*xUnit,Y4+8*yUnit)));
    loadItems.append(new Loaditem(QPointF(X4+3*xUnit,Y4+5*yUnit)));
    loadItems.append(new Loaditem(QPointF(X4+3.25*xUnit,Y4+3.5*yUnit),1));
    /* Load 71-80 */
    loadItems.append(new Loaditem(QPointF(X4+5.5*xUnit,Y4+yUnit)));
    loadItems.append(new Loaditem(QPointF(X4+2.4*xUnit,Y4+yUnit)));
    loadItems.append(new Loaditem(QPointF(X3+4*xUnit,Y4+yUnit)));
    loadItems.append(new Loaditem(QPointF(X4+3.5*xUnit,Y3+8*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X4+5.5*xUnit,Y3+6*yUnit)));

    loadItems.append(new Loaditem(QPointF(X4+8*xUnit,Y4+yUnit),1));
    loadItems.append(new Loaditem(QPointF(X4+7.5*xUnit,Y4+7*yUnit)));
    loadItems.append(new Loaditem(QPointF(X4+4*xUnit,Y4+7*yUnit)));
    loadItems.append(new Loaditem(QPointF(X5+0.5*xUnit,Y4+7*yUnit)));
    loadItems.append(new Loaditem(QPointF(X5+1.2*xUnit,Y4+yUnit),1));
    /* Load 81-90 */
    loadItems.append(new Loaditem(QPointF(X5+3*xUnit,Y4+yUnit),1));
    loadItems.append(new Loaditem(QPointF(X5+3*xUnit,Y3+8*yUnit),1));
    loadItems.append(new Loaditem(QPointF(X5+6*xUnit,Y4+yUnit)));
    loadItems.append(new Loaditem(QPointF(X5+4.5*xUnit,Y4+3*yUnit),3));
    loadItems.append(new Loaditem(QPointF(X5+4.5*xUnit,Y4+6*yUnit),3));

    loadItems.append(new Loaditem(QPointF(X5+3*xUnit,Y4+8.5*yUnit)));
    loadItems.append(new Loaditem(QPointF(X5+7*xUnit,Y4+8.5*yUnit)));
    loadItems.append(new Loaditem(QPointF(X1+0.5*xUnit,Y2+8.5*yUnit)));
    loadItems.append(new Loaditem(QPointF(X1+4.5*xUnit,Y2+8.5*yUnit)));
    loadItems.append(new Loaditem(QPointF(X1+6*xUnit,5.5*yUnit)));

    loadItems.append(new Loaditem(QPointF(X3+6*xUnit,Y3+4.5*yUnit),1));


    QVectorIterator<Loaditem*> itload(loadItems);
    while(itload.hasNext()){
        Loaditem* temp=itload.next();
        scene->addItem(temp);
    }
}

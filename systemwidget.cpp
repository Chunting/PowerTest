#include "systemwidget.h"
#include "mainwindow.h"
#include "flashitem.h"
#include "startitem.h"
#include "busitem.h"
#include "lineitem.h"
#include "math.h"
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

SystemWidget::SystemWidget(QWidget *parent) :
    QWidget(parent)
{
    _loop=1;
    _auto=true;
    x1=60;
    y1=60;
    Width=60;
    Height=10;
    Hspace=30;
    Vspace=70;
    shortY=15;
    shortX=15;
    Hbase=Width+Hspace;
    Vbase=Height+Vspace;
    Radius=shortX;
    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,8*Hbase+2*x1,6*Vbase+2*y1);
    scene->setBackgroundBrush(QColor(200,200,200));
    initScene_bus();
    view = new QGraphicsView();
    view->setScene(scene);

    view->setMinimumSize(400,400);
    ctrlFrame = new QFrame;
    ctrlFrame->setMaximumWidth(2*Hbase);
    //ctrlFrame->setFrameRect(QRect(0,0,Hbase,Vbase));

    createControlFrame(30,41,6);
    //主窗口布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setObjectName("mainlayout");
    mainLayout->setMargin(10);
    mainLayout->setSpacing(20);
    mainLayout->addWidget(ctrlFrame);
    mainLayout->addWidget(view);

    QVBoxLayout *vLayout = new QVBoxLayout;
    QFont font;
    font.setPointSize(48);
    font.setBold(true);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    label = new QLabel;
    label->setObjectName(QStringLiteral("label"));
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);
    label->setFrameShape(QFrame::StyledPanel);
    label->setText("电网异常状态传播仿真系统");
    label->setPalette(pa);
    vLayout->addWidget(label);
    vLayout->addLayout(mainLayout);
    setLayout(vLayout);
    setWindowTitle(tr("Power Flow Test"));   //设置主窗体的标题
}
void SystemWidget::mousePressEvent(QMouseEvent *e)
{
    QTextStream out(stdout);
    int straIndex = strategyCom->currentIndex()+1;
    if(e->button() == Qt::LeftButton)
    {
        if(straIndex == 2){
            _auto=false;
            if(_loop>0 && _loop<6){
                stateLabel->setText(QObject::tr("状态： ")+QString::number(_loop%6));
                int count = stateCom->count();
                if( count >= 0 && count < 6 )
                    stateCom->addItem(QObject::tr("状态 ")+QString::number(_loop%6));
                QString filename = ":/data/loop"+QObject::tr("%1").arg(_loop) + ".txt";
                _loop++;
                ReadInfo(filename,lineItems.size());
            }
        }
        if(straIndex == 4){
            _auto=false;
            if(_loop>0 && _loop<7){
                stateLabel->setText(QObject::tr("状态： ")+QString::number(_loop));
                int count = stateCom->count();
                if( count >= 0 && count < 7 )
                    stateCom->addItem(QObject::tr("状态 ")+QString::number(_loop));
                QString filename = ":/Bus30_System/30_system_data/loop"+QObject::tr("%1").arg(_loop-1)+".txt";
                _loop++;
                ReadInfo(filename,lineItems.size());
            }
        }
    }
    else if(e->button() == Qt::RightButton)
    {
        //   out<<"Here is right: "<<str<<endl;
        _auto=true;
        /*
        if( straIndex == 4 ) {
            while(_auto && _loop < 7 && _loop > 0) {
                    stateLabel->setText(QObject::tr("状态： ") + QString::number(_loop%7));
                    ReadInfo( ":/Bus30_System/30_system_data/loop" + QObject::tr("%1").arg(_loop) + ".txt", lineItems.size());
                    int count = stateCom->count();
                    if( count >= 0 && count <=7 )
                        stateCom->addItem(QObject::tr("状态 ") + QString::number(_loop%7));
                    if( _loop == 4 ) {
                        QMessageBox msgBox;
                        msgBox.setText("对方策略发生改变，重新计算攻击点...");
                        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                        msgBox.setDefaultButton(QMessageBox::Ok);
                        int ret = msgBox.exec();
                    }
                    ++_loop;
                    delay(3000);
            }
        } else if( straIndex == 2 ) {
            while(_auto && _loop < 6) {
                    stateLabel->setText(QObject::tr("状态： ") + QString::number(_loop%6));
                    ReadInfo( ":/data/loop" + QObject::tr("%1").arg(_loop++) + ".txt", lineItems.size());
                    int count = stateCom->count();
                    if( count >= 0 && count <=6 )
                        stateCom->addItem(QObject::tr("状态 ") + QString::number(_loop%6));
                    delay(3000);
            }
        } else if (straIndex == 3) {
            while(_auto && _loop < 7) {
                stateLabel->setText(QObject::tr("状态： ")+QString::number(_loop%7));
                ReadInfo(":/data/Line"+QObject::tr("%1").arg(_loop++)+".txt",lineItems.size());
                stateCom->addItem(QObject::tr("状态 ")+QString::number(_loop%7));
                delay(3000);
            }
        }
        */
    }
}


void SystemWidget::createLine(QVector<QPointF> &points)
{
    QPolygonF* polyline=new QPolygonF(points);
    QPen Lpen;
    Lpen.setWidth(2);
    Lpen.setColor(QColor(0,0,0));
    QPainterPath* path = new QPainterPath();
    path->addPolygon(*polyline);
    QGraphicsPathItem* Line= new QGraphicsPathItem(*path);
    Line->setPen(Lpen);
    scene->addItem(Line);
}
void SystemWidget::createLoad(QPointF &point)
{
    QVector<QPointF>* gLoad=new QVector<QPointF>();
    gLoad->append(QPointF(point.rx(),point.ry()));
    gLoad->append(QPointF(point.rx(),point.ry()+shortY));
    gLoad->append(QPointF(point.rx()-0.3*shortX,point.ry()+shortY));
    gLoad->append(QPointF(point.rx(),point.ry()+1.3*shortY));
    gLoad->append(QPointF(point.rx()+0.3*shortX,point.ry()+shortY));
    gLoad->append(QPointF(point.rx(),point.ry()+shortY));
    createLine(*gLoad);
}

void SystemWidget::createControlFrame(int busnum,int linenum,int gennum)
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
    //connect(clearbutton, SIGNAL(released()), this, SLOT(slotClear()));
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
    strategyCom->addItem(tr("博弈"));
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
    syslabel->setText(QObject::tr("30节点系统 "));
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

    QLabel *warnLabel =new QLabel(tr("线路预警"));
    warnLabel->setObjectName("warnLabel");
    pa.setColor(QPalette::WindowText,QColor(255,97,0));
    warnLabel->setPalette(pa);

    warnCom=new QComboBox;
    warnCom->setMinimumContentsLength(10);
    warnCom->addItem(tr(" "));

    QHBoxLayout *warnLayout = new QHBoxLayout;
    warnLayout->addWidget(warnLabel);
    warnLayout->addWidget(warnCom);
    // connect(warntegyCom,SIGNAL(activated(int)),this,SLOT(slotwarntegy()));
    QGroupBox *warnGroup = new QGroupBox(tr("实时预警"));
    warnGroup->setAlignment(Qt::AlignCenter);
    warnGroup->setLayout(warnLayout);
    breakCom=new QComboBox;
    QLabel *breakLabel =new QLabel(tr("线路故障"));
    breakLabel->setObjectName("breakLabel");
    pa.setColor(QPalette::WindowText,Qt::red);
    breakLabel->setPalette(pa);

    breakCom=new QComboBox;
    breakCom->setMinimumContentsLength(10);
    breakCom->adjustSize();
    breakCom->addItem(tr(" "));
    QHBoxLayout *breakLayout = new QHBoxLayout;
    breakLayout->addWidget(breakLabel);
    breakLayout->addWidget(breakCom);
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
void SystemWidget::slotNew()        		//新建一个显示窗体
{
    slotClear();
    initScene_bus();

    MainWindow *newWin = new MainWindow;
    newWin->show();
}
void SystemWidget::slotPause(){
    slotClear();
    initScene_bus();
    MainWindow *newWin = new MainWindow;
    newWin->show();
}

void SystemWidget::slotStart()
{
    QTimeLine *timer = new QTimeLine(5000);
    timer->setFrameRange(0, 100);
    timer->start();
    ReadInfo(":/data/init.txt",lineItems.size());
    ReadInfo(":/data/loop0.txt",lineItems.size());
    stateCom->addItem(QObject::tr("状态 0"));
    QMessageBox::information(this,tr("策略选择"),tr("请进行策略选择！"));

}


void SystemWidget::slotClear()        	//清除场景中所有的图元
{
    QList<QGraphicsItem*> listItem = scene->items();

    while(!listItem.empty())
    {
        scene->removeItem(listItem.at(0));
        listItem.removeAt(0);
    }
    _loop = 0;
    initScene_bus();
}
void SystemWidget::slotScale(int value)
{
    qreal s;
    if(value>scaleValue)
        s=pow(1.1,(value-scaleValue));
    else
        s=pow(1/1.1,(scaleValue-value));
    view->scale(s,s);
    scaleValue=value;
}
void SystemWidget::slotStrategy(){
    int straIndex=strategyCom->currentIndex()+1;
    //  out << "Here is straIndex： " << straIndex <<endl;
    slotStrategyMenu(straIndex);
}
void SystemWidget::slotStrategyMenu(int straIndex){

    if(straIndex == 4){
        QMessageBox msgBox;
        msgBox.setText("确认使用博弈攻击策略？");
        //msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    //    msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Ok:
            strategyCom->setCurrentIndex(0);
            stralabel->setText(QObject::tr("博弈演示"));
            for(int i=1; i<7; ++i){
                if(_auto){
                    stateLabel->setText(QObject::tr("状态： ") + QString::number(_loop%7));
                    ReadInfo( ":/Bus30_System/30_system_data/loop" + QObject::tr("%1").arg((_loop++)%7) + ".txt", lineItems.size());
                    int count = stateCom->count();
                    if( count >= 0 && count < 7 )
                        stateCom->addItem(QObject::tr("状态 ") + QString::number(_loop-1));
                    if( _loop == 4 ) {
                        /*
                        QMessageBox msgBox;
                        msgBox.setIcon(QMessageBox::Information);
                        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                        msgBox.setText("对方采取应急措施，正在重新计算攻击点......");
                        int ret = msgBox.exec();
                        */
                        QMessageBox *msgBox = new QMessageBox( QMessageBox::Information, "提示",
                                   "对方采取应急措施，正在重新计算攻击点......", QMessageBox::Ok);
                        QTimer *msgBoxCloseTimer = new QTimer( this );
                        msgBoxCloseTimer->setInterval( 10000 );
                        msgBoxCloseTimer->setSingleShot( true );
                        connect( msgBoxCloseTimer, SIGNAL(timeout()), msgBox, SLOT(accept()) );
                        msgBoxCloseTimer->start();
                        msgBox->exec();

                        QMessageBox *msgBox1 = new QMessageBox( QMessageBox::Information, "提示",
                                   "计算完毕，以选中目标，准备再次攻击？", QMessageBox::Ok | QMessageBox::Cancel);
                        msgBox1->exec();
                    }
                    delay(3000);
                }else{
                    while(!_auto){
                        delay(6000);
                    }
                }
            }
            _loop = 0;
            break;
            QMessageBox::information(this,tr("Evaluation"),tr("影响评估： 59.25% ！"),QMessageBox::NoButton);
        case QMessageBox::Cancel:
            // Cancel was clicked
            break;
        default:
            // should never be reached
            break;
        }

    }

    if(straIndex == 2){
        QMessageBox msgBox;
        msgBox.setText("确认使用重载优先攻击策略？");
        //msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Ok:
            strategyCom->setCurrentIndex(1);
            stralabel->setText(QObject::tr("重载策略攻击演示"));
            for(int i=1;i<6;++i){
                if(_auto){
                    stateLabel->setText(QObject::tr("状态： ")+QString::number(_loop%6));
                    ReadInfo(":/data/loop"+QObject::tr("%1").arg((_loop++)%6)+".txt",lineItems.size());
                    int count = stateCom->count();
                    if( count > 0 && count < 6 )
                        stateCom->addItem(QObject::tr("状态 ")+QString::number(_loop -1 ));
                    delay(3000);
                }else{
                    while(!_auto){
                        delay(6000);
                    }
                }
            }
            _loop = 0;
            QMessageBox::information(this,tr("Evaluation"),tr("影响评估： 93.13% ！"),QMessageBox::NoButton);
            break;
        case QMessageBox::Cancel:
            // Cancel was clicked
            break;
        default:
            // should never be reached
            break;
        }
    }
    if(straIndex == 3) {
        QMessageBox msgBox;
        // msgBox.setWindowTitle(tr("策略选择"));
        msgBox.setText("确认使用随机选择攻击策略？");
        //msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Ok:
            strategyCom->setCurrentIndex(2);
            stralabel->setText(QObject::tr("随机策略攻击演示"));
            for(int i=1;i<7;++i){
                if(_auto){
                    stateLabel->setText(QObject::tr("状态： ")+QString::number(_loop%7));
                    ReadInfo(":/data/Line"+QObject::tr("%1").arg((_loop++)%7)+".txt",lineItems.size());
                    int count = stateCom->count();
                    if( count > 0 && count < 7 )
                        stateCom->addItem(QObject::tr("状态 ")+QString::number(_loop - 1));
                    delay(3000);
                }else{
                    while(!_auto){
                        delay(6000);
                    }
                }
            }
            _loop = 0;
            QMessageBox::information(this,tr("Evaluation"),tr("影响评估： 38% ！"),QMessageBox::NoButton);
        case QMessageBox::Cancel:
            // Cancel was clicked
            break;
        default:
            // should never be reached
            break;
        }
    }
}
void SystemWidget::slotstate(){

    int stateIndex = stateCom->currentIndex();
    QTextStream out(stdout);
    int index = strategyCom->currentIndex() + 1;
    out << "The state num is " << stateIndex <<endl;

    _auto = false;
    if(stateIndex == 0){
        flashboom->hide();
        flashboom_1->hide();

        out << "Here is the strategyCom :" << index  <<endl;
    }else{
        flashboom->show();
        flashboom_1->show();
    }
    QVectorIterator<BusItem*> itbus(busItems);
    while(itbus.hasNext()){
        BusItem* temp = itbus.next();
        temp->setStop(false);
    }
    QVectorIterator<LineItem*> itline(lineItems);
    while(itline.hasNext()){
        LineItem* temp = itline.next();
        temp->setStop(false);
    }

    QVectorIterator<GenItem*> itgen(genItems);
    while(itgen.hasNext()){
        GenItem* temp = itgen.next();
        temp->setStop(false);
    }

    stateLabel->setText(QObject::tr("状态： ")+QString::number(stateIndex));
    QString filename = ":/data/loop"+QObject::tr("%1").arg(stateIndex)+".txt";
    if(index == 2){
        filename = ":/data/Line"+QObject::tr("%1").arg(stateIndex)+".txt";
    }
    if(index == 1){
        filename = ":/Bus30_System/30_system_data/loop" + QObject::tr("%1").arg(stateIndex) + ".txt";
    }
    ReadInfo(filename,lineItems.size());
}
void SystemWidget::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}
void SystemWidget::ReadInfo(QString filename,int length){
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
                //     out << "This is the first line: Line paramaters." << endl;
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
                    if(filename!=":/data/init.txt"){
                        linetemp->setLineCap(temp.toFloat());
                        float linef=temp.toFloat();
                        if(linef<9999&&linef!=0){
                            linetemp->setStart(true);
                            linetemp->setrunstate(true);
                        }
                        else if(linef==10000){
                            if(filename == ":/Bus30_System/30_system_data/loop1.txt"){
                                flashboom = new FlashItem;
                                flashboom_1 = new FlashItem;
                                flashboom->setPos(x1+4.6*Hbase,y1+2.6*Vbase);
                                flashboom_1 -> setPos( x1+0.8*Hbase,y1+4.25*Vbase );
                                scene->addItem(flashboom);
                                scene->addItem(flashboom_1);

                            } else if(filename==":/data/loop1.txt"){
                                flashboom = new FlashItem;
                                flashboom_1 = new FlashItem;
                                flashboom->setPos(x1+5.5*Hbase,y1+5.15*Vbase);
                                scene->addItem(flashboom);
                            }else if(filename==":/data/Line1.txt"){
                                flashboom = new FlashItem;
                                flashboom_1 = new FlashItem;
                                flashboom->setPos(x1+1.7*Hbase,y1+1.4*Vbase);
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
                linelabel->setText(QObject::tr("线路： ")+QString::number(count));
                ++linenum;
            }
            else if(linenum == 2){
                QStringList v = data.split(QRegExp("\\s+"));
                int size = v.size()-1;
                int count = size;
                for(int i = 0;i<size;++i){
                    GenItem* gentemp = genItems[i];
                    QString temp = v.at(i);
                    if(filename != ":/data/init.txt"){
                        float geni=temp.toFloat();
                        if(geni==0){
                            gentemp->setStop(true);
                            gentemp->setChuli(0);
                            --count;
                        }
                        else{
                            gentemp->setStart(true);
                            gentemp->setrunstate(true);
                            gentemp->setChuli(temp.toFloat());
                            out << "chuli / genCap: " << temp.toFloat() <<"  " << gentemp->getgenCap() << endl;
                        }
                    }else{
                        gentemp->setgenCap(temp.toFloat());
                        //  out << "genCap from init.txt: " << temp.toFloat() << endl;
                    }
                }
                genlabel->setText(QObject::tr("机组： ")+QString::number(count));
                ++linenum;
            }
            else if(linenum == 3){
                //  out << "This is the third line: Bus paramaters." << endl;
                QStringList v=data.split(QRegExp(";"));
                int size=v.size();
                QVector<int> mark(30,1);
                //  out<< "quartier size: "<< size << endl;
                for(int q=0; q<size; ++q) {
                    QString quatier = v.at(q);
                    QStringList qtemp = quatier.split(QRegularExpression("\\s+"));
                    int qsize = qtemp.size() - 1;
                    for(int i=0; i<qsize; ++i) {
                        QString stemp = qtemp.at(i);
                        int index = stemp.toInt()-1;
                        BusItem* bustemp = busItems[index];
                        bustemp -> setBusquar(q);
                        mark[index] = 0;
                        bustemp->setrunstate(true);
                        bustemp->setColor(40*q+40);
                    }
                }
                int count=30;
                for(int i=0; i<30; ++i){
                    int bustemp = mark[i];
                    if(bustemp != 0){
                        --count;
                        busItems[i]->setStop(true);
                        //      out << "Stop bus Num: "<< i <<endl;
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
        qDebug()<<"Read Error SystemWidget::ReadInfo"<<filename<<endl;
    }
}
void SystemWidget::initScene_bus()
{
    qreal x2=x1+Hbase;
    qreal x3=x2+Hbase;
    qreal x4=x3+Hbase;
    qreal x5=x4+Hbase;
    qreal x6=x5+Hbase;
    qreal x7=x6+Hbase;
    qreal x8=x7+Hbase;

    qreal y2=y1+Vbase;
    qreal y3=y2+Vbase;
    qreal y4=y3+Vbase;
    qreal y5=y4+Vbase;
    qreal y6=y5+Vbase;
    qreal y7=y6+Vbase;

    busItems.append(new BusItem(x1,y6,Width,Height));
    busItems.append(new BusItem(x2,y7,Width,Height));
    busItems.append(new BusItem(x2,y6,Width,Height));
    busItems.append(new BusItem(x3,y6,Width,Height));
    busItems.append(new BusItem(x4,y7,Width,Height));

    busItems.append(new BusItem(x5,y6,Width,Height));
    busItems.append(new BusItem(x5,y7,Width,Height));
    busItems.append(new BusItem(x8,y6,Width,Height));
    busItems.append(new BusItem(x5,y5,Width,Height));
    busItems.append(new BusItem(x4,y5,Width,Height));

    busItems.append(new BusItem(x6,y5,Width,Height));
    busItems.append(new BusItem(x2,y5,Width,Height));
    busItems.append(new BusItem(x1,y5,Width,Height));
    busItems.append(new BusItem(x1,y4,Width,Height));
    busItems.append(new BusItem(x2,y3,Width,Height));

    busItems.append(new BusItem(x3,y3,Width,Height));
    busItems.append(new BusItem(x3,y4,Width,Height));
    busItems.append(new BusItem(x3,y2,Width,Height));
    busItems.append(new BusItem(x4,y3,Width,Height));
    busItems.append(new BusItem(x4,y4,Width,Height));

    busItems.append(new BusItem(x5,y4,Width,Height));
    busItems.append(new BusItem(x6,y4,Width,Height));
    busItems.append(new BusItem(x3,y1,Width,Height));
    busItems.append(new BusItem(x6,y3,Width,Height));
    busItems.append(new BusItem(x7,y3,Width,Height));
    busItems.append(new BusItem(x7,y4,Width,Height));
    busItems.append(new BusItem(x7,y2,Width,Height));
    busItems.append(new BusItem(x8,y4,Width,Height));
    busItems.append(new BusItem(x7,y1,Width,Height));
    busItems.append(new BusItem(x6,y1,Width,Height));
    QVectorIterator<BusItem*> itbus(busItems);
    int busnum=0;
    while(itbus.hasNext()){
        BusItem* temp=itbus.next();
        temp->setBusnum(++busnum);
        scene->addItem(temp);
    }
    /* Line 1 - 5 */
    lineItems.append(new LineItem(QPointF(x1+3*shortX,y6+Height),QPointF(x1+3*shortX,y6+Height+shortY),
                                  QPointF(x2+0.5*shortX,y7-shortY),QPointF(x2+0.5*shortX,y7)));
    lineItems.append(new LineItem(QPointF(x1+2*shortX,y6),QPointF(x1+2*shortX,y6-shortY),
                                  QPointF(x2+shortX,y6-shortY),QPointF(x2+shortX,y6)));
    lineItems.append(new LineItem(QPointF(x2+1.5*shortX,y7), QPointF(x2+1.5*shortX,y7-shortY),
                                  QPointF(x3+shortX,y6+Height+shortY),QPointF(x3+shortX,y6+Height)));
    lineItems.append(new LineItem(QPointF(x2+3.5*shortX,y7),QPointF(x2+3.5*shortX,y7-0.5*shortY),
                                  QPointF(x4+shortX,y7-0.5*shortY),QPointF(x4+shortX,y7)));
    lineItems.append(new LineItem(QPointF(x2+2.5*shortX,y7),QPointF(x2+2.5*shortX,y7-shortY),
                                  QPointF(x5+shortX,y6+Height+shortY), QPointF(x5+shortX,y6+Height)));
    /* Line 6 - 10 */
    lineItems.append(new LineItem(QPointF(x2+3*shortX,y6), QPointF(x2+3*shortX,y6-shortY),
                                  QPointF(x3+shortX,y6-shortY),QPointF(x3+shortX,y6)));
    lineItems.append(new LineItem(QPointF(x3+3*shortX,y6),QPointF(x3+3*shortX,y6-shortY),
                                  QPointF(x5+0.5*shortX,y6-shortY),QPointF(x5+0.5*shortX,y6)));
    lineItems.append(new LineItem(QPointF(x3+2*shortX,y6),QPointF(x3+2*shortX,y6-shortY),
                                  QPointF(x2+3*shortX,y5+Height+shortY),QPointF(x2+3*shortX,y5+Height)));
    lineItems.append(new LineItem(QPointF(x4+3*shortX,y7),QPointF(x4+3*shortX,y7-0.5*shortY),
                                  QPointF(x5+shortX,y7-0.5*shortY),QPointF(x5+shortX,y7)));
    lineItems.append(new LineItem(QPointF(x5+2*shortX,y6+Height),QPointF(x5+2*shortX,y7)));

    /* Line 11 - 15 */
    lineItems.append(new LineItem(QPointF(x5+3.5*shortX,y6+Height),QPointF(x5+3.5*shortX,y6+Height+shortY),
                                  QPointF(x8+shortX,y6+Height+shortY),QPointF(x8+shortX,y6+Height)));

    lineItems.append(new LineItem(QPointF(x5+2*shortX,y6), QPointF(x5+2*shortX,y5+Height)));
    lineItems.append(new LineItem(QPointF(x5+1.5*shortX,y6),QPointF(x5+1.5*shortX,y6-shortY),
                                  QPointF(x4+2*shortX,y5+Height+shortY),QPointF(x4+2*shortX,y5+Height)));
    lineItems.append(new LineItem(QPointF(x5+3.5*shortX,y6),QPointF(x5+3.5*shortX,y6-0.5*shortY),
                                  QPointF(x8+shortX,y4+Height+0.5*shortY),QPointF(x8+shortX,y4+Height)));
    lineItems.append(new LineItem(QPointF(x8+2*shortX,y6),QPointF(x8+2*shortX,y4+Height)));

    /* Line 16 - 20 */
    lineItems.append(new LineItem(QPointF(x5+shortX,y5+Height),QPointF(x5+shortX,y5+Height+shortY),
                                  QPointF(x4+3*shortX,y5+Height+shortY),QPointF(x4+3*shortX,y5+Height)));
    lineItems.append(new LineItem(QPointF(x5+3*shortX,y5),QPointF(x5+3*shortX,y5-shortY),
                                  QPointF(x6+2*shortX,y5-shortY),QPointF(x6+2*shortX,y5)));
    lineItems.append(new LineItem(QPointF(x4+0.5*shortX,y5),QPointF(x4+0.5*shortX,y5-shortY),
                                  QPointF(x3+shortX,y4+Height+shortY),QPointF(x3+shortX,y4+Height)));
    lineItems.append(new LineItem(QPointF(x4+1.5*shortX,y5),QPointF(x4+1.5*shortX,y4+Height)));
    lineItems.append(new LineItem(QPointF(x4+2.5*shortX,y5),QPointF(x4+2.5*shortX,y5-shortY),
                                  QPointF(x5+shortX,y4+Height+shortY),QPointF(x5+shortX,y4+Height)));
    /* Line 21 - 25 */
    lineItems.append(new LineItem(QPointF(x4+3.5*shortX,y5),QPointF(x4+3.5*shortX,y5-shortY),
                                  QPointF(x6+shortX,y4+Height+shortY),QPointF(x6+shortX,y4+Height)));
    lineItems.append(new LineItem(QPointF(x2+shortX,y5+Height),QPointF(x2+shortX,y5+Height+shortY),
                                  QPointF(x1+3*shortX,y5+Height+shortY),QPointF(x1+3*shortX,y5+Height)));
    lineItems.append(new LineItem(QPointF(x2+shortX,y5),QPointF(x2+shortX,y5-shortY),
                                  QPointF(x1+3*shortX,y4+Height+shortY),QPointF(x1+3*shortX,y4+Height)));
    lineItems.append(new LineItem(QPointF(x2+2*shortX,y5),QPointF(x2+2*shortX,y3+Height)));
    lineItems.append(new LineItem(QPointF(x2+3*shortX,y5),QPointF(x2+3*shortX,y5-shortY),
                                  QPointF(x3+shortX,y3+Height+shortY), QPointF(x3+shortX,y3+Height)));
    /* Line 26 - 30 */
    lineItems.append(new LineItem(QPointF(x1+2*shortX,y4),QPointF(x1+2*shortX,y4-shortY),
                                  QPointF(x2+shortX,y3+Height+shortY),QPointF(x2+shortX,y3+Height)));

    lineItems.append(new LineItem(QPointF(x2+3*shortX,y3),QPointF(x2+3*shortX,y3-shortY),
                                  QPointF(x3+shortX,y2+Height+shortY),QPointF(x3+shortX,y2+Height)));

    lineItems.append(new LineItem(QPointF(x2+2*shortX,y3),QPointF(x2+2*shortX,y3-shortY),
                                  QPointF(x3+shortX,y1+Height+shortY),QPointF(x3+shortX,y1+Height)));
    lineItems.append(new LineItem(QPointF(x3+2*shortX,y3+Height), QPointF(x3+2*shortX,y4)));
    lineItems.append(new LineItem(QPointF(x3+3*shortX,y2+Height),QPointF(x3+3*shortX,y2+Height+shortY),
                                  QPointF(x4+2*shortX,y3-shortY), QPointF(x4+2*shortX,y3)));
    /* Line 31 - 35 */
    lineItems.append(new LineItem(QPointF(x4+2*shortX,y3+Height),QPointF(x4+2*shortX,y4)));
    lineItems.append(new LineItem(QPointF(x5+2*shortX,y4),QPointF(x5+2*shortX,y4-shortY),
                                  QPointF(x6+shortX,y4-shortY),QPointF(x6+shortX,y4)));
    lineItems.append(new LineItem(QPointF(x6+2*shortX,y4),QPointF(x6+2*shortX,y3+Height)));
    lineItems.append(new LineItem(QPointF(x3+3*shortX,y1+Height), QPointF(x3+3*shortX,y1+Height+shortY),
                                  QPointF(x6+2*shortX,y3-shortY),QPointF(x6+2*shortX,y3)));
    lineItems.append(new LineItem(QPointF(x6+3*shortX,y3+Height),QPointF(x6+3*shortX,y3+Height+shortY),
                                  QPointF(x7+shortX,y3+Height+shortY),QPointF(x7+shortX,y3+Height)));
    /* Line 36 - 41 */
    lineItems.append(new LineItem(QPointF(x7+2*shortX,y3+Height),QPointF(x7+2*shortX,y4)));
    lineItems.append(new LineItem(QPointF(x7+2*shortX,y3),QPointF(x7+2*shortX,y2+Height)));
    lineItems.append(new LineItem(QPointF(x7+3*shortX,y2+Height),QPointF(x7+3*shortX,y2+Height+shortY),
                                  QPointF(x8+2*shortX,y4-shortY),QPointF(x8+2*shortX,y4)));
    lineItems.append(new LineItem(QPointF(x7+2*shortX,y2),QPointF(x7+2*shortX,y1+Height)));
    lineItems.append(new LineItem(QPointF(x7+0.5*shortX,y2),QPointF(x7+0.5*shortX,y2-shortY),
                                  QPointF(x6+2*shortX,y1+Height+shortY),QPointF(x6+2*shortX,y1+Height)));
    lineItems.append(new LineItem(QPointF(x7+shortX,y1+Height),QPointF(x7+shortX,y1+Height+shortY),
                                  QPointF(x6+3*shortX,y1+Height+shortY),QPointF(x6+3*shortX,y1+Height)));

    gLineItems.append(new LineItem(QPointF(x1+Radius,y6+Height),QPointF(x1+Radius,y6+Height+shortY)));
    genItems.append(new GenItem(Radius,QPointF(x1,y6+Height+shortY)));
    gLineItems.append(new LineItem(QPointF(x2+Radius,y7+Height),QPointF(x2+Radius,y7+Height+shortY)));
    genItems.append(new GenItem(Radius,QPointF(x2,y7+Height+shortY)));
    gLineItems.append(new LineItem(QPointF(x1+Radius,y5+Height), QPointF(x1+Radius,y5+Height+shortY)));
    genItems.append(new GenItem(Radius,QPointF(x1,y5+Height+shortY)));
    gLineItems.append(new LineItem(QPointF(x6+shortX+Radius,y4+Height),QPointF(x6+shortX+Radius,y4+Height+shortY)));
    genItems.append(new GenItem(Radius,QPointF(x6+shortX,y4+Height+shortY)));
    gLineItems.append(new LineItem(QPointF(x3+shortX+Radius,y1),QPointF(x3+shortX+Radius,y1-shortY)));
    genItems.append(new GenItem(Radius,QPointF(x3+shortX,y1-shortY-2*Radius)));
    gLineItems.append(new LineItem(QPointF(x7-0.5*shortX+Radius,y2+Height),QPointF(x7-0.5*shortX+Radius,y2+Height+shortY)));
    genItems.append(new GenItem(Radius,QPointF(x7-0.5*shortX,y2+Height+shortY)));
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
    L2=new QPointF(x2+3*shortX,y7+Height);
    L3=new QPointF(x2+2*shortX,y6+Height);
    L4=new QPointF(x3+2*shortX,y6+Height);
    L7=new QPointF(x5+3*shortX,y7+Height);
    L8=new QPointF(x8+3*shortX,y6+Height);
    L10=new QPointF(x4+shortX,y5+Height);
    L12=new QPointF(x2+2*shortX,y5+Height);
    L14=new QPointF(x1+shortX,y4+Height);
    L15=new QPointF(x2+3*shortX,y3+Height);
    L16=new QPointF(x3+3*shortX,y3+Height);
    L17=new QPointF(x3+3*shortX,y4+Height);
    L18=new QPointF(x3+2*shortX,y2+Height);
    L19=new QPointF(x4+3*shortX,y3+Height);
    L20=new QPointF(x4+3*shortX,y4+Height);
    L21=new QPointF(x5+3*shortX,y4+Height);
    L23=new QPointF(x3+2*shortX,y1+Height);
    L24=new QPointF(x6+shortX,y3+Height);
    L26=new QPointF(x7+2*shortX,y4+Height);
    L29=new QPointF(x7+3*shortX,y1+Height);
    L30=new QPointF(x6+shortX,y1+Height);
    createLoad(*L2);
    createLoad(*L3);
    createLoad(*L4);
    createLoad(*L7);
    createLoad(*L8);
    createLoad(*L10);
    createLoad(*L12);
    createLoad(*L14);
    createLoad(*L15);
    createLoad(*L16);
    createLoad(*L17);
    createLoad(*L18);
    createLoad(*L19);
    createLoad(*L20);
    createLoad(*L21);
    createLoad(*L23);
    createLoad(*L24);
    createLoad(*L26);
    createLoad(*L29);
    createLoad(*L30);
}

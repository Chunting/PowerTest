#include "mainwindow.h"
#include "flashitem.h"
#include "startitem.h"
#include "systemwidget.h"
#include "system118_widget.h"
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QPolygon>
#include<QTextStream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    welwidget=new ShowWidget;
    widget=new SystemWidget;
    widget118=new System118_Widget;
    setCentralWidget(welwidget);
    createActions();                    	//创建主窗体的所有动作
    createMenus();                       	//创建主窗体的菜单栏
    // setMinimumSize(1000,800);       	//设置主窗口的最小尺寸

    qreal widget_width=widget->width();
    qreal widget_height=widget->height();
    resize(QSize(1.5*widget_width,1.5*widget_height));
    showMaximized();

}
MainWindow::~MainWindow()
{

}

void MainWindow::createActions()       	//创建主窗体的所有动作
{
    newAct = new QAction(tr("新建"),this);
    clearAct = new QAction(tr("清除"),this);
    exitAct = new QAction(tr("退出"),this);
    _24BusAct = new QAction(tr("24节点系统"),this);
    _30BusAct = new QAction(tr("30节点系统"),this);
    _118BusAct = new QAction(tr("118节点系统"),this);
    exhauAct= new QAction(tr("穷举"),this);
    overloadAct= new QAction(tr("重载"),this);
    allowanceAct= new QAction(tr("裕度"),this);
    startAct=new QAction(tr("开始"),this);
    pauseAct=new QAction(tr("暂停"),this);
    stopAct=new QAction(tr("停止"),this);
    clearAct=new QAction(tr("清除"),this);
    timeAct=new QAction(tr("时间设置"),this);
    renameAct=new QAction(tr("标题"),this);

    //    connect(newAct,SIGNAL(triggered()),this,SLOT(slotNew()));
    //    connect(clearAct,SIGNAL(triggered()),this,SLOT(slotClear()));
    //    connect(exitAct,SIGNAL(triggered()),this,SLOT(close()));

    connect(_30BusAct,SIGNAL(triggered()),this,SLOT(slotNew()));
    connect( _118BusAct,SIGNAL(triggered()),this,SLOT(slotNew118()));
    //    connect(exitAct,SIGNAL(triggered()),this,SLOT(close()));
    connect(overloadAct,SIGNAL(triggered()),this,SLOT (slotStrategy()));
    connect(startAct,SIGNAL(triggered()),this,SLOT (slotStart()));
    connect(clearAct,SIGNAL(triggered()),this,SLOT (slotClear()));
    connect(timeAct,SIGNAL(triggered()),this,SLOT (slotTime()));
    connect(renameAct,SIGNAL(triggered()),this,SLOT (slotRename()));
    //    connect(addTextItemAct,SIGNAL(triggered()),this,SLOT (slotAddTextItem()));
    //    connect(addRectItemAct,SIGNAL(triggered()),this,SLOT (slotAddRectItem()));
    //    connect(addAlphaItemAct,SIGNAL(triggered()),this,SLOT (slotAddAlphaItem()));
    //    connect(addFlashItemAct,SIGNAL(triggered()),this,SLOT(slotAddFlashItem()));
    //    connect(addAnimItemAct,SIGNAL(triggered()),this,SLOT(slotAddAnimationItem()));
}

void MainWindow::createMenus()         	//创建主窗体的菜单栏
{
    systemsMenu = menuBar()->addMenu(tr("系统选择"));
  //  systemsMenu->addAction(_24BusAct);
    systemsMenu->addAction(_30BusAct);
    systemsMenu->addAction(_118BusAct);
    strategyMenu = menuBar()->addMenu(tr("策略选择"));
    strategyMenu->addAction(exhauAct);
    strategyMenu->addAction(overloadAct);
    strategyMenu->addAction(allowanceAct);
    runMenu = menuBar()->addMenu(tr("控制"));
    runMenu->addAction(startAct);
   // runMenu->addAction(pauseAct);
   // runMenu->addAction(stopAct);
    runMenu->addAction(clearAct);
    setMenu=menuBar()->addMenu(tr("设置"));
    setMenu->addAction(renameAct);
    setMenu->addAction(timeAct);
}

void MainWindow::initScene()        	//初始化场景
{

}
void MainWindow::slotNew()        		//新建一个显示窗体
{
    widget=new SystemWidget;
    setCentralWidget(widget);
    qreal widget_width=widget->width();
    qreal widget_height=widget->height();
    resize(QSize(1.5*widget_width,1.5*widget_height));
    sysIndex=2;
    setWindowFlags(windowFlags()&Qt::WindowMaximizeButtonHint&Qt::WindowMinimizeButtonHint);
    showMaximized();
}
void MainWindow::slotNew118()        		//新建一个显示窗体
{
    widget118=new System118_Widget;
    setCentralWidget(widget118);
//    QDesktopWidget* desktopWidget = QApplication::desktop();
//    qreal widget_width=desktopWidget->width();
//    qreal widget_height=desktopWidget->height();
    qreal widget_width=widget->width();
    qreal widget_height=widget->height();
    resize(QSize(1.5*widget_width,1.5*widget_height));
    resize(QSize(widget_width,widget_height));
    sysIndex=3;
    setWindowFlags(windowFlags()&Qt::WindowMaximizeButtonHint&Qt::WindowMinimizeButtonHint);
    showMaximized();

}
void MainWindow::slotStart()        		//新建一个显示窗体
{
   if (sysIndex==3){
       widget118->slotStart();
   }else if (sysIndex==2){
       widget->slotStart();
   }

}
void MainWindow::slotStrategy(){
    if (sysIndex==3){
        widget118->slotStrategyMenu(2);
    }else if (sysIndex==2){
        widget->slotStrategyMenu(2);
    }

}

void MainWindow::slotClear()        	//清除场景中所有的图元
{
    if (sysIndex==3){
        widget118->close();
        slotNew118();

    }else if (sysIndex==2){
        //widget->slotStrategyMenu(2);
        widget->close();
        slotNew();
    }
}
void MainWindow::slotRename()
{
    SetDialog setdlg;
        if(setdlg.exec()==QDialog::Accepted){

            widget118->setTimer(setdlg.getTime());
            widget118->setTitle(setdlg.getName());

        }
}
void MainWindow::slotTime()
{
    SetDialog setdlg;
        if(setdlg.exec()==QDialog::Accepted){
            widget118->setTimer(setdlg.getTime());
            widget118->setTitle(setdlg.getName());

        }
}

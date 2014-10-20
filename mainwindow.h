#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenuBar>
#include <QGraphicsEllipseItem>
#include <QPoint.h>
#include "systemwidget.h"
#include "system118_widget.h"
#include "showwidget.h"
#include "setdialog.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void setupUi(QMainWindow *MainWindow);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SystemWidget * widget;
    System118_Widget * widget118;
    ShowWidget *welwidget;

    void initScene();            			//初始化场景
    void createActions();       			//创建主窗体的所有动作
    void createMenus();         			//创建主窗体的菜单栏
public slots:
    void slotNew();              			//新建一个显示窗体
    void slotNew118();
    void slotClear();           			//清除场景中所有的图元
    void slotStrategy();
    void slotStart();
    void slotTime();
    void slotRename();


private:
    QGraphicsScene *scene;
    QMenu *systemsMenu;
    QMenu *strategyMenu;
    QMenu *runMenu;
    QMenu *setMenu;
    int sysIndex;
    int straIndex;
    int conIndex;

    QAction *newAct;
    QAction *exitAct;
    QAction *_24BusAct;
    QAction *_30BusAct;
    QAction *_118BusAct;

    QAction *exhauAct;
    QAction *overloadAct;
    QAction *allowanceAct;

    QAction *startAct;
    QAction *pauseAct;
    QAction *stopAct;
    QAction *clearAct;
//    QAction *addAnimItemAct;
    QAction *timeAct;
    QAction *renameAct;
    int _Time;
    QString _Name;


};

#endif // MAINWINDOW_H

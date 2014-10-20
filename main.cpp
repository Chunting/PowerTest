#include "mainwindow.h"
#include "logindialog.h"
#include "connection.h"
#include "widget.h"
#include <QLabel>
#include <QApplication>
#include <QPushButton>
#include <QTranslator>

int main(int argc, char *argv[])
{
    //const QString title="Power System";

    QApplication a(argc, argv);
    a.setApplicationDisplayName("Power System");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gb18030"));
    if(!createConnection()||!createXml())return 0;
    MainWindow w;
    LoginDialog dlg;
  //  if(dlg.exec()==QDialog::Accepted){
        w.setWindowFlags(w.windowFlags()&Qt::WindowMaximizeButtonHint&Qt::WindowMinimizeButtonHint);
        w.showMaximized();
        w.show();
        return a.exec();
  //  }else{
  //      return 0;
  //  }
}

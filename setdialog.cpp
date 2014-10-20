#include "setdialog.h"
#include "ui_setdialog.h"

SetDialog::SetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDialog)
{
    ui->setupUi(this);
}

SetDialog::~SetDialog()
{
    delete ui;
}

void SetDialog::on_setBtn_clicked()
{
    if(!ui->renamelineEdit->text().isEmpty()){
        setName(ui->renamelineEdit->text());
//        QMessageBox::information(this,tr("请输入密码"),tr("请先输入密码再登录！"),QMessageBox::Ok);
//        ui->pwdLineEdit->setFocus();
    }
    if(!ui->timelineEdit->text().isEmpty()){
        int temp=ui->timelineEdit->text().toInt();
        setTime(temp);
//        QSqlQuery query;
//        query.exec("select pwd from password");
//        query.next();
//        if(query.value(0).toString() == ui->pwdLineEdit->text()){
//            QDialog::accept();
//        }else{
//            QMessageBox::warning(this,tr("密码错误"),tr("请输入正确的密码再登录！"),QMessageBox::Ok);
//            ui->pwdLineEdit->clear();
//            ui->pwdLineEdit->setFocus();
//        }
    }
    QDialog::accept();
}

void SetDialog::on_quitBtn_clicked()
{
    QDialog::reject();
}
void SetDialog::setTime(int time){
    if(time>0){
        _time=1000*time;
    }else{
        _time=3000;
    }
}
int SetDialog::getTime(){
    return _time;
}
void SetDialog::setName(QString name){
    if(!name.isEmpty()){
        _name=name;
    }else{
        _name="演示系统";
    }
}
QString SetDialog::getName(){
    return _name;
}

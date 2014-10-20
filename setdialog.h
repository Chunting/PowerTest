#ifndef SETDIALOG_H
#define SETDIALOG_H

#include <QDialog>

namespace Ui {
class SetDialog;
}

class SetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetDialog(QWidget *parent = 0);
    ~SetDialog();
    void setTime(int time);
    void setName(QString name);
    int getTime();
    QString getName();

private slots:
    void on_setBtn_clicked();

    void on_quitBtn_clicked();
private:
    Ui::SetDialog *ui;
    int _time;
    QString _name;
};

#endif // SETDIALOG_H

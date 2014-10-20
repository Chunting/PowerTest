/********************************************************************************
** Form generated from reading UI file 'setdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETDIALOG_H
#define UI_SETDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SetDialog
{
public:
    QLabel *rename;
    QLabel *settime;
    QLineEdit *renamelineEdit;
    QLineEdit *timelineEdit;
    QPushButton *setBtn;
    QPushButton *quitBtn;

    void setupUi(QDialog *SetDialog)
    {
        if (SetDialog->objectName().isEmpty())
            SetDialog->setObjectName(QStringLiteral("SetDialog"));
        SetDialog->resize(400, 300);
        SetDialog->setSizeGripEnabled(false);
//        buttonBox = new QDialogButtonBox(SetDialog);
//        buttonBox->setObjectName(QStringLiteral("buttonBox"));
//        buttonBox->setGeometry(QRect(30, 240, 341, 32));
//        buttonBox->setOrientation(Qt::Horizontal);
//        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        rename = new QLabel(SetDialog);
        rename->setObjectName(QStringLiteral("rename"));
        rename->setGeometry(QRect(80, 70, 41, 21));
        settime = new QLabel(SetDialog);
        settime->setObjectName(QStringLiteral("settime"));
        settime->setGeometry(QRect(50, 110, 61, 16));
        renamelineEdit = new QLineEdit(SetDialog);
        renamelineEdit->setObjectName(QStringLiteral("renamelineEdit"));
        renamelineEdit->setGeometry(QRect(120, 70, 113, 20));
        timelineEdit = new QLineEdit(SetDialog);
        timelineEdit->setObjectName(QStringLiteral("timelineEdit"));
        timelineEdit->setGeometry(QRect(120, 110, 113, 20));

        setBtn = new QPushButton(SetDialog);
        setBtn->setObjectName(QStringLiteral("setBtn"));
        setBtn->setGeometry(QRect(100, 220, 75, 23));
        quitBtn = new QPushButton(SetDialog);
        quitBtn->setObjectName(QStringLiteral("quitBtn"));
        quitBtn->setGeometry(QRect(230, 220, 75, 23));
        SetDialog->setWindowTitle("设置");
        setBtn->setText("确定");
        quitBtn->setText("退出");
        rename->setText("标题");
        settime->setText("时间(s)");

        retranslateUi(SetDialog);
//        QObject::connect(buttonBox, SIGNAL(accepted()), SetDialog, SLOT(accept()));
//        QObject::connect(buttonBox, SIGNAL(rejected()), SetDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(SetDialog);
    } // setupUi

    void retranslateUi(QDialog *SetDialog)
    {
//        SetDialog->setWindowTitle(QApplication::translate("SetDialog", "\350\256\276\347\275\256", 0));
//        rename->setText(QApplication::translate("SetDialog", "\346\240\207\351\242\230\357\274\232", 0));
//        settime->setText(QApplication::translate("SetDialog", "\346\227\266\351\227\264\357\274\210s\357\274\211:", 0));
    } // retranslateUi

};

namespace Ui {
    class SetDialog: public Ui_SetDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETDIALOG_H

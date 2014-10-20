/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QWidget>
#include <QTextStream>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QStackedWidget *stackedWidget;
    QWidget *managePage;
    QLabel *label;
    QToolBox *toolBox;
    QWidget *page;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QComboBox *sellFactoryComboBox;
    QComboBox *sellBrandComboBox;
    QTextEdit *sellPriceLineEdit;
    QPushButton *sellOkBtn;
    QPushButton *sellCancelBtn;
    QPushButton *nextRunBtn;
    QWidget *page_3;
    QListWidget *dailyList;
    QWidget *chartPage;
    QLabel *label_8;

    void setupUi(QWidget *Widget)
    {
            QTextStream out(stdout);
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->setEnabled(true);
        qreal wheight=Widget->height();
        qreal wwidth=Widget->width();
        Widget->resize(wwidth, wheight);
        out<<"Widget: "<<wwidth<<" "<<wheight<<endl;
        Widget->setMaximumSize(2*wwidth, 2*wheight);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(5);
        sizePolicy.setVerticalStretch(5);
        sizePolicy.setHeightForWidth(Widget->sizePolicy().hasHeightForWidth());
        Widget->setSizePolicy(sizePolicy);
//        Widget->setSizeIncrement(QSize(0, 0));

        stackedWidget = new QStackedWidget(Widget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setGeometry(QRect(0.031*wwidth, 0.083*wheight, wwidth, 0.8*wheight));
 out<<"stackedWidget: "<<stackedWidget->width()<<" "<<stackedWidget->height()<<endl;
//        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
//        sizePolicy1.setHorizontalStretch(0);
//        sizePolicy1.setVerticalStretch(0);
//        sizePolicy1.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy);
        managePage = new QWidget();

        managePage->resize(wwidth, 0.8*wheight);
         out<<"managePage: "<<managePage->width()<<" "<<managePage->height()<<endl;
        managePage->setObjectName(QStringLiteral("managePage"));
        managePage->setSizePolicy(sizePolicy);

        label = new QLabel(managePage);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(managePage->x(), managePage->y(), managePage->width(), 30));

        QFont font;
        font.setPointSize(12);
        label->setFont(font);
        label->setFrameShape(QFrame::StyledPanel);
        label->setAlignment(Qt::AlignCenter);
        toolBox = new QToolBox(managePage);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        toolBox->setGeometry(QRect(0, 50, 0.35*wwidth, 0.5*wheight));

        out<<"toolBox: "<<toolBox->width()<<" "<<toolBox->height()<<endl;
       // sizePolicy.setHeightForWidth(toolBox->sizePolicy().hasHeightForWidth());
        toolBox->setSizePolicy(sizePolicy);
        toolBox->setFrameShape(QFrame::WinPanel);
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        page->setGeometry(QRect(0, 0, 376, 365));
         out<<"page: "<<page->width()<<" "<<page->height()<<endl;
        label_2 = new QLabel(page);
        label_2->setObjectName(QStringLiteral("label_2"));
//        label_2->setGeometry(QRect(30, 20, 61, 16));
        label_3 = new QLabel(page);
        label_3->setObjectName(QStringLiteral("label_3"));
//        label_3->setGeometry(QRect(30, 60, 61, 16));
        label_4 = new QLabel("备注：",page);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(30, 100, 31, 16));
        sellFactoryComboBox = new QComboBox(page);
        sellFactoryComboBox->setObjectName(QStringLiteral("sellFactoryComboBox"));
        sellFactoryComboBox->setGeometry(QRect(70, 20, 91, 22));
        sellBrandComboBox = new QComboBox(page);
        sellBrandComboBox->setObjectName(QStringLiteral("sellBrandComboBox"));
        sellBrandComboBox->setGeometry(QRect(70, 60, 71, 22));
        sellPriceLineEdit = new QTextEdit(page);
        sellPriceLineEdit->setObjectName(QStringLiteral("sellPriceLineEdit"));
        sellPriceLineEdit->setGeometry(QRect(70, 100, 160, 110));
        sellOkBtn = new QPushButton(page);
        sellOkBtn->setObjectName(QStringLiteral("sellOkBtn"));
        sellOkBtn->setGeometry(QRect(60, 240, 75, 23));
        sellCancelBtn = new QPushButton(page);
        sellCancelBtn->setObjectName(QStringLiteral("sellCancelBtn"));
//        sellCancelBtn->setGeometry(QRect(190, 240, 75, 23));
        nextRunBtn=new QPushButton("下一步",page);
        nextRunBtn->setObjectName("nextRunBtn");

        QGridLayout* leftLayout=new QGridLayout;
        leftLayout->setMargin(20);
        leftLayout->setSpacing(15);
        leftLayout->addWidget(label_2,0,0);
        leftLayout->addWidget(label_3,1,0);
        leftLayout->addWidget(label_4,2,0);
        leftLayout->addWidget(sellFactoryComboBox,0,1);
        leftLayout->addWidget(sellBrandComboBox,1,1);
        leftLayout->addWidget(sellPriceLineEdit,2,1);

        QHBoxLayout* hLayout=new QHBoxLayout;
        leftLayout->addWidget(sellOkBtn);
        leftLayout->addWidget(sellCancelBtn);
        QVBoxLayout* vLayout=new QVBoxLayout(page);
        vLayout->addLayout(leftLayout);
        vLayout->addLayout(hLayout);
        vLayout->addWidget(nextRunBtn);

        toolBox->addItem(page, QString::fromUtf8("\347\263\273\347\273\237\345\217\202\346\225\260"));
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        page_3->setGeometry(QRect(0, 0, 376, 365));
        toolBox->addItem(page_3, QString::fromUtf8("\346\231\272\350\203\275\347\275\221\347\273\234\344\270\216\347\275\221\347\273\234\345\256\211\345\205\250\346\225\231\350\202\262\351\203\250\351\207\215\347\202\271\345\256\236\351\252\214\345\256\244"));
        dailyList = new QListWidget(managePage);
        dailyList->setObjectName(QStringLiteral("dailyList"));
        dailyList->setGeometry(QRect(440, 50, 361, 371));
//        sizePolicy1.setHeightForWidth(dailyList->sizePolicy().hasHeightForWidth());
        dailyList->setSizePolicy(sizePolicy);
        out<<"dailyList: "<<dailyList->width()<<" "<<dailyList->height()<<endl;

        stackedWidget->addWidget(managePage);
        toolBox->raise();
        label->raise();
        dailyList->raise();
        chartPage = new QWidget();
        out<<"chartPage: "<<chartPage->width()<<" "<<chartPage->height()<<endl;

       // chartPage->resize(wwidth, wheight);
        chartPage->setObjectName(QStringLiteral("chartPage"));
        label_8 = new QLabel(chartPage);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(20, 10, 801, 30));
        label_8->setFont(font);
        label_8->setFrameShape(QFrame::StyledPanel);
        label_8->setAlignment(Qt::AlignCenter);
        stackedWidget->addWidget(chartPage);

        retranslateUi(Widget);

        stackedWidget->setCurrentIndex(0);
        toolBox->setCurrentIndex(0);
        QHBoxLayout *mainLayout = new QHBoxLayout(managePage);
        mainLayout->setMargin(50);
        mainLayout->setSpacing(30);
        mainLayout->addWidget(toolBox);
        mainLayout->addWidget(dailyList);



        QMetaObject::connectSlotsByName(Widget);
        //update();
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "XXXX\346\274\224\347\244\272\347\263\273\347\273\237", 0));
        label->setText(QApplication::translate("Widget", "XXXXX\347\263\273\347\273\237\345\217\202\346\225\260\350\256\276\345\256\232", 0));
        label_2->setText(QApplication::translate("Widget", "\347\263\273\347\273\237\357\274\232", 0));
        label_3->setText(QApplication::translate("Widget", "\347\255\226\347\225\245\357\274\232", 0));
      //  label_4->setText(QApplication::translate("Widget", "\345\244\207\346\263\250\357\274\232\357\274\232", 0));
        sellOkBtn->setText(QApplication::translate("Widget", "\347\241\256\345\256\232", 0));
        sellCancelBtn->setText(QApplication::translate("Widget", "\345\217\226\346\266\210", 0));
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("Widget", "\347\263\273\347\273\237\345\217\202\346\225\260", 0));
        toolBox->setItemText(toolBox->indexOf(page_3), QApplication::translate("Widget", "\346\231\272\350\203\275\347\275\221\347\273\234\344\270\216\347\275\221\347\273\234\345\256\211\345\205\250\346\225\231\350\202\262\351\203\250\351\207\215\347\202\271\345\256\236\351\252\214\345\256\244", 0));
        label_8->setText(QApplication::translate("Widget", "XXX\346\274\224\347\244\272\347\263\273\347\273\237", 0));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H

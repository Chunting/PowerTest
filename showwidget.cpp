#include "showwidget.h"
#include <QHBoxLayout>
ShowWidget::ShowWidget(QWidget *parent) :
    QWidget(parent)
{
    imageLabel =new QLabel;
    imageLabel->setScaledContents(true);
    if(img.load(":images/logIn.gif"))
    {
        imageLabel->setPixmap(QPixmap::fromImage(img));
        label_1 =new QLabel;
        QFont font;
        font.setPointSize(20);
        label_1->setScaledContents(true);
        label_1 = new QLabel;
        label_1->setObjectName(QStringLiteral("label_1"));
        label_1->setFont(font);
        label_1->setAlignment(Qt::AlignCenter);
        label_1->setText("西安交通大学");

        QFont font2;
        font2.setPointSize(40);
        font2.setBold(true);
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        label_2 = new QLabel;
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);
        label_2->setText("电网异常状态传播仿真系统");
        label_2->setFont(font2);
        label_2->setPalette(pa);

        label_3 = new QLabel;
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);
        label_4 = new QLabel;
        label_4->setObjectName(QStringLiteral("label_4"));

        label_3->setText("智能网络与网络安全教育部重点实验室");
        label_4->setText("2014年5月");

        label_3->setAlignment(Qt::AlignCenter);
        label_4->setAlignment(Qt::AlignCenter);
        label_4->setFont(font);
    }else{
        qDebug()<<"Load image error: :images/logIn.gif"<<endl;
    }
    QVBoxLayout *mainLayout =new QVBoxLayout(this);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(imageLabel);
    mainLayout->addSpacing(50);
    mainLayout->addWidget(label_2);
    mainLayout->addWidget(label_1);
    mainLayout->addSpacing(-50);
    mainLayout->addWidget(label_3);
    mainLayout->addSpacing(-50);
    mainLayout->addWidget(label_4);
}

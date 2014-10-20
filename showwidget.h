#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QImage>
#include <QDebug>

class ShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowWidget(QWidget *parent = 0);
    QImage img;
    QLabel *imageLabel;
    QLabel *label_1;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QTextEdit *text;
signals:
    
public slots:
    
};

#endif // SHOWWIDGET_H

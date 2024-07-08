#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H
#include <QPushButton>
#include <QDebug>

class MyPushButton:public QPushButton
{
    Q_OBJECT;
public:
    explicit MyPushButton(QObject *parent = nullptr);

    //重写构造函数 两个参数分别传入正常情况和按下情况的图片路径
    MyPushButton(QString normalImg, QString pressImg="");

    QString normalPath;
    QString pressPath;

    //重写鼠标摁下事件
    void mousePressEvent(QMouseEvent *event);

    //重写鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // MYPUSHBUTTON_H

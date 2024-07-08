#include "MyPushButton.h"

MyPushButton::MyPushButton(QString normalImg, QString pressImg)
{

    //正常状态下的图片路径
    this->normalPath = normalImg;
    //按下后的图片路径(较正常大一点 以实现动画效果)
    this->pressPath = pressImg;

    //创建QPixmap
    QPixmap pix;
    bool ret = pix.load(this->normalPath);
    if(!ret)
    {
        QString str = QString("图片加载失败，失败的路径是：%1！").arg(this->normalPath);
        qDebug()<<str;
    }

    //设置按钮固定尺寸
    this->setFixedSize(pix.width(),pix.height());
    //设置样式 不显示边框
    this->setStyleSheet("border:0px;");
    //设置图标
    this->setIcon(pix);
    //设置图标大小
    this->setIconSize(QSize(pix.width(),pix.height()));

}

//便于实现动画效果
void MyPushButton::mousePressEvent(QMouseEvent *event)
{
    //按下路径不为空 切图
    if(this->pressPath!="")
    {
        QPixmap pix;
        bool ret = pix.load(this->pressPath);
        if(!ret)
        {
            QString str = QString("图片加载失败，失败的路径是：%1！").arg(this->pressPath);
            qDebug()<<str;
        }

        //设置按钮固定尺寸
        this->setFixedSize(pix.width(),pix.height());
        //设置样式 不显示边框
        this->setStyleSheet("border:0px;");
        //设置图标
        this->setIcon(pix);
        //设置图标大小
        this->setIconSize(QSize(pix.width(),pix.height()));
    }

    //其他事件交给父类处理
    QPushButton::mousePressEvent(event);
}

//释放后的响应
void MyPushButton::mouseReleaseEvent(QMouseEvent *event)
{
    //按下路径不为空 还原
    if(this->pressPath!="")
    {
        QPixmap pix;
        bool ret = pix.load(this->normalPath);
        if(!ret)
        {
            QString str = QString("图片加载失败，失败的路径是：%1！").arg(this->normalPath);
            qDebug()<<str;
        }

        //设置按钮固定尺寸
        this->setFixedSize(pix.width(),pix.height());
        //设置样式 不显示边框
        this->setStyleSheet("QPushButton{border:0px;}");
        //设置图标
        this->setIcon(pix);
        //设置图标大小
        this->setIconSize(QSize(pix.width(),pix.height()));
    }

    //其他事件交给父类处理
    QPushButton::mouseReleaseEvent(event);
}


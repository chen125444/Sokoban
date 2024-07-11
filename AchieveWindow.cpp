#include "AchieveWindow.h"
#include "ui_AchieveWindow.h"

//绘制游戏关卡相关信息
void AchieveWindow::paintEvent(QPaintEvent *)
{
    QPainter * painter=new QPainter(this);
    //加载胜利数据
    LoadWinData();
    DrawStatus(painter);
    painter->end();
    update();
}

AchieveWindow::AchieveWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AchieveWindow)
{
    ui->setupUi(this);

    //加载相关数据
    LoadWinData();
    //创建返回按钮
    MyPushButton * returnBtn=new MyPushButton(":/myimages/images/return.png",":/myimages/images/returnBtn.png");
    returnBtn->setParent(this);
    returnBtn->move(0,0);
    //发出返回信号 待选关界面接收
    connect(returnBtn,&QPushButton::clicked,[=]
    {
        emit BackSignal();
    });
    //创建删除按钮
    MyPushButton * deleteBtn=new MyPushButton(":/myimages/images/delete.png",":/myimages/images/deleteBtn.png");
    deleteBtn->setParent(this);
    deleteBtn->move(this->width()-deleteBtn->width(),0);
    connect(deleteBtn,&QPushButton::clicked,[=]
    {
        //重置数据
        DeleteData();
    });

    //存入map 便于绘图
    stepMap["step1"]=this->ui->step1;
    stepMap["step2"]=this->ui->step2;
    stepMap["step3"]=this->ui->step3;
    stepMap["step4"]=this->ui->step4;

    timeMap["time1"]=this->ui->time1;
    timeMap["time2"]=this->ui->time2;
    timeMap["time3"]=this->ui->time3;
    timeMap["time4"]=this->ui->time4;

    //先把标签设为不可见
    for (int i=1;i<=MAP_SUM;i++)
    {
        stepMap["step"+QString::number(i)]->setVisible(false);
        timeMap["time"+QString::number(i)]->setVisible(false);
    }

}

//绘制相应数据
void AchieveWindow::DrawStatus(QPainter *painter)
{
    for(int i=1;i<=MAP_SUM;i++)
    {

        //加载是否通关图片
        if(isLevelWin[i-1])
        {
            //已解锁
            QPixmap winPixmap(":/myimages/images/unlock.png");
            painter->drawPixmap(160,135+180*(i-1),winPixmap);
            //设为可见
            stepMap["step"+QString::number(i)]->setVisible(true);
            timeMap["time"+QString::number(i)]->setVisible(true);
            //文字显示
            painter->setPen("#333333");
            painter->setFont(QFont("幼圆", 26, QFont::Bold));
            painter->drawText(410,200+180*(i-1),TimeLoad(winData[i][0]));
            painter->drawText(700,200+180*(i-1),QString::number(winData[i][1]));
            StarDraw(painter,winData[i][2],i);
        }
        else
        {
            QPixmap winPixmap(":/myimages/images/lock.png");
            painter->drawPixmap(160,135+180*(i-1),winPixmap);
            //设为不可见
            stepMap["step"+QString::number(i)]->setVisible(false);
            timeMap["time"+QString::number(i)]->setVisible(false);
        }

    }
}

//用来格式化时间
QString AchieveWindow::TimeLoad(int timeCount)
{
    int minutes = timeCount / 60;
    int seconds = timeCount % 60;
    QString min,sec;
    //如果分钟小于10，在前面添加一个0
    if (minutes < 10)
    {
        min = "0" + QString::number(minutes);
    }
    else
    {
        min = QString::number(minutes);
    }
    //如果秒数小于10，在前面添加一个0
    if (seconds < 10)
    {
        //转成字符串
        sec = "0" + QString::number(seconds);
    }
    else
    {
        sec = QString::number(seconds);
    }
    return min + ":" + sec;
}


//加载数据
void AchieveWindow::LoadWinData()
{
    for(int i=1;i<=MAP_SUM;i++)
    {
        QFile file("D:\\Qtcode\\Sokoban\\GameDataRecord"+QString::number(i)+".json");
        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray jsonData=file.readAll();
            QJsonDocument jsonDoc=QJsonDocument::fromJson(jsonData);
            if(!jsonDoc.isNull())
            {
                QJsonObject gameObject=jsonDoc.object();
                //存是否胜利
                isLevelWin[i-1]=gameObject["win"].toBool();
                //存时间 步数 星星数
                QVector<int> vec;
                vec.push_back(gameObject["time"].toInt());
                vec.push_back(gameObject["step"].toInt());
                vec.push_back(gameObject["star"].toInt());
                //存入图中 一一对应
                winData.insert(i,vec);
                file.close();
            }
        }
    }
}


void AchieveWindow::DeleteData()
{
    for(int i=1;i<=MAP_SUM;i++)
    {
        //重置存储的记录
        QFile file("D:\\Qtcode\\Sokoban\\GameDataRecord"+QString::number(i)+".json");
        if (file.open(QIODevice::ReadWrite))
        {
            QJsonObject gameObject;
            gameObject["win"]=false;
            gameObject["time"]=1000;
            gameObject["step"]=1000;
            gameObject["star"]=0;

            QVector<int> vec;
            winData.insert(i,vec);

            QJsonDocument jsonDoc(gameObject);
            file.write(jsonDoc.toJson());
            file.close();
        }
    }
}

//根据使用步数来绘制相应星星
void AchieveWindow::StarDraw(QPainter *painter,int starCount,int levelIndex)
{
    //Perfect 三颗星
    if(starCount==3)
    {
        for(int i=0;i<3;i++)
        {
            QPixmap pix;
            pix.load(":/myimages/images/star.png");
            painter->drawPixmap(800+75*i,140+180*(levelIndex-1),pix);
        }
    }
    //Good 两颗星
    else if(starCount==2)
    {
        for(int i=0;i<3;i++)
        {
            QPixmap pix;
            if(i<2)
            {
                pix.load(":/myimages/images/star.png");
            }
            else
            {
                pix.load(":/myimages/images/darkstar.png");
            }
            painter->drawPixmap(800+75*i,140+180*(levelIndex-1),pix);
        }
    }
    //Normal 一颗星
    else
    {
        for(int i=0;i<3;i++)
        {
            QPixmap pix;
            if(i<1)
            {
                pix.load(":/myimages/images/star.png");
            }
            else
            {
                pix.load(":/myimages/images/darkstar.png");
            }
            painter->drawPixmap(800+75*i,140+180*(levelIndex-1),pix);
        }
    }
}

AchieveWindow::~AchieveWindow()
{
    delete ui;
}

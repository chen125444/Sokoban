#include "AchieveWindow.h"
#include "ui_AchieveWindow.h"

//绘制游戏关卡相关信息
void AchieveWindow::paintEvent(QPaintEvent *)
{

    QPainter * painter=new QPainter;
    painter->begin(this);
//    //画人物移动提示(WASD)
//    QPixmap wBtn(":/myimages/images/wBtn.png");
//    QPixmap aBtn(":/myimages/images/aBtn.png");
//    QPixmap sBtn(":/myimages/images/sBtn.png");
//    QPixmap dBtn(":/myimages/images/dBtn.png");
//    painter->drawPixmap(60,95,wBtn);
//    painter->drawPixmap(0,150,aBtn);
//    painter->drawPixmap(60,150,sBtn);
//    painter->drawPixmap(120,150,dBtn);

//    if(gameLoad->GameOver()&&gameLoad->isSucceed)
//    {
//        isWin=gameLoad->isSucceed;
//        //加载胜利图片
//        QPixmap winPixmap(":/myimages/images/win.png");
//        painter->drawPixmap(this->width()*0.5-winPixmap.width()*0.5,this->height()*0.5-winPixmap.height()*0.5-150,winPixmap);

//        //文字显示
//        QPainter * painter=new QPainter;
//        painter->begin(this);
//        painter->setPen("#333333");
//        painter->setFont(QFont("幼圆", 30, QFont::Bold));
//        painter->drawText(270,500,"第"+QString::number((levelIndex+1)>=4?4:(levelIndex+1))+"关已解锁");
//        painter->drawText(312,575,"用时"+gameLoad->TimeLoad());
//        painter->drawText(312,650,"用了"+QString::number(gameLoad->totalStep-gameLoad->stepRemain)+"步！");
//        painter->end();

//        //保存数据
//        SaveWinData();
//    }
//    //游戏结束但未获胜
//    else if(gameLoad->GameOver()&&!gameLoad->isSucceed)
//    {
//        //加载失败图片
//        QPixmap winPixmap(":/myimages/images/lose.png");
//        painter->drawPixmap(this->width()*0.5-winPixmap.width()*0.5,this->height()*0.5-winPixmap.height()*0.5-150,winPixmap);

//        //文字显示
//        QPainter * painter=new QPainter;
//        painter->begin(this);
//        painter->setPen("#333333");
//        painter->setFont(QFont("幼圆", 30, QFont::Bold));
//        painter->drawText(147,500,"请点击右上角按钮重试！");
//        painter->end();
//    }
    painter->end();
    update();
}

AchieveWindow::AchieveWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AchieveWindow)
{
    ui->setupUi(this);
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
}

//加载数据
void AchieveWindow::LoadWinData()
{
    QFile file("GameDataRecord.json");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray jsonData = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        QJsonObject gameObject=doc.object();

        //是否解锁
        this->isLevel2Unlock=gameObject["isLevel2Unlock"].toBool();
        this->isLevel3Unlock=gameObject["isLevel3Unlock"].toBool();
        this->isLevel4Unlock=gameObject["isLevel4Unlock"].toBool();

        for(int i=0;i<4;i++)
        {
            QVector<int> vec;
            //存时间和步数
            vec.push_back(gameObject["time"+QString::number(i)].toInt());
            vec.push_back(gameObject["step"+QString::number(i)].toInt());
            //存入图中 一一对应
            winData.insert(i+1,vec);
        }
    }
}


void AchieveWindow::DeleteData()
{
    //重置存储的记录
    QFile file("GameDataRecord.json");
    if (file.open(QIODevice::ReadWrite))
    {
        QByteArray jsonData = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        QJsonObject gameObject = doc.object();

        gameObject["isLevel2Unlock"] = false;
        gameObject["isLevel3Unlock"] = false;
        gameObject["isLevel4Unlock"] = false;

        for(int i=0;i<4;i++)
        {
            QVector<int> vec;
            gameObject["time"+QString::number(i)]=10000;
            gameObject["step"+QString::number(i)]=10000;
            winData.insert(i+1,vec);
        }
        //重置后保存
        file.write(doc.toJson());
    }
    else
    {
        qDebug() << "Failed";
    }
}


AchieveWindow::~AchieveWindow()
{
    delete ui;
}

#include "GameScene.h"

//重写绘制事件
void GameScene::paintEvent(QPaintEvent *)
{
    mapPainter->begin(this);
    gameLoad->MapDraw(mapPainter);
    //画人物移动提示(WASD)
    QPixmap wBtn(":/myimages/images/wBtn.png");
    QPixmap aBtn(":/myimages/images/aBtn.png");
    QPixmap sBtn(":/myimages/images/sBtn.png");
    QPixmap dBtn(":/myimages/images/dBtn.png");
    mapPainter->drawPixmap(60,95,wBtn);
    mapPainter->drawPixmap(0,150,aBtn);
    mapPainter->drawPixmap(60,150,sBtn);
    mapPainter->drawPixmap(120,150,dBtn);

    if(gameLoad->GameOver()&&gameLoad->isSucceed)
    {
        isWin=gameLoad->isSucceed;
        //加载胜利图片
        QPixmap winPixmap(":/myimages/images/win.png");
        mapPainter->drawPixmap(this->width()*0.5-winPixmap.width()*0.5,this->height()*0.5-winPixmap.height()*0.5-150,winPixmap);

        //文字显示
        QPainter * painter=new QPainter;
        painter->begin(this);
        painter->setPen("#333333");
        painter->setFont(QFont("幼圆", 30, QFont::Bold));
        painter->drawText(270,500,"第"+QString::number((leverIndex+1)>=4?4:(leverIndex+1))+"关已解锁");
        painter->drawText(312,575,"用时"+gameLoad->TimeLoad());
        painter->drawText(312,650,"用了"+QString::number(totalStep-gameLoad->stepRemain)+"步！");
        painter->end();
    }
    //游戏结束但未获胜
    else if(gameLoad->GameOver()&&!gameLoad->isSucceed)
    {
        //加载失败图片
        QPixmap winPixmap(":/myimages/images/lose.png");
        mapPainter->drawPixmap(this->width()*0.5-winPixmap.width()*0.5,this->height()*0.5-winPixmap.height()*0.5-150,winPixmap);

        //文字显示
        QPainter * painter=new QPainter;
        painter->begin(this);
        painter->setPen("#333333");
        painter->setFont(QFont("幼圆", 30, QFont::Bold));
        painter->drawText(147,500,"请点击右上角按钮重试！");
        painter->end();
    }
    mapPainter->end();
    update();
}


//重写键盘事件
void GameScene::keyPressEvent(QKeyEvent *event)
{
    //游戏结束后不响应键盘事件
    if(!gameLoad->isOver)
    {
        gameLoad->KeyPress(event);
    }
    //确保计时器只开始一次
    if(gameLoad->isPress && gameLoad->pressConut==1)
    {
        //间隔1秒
        gameLoad->timer->start(1000);
        gameLoad->stepTimer->start(0);
    }
}

GameScene::GameScene(int Index)
{
    //记录关卡数
    leverIndex=Index;

    //设置界面尺寸
    this->setFixedSize(900,990);

    //设置窗口标题
    char c =  Index+48;
    QString str = QString("推箱子游戏 第%1关").arg(c);
    this->setWindowTitle(str);

    //设置窗口图标
    this->setWindowIcon(QIcon(":/myimages/images/box.png"));

    //创建画家对象
    mapPainter = new QPainter(this);

    //创建GameLoad对象
    gameLoad = new GameLoad(Index);

    //创建返回按钮
    MyPushButton * returnBtn=new MyPushButton(":/myimages/images/return.png");
    returnBtn->setParent(this);
    returnBtn->move(0,0);
    //发出返回信号 待选关界面接收
    connect(returnBtn,&QPushButton::clicked,[=]
    {
        emit BackSignal();
    });


    //计时模块
    QLabel * timeIconLabel=new QLabel;
    QImage img;
    img.load(":/myimages/images/time.png");
    timeIconLabel->setParent(this);
    timeIconLabel->setPixmap(QPixmap::fromImage(img));
    timeIconLabel->setGeometry(90,0,90,90);
    //显示时间标签
    QLabel * timeLabel=new QLabel;
    timeLabel->setParent(this);
    timeLabel->setGeometry(180,0,225,90);
    timeLabel->setText("00:00");
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setStyleSheet("color: #333333;font-size: 64px;font-weight: bold;font-family: Arial;");
    //接收gameLoad中计时器的timeout信号
    connect(gameLoad->timer, &QTimer::timeout, [=]
    {
        gameLoad->timeCount++;
        // 更新显示秒数的标签
        timeLabel->setText(gameLoad->TimeLoad());
    });


    //剩余步数模块
    QLabel * stepIconLabel=new QLabel;
    img.load(":/myimages/images/step.png");
    stepIconLabel->setParent(this);
    stepIconLabel->setPixmap(QPixmap::fromImage(img));
    stepIconLabel->setGeometry(405,0,90,90);
    //记录最大可用步数
    totalStep=gameLoad->stepRemain;
    //显示步数标签
    QLabel * stepLabel=new QLabel;
    stepLabel->setParent(this);
    stepLabel->setGeometry(495,0,225,90);
    stepLabel->setText(QString::number(gameLoad->stepRemain)+"/"+QString::number(totalStep));
    stepLabel->setAlignment(Qt::AlignCenter);
    stepLabel->setStyleSheet("color: #333333;font-size: 60px;font-weight: bold;font-family: Arial;");
    //接收gameLoad中stepTimer计时器的timeout信号
    connect(gameLoad->stepTimer, &QTimer::timeout, [=]
    {
        stepLabel->setText(QString::number(gameLoad->stepRemain)+"/"+QString::number(totalStep));
    });

    //创建撤销按钮
    MyPushButton * revokeBtn=new MyPushButton(":/myimages/images/revoke.png","");
    revokeBtn->setParent(this);
    revokeBtn->move(this->width()-returnBtn->width()*2,0);
    connect(revokeBtn,&QPushButton::clicked,[=]
    {
        //防止游戏结束后的撤销操作
        if(!gameLoad->isOver)
        {
            gameLoad->Revoke();
        }
    });


    //创建重玩按钮
    MyPushButton * restartBtn=new MyPushButton(":/myimages/images/restart.png");
    restartBtn->setParent(this);
    restartBtn->move(this->width()-restartBtn->width(),0);

    connect(restartBtn,&QPushButton::clicked,[=]
    {
        gameLoad->Restart();
        //重置时间
        timeLabel->setText("00:00");
        //重置步数
        gameLoad->stepRemain=totalStep;
    });
}

GameScene::~GameScene()
{

}

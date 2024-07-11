#include "GameScene.h"

//重写绘制事件
void GameScene::paintEvent(QPaintEvent *)
{
    mapPainter->begin(this);
    gameLoad->MapDraw(mapPainter);
    //画人物移动提示(WASD) 撤销快捷键R
    QPixmap wBtn(":/myimages/images/wBtn.png");
    QPixmap aBtn(":/myimages/images/aBtn.png");
    QPixmap sBtn(":/myimages/images/sBtn.png");
    QPixmap dBtn(":/myimages/images/dBtn.png");
    QPixmap rBtn(":/myimages/images/rBtn.png");
    QPixmap tBtn(":/myimages/images/tBtn.png");
    QPixmap escBtn(":/myimages/images/escBtn.png");
    mapPainter->drawPixmap(170,95,wBtn);
    mapPainter->drawPixmap(110,150,aBtn);
    mapPainter->drawPixmap(170,150,sBtn);
    mapPainter->drawPixmap(230,150,dBtn);
    mapPainter->drawPixmap(728,95,rBtn);
    mapPainter->drawPixmap(828,95,tBtn);
    mapPainter->drawPixmap(10,95,escBtn);
    mapPainter->setPen("#333333");
    mapPainter->setFont(QFont("幼圆", 26, QFont::Bold));
    mapPainter->drawText(310,160,"移动");
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

        starCount=gameLoad->StarDraw(painter);//评级并记录星星数
        painter->drawText(312,575,"用时"+gameLoad->TimeLoad());
        painter->drawText(312,650,"用了"+QString::number(gameLoad->totalStep-gameLoad->stepRemain)+"步！");
        painter->end();
        if(playCount==0)
        {
            //胜利音效
            player->setMedia(QUrl("qrc:/audios/win.mp3"));
            player->setVolume(100);
            player->play();
            playCount++;
            LoadData();
            //保存胜利数据
            SaveWinData();
        }
    }
    //游戏结束但未获胜
    else if(gameLoad->GameOver()&&!gameLoad->isSucceed)
    {
        if(playCount==0)
        {
            //失败音效
            player->setMedia(QUrl("qrc:/audios/lose.mp3"));
            player->setVolume(100);
            player->play();
            playCount++;
        }
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
        //开始计时
        gameLoad->timer->start(1000);
        gameLoad->updateTimer->start(0);
    }
}

GameScene::GameScene(int Index)
{
    //创建播放器对象
    player=new QMediaPlayer;

    //创建画家对象
    mapPainter = new QPainter(this);

    //创建GameLoad对象
    gameLoad = new GameLoad(Index);

    //记录关卡数
    levelIndex=Index;

    //设置界面尺寸
    this->setFixedSize(900,990);

    //设置窗口标题
    char c =  Index+48;
    QString str = QString("推箱子游戏 第%1关 最优步数:"+QString::number(gameLoad->perfectStep)+"步").arg(c);
    this->setWindowTitle(str);

    //设置窗口图标
    this->setWindowIcon(QIcon(":/myimages/images/box.png"));


    //创建返回按钮
    MyPushButton * returnBtn=new MyPushButton(":/myimages/images/return.png",":/myimages/images/returnBtn.png");
    returnBtn->setParent(this);
    returnBtn->setShortcut(Qt::Key_Escape);
    returnBtn->setFocusPolicy(Qt::ClickFocus);
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
    //显示步数标签
    QLabel * stepLabel=new QLabel;
    stepLabel->setParent(this);
    stepLabel->setGeometry(495,0,225,90);
    stepLabel->setText(QString::number(gameLoad->stepRemain)+"/"+QString::number(gameLoad->totalStep));
    stepLabel->setAlignment(Qt::AlignCenter);
    stepLabel->setStyleSheet("color: #333333;font-size: 60px;font-weight: bold;font-family: Arial;");
    //接收gameLoad中updateTimer计时器的timeout信号 间隔为0 实时更新步数和秒数
    connect(gameLoad->updateTimer, &QTimer::timeout, [=]
    {
        stepLabel->setText(QString::number(gameLoad->stepRemain)+"/"+QString::number(gameLoad->totalStep));
        // 更新显示秒数的标签
        timeLabel->setText(gameLoad->TimeLoad());
    });


    //创建撤销按钮
    MyPushButton * revokeBtn=new MyPushButton(":/myimages/images/revoke.png","");
    revokeBtn->setParent(this);
    revokeBtn->setFocusPolicy(Qt::ClickFocus);
    revokeBtn->setShortcut(Qt::Key_R);//快捷键
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
    restartBtn->setFocusPolicy(Qt::ClickFocus);
    restartBtn->setShortcut(Qt::Key_T);//快捷键
    restartBtn->move(this->width()-restartBtn->width(),0);

    connect(restartBtn,&QPushButton::clicked,[=]
    {
        gameLoad->Restart();
        playCount=0;
    });
}

//加载文件中存储时间等
void GameScene::LoadData()
{
    QFile file("D:\\Qtcode\\Sokoban\\GameDataRecord"+QString::number(levelIndex)+".json");
     //先读取
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray jsonData=file.readAll();
        QJsonDocument jsonDoc=QJsonDocument::fromJson(jsonData);
        if(!jsonDoc.isNull())
        {
            QJsonObject gameObject=jsonDoc.object();
            timeData=gameObject["time"].toInt();
            stepData=gameObject["step"].toInt();
            starData=gameObject["star"].toInt();
        }
        file.close();
    }
}
//游戏胜利后保存数据
void GameScene::SaveWinData()
{
    QFile file("D:\\Qtcode\\Sokoban\\GameDataRecord"+QString::number(levelIndex)+".json");
    //写入
    if(file.open(QIODevice::WriteOnly))
    {
        QJsonObject winObject;
        int stepUsed=gameLoad->totalStep-gameLoad->stepRemain;
        //记录时间 步数 星星数
        if(timeData>=gameLoad->timeCount)
        {
            winObject["time"]=gameLoad->timeCount;
        }
        else
        {
            winObject["time"]=timeData;
        }
        if(stepData>=stepUsed)
        {
            winObject["step"]=stepUsed;
        }
        else
        {
            winObject["step"]=stepData;
        }
        if(starData<=starCount)
        {
            winObject["star"]=starCount;
        }
        else
        {
            winObject["star"]=starData;
        }
        //记录胜利
        winObject["win"]=true;

        QJsonDocument jsonDoc(winObject);
        file.write(jsonDoc.toJson());
        file.close();
        qDebug()<<"winData saved";
    }
}

GameScene::~GameScene()
{

}

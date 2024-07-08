#include "LevelWindow.h"

//画背景图
void LevelWindow::paintEvent(QPaintEvent *)
{
    //创建画家对象
    QPainter painter(this);
    //创建PixMap对象
    QPixmap pix;
    //加载图片
    pix.load(":/myimages/images/sky.png");
    //画背景图
//    pix=pix.scaled(pix.width(),pix.height());
    painter.drawPixmap(0,0,pix.width(),pix.height(),pix);
}

LevelWindow::LevelWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //设置固定尺寸
    this->setFixedSize(1500,844);

    //设置标题
    this->setWindowTitle("推箱子 选择关卡");

    //设置图标
    this->setWindowIcon(QIcon(":/myimages/images/box.png"));

//    //加载历史记录
//    LoadLevelUnlockData(isLevel2Unlock, isLevel3Unlock, isLevel4Unlock, "GameDataRecord.json");
    //创建返回按钮
    MyPushButton * backBtn = new MyPushButton(":/myimages/images/BACK.png",":/myimages/images/backBtn.png");
    backBtn->setParent(this);
    //将返回按钮放至选关界面右下角
    backBtn->move(this->width()-backBtn->width()-10,this->height()-backBtn->height()-10);

    //点击BACK后发出返回信号 待mainmenu.cpp中相应函数接受
    connect(backBtn,&QPushButton::clicked,[=]
    {
        //延时
        QTimer::singleShot(200,this,[=]
        {
            emit BackSignal();
        });
    });


    //创建选关按钮
    level1=new MyPushButton(":/myimages/images/level1.png");
    level1->setParent(this);
    level1->move(pos[0][0],pos[0][1]);
    //先把其他按钮设置成不可用(关卡未解锁)
    level2=new MyPushButton(":/myimages/images/level2.png");
    level2->setParent(this);
    level2->move(pos[1][0],pos[1][1]);
    level2->setDisabled(true);

    level3=new MyPushButton(":/myimages/images/level3.png");
    level3->setParent(this);
    level3->move(pos[2][0],pos[2][1]);
    level3->setDisabled(true);

    level4=new MyPushButton(":/myimages/images/level4.png");
    level4->setParent(this);
    level4->move(pos[3][0],pos[3][1]);
    level4->setDisabled(true);

    //利用计时器来实时更新关卡解锁情况
    timer=new QTimer;
    timer->start(0);
    //用来更新各个关卡的解锁情况
    connect(timer,&QTimer::timeout,[=]
    {
        if(isLevel2Unlock)
        {
            level2->setDisabled(false);
        }
        else
        {
            level2->setDisabled(true);
        }
        if(isLevel3Unlock)
        {
            level3->setDisabled(false);
        }
        else
        {
            level3->setDisabled(true);
        }
        if(isLevel4Unlock)
        {
            level4->setDisabled(false);
        }
        else
        {
            level4->setDisabled(true);
        }
    });

    connect(level1,&MyPushButton::clicked,[=]
    {
        //实例化当前关卡
        gameScene = new GameScene(1);
        gameScene->show();//显示游戏界面
        this->hide();//隐藏选关界面

        //监听游戏界面的返回信号
        connect(gameScene,&GameScene::BackSignal,[=]
        {
            //关卡1胜利 解锁关卡2
            isLevel2Unlock=gameScene->isWin;
            gameScene->close();//关闭游戏界面
            this->show();//显示选关界面
            //保存数据
            SaveLevelUnlockData(isLevel2Unlock,isLevel3Unlock,isLevel4Unlock,"GameDataRecord.json");
        });
    });
    connect(level2,&MyPushButton::clicked,[=]
    {
        //实例化当前关卡
        gameScene = new GameScene(2);
        gameScene->show();//显示游戏界面
        this->hide();//隐藏选关界面

        //监听游戏界面的返回信号
        connect(gameScene,&GameScene::BackSignal,[=]
        {
            //关卡2胜利 解锁关卡3
            isLevel3Unlock=gameScene->isWin;
            gameScene->close();//关闭游戏界面
            this->show();//显示选关界面
            //保存数据
            SaveLevelUnlockData(isLevel2Unlock,isLevel3Unlock,isLevel4Unlock,"GameDataRecord.json");
        });
    });
    connect(level3,&MyPushButton::clicked,[=]
    {
        //实例化当前关卡
        gameScene = new GameScene(3);
        gameScene->show();//显示游戏界面
        this->hide();//隐藏选关界面

        //监听游戏界面的返回信号
        connect(gameScene,&GameScene::BackSignal,[=]
        {
            //关卡3胜利 解锁关卡4
            isLevel4Unlock=gameScene->isWin;
            gameScene->close();//关闭游戏界面
            this->show();//显示选关界面
            //保存数据
            SaveLevelUnlockData(isLevel2Unlock,isLevel3Unlock,isLevel4Unlock,"GameDataRecord.json");
        });
    });
    connect(level4,&MyPushButton::clicked,[=]
    {
        //实例化当前关卡
        gameScene = new GameScene(4);
        gameScene->show();//显示游戏界面
        this->hide();//隐藏选关界面

        //监听游戏界面的返回信号
        connect(gameScene,&GameScene::BackSignal,[=]
        {
            gameScene->close();//关闭游戏界面
            this->show();//显示选关界面
            //保存数据
            SaveLevelUnlockData(isLevel2Unlock,isLevel3Unlock,isLevel4Unlock,"GameDataRecord.json");
        });
    });
}

//保存
void LevelWindow::SaveLevelUnlockData(bool isLevel2Unlock, bool isLevel3Unlock, bool isLevel4Unlock, const QString& filename)
{
    QJsonObject levelUnlockObject;
    levelUnlockObject["isLevel2Unlock"] = isLevel2Unlock;
    levelUnlockObject["isLevel3Unlock"] = isLevel3Unlock;
    levelUnlockObject["isLevel4Unlock"] = isLevel4Unlock;

    QJsonDocument doc(levelUnlockObject);

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson());
        file.close();
        qDebug() << "Level unlock data saved to" << filename;
    }
    else
    {
        qDebug() << "Failed to save level unlock data to" << filename;
    }
}

//加载
void LevelWindow::LoadLevelUnlockData(bool& isLevel2Unlock, bool& isLevel3Unlock, bool& isLevel4Unlock, const QString& filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray jsonData = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        QJsonObject levelUnlockObject = doc.object();

        isLevel2Unlock = levelUnlockObject["isLevel2Unlock"].toBool();
        isLevel3Unlock = levelUnlockObject["isLevel3Unlock"].toBool();
        isLevel4Unlock = levelUnlockObject["isLevel4Unlock"].toBool();

        qDebug() << "Level unlock data loaded from" << filename;
    }
    else
    {
        qDebug() << "Failed to load level unlock data from" << filename;
    }
}

LevelWindow::~LevelWindow()
{

}

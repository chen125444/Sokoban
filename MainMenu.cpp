#include "MainMenu.h"

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
{
    //设置主菜单尺寸
    this->setFixedSize(600,900);

    //设置标题
    this->setWindowTitle("推箱子游戏");

    //设置图标
    this->setWindowIcon(QIcon(":/myimages/images/box.png"));
    this->setStyleSheet("background::/myimages/images/background.png");
    //创建选关界面对象
    levelWindow=new LevelWindow();

    //创建PLAY按钮
    MyPushButton * playBtn=new MyPushButton(":/myimages/images/PLAY.png",":/myimages/images/playBtn.png");
    playBtn->setParent(this);
    //水平居中
    playBtn->move(this->width()*0.5-playBtn->width()*0.5,this->height()*0.6);

    //用Lambda表达式连接信号与槽
    connect(playBtn,&MyPushButton::clicked,[=]
    {
        //延时 以显示动画效果
        QTimer::singleShot(400,this,[=]
        {
            this->hide();//隐藏主菜单界面
            levelWindow->show(); //显示选关界面
            levelWindow->loadLevelUnlockData(levelWindow->isLevel2Unlock,levelWindow->isLevel3Unlock,levelWindow->isLevel4Unlock,"GameDataRecord.json");
        });
    });

    //监听选关界面中的返回信号
    connect(levelWindow,&LevelWindow::BackSignal,[=]
    {
        levelWindow->hide();//隐藏选关界面
        this->show();//显示主菜单
    });

    //创建EXIT按钮
    MyPushButton * exitBtn=new MyPushButton(":/myimages/images/EXIT.png",":/myimages/images/exitBtn.png");
    exitBtn->setParent(this);
    //水平居中
    exitBtn->move(this->width()*0.5-playBtn->width()*0.5,this->height()*0.7);

    //点击EXIT后退出
    connect(exitBtn,&MyPushButton::clicked,[=]
    {
        //延时
        QTimer::singleShot(300,this,[=]
        {
            this->close();//关闭界面
        });
    });

    //创建成就按钮
    MyPushButton * achieveBtn=new MyPushButton(":/myimages/images/achievement.png");
    achieveBtn->setParent(this);
    achieveBtn->move(this->width()-achieveBtn->width()-10,this->height()-achieveBtn->height()-10);

    //点击成就按钮后跳转
    connect(achieveBtn,&MyPushButton::clicked,[=]
    {
        //延时
        QTimer::singleShot(300,this,[=]
        {
            //重置存储的记录
            QString filename = "GameDataRecord.json";
            QFile file(filename);
            if (file.open(QIODevice::ReadWrite))
            {
                QByteArray jsonData = file.readAll();
                QJsonDocument doc = QJsonDocument::fromJson(jsonData);
                QJsonObject levelUnlockObject = doc.object();

                levelUnlockObject["isLevel2Unlock"] = false;
                levelUnlockObject["isLevel3Unlock"] = false;
                levelUnlockObject["isLevel4Unlock"] = false;
                //重置后保存
                file.write(doc.toJson());
            }
            else
            {
                qDebug() << "Failed to load level unlock data from" << filename;
            }
        });
    });
}

MainMenu::~MainMenu()
{

}


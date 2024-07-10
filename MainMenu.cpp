#include "MainMenu.h"

//画背景图
void MainMenu::paintEvent(QPaintEvent *)
{
    //创建画家对象
    QPainter painter(this);
    //创建PixMap对象
    QPixmap pix;
    //加载图片
    pix.load(":/myimages/images/mainback.png");
    //画背景图
    pix=pix.scaled(pix.width(),pix.height());
    painter.drawPixmap(0,0,pix.width(),pix.height(),pix);

    //加载标题资源
    pix.load(":/myimages/images/tittle.png");
    //拉伸图片
    pix = pix.scaled(pix.width()*0.25,pix.height()*0.30);
    //画标题boxman
    painter.drawPixmap(this->width()*0.5-pix.width()*0.5,200,pix);

    //加载box
    pix.load(":/myimages/images/box.png");
    //画box
    painter.drawPixmap(this->width()*0.25-pix.width()*0.5+16,300,pix);

    //加载man
    pix.load(":/myimages/images/man.png");
    //拉伸图片
    pix = pix.scaled(pix.width()*1.75,pix.height()*1.75);
    //画man
    painter.drawPixmap(this->width()*0.75-pix.width()*0.5-16,278,pix);


}

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
{
    //设置主菜单尺寸
    this->setFixedSize(650,1156);

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
    playBtn->setFocusPolicy(Qt::NoFocus);
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
    exitBtn->setFocusPolicy(Qt::NoFocus);
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
    MyPushButton * achieveBtn=new MyPushButton(":/myimages/images/achievement.png",":/myimages/images/achievementBtn.png");
    achieveBtn->setParent(this);
    achieveBtn->setFocusPolicy(Qt::NoFocus);
    achieveBtn->move(this->width()-achieveBtn->width()-125,this->height()-achieveBtn->height()-10);

    //创建成就界面
    achieveWindow = new AchieveWindow;

    //点击成就按钮后跳转
    connect(achieveBtn,&MyPushButton::clicked,[=]
    {
        //延时
        QTimer::singleShot(300,this,[=]
        {
            this->hide();//隐藏主菜单
            achieveWindow->show();//显示成就页面
        });
    });

    //监听成就界面中的返回信号
    connect(achieveWindow,&AchieveWindow::BackSignal,[=]
    {
        achieveWindow->close();//关闭成就界面
        this->show();//显示主菜单
    });


    //创建设置按钮
    MyPushButton * settingBtn=new MyPushButton(":/myimages/images/setting.png",":/myimages/images/settingBtn.png");
    settingBtn->setParent(this);
    settingBtn->setFocusPolicy(Qt::NoFocus);
    settingBtn->move(this->width()-settingBtn->width()-10,this->height()-settingBtn->height()-10);

    //创建设置界面
    setting=new Setting();

    //点击设置按钮后跳转
    connect(settingBtn,&MyPushButton::clicked,[=]
    {
        //延时
        QTimer::singleShot(300,this,[=]
        {
            this->hide();//隐藏主菜单
            setting->show();//显示设置页面
        });
    });

    //监听设置界面中的返回信号
    connect(setting,&Setting::BackSignal,[=]
    {
        setting->close();//关闭设置界面
        this->show();//显示主菜单
    });

}

MainMenu::~MainMenu()
{
    delete levelWindow;
    delete achieveWindow;
    delete setting;
}


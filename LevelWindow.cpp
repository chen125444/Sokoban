#include "LevelWindow.h"

//画背景图
void LevelWindow::paintEvent(QPaintEvent *)
{
    //创建画家对象
    QPainter painter(this);
    //创建PixMap对象
    QPixmap pix;
    //加载图片
    pix.load(":/myimages/images/sky.jpg");
    //画背景图
    pix=pix.scaled(pix.width(),pix.height());
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

    //创建返回按钮
    MyPushButton * backBtn = new MyPushButton(":/myimages/images/BACK.png",":/myimages/images/backBtn.png");
    backBtn->setParent(this);
    backBtn->setFocusPolicy(Qt::NoFocus);
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
        //利用计时器来实时更新关卡完成情况
        QTimer * timer=new QTimer;
        timer->start(0);

    //创建选关按钮 游戏场景
    for(int i=0;i<MAP_SUM;i++)
    {
        QString num=QString::number(i+1);
        levelBtn=new MyPushButton(":/myimages/images/level"+num+".png");
        levelBtn->setParent(this);
        levelBtn->setFocusPolicy(Qt::NoFocus);
        levelBtn->move(pos[i][0],pos[i][1]);

        connect(levelBtn,&MyPushButton::clicked,[=]
        {
            //实例化当前关卡
            gameScene = new GameScene(i+1);
            gameScene->show();//显示游戏界面
            this->hide();//隐藏选关界面

            //监听游戏界面的返回信号
            connect(gameScene,&GameScene::BackSignal,[=]
            {
                gameScene->close();//关闭游戏界面
                this->show();//显示选关界面
                if(!isLevelWin[i])
                {
                   isLevelWin[i]=gameScene->isWin;
                }
            });
        });
    }
}

LevelWindow::~LevelWindow()
{
    //释放内存
    delete timer;
    delete gameScene;
    delete levelBtn;
}

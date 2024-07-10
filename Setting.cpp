#include "Setting.h"
#include "ui_setting.h"

Setting::Setting(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);

    //创建播放器对象
    player=new QMediaPlayer;
    //创建返回按钮
    MyPushButton * returnBtn=new MyPushButton(":/myimages/images/return.png",":/myimages/images/returnBtn.png");
    returnBtn->setParent(this);
    returnBtn->move(0,0);
    //发出返回信号 待选关界面接收
    connect(returnBtn,&QPushButton::clicked,[=]
    {
        emit BackSignal();
    });

    connect(this->ui->play,SIGNAL(clicked()),this,SLOT(on_play_clicked()));
    connect(this->ui->stop,SIGNAL(clicked()),this,SLOT(on_stop_clicked()));
    connect(this->ui->volumeChange,SIGNAL(valueChanged(int)),this,SLOT(on_volumeChange_valueChanged(int)));

    //默认显示为0 且选中stop按钮
    this->ui->volumeChange->setValue(0);
    this->ui->volumeLabel->setText(QString::number(0));

}

Setting::~Setting()
{
    delete ui;
    delete player;
}

void Setting::on_play_clicked()
{
    //播放 音量设为100
    player->setMedia(QUrl("qrc:/audios/background.mp3"));
    this->ui->volumeChange->setValue(100);
    player->setVolume(100);
    player->play();
}

void Setting::on_stop_clicked()
{
    //静音
    this->ui->volumeChange->setValue(0);
    player->stop();
}

void Setting::on_volumeChange_valueChanged(int value)
{
    //改变播放器音量
    player->setVolume(value);
    //改变label显示数字
    this->ui->volumeLabel->setText(QString::number(value));

}

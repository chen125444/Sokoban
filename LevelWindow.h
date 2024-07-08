#ifndef LEVELWINDOW_H
#define LEVELWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "MyPushButton.h"
#include "GameScene.h"

class LevelWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LevelWindow(QWidget *parent = nullptr);
    ~LevelWindow();

    //重写绘制事件 画背景图
    void paintEvent(QPaintEvent *);
    //各关卡按钮的坐标
    double pos[4][2]={
        {140,320},
        {500,225},
        {850,430},
        {1150,350}
    };

    //计时器 用于更新解锁情况
    QTimer * timer;

    //创建游戏场景
    GameScene * gameScene;

    //关卡按钮
    MyPushButton * level1;
    MyPushButton * level2;
    MyPushButton * level3;
    MyPushButton * level4;

    //判断各关卡是否解锁 默认为未解锁
    bool isLevel2Unlock=false;
    bool isLevel3Unlock=false;
    bool isLevel4Unlock=false;

    //保存游戏记录
    void saveLevelUnlockData(bool isLevel2Unlock, bool isLevel3Unlock, bool isLevel4Unlock, const QString& filename);
    //加载记录
    void loadLevelUnlockData(bool& isLevel2Unlock, bool& isLevel3Unlock, bool& isLevel4Unlock, const QString& filename);
signals:
    //返回信号
    void BackSignal();
};

#endif // LEVELWINDOW_H

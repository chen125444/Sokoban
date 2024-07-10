#ifndef LEVELWINDOW_H
#define LEVELWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDebug>
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

    MyPushButton * levelBtn;

    bool isLevelWin[MAP_SUM]={false};

signals:
    //返回信号
    void BackSignal();
};

#endif // LEVELWINDOW_H

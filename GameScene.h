#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QMainWindow>
#include <QPushButton>
#include <QTest>
#include <QDateTime>
#include <QLabel>
#include <QTimer>
#include "MyPushButton.h"
#include "GameLoad.h"

class GameScene : public QMainWindow
{
    Q_OBJECT

    //创建相应对象
    QPainter * mapPainter;
    GameLoad * gameLoad;


public:
    explicit GameScene(int Index);
    ~GameScene();

    //重写绘制事件
    void paintEvent(QPaintEvent * event);

    //重写键盘事件
    void keyPressEvent(QKeyEvent *event);

    //关卡数
    int leverIndex;
    //可用步数
    int totalStep;
    //记录是否胜利
    bool isWin=false;

signals:
    //返回信号
    void BackSignal();
};

#endif // GAMESCENE_H

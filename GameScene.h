#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QMainWindow>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonDocument>
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
    QMediaPlayer * player;

public:
    explicit GameScene(int Index);
    ~GameScene();

    //重写绘制事件
    void paintEvent(QPaintEvent * event);

    //重写键盘事件
    void keyPressEvent(QKeyEvent *event);

    //游戏胜利后保存  胜利 时间 步数数据
    void SaveWinData();
    //关卡数
    int levelIndex;
    //记录是否胜利
    bool isWin=false;
    //防止重复播放
    int playCount=0;

signals:
    //返回信号
    void BackSignal();
};

#endif // GAMESCENE_H

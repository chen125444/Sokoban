#ifndef GAMELOAD_H
#define GAMELOAD_H

#include <QPainter>
#include <QStack>
#include <QVector>
#include <QTimer>
#include <QKeyEvent>
#include <QMediaPlayer>
#include "MapData.h"

class GameLoad
{
public:
    GameLoad(int Index);
    ~GameLoad();
    //音频播放
    QMediaPlayer * player1;
    QMediaPlayer * player2;
    QMediaPlayer * player3;

    void PushPlay();//推动箱子的音效
    void PushDesPlay();//箱子到目的地
    void walkPlay();//行走音效

    //地图绘制
    void MapDraw(QPainter * painter);

    //评级绘制
    int StarDraw(QPainter * painter);

    //地图更新
    void MapUpdate();

    //撤销上一步
    void Revoke();
    //利用栈来保存地图所有数据 便于撤销(Revoke)
    QStack<QVector<QVector<int>>> mapData;
    QStack<QVector<int>> manData;
    //存放地图数据和人相关的信息至栈
    void LastMapStore(QVector<QVector<int>>,QVector<int>);

    //重玩按钮
    void Restart();

    //按下按钮后的处理
    void KeyPress(QKeyEvent * event);
    bool isPress=false;//判断是否开始游戏 便于计时
    int pressConut=0;
    //标记按下键盘的上下左右
    int keyMark=1;

    //创建计时器
    QTimer * timer;//用于更新时间
    QTimer * updateTimer;//更新步数和秒数

    int timeCount=0;//记录已过秒数
    bool isTimerStop=false;//记录是否暂停
    //加载时间
    QString TimeLoad();

    //朝向转变 分成是否在目的地
    int walkTowardsChange(int oStatus,int keyMark);

    //判断游戏结束
    int fixedSum=0;//记录总箱子数
    bool GameOver();
    bool isOver=false;//判断结束
    bool isSucceed=false;//判断是否胜利

    //当前关卡数
    int levelIndex;
    //关卡可用步数 最优步数
    int totalStep;
    int stepRemain;
    int perfectStep;

    //人的坐标
    int man_x;
    int man_y;

    //记录当前地块原始状态 默认为MAP_WAY
    int oStatus=MAP_WAY;

    //记录行走方向上的位置
    int x_0,y_0 , x_1,y_1 , x_2,y_2;

    //地图信息 人相关信息
    QVector<QVector<int>> gameMap;
    QVector<int> manMap;

};

#endif // GAMELOAD_H

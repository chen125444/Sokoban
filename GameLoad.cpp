#include "GameLoad.h"

//音频播放
void GameLoad::PushPlay()
{
    player1->setMedia(QUrl("qrc:/audios/push.mp3"));
    player1->setVolume(100);
    player1->play();
}
void GameLoad::PushDesPlay()
{
    player2->setMedia(QUrl("qrc:/audios/push_des.mp3"));
    player2->setVolume(100);
    player2->play();
}
void GameLoad::walkPlay()
{
    player3->setMedia(QUrl("qrc:/audios/walk.mp3"));
    player3->setVolume(100);
    player3->play();
}
GameLoad::GameLoad(int Index)
{
    //创建播放器对象
    player1=new QMediaPlayer;
    player2=new QMediaPlayer;
    player3=new QMediaPlayer;
    //记录当前关卡数
    levelIndex=Index;
    //计时器
    timer=new QTimer;
    updateTimer=new QTimer;

    //创建地图对象 加载数据
    MapData * mapdata=new MapData;
    for(int i=0; i<15; i++)
    {
        QVector<int> vec;
        for(int j=0; j<15; j++)
        {
            vec.push_back(mapdata->mData[levelIndex][i][j]);
        }
        gameMap.push_back(vec);
    }
    totalStep=mapdata->stepData[levelIndex];//加载关卡可用步数
    perfectStep=mapdata->bestStepData[levelIndex];//加载关卡最优步数
    stepRemain=totalStep;

    //记录当前人的位置 计算箱子总数
    for(int i=0; i<MAP_WIDTH; i++)
    {
        for(int j=0;j<MAP_HEIGTHT; j++)
        {
            if(gameMap[i][j] == MAP_MAN_DOWN)//地图数据中人默认朝下
            {
                man_x = i;
                man_y = j;
            }
            else if(gameMap[i][j]==MAP_BOX||gameMap[i][j]==MAP_BOX_FIXED)
            {
                fixedSum++;
            }
        }
    }
}

//地图绘制
void GameLoad::MapDraw(QPainter * painter)
{
    QString path;
    //获取图片路径并绘制
    for(int i=0; i<MAP_HEIGTHT; i++)
    {
        for(int j=0;j<MAP_WIDTH; j++)
        {
            switch (gameMap[i][j])
            {
            //背景
            case MAP_BACKGROUND:
                path=":/myimages/images/background.png";
                break;
            //墙壁
            case MAP_WALL:
                path=":/myimages/images/wall.png";
                break;
            //道路
            case MAP_WAY:
                path=":/myimages/images/way.png";
                break;
            //目的地
            case MAP_DESTINATION:
                path=":/myimages/images/destination.png";
                break;
            //箱子
            case MAP_BOX:
                path=":/myimages/images/box.png";
                break;
            //箱子到达目的地
            case MAP_BOX_FIXED:
                path=":/myimages/images/box_fixed.png";
                break;
            //人朝下
            case MAP_MAN_DOWN:
                path=":/myimages/images/1.png";
                break;
            //人朝上
            case MAP_MAN_UP:
                path=":/myimages/images/2.png";
                break;
            //人朝左
            case MAP_MAN_LEFT:
                path=":/myimages/images/3.png";
                break;
            //人朝右
            case MAP_MAN_RIGHT:
                path=":/myimages/images/4.png";
                break;
            //人朝下且在目的地
            case MAP_MAN_DOWN_DES:
                path=":/myimages/images/1_des.png";
                break;
            //人朝上且在目的地
            case MAP_MAN_UP_DES:
                path=":/myimages/images/2_des.png";
                break;
            //人朝左且在目的地
            case MAP_MAN_LEFT_DES:
                path=":/myimages/images/3_des.png";
                break;
            //人朝右且在目的地
            case MAP_MAN_RIGHT_DES:
                path=":/myimages/images/4_des.png";
                break;

            }
            QPixmap pix;
            pix.load(path);
            pix.scaled(pix.width(),pix.height());
            painter->drawPixmap(j*M_BLOCK_WIDTH,90+i*M_BLOCK_HEITHT,pix.width(),pix.height(),pix);
        }
    }
}


//键盘响应
void GameLoad::KeyPress(QKeyEvent *event)
{
    //开始游戏 计时开始
    isPress=true;
    isTimerStop=false;
    pressConut++;
    //记录人位置
    x_0=man_x;
    y_0=man_y;

    switch (event->key())
    {
    //ps:注意这里x表示的是行 相当于纵坐标
    //向下走
    case Qt::Key_S:
    case Qt::Key_Down:
        x_1=man_x+1;
        y_1=man_y;
        x_2=man_x+2;
        y_2=man_y;

        //记录 便于切图
        keyMark=1;
        break;

    //向上走
    case Qt::Key_W:
    case Qt::Key_Up:
        x_1=man_x-1;
        y_1=man_y;
        x_2=man_x-2;
        y_2=man_y;

        keyMark=2;
        break;

    //向左走
    case Qt::Key_A:
    case Qt::Key_Left:
        x_1=man_x;
        y_1=man_y-1;
        x_2=man_x;
        y_2=man_y-2;

        keyMark=3;
        break;

    //向右走
    case Qt::Key_D:
    case Qt::Key_Right:
        x_1=man_x;
        y_1=man_y+1;
        x_2=man_x;
        y_2=man_y+2;

        keyMark=4;
        break;

    default:
        return;
    }

    //更新地图
    MapUpdate();
}


//撤销功能实现 获取上个地图所有数据
void GameLoad::Revoke()
{
    if(!mapData.empty()&&!manData.empty())
    {
        //取出地图数据
        gameMap=mapData.pop();
        //取出与人相关的数据
        QVector<int> man=manData.pop();

        man_x=man[0];
        man_y=man[1];
        oStatus=man[2];
        //计时器已停则不还原步数
        if(isTimerStop)
        {
            return;
        }
        //步数还原
        stepRemain++;
    }
}
//存放地图数据至栈
void GameLoad::LastMapStore(QVector<QVector<int>> gameMap,QVector<int> manMap)
{
    mapData.push(gameMap);

    manData.push(manMap);
}

//重玩功能实现 还原各种数据
void GameLoad::Restart()
{
    //先清空
    gameMap.clear();
    //加载一遍原始数据
    MapData * data=new MapData;
    for(int i=0; i<15; i++)
    {
        QVector<int> vec;
        for(int j=0; j<15; j++)
        {
            vec.push_back(data->mData[levelIndex][i][j]);
        }
        gameMap.push_back(vec);
    }
    //重新确定人的位置
    for(int i=0; i<MAP_WIDTH; i++)
    {
        for(int j=0;j<MAP_HEIGTHT; j++)
        {
            if(gameMap[i][j] == MAP_MAN_DOWN)//地图数据中人默认朝下
            {
                man_x = i;
                man_y = j;
            }
        }
    }
    //还原地块状态 计时器状态 步数 结束、胜利状态 存放地图数据的栈
    oStatus=MAP_WAY;

    isPress=false;
    pressConut=0;
    timeCount=0;
    isTimerStop=false;
    timer->stop();

    stepRemain=totalStep;

    isOver=false;
    isSucceed=false;

    mapData.clear();
    manData.clear();
}


//计时功能实现
QString GameLoad::TimeLoad()
{
    int minutes = timeCount / 60;
    int seconds = timeCount % 60;
    QString min,sec;
    //如果分钟小于10，在前面添加一个0
    if (minutes < 10)
    {
        min = "0" + QString::number(minutes);
    }
    else
    {
        min = QString::number(minutes);
    }
    //如果秒数小于10，在前面添加一个0
    if (seconds < 10)
    {
        //转成字符串
        sec = "0" + QString::number(seconds);
    }
    else
    {
        sec = QString::number(seconds);
    }
    return min + ":" + sec;
}


//判断朝向函数 便于切图 nextStatus表示下一个位置的地块类型
int GameLoad::walkTowardsChange(int nextStatus, int keyMark)
{
    //移动到的位置在目的地
    if(nextStatus==MAP_DESTINATION)
    {
        switch (keyMark)
        {
        case 1:
            return MAP_MAN_DOWN_DES;
        case 2:
            return MAP_MAN_UP_DES;
        case 3:
            return MAP_MAN_LEFT_DES;
        case 4:
            return MAP_MAN_RIGHT_DES;
        default:
            return MAP_MAN_DOWN_DES;
        }
    }
    else
    {
        switch (keyMark)
        {
        case 1:
            return MAP_MAN_DOWN;
        case 2:
            return MAP_MAN_UP;
        case 3:
            return MAP_MAN_LEFT;
        case 4:
            return MAP_MAN_RIGHT;
        default:
            return MAP_MAN_DOWN;
        }
    }
}


//地图更新
void GameLoad::MapUpdate()
{
    //先清空
    if(!manMap.empty())
    {
        manMap.clear();
    }
    //存入相关数据
    manMap.append(man_x);
    manMap.append(man_y);
    manMap.append(oStatus);

    switch (gameMap[x_1][y_1])
    {
    //下一个位置是墙
    case MAP_WALL:
        //人不移动 朝向变化 nextStatus即oStatus
        gameMap[man_x][man_y]=walkTowardsChange(oStatus,keyMark);
        break;

    //下一个位置是道路
    case MAP_WAY:
        //保存当前地图所有数据
        LastMapStore(gameMap,manMap);
        //人移动到下一位置
        gameMap[x_1][y_1]=walkTowardsChange(MAP_WAY,keyMark);
        //播放音效
        walkPlay();
        //更新步数
        stepRemain--;
        //更新人的位置
        man_x=x_1;
        man_y=y_1;

        //原位置是目的地
        if(oStatus==MAP_DESTINATION)
        {
            //原位置还原成目的地
            gameMap[x_0][y_0]=MAP_DESTINATION;
        }
        else
        {
            //原位置还原成道路
            gameMap[x_0][y_0]=MAP_WAY;
        }
        //记录当前地块
        oStatus=MAP_WAY;
        break;

    //下一个位置是目的地
    case MAP_DESTINATION:
        //保存当前地图所有数据
        LastMapStore(gameMap,manMap);
        //人移动到下一位置
        gameMap[x_1][y_1]=walkTowardsChange(MAP_DESTINATION,keyMark);
        //播放音效
        walkPlay();
        //更新步数
        stepRemain--;
        //更新人的位置
        man_x=x_1;
        man_y=y_1;

        //原位置是目的地
        if(oStatus==MAP_DESTINATION)
        {
            //原位置还原成目的地
            gameMap[x_0][y_0]=MAP_DESTINATION;
        }
        else
        {
            //原位置还原成道路
            gameMap[x_0][y_0]=MAP_WAY;
        }
        //记录当前地块
        oStatus=MAP_DESTINATION;
        break;


    //下一个位置是箱子
    case MAP_BOX:
        //判断下下个位置的情况
        //如果为箱子或墙
        if(gameMap[x_2][y_2]==MAP_BOX||gameMap[x_2][y_2]==MAP_BOX_FIXED||gameMap[x_2][y_2]==MAP_WALL)
        {
            //人不移动 朝向变化
            gameMap[man_x][man_y]=walkTowardsChange(oStatus,keyMark);

            break;
        }
        //如果为道路
        else if(gameMap[x_2][y_2]==MAP_WAY)
        {
            //保存当前地图所有数据
            LastMapStore(gameMap,manMap);
            //箱子移动
            gameMap[x_2][y_2]=MAP_BOX;
            //人移动
            gameMap[x_1][y_1]=walkTowardsChange(MAP_WAY,keyMark);
            //播放音效
            PushPlay();
            //更新步数
            stepRemain--;
            //更新人的位置
            man_x=x_1;
            man_y=y_1;


            //原位置是目的地
            if(oStatus==MAP_DESTINATION)
            {
                gameMap[x_0][y_0]=MAP_DESTINATION;
            }
            else
            {
                gameMap[x_0][y_0]=MAP_WAY;
            }
            //记录当前地块
            oStatus=MAP_WAY;
            break;
        }
        //如果为目的地
        else
        {
            //保存当前地图所有数据
            LastMapStore(gameMap,manMap);
            //箱子移动至目的地
            gameMap[x_2][y_2]=MAP_BOX_FIXED;
            //播放音效
            PushDesPlay();
            //人移动
            gameMap[x_1][y_1]=walkTowardsChange(MAP_WAY,keyMark);
            //更新步数
            stepRemain--;
            //更新人的位置
            man_x=x_1;
            man_y=y_1;

            //原位置是目的地
            if(oStatus==MAP_DESTINATION)
            {
                gameMap[x_0][y_0]=MAP_DESTINATION;
            }
            else
            {
                gameMap[x_0][y_0]=MAP_WAY;
            }
            //记录当前地块
            oStatus=MAP_WAY;
            break;
        }

    //下一个位置是目的地箱子(MAP_BOX_FIXED)
    case MAP_BOX_FIXED:
        //判断下下个位置的情况
        //如果为箱子或墙
        if(gameMap[x_2][y_2]==MAP_BOX||gameMap[x_2][y_2]==MAP_BOX_FIXED||gameMap[x_2][y_2]==MAP_WALL)
        {
            //人不移动 朝向变化
            gameMap[man_x][man_y]=walkTowardsChange(oStatus,keyMark);
            break;
        }
        //如果为道路
        else if(gameMap[x_2][y_2]==MAP_WAY)
        {
            //保存当前地图所有数据
            LastMapStore(gameMap,manMap);
            //箱子移动
            gameMap[x_2][y_2]=MAP_BOX;
            //人移动
            gameMap[x_1][y_1]=walkTowardsChange(MAP_DESTINATION,keyMark);
            //播放音效
            PushPlay();
            //更新步数
            stepRemain--;
            //更新人的位置
            man_x=x_1;
            man_y=y_1;

            //原位置是目的地
            if(oStatus==MAP_DESTINATION)
            {
                gameMap[x_0][y_0]=MAP_DESTINATION;
            }
            else
            {
                gameMap[x_0][y_0]=MAP_WAY;
            }
            //记录当前地块
            oStatus=MAP_DESTINATION;
            break;
        }
        //如果为目的地
        else
        {
            //保存当前地图所有数据
            LastMapStore(gameMap,manMap);
            //箱子移动至目的地
            gameMap[x_2][y_2]=MAP_BOX_FIXED;
            //人移动
            gameMap[x_1][y_1]=walkTowardsChange(MAP_DESTINATION,keyMark);
            //播放音效
            PushDesPlay();
            //更新步数
            stepRemain--;
            //更新人的位置
            man_x=x_1;
            man_y=y_1;

            //原位置是目的地
            if(oStatus==MAP_DESTINATION)
            {
                gameMap[x_0][y_0]=MAP_DESTINATION;
            }
            else
            {
                gameMap[x_0][y_0]=MAP_WAY;
            }
            //记录当前地块
            oStatus=MAP_DESTINATION;
            break;
        }
    }
}

//判断游戏结束
bool GameLoad::GameOver()
{
    int fixedNum=0;
    for(int i=0;i<MAP_WIDTH;i++)
    {
        for(int j=0;j<MAP_HEIGTHT;j++)
        {
            if(gameMap[i][j]==MAP_BOX_FIXED)
            {
                fixedNum++;
            }
        }
    }
    //规定步数内胜利
    if(fixedNum==fixedSum)
    {
        //停止计时
        timer->stop();
        isTimerStop=true;//已暂停
        isOver=true;
        isSucceed=true;
        return true;
    }
    //步数完
    else if(stepRemain==0)
    {
        //停止计时
        timer->stop();
        //考虑步数刚好为0时胜利的情况
        if(isSucceed)
        {
            isOver=true;
            return true;
        }
        else
        {
            //结束但未胜利
            isOver=true;
            isSucceed=false;

            return true;
        }
    }
    else
    {
        return false;
    }
}
GameLoad::~GameLoad()
{
    //释放内存
    delete player1;
    delete player2;
    delete player3;
    delete timer;
}

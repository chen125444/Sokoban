#ifndef MAPDATA_H
#define MAPDATA_H

#include <QMap>
#include <QVector>
#include "Config.h"
class MapData
{
public:
    MapData();
public:
    //关卡号   Qvector的二维数组用于记录地图数据
    QMap<int, QVector<QVector<int>>> mData;
    QMap<int,int> stepData;//记录每关可用步数
};

#endif // MAPDATA_H

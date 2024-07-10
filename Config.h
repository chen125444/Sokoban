#ifndef CONFIG_H
#define CONFIG_H
/********** 游戏相关配置数据 **********/
#define GAME_TITLE "推箱子游戏" //窗口标题
#define MAP_SUM 4  //地图数量
#define MAP_WIDTH 15    //横向方块数目
#define MAP_HEIGTHT 15  //纵向方块数目
#define M_BLOCK_WIDTH 60  //方块宽度
#define M_BLOCK_HEITHT 60 //方块高度
#define MAP_BACKGROUND 0 //'0' 表示背景
#define MAP_WALL 1       //'1' 表示墙
#define MAP_WAY 2        //'2' 表示道路
#define MAP_DESTINATION 3    //'3' 表示目的地
#define MAP_BOX 4        //'4' 表示箱子
#define MAP_BOX_FIXED 5  //'5' 表示箱子在目的地
#define MAP_MAN_DOWN   6  //'6' 表示人向下走
#define MAP_MAN_UP 7  //'7' 表示人向上走
#define MAP_MAN_LEFT 8  //'8' 表示人向左走
#define MAP_MAN_RIGHT 9  //'9' 表示人向右走
#define MAP_MAN_DOWN_DES   10  //'10' 表示人向下走且在目的地上
#define MAP_MAN_UP_DES 11  //'11' 表示人向上走且在目的地上
#define MAP_MAN_LEFT_DES 12  //'12' 表示人向左走且在目的地上
#define MAP_MAN_RIGHT_DES 13  //'13' 表示人向右走且在目的地上
#endif // CONFIG_H

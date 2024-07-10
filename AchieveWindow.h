#ifndef ACHIEVEWINDOW_H
#define ACHIEVEWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QMap>
#include <QLabel>
#include <QVector>
#include <QPainter>
#include "MyPushButton.h"
#include "Config.h"

namespace Ui {
class AchieveWindow;
}

class AchieveWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AchieveWindow(QWidget *parent = nullptr);
    ~AchieveWindow();

    void paintEvent(QPaintEvent *);
    //保存各关通关情况及相应数据
    bool isLevelWin[4]={false,false,false,false};

    //存储各关通关所花时间 步数
    QMap<int,QVector<int>> winData;
    //用来存不同的label 便于绘制
    QMap<QString,QLabel *> stepMap;

    QMap<QString,QLabel *> timeMap;
    //绘制相应数据
    void DrawStatus(QPainter * painter);

    //用来格式化时间
    QString TimeLoad(int timeCount);

    //加载数据 (是否通关 完成各关卡时间 步数)
    void LoadWinData();

    //清空数据
    void DeleteData();


signals:
    void BackSignal();

private:
    Ui::AchieveWindow *ui;
};

#endif // ACHIEVEWINDOW_H

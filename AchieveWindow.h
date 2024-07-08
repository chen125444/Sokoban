#ifndef ACHIEVEWINDOW_H
#define ACHIEVEWINDOW_H

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QMap>
#include <QVector>
#include <QPainter>
#include "MyPushButton.h"

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
    bool isLevel1Unlock=false;
    bool isLevel2Unlock=false;
    bool isLevel3Unlock=false;
    bool isLevel4Unlock=false;

    //存储各关通关所花时间 步数
    QMap<int,QVector<int>> winData;

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

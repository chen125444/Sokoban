#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QTimer>
#include "LevelWindow.h"
#include "AchieveWindow.h"
#include "MyPushButton.h"
#include "Setting.h"

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

    //画背景图
    void paintEvent(QPaintEvent *);

    //选关界面
    LevelWindow * levelWindow;

    //成就界面
    AchieveWindow * achieveWindow;

    //设置界面
    Setting * setting;

};
#endif // MAINMENU_H

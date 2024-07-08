#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include "LevelWindow.h"
#include "MyPushButton.h"

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

    //选关界面
    LevelWindow * levelWindow;

};
#endif // MAINMENU_H

#ifndef SETTING_H
#define SETTING_H

#include <QMainWindow>
#include <QMediaPlayer>
#include "MyPushButton.h"

namespace Ui {
class Setting;
}

class Setting : public QMainWindow
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();

    //播放背景音乐
    QMediaPlayer * player;

signals:
    void BackSignal();

private slots:
    void on_play_clicked();

    void on_stop_clicked();

    void on_volumeChange_valueChanged(int value);

private:
    Ui::Setting *ui;
};

#endif // SETTING_H

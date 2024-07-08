#include "MainMenu.h"
#include<QLabel>
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainMenu w;
    w.show();
    return a.exec();
}

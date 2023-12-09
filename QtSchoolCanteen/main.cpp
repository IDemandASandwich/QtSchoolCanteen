#include "mainWindow.h"
#include "loginWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainWindow w;
    loginWindow l;

    l.show();

    return a.exec();
}

#include <QApplication>
#include <QtGlobal>
#include <QVector>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app {argc,argv};
    qRegisterMetaType<QVector<double>>();
    MainWindow mainWindow {};
    mainWindow.resize(800,600);
    mainWindow.show();
    return app.exec();
}

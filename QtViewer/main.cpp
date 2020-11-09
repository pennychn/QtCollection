#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
		Q_INIT_RESOURCE(images); // ref **.qrc file
    MainWindow w;
    w.show();
    return a.exec();
}

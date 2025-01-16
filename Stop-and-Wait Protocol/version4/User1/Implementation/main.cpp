#include "mainwindow.h"
#include "udphandler.h"
#include "wrapper.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow window;
    window.show();
    return a.exec();
}

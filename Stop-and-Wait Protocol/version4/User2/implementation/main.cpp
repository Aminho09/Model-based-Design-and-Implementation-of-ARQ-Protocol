#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(nullptr, "127.0.0.1", 12346, "127.0.0.1", 12345);
    w.show();
    return a.exec();
}

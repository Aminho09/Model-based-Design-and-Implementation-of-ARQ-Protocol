#include "mainwindow.h"
#include "udphandler.h"
#include "wrapper.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow window(nullptr, "127.0.0.1", 12345, "127.0.0.1", 12346);
    window.show();
    return a.exec();
}

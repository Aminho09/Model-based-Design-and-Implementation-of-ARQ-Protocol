#include "mainwindow.h"
#include "udphandler.h"
#include "wrapper.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    quint16 sendPort = 12346;
    quint16 receivePort = 12345;

    if (argc >= 3) {
        sendPort = QString(argv[1]).toUShort();
        receivePort = QString(argv[2]).toUShort();
    }

    MainWindow window(nullptr, "127.0.0.1", sendPort, "127.0.0.1", receivePort);
    window.show();
    return a.exec();
}

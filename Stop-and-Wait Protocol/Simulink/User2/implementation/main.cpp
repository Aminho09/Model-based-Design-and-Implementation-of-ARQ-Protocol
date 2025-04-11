#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    quint16 sendPort = 12345;
    quint16 receivePort = 12346;

    if (argc >= 3) {
        sendPort = QString(argv[1]).toUShort();
        receivePort = QString(argv[2]).toUShort();
    }

    MainWindow w(nullptr, "127.0.0.1", sendPort, "127.0.0.1", receivePort);
    w.show();
    return a.exec();
}

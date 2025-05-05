#include "metrics.h"
#include "udphandler.h"
#include "wrapper.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    quint16 sendPort = 12346;
    quint16 receivePort = 12345;

    if (argc >= 3) {
        sendPort = QString(argv[1]).toUShort();
        receivePort = QString(argv[2]).toUShort();
    }

    Metrics window(nullptr, "127.0.0.1", sendPort, "127.0.0.1", receivePort);
    window.sendMessage();
    return a.exec();
}

#include <QCoreApplication>
#include <QQueue>
#include <QTextStream>
#include "udphandler.h"
#include "inputsource.h"
#include "wrapper.h"
#include "outputsink.h"

QQueue<uint8_t> globalQueue;

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Initialize UdpHandler with local and target settings
    UdpHandler handler(nullptr, "127.0.0.1", 12345, "127.0.0.1", 12346);
    InputSource input;
    Wrapper wrapper;
    OutputSink output;
    QTextStream qin(stdin);
    QString line;

    QObject::connect(&handler, &UdpHandler::messageReceived, &input, &InputSource::generateInput);
    QObject::connect(&input, &InputSource::inputReady, &wrapper, &Wrapper::onInputReady);
    QObject::connect(&wrapper, &Wrapper::outputsReady, &output, &OutputSink::processOutput);
    QObject::connect(&output, &OutputSink::sendtoUDP, &handler, &UdpHandler::sendMessage);

    wrapper.startModel(20);
    qDebug() << "Input your text (Type \"exit\" to close the program)";

    do{
        qDebug() << ">";
        line = qin.readLine();

        if (line.toLower() == "exit"){
            QCoreApplication::quit();
            return 0;
        }

        for (const QChar &ch : line) {
            globalQueue.enqueue(static_cast<uint8_t>(ch.unicode()));
        }
    } while (globalQueue.isEmpty());

    return a.exec();
}

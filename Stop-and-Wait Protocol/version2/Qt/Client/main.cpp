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
    QTextStream in(stdin);
    QTextStream out(stdout);

    QObject::connect(&handler, &UdpHandler::messageReceived, &input, &InputSource::generateInput);
    QObject::connect(&input, &InputSource::inputReady, &wrapper, &Wrapper::onInputReady);
    QObject::connect(&wrapper, &Wrapper::outputsReady, &output, &OutputSink::processOutput);
    QObject::connect(&output, &OutputSink::sendtoUDP, &handler, &UdpHandler::sendMessage);

    wrapper.startModel(20);

    while (true) { // Infinite loop
        out << "Input: " << flush;
        QString input = in.readLine();

        if (input == "exit") {
            out << "Exiting the program." << endl;
            QCoreApplication::quit();
            return 0;
        }

        // Enqueue each character of the input string into the globalQueue
        for (const QChar &ch : input) {
            globalQueue.enqueue(static_cast<uint8_t>(ch.unicode()));
        }
    }

    return a.exec();
}

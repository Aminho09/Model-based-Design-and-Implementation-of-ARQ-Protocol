#include <QCoreApplication>
#include <QQueue>
#include <QTextStream>
#include "udphandler.h"
#include "inputsource.h"
#include "wrapper.h"
#include "outputsink.h"
#include "consolereader.h"

QQueue<uint8_t> globalQueue;

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Initialize UdpHandler with local and target settings
    UdpHandler handler(nullptr, "127.0.0.1", 12345, "127.0.0.1", 12346);
    InputSource input;
    Wrapper wrapper;
    OutputSink output;
    ConsoleReader console;
    QTextStream qin(stdin);
    QString line;

    QObject::connect(&handler, &UdpHandler::messageReceived, &input, &InputSource::generateInput);
    QObject::connect(&input, &InputSource::inputReady, &wrapper, &Wrapper::onInputReady);
    QObject::connect(&wrapper, &Wrapper::outputsReady, &output, &OutputSink::processOutput);
    QObject::connect(&output, &OutputSink::sendtoUDP, &handler, &UdpHandler::sendMessage);
    QObject::connect(&output, &OutputSink::getInput, &console, &ConsoleReader::readInput);

    wrapper.startModel(1);

    return a.exec();
}

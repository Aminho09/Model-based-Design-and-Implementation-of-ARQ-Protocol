#include <QCoreApplication>
#include "inputsource.h"
#include "outputsink.h"
#include "wrapper.h"
#include "udphandler.h"


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Initialize UdpHandler with local and target settings
    UdpHandler handler(nullptr, "127.0.0.1", 12346, "127.0.0.1", 12345);
    InputSource input;
    Wrapper wrapper;
    OutputSink output;
    wrapper.receive_Obj.initialize();

    QObject::connect(&handler, &UdpHandler::messageReceived, &input, &InputSource::generateInput);
    QObject::connect(&input, &InputSource::inputReady, &wrapper, &Wrapper::onInputReady);
    QObject::connect(&wrapper, &Wrapper::outputsReady, &output, &OutputSink::processOutput);
    QObject::connect(&output, &OutputSink::sendtoUDP, &handler, &UdpHandler::sendMessage);

    // Run the event loop
    return app.exec();
}

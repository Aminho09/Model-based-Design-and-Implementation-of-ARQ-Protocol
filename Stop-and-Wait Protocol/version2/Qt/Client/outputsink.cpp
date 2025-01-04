#include "outputsink.h"

OutputSink::OutputSink(QObject *parent) : QObject(parent){}

void OutputSink::processOutput(uint16_t packet, bool ready) {
    if (ready)
        emit sendtoUDP(packet);
}

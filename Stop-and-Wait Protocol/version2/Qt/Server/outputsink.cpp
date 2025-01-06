#include "outputsink.h"

OutputSink::OutputSink(QObject *parent) : QObject(parent){}

void OutputSink::processOutput(uint8_t data, uint8_t ACK) {

    qDebug() << "Data validated, int:" << data << "char:" << (char)data;
    emit sendtoUDP(ACK);
}

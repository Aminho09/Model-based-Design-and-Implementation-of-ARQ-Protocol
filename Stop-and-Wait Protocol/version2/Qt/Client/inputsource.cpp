#include "inputsource.h"

InputSource::InputSource(QObject *parent) : QObject(parent){}

void InputSource::generateInput(const QString &sender, quint16 senderPort, uint8_t ACK) {
    qDebug() << qPrintable(QString("Received from %1:%2 - %3").arg(sender).arg(senderPort).arg(ACK));
    emit inputReady(ACK);
}


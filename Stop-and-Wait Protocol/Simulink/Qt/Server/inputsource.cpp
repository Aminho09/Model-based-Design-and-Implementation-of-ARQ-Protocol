#include "inputsource.h"

InputSource::InputSource(QObject *parent) : QObject(parent){}

void InputSource::generateInput(const QString &sender, quint16 senderPort, uint16_t packet) {
    qDebug() << qPrintable(QString("A packet received from %1:%2").arg(sender).arg(senderPort));
    emit inputReady(packet);
}


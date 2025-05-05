#include "metrics.h"

Metrics::Metrics(QObject *parent, const QString &localIP, int localPort,
                       const QString &targetIP, int targetPort)
    : QObject(parent), handler(nullptr, localIP, localPort, targetIP, targetPort)
{

    QObject::connect(this, &Metrics::sendData, &wrapper, &Wrapper::sendData);
    QObject::connect(&wrapper, &Wrapper::send_packet, &handler, &UdpHandler::sendPacket);
    QObject::connect(&wrapper, &Wrapper::receive_ACK, &handler, &UdpHandler::sendAck);
    QObject::connect(&wrapper, &Wrapper::receive_data, this, &Metrics::processOutputs);
    QObject::connect(this, &Metrics::reset_receiver, &wrapper, &Wrapper::reset_receiver);
    QObject::connect(&handler, &UdpHandler::ackReceived, &wrapper, &Wrapper::sendAck);
    QObject::connect(&handler, &UdpHandler::packetReceived, &wrapper, &Wrapper::receivePacket);
    QObject::connect(this, &Metrics::reset_sender, &wrapper, &Wrapper::reset_sender);
    QObject::connect(&wrapper, &Wrapper::dequeue, this, &Metrics::send_next);
    QObject::connect(&handler, &UdpHandler::countTransmissions, this, &Metrics::countTransmissions);
    QObject::connect(&wrapper, &Wrapper::countRetransmissions, this, &Metrics::countRetransmissions);

    wrapper.initialize();
}

void Metrics::sendMessage()
{
    QTextStream in(stdin);
    QTextStream out(stdout);

    out << "Enter your message: " << flush;
    QString message = in.readLine();
    qDebug() << "Performance Metrics:";

    for (QChar ch: message) {
            queue.enqueue(static_cast<uint8_t>(ch.unicode()));
    }
    queue.enqueue(static_cast<uint8_t>('\r'));
    queue.enqueue(static_cast<uint8_t>('\n'));
    uint8_t first_char = queue.dequeue();
    last_char = first_char;
    delay.start();
    string_delay.start();
    emit sendData(first_char);
}

void Metrics::send_next(){
    qint16 rtt = delay.elapsed();
    qDebug() << "data: " << static_cast<QChar>(last_char) << " - RTT: " << rtt << " ms";
    delay.restart();
    if (!queue.isEmpty()){
        last_char = queue.dequeue();
        emit sendData(last_char);
    }
    else{
        last_char = 0;
        emit reset_sender();
        calculateMetrics();
    }
    countPacket();
}

void Metrics::processOutputs(uint8_t data){
    char character = static_cast<char>(data);
    if (receivedMessage.isEmpty()){
        receivedMessage.append(character);
    }
    else if (receivedMessage.back() == '\r' && character == '\n'){
        receivedMessage.chop(1);
        emit reset_receiver();
        receivedMessage = "";
    }
    else {
        receivedMessage.append(character);
    }

}

void Metrics::countPacket(){
    sent_packets++;
}

void Metrics::countTransmissions(){
    transmissions++;
}

void Metrics::countRetransmissions(){
    retransmissions++;
}

void Metrics::calculateMetrics(){
    quint32 total_time = string_delay.elapsed();
    double retransmission_rate = (retransmissions / (double)transmissions) * 100.0;
    double throughput = (sent_packets / (double)total_time) * 1000.0;
    qDebug() << "Total Time Spent: " << total_time << "ms - Throughput: " << throughput << "Bytes/Second";
    qDebug() << "Retransmission Rate: " <<  retransmission_rate << "%";
    exit(0);
}

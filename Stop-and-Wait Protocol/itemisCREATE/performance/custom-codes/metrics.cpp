#include "metrics.h"

Metrics::Metrics(QObject *parent): QObject(parent){}

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

void Metrics::sendNext(){
    qint16 rtt = delay.elapsed();
    qDebug() << "data: " << static_cast<QChar>(last_char) << " - RTT: " << rtt << " ms";
    delay.restart();
    if (!queue.isEmpty()){
        last_char = queue.dequeue();
        emit sendData(last_char);
    }
    else{
        last_char = 0;
        emit resetSend();
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
        emit resetReceive();
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

void Metrics::countRetransmissions(uint8_t data){
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

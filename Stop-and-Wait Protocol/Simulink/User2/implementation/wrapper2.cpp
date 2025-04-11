#include <QDebug>
#include "wrapper2.h"

Wrapper::Wrapper(QObject *parent) : QObject(parent){
    connect(&timeoutTimer, &QTimer::timeout, this, &Wrapper::setTimeoutEvent);
    connect(&stepTimer, &QTimer::timeout, this, &Wrapper::onModelStep);
}

void Wrapper::storeString(const QString message){
    for (QChar ch: message) {
            queue.enqueue(static_cast<uint8_t>(ch.unicode()));
    }
    queue.enqueue(static_cast<uint8_t>('\r'));
    queue.enqueue(static_cast<uint8_t>('\n'));
    startModel(1);
}

void Wrapper::startModel(int intervalMs) {
    user_Obj.initialize();
    user_Obj.setsend_data_call(false);
    user_Obj.setsend_ACK_call(false);
    user_Obj.setreceive_packet_call(false);
    user_Obj.settimeout(false);
    user_Obj.step();
    stepTimer.start(intervalMs);
}

void Wrapper::onModelStep(){
    user_Obj.step();
//    qDebug() << user_Obj.User2_U.send_data_call;
//    qDebug() << receivedMessage;
//    qDebug() << user_Obj.User2_U.send_data;
//    qDebug() << user_Obj.User2_Y.send_packet;
    processOutputs();
}

void Wrapper::stopModel(){
    stepTimer.stop();
}

void Wrapper::setDataEvent(uint8_t data){
    data_event = true xor data_event;
    user_Obj.setsend_data(data);
    user_Obj.setsend_data_call(data_event);
}

void Wrapper::setTimeoutEvent(){
    qDebug() << "hello mf!";
    timeout_event = true xor timeout_event;
    user_Obj.settimeout(timeout_event);
}

void Wrapper::setAckEvent(uint8_t ack){
    ack_event = true xor ack_event;
    user_Obj.setsend_ACK(ack);
    user_Obj.setsend_ACK_call(ack_event);
}

void Wrapper::setPacketEvent(uint16_t packet){
    packet_event = true xor packet_event;
    user_Obj.setreceive_packet(packet);
    user_Obj.setreceive_packet_call(packet_event);
}

void Wrapper::processOutputs(){
    char character = static_cast<char>(user_Obj.getreceive_data());
    bool send_ready = user_Obj.getsend_ready();
    bool receive_ready = user_Obj.getreceive_ready();
//    qDebug() << "hi mf";
//    qDebug() << user_Obj.getdequeue();
    if (user_Obj.getdequeue() == false){
        timeoutTimer.start(4000);
        qDebug() << "mf";
    }
    else{
        timeoutTimer.stop();
        if (!queue.isEmpty())
            setDataEvent(queue.dequeue());
        else
            emit messageSent();
    }

    if (send_ready != user_Obj.getsend_ready())
        emit sendOutputReady(user_Obj.getsend_packet());

    if (receive_ready != user_Obj.getreceive_ready()){
        if (receivedMessage.isEmpty()){
            receivedMessage.append(character);
        }
        else if (receivedMessage.back() == '\r' && character == '\n'){
            receivedMessage.chop(1);
            stopModel();
            emit showMessage(receivedMessage);
            receivedMessage = "";
        }
        else {
            receivedMessage.append(character);
        }
        emit ackReady(user_Obj.getreceive_ACK());
    }
}



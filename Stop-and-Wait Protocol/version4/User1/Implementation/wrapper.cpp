#include "wrapper.h"

Wrapper::Wrapper(QObject *parent) : QObject(parent){
    connect(&timer, &QTimer::timeout, this, &Wrapper::timeoutCall);
}

void Wrapper::storeString(const QString message){
    for (QChar ch: message) {
            queue.enqueue(static_cast<uint8_t>(ch.unicode()));
    }
    queue.enqueue(static_cast<uint8_t>('\r'));
    queue.enqueue(static_cast<uint8_t>('\n'));

    uint8_t first_char = queue.dequeue();
    user_Obj.initialize();
    sendDataCall(first_char);
}

void Wrapper::sendDataCall(uint8_t data){
    bool send_ready = user_Obj.getsend_ready();

    user_Obj.setsend_data(data);
    user_Obj.send_data_call();

    if (user_Obj.getdequeue() == false)
        timer.start(4000);

    if (send_ready != user_Obj.getsend_ready())
        emit sendOutputReady(user_Obj.getsend_packet());
}

void Wrapper::timeoutCall(){
    bool send_ready = user_Obj.getsend_ready();

    user_Obj.timeout();

    if (send_ready != user_Obj.getsend_ready())
        emit sendOutputReady(user_Obj.getsend_packet());
}

void Wrapper::sendAckCall(uint8_t ack){
    user_Obj.setsend_ACK(ack);
    user_Obj.send_ACK_call();

    if (user_Obj.getdequeue() == true){
        timer.stop();
        if (!queue.isEmpty())
            sendDataCall(queue.dequeue());
        else
            user_Obj.reset_sender();
            emit messageSent();
    }
}

void Wrapper::receivePacketCall(uint16_t packet){
    bool receive_ready = user_Obj.getreceive_ready();

    user_Obj.setreceive_packet(packet);
    user_Obj.receive_packet_call();

    if (receive_ready != user_Obj.getreceive_ready())
        processOutputs(user_Obj.getreceive_data(), user_Obj.getreceive_ACK());
}

void Wrapper::processOutputs(uint8_t data, uint8_t ack){
    char character = static_cast<char>(data);
    if (receivedMessage.isEmpty()){
        receivedMessage.append(character);
    }
    else if (receivedMessage.back() == '\r' && character == '\n'){
        receivedMessage.chop(1);
        user_Obj.reset_receiver();
        emit showMessage(receivedMessage);
        receivedMessage = "";
    }
    else {
        receivedMessage.append(character);
    }
    emit ackReady(ack);
}



#include "wrapper.h"

Wrapper::Wrapper(QObject *parent) : QObject(parent){
    connect(&timeoutTimer, &QTimer::timeout, this, &Wrapper::timeout);
    connect(&stepTimer, &QTimer::timeout, this, &Wrapper::onModelStep);
}

void Wrapper::initialize(){
    user_Obj.initialize();
    user_Obj.setsend_data_call(data_event);
    user_Obj.setsend_ACK(ack_event);
    user_Obj.setreceive_packet_call(packet_event);
    user_Obj.settimeout(timeout_event);
    user_Obj.setreset_sender(reset_send_event);
    user_Obj.setreset_receiver(reset_receive_event);
    user_Obj.step();
    startModel(1);
}

void Wrapper::startModel(int intervalMS){
    stepTimer.start(intervalMS);
}

void Wrapper::onModelStep(){
    bool send_ready = user_Obj.getsend_ready();
    bool receive_ready = user_Obj.getreceive_ready();
    user_Obj.step();
    processOutputs(send_ready != user_Obj.getsend_ready(),
                   receive_ready != user_Obj.getreceive_ready(),
                   user_Obj.getdequeue());
}

void Wrapper::storeString(const QString message){
    for (QChar ch: message) {
            queue.enqueue(static_cast<uint8_t>(ch.unicode()));
    }
    queue.enqueue(static_cast<uint8_t>('\r'));
    queue.enqueue(static_cast<uint8_t>('\n'));

    if (!initial_flag){
        initialize();
        initial_flag = true;
    }
}

void Wrapper::sendData(uint8_t data){
    data_event = data_event xor true;
    user_Obj.setsend_data(data);
    user_Obj.setsend_data_call(data_event);
}

void Wrapper::sendAck(uint8_t ack){
    ack_event = ack_event xor true;
    user_Obj.setsend_ACK(ack);
    user_Obj.setsend_ACK_call(ack_event);
}

void Wrapper::receivePacket(uint16_t packet){
    packet_event = packet_event xor true;
    user_Obj.setreceive_packet(packet);
    user_Obj.setreceive_packet_call(packet_event);
}

void Wrapper::timeout(){
    timeout_event = timeout_event xor true;
    user_Obj.settimeout(timeout_event);
}

void Wrapper::resetSender(){
    reset_send_event = reset_send_event xor true;
    user_Obj.setreset_sender(reset_send_event);
}

void Wrapper::resetReceiver(){
    reset_receive_event = reset_receive_event xor true;
    user_Obj.setreset_receiver(reset_receive_event);
}

void Wrapper::processOutputs(bool send_ready, bool receive_ready, bool dequeue){
    char character = static_cast<char>(user_Obj.getreceive_data());
    if (send_ready)
        emit sendOutputReady(user_Obj.getsend_packet());

    if (receive_ready){
        if (receivedMessage.isEmpty()){
            receivedMessage.append(character);
        }
        else if (receivedMessage.back() == '\r' && character == '\n'){
            receivedMessage.chop(1);
            resetReceiver();
            emit showMessage(receivedMessage);
            receivedMessage = "";
        }
        else {
            receivedMessage.append(character);
        }
        emit ackReady(user_Obj.getreceive_ACK());
    }

    if (!dequeue){
        timeoutTimer.start(4000);
    }
    else{
        timeoutTimer.stop();
        if (!queue.isEmpty())
            sendData(queue.dequeue());
        else
            resetSender();
            emit messageSent();
    }

}

#include "wrapper.h"

Wrapper::Wrapper(QObject *parent) : QObject(parent){
    connect(&stepTimer, &QTimer::timeout, this, &Wrapper::onModelStep);
}

void Wrapper::initialize(int intervalMS){
    user_Obj.initialize();
    user_Obj.setsend_data_call(data_event);
    user_Obj.setsend_ACK(ack_event);
    user_Obj.setreceive_packet_call(packet_event);
    user_Obj.setreset_sender(reset_send_event);
    user_Obj.setreset_receiver(reset_receive_event);
    user_Obj.step();
    stepTimer.start(intervalMS);
}

void Wrapper::onModelStep(){
    bool send_ready = user_Obj.getsend_ready();
    bool receive_ready = user_Obj.getreceive_ready();
    bool dequeue = user_Obj.getdequeue();
    user_Obj.step();
    processOutputs(send_ready != user_Obj.getsend_ready(),
                   receive_ready != user_Obj.getreceive_ready(),
                   dequeue != user_Obj.getdequeue());
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

void Wrapper::resetSender(){
    reset_send_event = reset_send_event xor true;
    user_Obj.setreset_sender(reset_send_event);
}

void Wrapper::resetReceiver(){
    reset_receive_event = reset_receive_event xor true;
    user_Obj.setreset_receiver(reset_receive_event);
}

void Wrapper::processOutputs(bool send_ready, bool receive_ready, bool dequeue_char){
    if (send_ready)
        emit sendPacket(user_Obj.getsend_packet());

    if (receive_ready){
        emit receiveData(user_Obj.getreceive_data());
        emit receiveAck(user_Obj.getreceive_ACK());
    }

    if (dequeue_char)
        emit dequeue();
}

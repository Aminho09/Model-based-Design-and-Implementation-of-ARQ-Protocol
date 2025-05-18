#include "wrapper.h"

Wrapper::Wrapper(QObject *parent) : QObject(parent){
    connect(&timer, &QTimer::timeout, this, &Wrapper::timeout);
}

void Wrapper::initialize(){
    user_Obj.initialize();
}

void Wrapper::sendData(uint8_t data){
    bool send_ready = user_Obj.getsend_ready();

    user_Obj.setsend_data(data);
    user_Obj.send_data_call();

    if (user_Obj.getdequeue() == false)
        timer.start(4000);

    if (send_ready != user_Obj.getsend_ready())
        emit send_packet(user_Obj.getsend_packet());
}

void Wrapper::timeout(){
    bool send_ready = user_Obj.getsend_ready();

    user_Obj.timeout();

    if (send_ready != user_Obj.getsend_ready()){
        emit send_packet(user_Obj.getsend_packet());
        emit countRetransmissions();
    }
}

void Wrapper::sendAck(uint8_t ack){
    user_Obj.setsend_ACK(ack);
    user_Obj.send_ACK_call();

    if (user_Obj.getdequeue() == true){
        timer.stop();
        emit dequeue();
    }
}

void Wrapper::receivePacket(uint16_t packet){
    bool receive_ready = user_Obj.getreceive_ready();

    user_Obj.setreceive_packet(packet);
    user_Obj.receive_packet_call();

    if (receive_ready != user_Obj.getreceive_ready()) {
        emit receive_ACK(user_Obj.getreceive_ACK());
        emit receive_data(user_Obj.getreceive_data());
    }
}

void Wrapper::reset_sender(){
    user_Obj.reset_sender();
}

void Wrapper::reset_receiver(){
    user_Obj.reset_receiver();
}



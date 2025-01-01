//
// File: send.cpp
//
// Code generated for Simulink model 'send'.
//
// Model version                  : 1.24
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Wed Jan  1 12:46:21 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "send.h"
#include <stdint.h>
#include <stdbool.h>
#include "rt_roundf_snf.h"

// Named constants for Chart: '<Root>/Chart1'
const uint8_t send_IN_Idle{ 1U };

const uint8_t send_IN_Send_packet{ 2U };

const uint8_t send_IN_Wait_for_ack{ 3U };

// user code (top of source file)
extern QQueue<uint8_t> globalQueue;

// Output and update for Simulink Function: '<Root>/Simulink Function'
void send::pop(uint8_t *rty_out)
{
  // CFunction: '<S2>/dequeuing'
  send_B.dequeuing = globalQueue.dequeue();

  // SignalConversion generated from: '<S2>/out'
  *rty_out = send_B.dequeuing;
}

// Output and update for Simulink Function: '<Root>/Simulink Function1'
void send::is_empty(bool *rty_f)
{
  // CFunction: '<S3>/emp'
  if (globalQueue.isEmpty())
    send_B.emp = true;
  else
    send_B.emp = false;

  // SignalConversion generated from: '<S3>/f'
  *rty_f = send_B.emp;
}

// Function for Chart: '<Root>/Chart1'
uint8_t send::send_ack_crc(uint8_t b_value)
{
  uint8_t crc;
  crc = 0U;
  for (int32_t i{0}; i < 12; i++) {
    uint8_t tmp;
    if (i - 7 >= 0) {
      tmp = static_cast<uint8_t>(b_value << static_cast<uint8_t>(-(7.0 -
        static_cast<double>(i))));
    } else {
      tmp = static_cast<uint8_t>(b_value >> static_cast<uint8_t>(7U -
        static_cast<uint32_t>(i)));
    }

    crc = static_cast<uint8_t>(static_cast<int32_t>(tmp & 1U) ^ crc);
    if ((crc & 16U) != 0U) {
      crc = static_cast<uint8_t>(crc ^ 19);
    }

    if (7 - i != -4) {
      crc = static_cast<uint8_t>(crc << 1);
    }
  }

  return static_cast<uint8_t>(crc & 15);
}

// Function for Chart: '<Root>/Chart1'
bool send::send_checkACK(uint8_t ack, uint8_t check)
{
  uint8_t ack_value;
  ack_value = static_cast<uint8_t>(ack & 15);
  return ack >> 4 == send_ack_crc(ack_value) && ack_value == check;
}

// Function for Chart: '<Root>/Chart1'
uint8_t send::send_crc4(uint8_t b_value, float t)
{
  float tmp;
  int32_t i;
  uint16_t crc;
  uint16_t input;
  uint16_t tmp_0;
  uint8_t header;
  crc = 0U;
  tmp = rt_roundf_snf(t);
  if (tmp < 65536.0F) {
    if (tmp >= 0.0F) {
      tmp_0 = static_cast<uint16_t>(tmp);
    } else {
      tmp_0 = 0U;
    }
  } else {
    tmp_0 = UINT16_MAX;
  }

  input = static_cast<uint16_t>(static_cast<uint16_t>(tmp_0 << 8) | b_value);
  for (i = 0; i < 13; i++) {
    if (i - 8 >= 0) {
      tmp_0 = static_cast<uint16_t>(input << static_cast<uint8_t>(-(8.0 -
        static_cast<double>(i))));
    } else {
      tmp_0 = static_cast<uint16_t>(input >> static_cast<uint8_t>(8U -
        static_cast<uint32_t>(i)));
    }

    crc = static_cast<uint16_t>(static_cast<int32_t>(tmp_0 & 1U) ^ crc);
    if ((crc & 16U) != 0U) {
      crc = static_cast<uint16_t>(crc ^ 19);
    }

    if (8 - i != -4) {
      crc = static_cast<uint16_t>(crc << 1);
    }
  }

  header = static_cast<uint8_t>(crc & 15U);
  return header;
}

// Function for Chart: '<Root>/Chart1'
uint16_t send::send_calculation(uint8_t input, float c_tag)
{
  float tmp;
  uint16_t output;
  uint16_t tmp_0;
  tmp = rt_roundf_snf(c_tag);
  if (tmp < 65536.0F) {
    if (tmp >= 0.0F) {
      tmp_0 = static_cast<uint16_t>(tmp);
    } else {
      tmp_0 = 0U;
    }
  } else {
    tmp_0 = UINT16_MAX;
  }

  output = static_cast<uint16_t>(static_cast<uint16_t>(tmp_0 << 8) | input |
    static_cast<uint16_t>(send_crc4(input, c_tag) << 9));
  return output;
}

// Function for Chart: '<Root>/Chart1'
uint8_t send::send_reset_ACK(uint8_t ca)
{
  uint8_t new_ca;
  if (ca == 255) {
    new_ca = 0U;
  } else {
    new_ca = static_cast<uint8_t>(ca + 1);
  }

  return new_ca;
}

// Model step function
void send::step()
{
  bool c;

  // Chart: '<Root>/Chart1' incorporates:
  //   Inport: '<Root>/ACK'

  if (send_DW.isNotInit && send_DW.temporalCounter_i1 < 255U) {
    send_DW.temporalCounter_i1 = static_cast<uint8_t>(send_DW.temporalCounter_i1
      + 1);
  }

  send_DW.isNotInit = true;
  switch (send_DW.is_c2_send) {
   case send_IN_Idle:
    is_empty(&c);
    if (!c) {
      pop(&send_DW.data);

      // Outport: '<Root>/packet'
      send_Y.packet = send_calculation(send_DW.data, send_DW.tag);

      // Outport: '<Root>/ready'
      send_Y.ready = true;
      send_DW.c_ACK = send_reset_ACK(send_DW.c_ACK);
      send_DW.tag = 1.0F - send_DW.tag;
      send_DW.is_c2_send = send_IN_Send_packet;
    }
    break;

   case send_IN_Send_packet:
    if (send_checkACK(send_U.ACK, send_DW.c_ACK)) {
      // Outport: '<Root>/ready'
      send_Y.ready = false;
      send_DW.is_c2_send = send_IN_Idle;
    } else {
      // Outport: '<Root>/ready'
      send_Y.ready = false;
      send_DW.temporalCounter_i1 = 0U;
      send_DW.is_c2_send = send_IN_Wait_for_ack;
    }
    break;

   default:
    // case IN_Wait_for_ack:
    if (send_checkACK(send_U.ACK, send_DW.c_ACK)) {
      send_DW.is_c2_send = send_IN_Idle;
    } else if (send_DW.temporalCounter_i1 >= 200) {
      // Outport: '<Root>/packet'
      send_Y.packet = send_calculation(send_DW.data, send_DW.tag);
      send_DW.c_ACK = send_reset_ACK(send_DW.c_ACK);
      send_DW.tag = 1.0F - send_DW.tag;

      // Outport: '<Root>/ready'
      send_Y.ready = true;
      send_DW.is_c2_send = send_IN_Send_packet;
    }
    break;
  }

  // End of Chart: '<Root>/Chart1'
}

// Model initialize function
void send::initialize()
{
  // SystemInitialize for Chart: '<Root>/Chart1'
  send_DW.c_ACK = 1U;
  send_DW.tag = 1.0F;

  // Chart: '<Root>/Chart1'
  send_DW.is_c2_send = send_IN_Idle;
}

// Model terminate function
void send::terminate()
{
  // (no terminate code required)
}

// Root inport: '<Root>/ACK' set method
void send::setACK(uint8_t localArgInput)
{
  send_U.ACK = localArgInput;
}

// Root outport: '<Root>/packet' get method
uint16_t send::getpacket() const
{
  return send_Y.packet;
}

// Root outport: '<Root>/ready' get method
bool send::getready() const
{
  return send_Y.ready;
}

const char* send::RT_MODEL_send_T::getErrorStatus() const
{
  return (errorStatus);
}

void send::RT_MODEL_send_T::setErrorStatus(const char* const volatile
  aErrorStatus)
{
  (errorStatus = aErrorStatus);
}

// Constructor
send::send() :
  send_U(),
  send_Y(),
  send_B(),
  send_DW(),
  send_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
send::~send() = default;

// Real-Time Model get method
send::RT_MODEL_send_T * send::getRTM()
{
  return (&send_M);
}

//
// File trailer for generated code.
//
// [EOF]
//

//
// File: receive.cpp
//
// Code generated for Simulink model 'receive'.
//
// Model version                  : 1.11
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Sun Jan  5 22:44:52 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "receive.h"
#include <stdint.h>
#include <stdbool.h>
#include <cmath>

extern float rt_roundf(float u);
float rt_roundf(float u)
{
  float y;
  if (std::abs(u) < 8.388608E+6F) {
    if (u >= 0.5F) {
      y = std::floor(u + 0.5F);
    } else if (u > -0.5F) {
      y = 0.0F;
    } else {
      y = std::ceil(u - 0.5F);
    }
  } else {
    y = u;
  }

  return y;
}

// Function for Chart: '<Root>/Chart'
uint8_t receive::receive_crc4(uint8_t b_value, float crc_tag)
{
  float tmp;
  int32_t i;
  uint16_t crc;
  uint16_t input;
  uint16_t tmp_0;
  uint8_t header;
  crc = 0U;
  tmp = rt_roundf(crc_tag);
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

// Function for Chart: '<Root>/Chart'
bool receive::receive_check_packet(uint16_t p, float t)
{
  bool f;
  f = ((receive_crc4(static_cast<uint8_t>(p & 255U), receive_DW.tag) == p >> 9) &&
       (((p & 256U) != 0U) == t));
  return f;
}

// Function for Chart: '<Root>/Chart'
uint8_t receive::receive_ack_crc(uint8_t b_value)
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

// Function for Chart: '<Root>/Chart'
void receive::receive_send_data_ack(uint16_t p, uint8_t c, uint8_t *d, uint8_t
  *a, uint8_t *nc)
{
  int32_t tmp;
  int32_t tmp_0;
  *d = static_cast<uint8_t>(p & 255U);
  if (c == 16) {
    *nc = 0U;
  } else {
    tmp_0 = static_cast<int32_t>(c + 1U);
    if (c + 1U > 255U) {
      tmp_0 = 255;
    }

    *nc = static_cast<uint8_t>(tmp_0);
  }

  tmp_0 = static_cast<int32_t>(c + 1U);
  if (c + 1U > 255U) {
    tmp_0 = 255;
  }

  tmp = static_cast<int32_t>(c + 1U);
  if (c + 1U > 255U) {
    tmp = 255;
  }

  *a = static_cast<uint8_t>(static_cast<uint8_t>(receive_ack_crc
    (static_cast<uint8_t>(tmp)) << 4) | tmp_0);
}

// Model step function
void receive::call()
{
  uint8_t tmp;
  bool out;

  // RootInportFunctionCallGenerator generated from: '<Root>/call' incorporates:
  //   Chart: '<Root>/Chart'

  // Chart: '<Root>/Chart' incorporates:
  //   Inport: '<Root>/packet'
  //   Outport: '<Root>/ACK'
  //   Outport: '<Root>/data'

  out = receive_check_packet(receive_U.packet, receive_DW.tag);
  if (out) {
    receive_send_data_ack(receive_U.packet, receive_DW.c_ack, &receive_Y.data,
                          &receive_Y.ACK, &tmp);
    receive_DW.c_ack = tmp;
    receive_DW.tag = 1.0F - receive_DW.tag;
    receive_DW.readyEventCounter++;
  }

  if (receive_DW.readyEventCounter > 0U) {
    // Outport: '<Root>/ready'
    receive_Y.ready = !receive_Y.ready;
    receive_DW.readyEventCounter--;
  }

  // End of Chart: '<Root>/Chart'
  // End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/call' 
}

// Model initialize function
void receive::initialize()
{
  // SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/call' incorporates:
  //   Chart: '<Root>/Chart'

  // SystemInitialize for Chart: '<Root>/Chart'
  receive_DW.tag = 1.0F;

  // End of SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/call' 
}

// Model terminate function
void receive::terminate()
{
  // (no terminate code required)
}

// Root inport: '<Root>/packet' set method
void receive::setpacket(uint16_t localArgInput)
{
  receive_U.packet = localArgInput;
}

// Root outport: '<Root>/data' get method
uint8_t receive::getdata() const
{
  return receive_Y.data;
}

// Root outport: '<Root>/ACK' get method
uint8_t receive::getACK() const
{
  return receive_Y.ACK;
}

// Root outport: '<Root>/ready' get method
bool receive::getready() const
{
  return receive_Y.ready;
}

const char* receive::RT_MODEL_receive_T::getErrorStatus() const
{
  return (errorStatus);
}

void receive::RT_MODEL_receive_T::setErrorStatus(const char* const volatile
  aErrorStatus)
{
  (errorStatus = aErrorStatus);
}

// Constructor
receive::receive() :
  receive_U(),
  receive_Y(),
  receive_DW(),
  receive_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
receive::~receive() = default;

// Real-Time Model get method
receive::RT_MODEL_receive_T * receive::getRTM()
{
  return (&receive_M);
}

//
// File trailer for generated code.
//
// [EOF]
//

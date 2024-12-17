
//
// File: receive.cpp
//
// Code generated for Simulink model 'receive'.
//
// Model version                  : 1.2
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Tue Dec 17 23:36:39 2024
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "receive.h"
#include <stdint.h>

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
  *d = static_cast<uint8_t>(p & 17U);
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

  // RootInportFunctionCallGenerator generated from: '<Root>/call' incorporates:
  //   Chart: '<Root>/Chart'

  // Chart: '<Root>/Chart' incorporates:
  //   Inport: '<Root>/packet'
  //   Outport: '<Root>/ACK'
  //   Outport: '<Root>/data'

  receive_send_data_ack(receive_U.packet, receive_DW.c_ack, &receive_Y.data,
                        &receive_Y.ACK, &tmp);
  receive_DW.c_ack = tmp;
  receive_DW.tag = 1.0F - receive_DW.tag;

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

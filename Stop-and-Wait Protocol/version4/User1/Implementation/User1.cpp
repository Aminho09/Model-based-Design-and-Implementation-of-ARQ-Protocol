//
// File: User1.cpp
//
// Code generated for Simulink model 'User1'.
//
// Model version                  : 1.17
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Sun Jan 26 14:47:16 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "User1.h"
#include <stdint.h>
#include <stdbool.h>
#include <cmath>

// Named constants for Chart: '<Root>/sw'
const uint8_t User1_IN_Idle{ 1U };

const uint8_t User1_IN_Wait_for_ack{ 2U };

const int32_t User1_event_receive_packet_call{ 191 };

const int32_t User1_event_reset_receiver{ 199 };

const int32_t User1_event_reset_sender{ 197 };

const int32_t User1_event_send_ACK_call{ 192 };

const int32_t User1_event_send_data_call{ 193 };

const int32_t User1_event_timeout{ 185 };

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

// Function for Chart: '<Root>/sw'
uint8_t User1::User1_crc4(uint8_t b_value, float t)
{
  float tmp;
  int32_t i;
  uint16_t crc;
  uint16_t input;
  uint16_t tmp_0;
  uint8_t header;
  crc = 0U;
  tmp = rt_roundf(t);
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

// Function for Chart: '<Root>/sw'
uint16_t User1::User1_calculation(uint8_t input, float c_tag)
{
  float tmp;
  uint16_t output;
  uint16_t tmp_0;
  tmp = rt_roundf(c_tag);
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
    static_cast<uint16_t>(User1_crc4(input, c_tag) << 9));
  return output;
}

// Function for Chart: '<Root>/sw'
uint8_t User1::User1_ack_crc(uint8_t b_value)
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

// Function for Chart: '<Root>/sw'
bool User1::User1_checkACK(uint8_t ack, uint8_t check)
{
  uint8_t ack_value;
  bool f;
  ack_value = static_cast<uint8_t>(ack & 15);
  f = ((ack >> 4 == User1_ack_crc(ack_value)) && (ack_value == check));
  return f;
}

// Function for Chart: '<Root>/sw'
bool User1::User1_check_packet(uint16_t p, float t)
{
  bool f;
  f = ((User1_crc4(static_cast<uint8_t>(p & 255U), User1_DW.receive_tag) == p >>
        9) && (((p & 256U) != 0U) == t));
  return f;
}

// Function for Chart: '<Root>/sw'
uint8_t User1::User1_reset_ACK(uint8_t ca)
{
  int32_t tmp;
  uint8_t new_ca;
  tmp = static_cast<int32_t>(ca + 1U);
  if (ca + 1U > 255U) {
    tmp = 255;
  }

  new_ca = static_cast<uint8_t>(tmp);
  if (static_cast<uint8_t>(tmp) == 16) {
    new_ca = 0U;
  }

  return new_ca;
}

// Function for Chart: '<Root>/sw'
void User1::User1_send_data_ack(uint16_t p, uint8_t c, uint8_t *d, uint8_t *a,
  uint8_t *nc)
{
  int32_t tmp;
  *d = static_cast<uint8_t>(p & 255U);
  *a = static_cast<uint8_t>(static_cast<uint8_t>(User1_ack_crc(c) << 4) | c);
  if (c == 15) {
    *nc = 0U;
  } else {
    tmp = static_cast<int32_t>(c + 1U);
    if (c + 1U > 255U) {
      tmp = 255;
    }

    *nc = static_cast<uint8_t>(tmp);
  }
}

// Function for Chart: '<Root>/sw'
void User1::User1_chartstep_c2_User1(const int32_t *sfEvent)
{
  uint8_t tmp;
  bool out;
  if (User1_DW.is_send == User1_IN_Idle) {
    switch (*sfEvent) {
     case User1_event_send_data_call:
      // InitializeConditions for Outport: '<Root>/send_packet' incorporates:
      //   Inport: '<Root>/send_data'

      User1_Y.send_packet = User1_calculation(User1_U.send_data,
        User1_DW.send_tag);
      User1_DW.send_readyEventCounter++;

      // InitializeConditions for Outport: '<Root>/dequeue'
      User1_Y.dequeue = false;
      User1_DW.is_send = User1_IN_Wait_for_ack;
      break;

     case User1_event_reset_sender:
      User1_DW.c_ACK = 0U;
      User1_DW.send_tag = 1.0F;
      break;
    }
  } else {
    // case IN_Wait_for_ack:
    if (*sfEvent == User1_event_send_ACK_call) {
      // Inport: '<Root>/send_ACK'
      out = User1_checkACK(User1_U.send_ACK, User1_DW.c_ACK);
    } else {
      out = false;
    }

    if (out) {
      User1_DW.c_ACK = User1_reset_ACK(User1_DW.c_ACK);
      User1_DW.send_tag = 1.0F - User1_DW.send_tag;

      // InitializeConditions for Outport: '<Root>/dequeue'
      User1_Y.dequeue = true;
      User1_DW.is_send = User1_IN_Idle;
    } else if (*sfEvent == User1_event_timeout) {
      // InitializeConditions for Outport: '<Root>/send_packet' incorporates:
      //   Inport: '<Root>/send_data'

      User1_Y.send_packet = User1_calculation(User1_U.send_data,
        User1_DW.send_tag);
      User1_DW.send_readyEventCounter++;
      User1_DW.is_send = User1_IN_Wait_for_ack;
    }
  }

  if (*sfEvent == User1_event_receive_packet_call) {
    // Inport: '<Root>/receive_packet'
    out = User1_check_packet(User1_U.receive_packet, User1_DW.receive_tag);
  } else {
    out = false;
  }

  if (out) {
    // Inport: '<Root>/receive_packet' incorporates:
    //   Outport: '<Root>/receive_ACK'
    //   Outport: '<Root>/receive_data'

    User1_send_data_ack(User1_U.receive_packet, User1_DW.receive_c_ack,
                        &User1_Y.receive_data, &User1_Y.receive_ACK, &tmp);
    User1_DW.receive_c_ack = tmp;
    User1_DW.receive_tag = 1.0F - User1_DW.receive_tag;
    User1_DW.receive_readyEventCounter++;
  } else if (*sfEvent == User1_event_reset_receiver) {
    User1_DW.receive_c_ack = 0U;
    User1_DW.receive_tag = 1.0F;
  }
}

// System initialize for function-call system: '<Root>/sw'
void User1::User1_sw_Init()
{
  User1_DW.send_tag = 1.0F;
  User1_DW.receive_tag = 1.0F;

  // Chart: '<Root>/sw'
  User1_DW.is_send = User1_IN_Idle;
}

// Output and update for function-call system: '<Root>/sw'
void User1::User1_sw(int32_t controlPortIdx)
{
  int32_t i;
  int32_t sfEvent;
  int8_t rtb_inputevents[6];

  // Chart: '<Root>/sw' incorporates:
  //   TriggerPort: '<S1>/input events'

  for (i = 0; i < 6; i++) {
    rtb_inputevents[i] = 0;
  }

  rtb_inputevents[controlPortIdx] = 2;
  i = 0;
  if (rtb_inputevents[0U] == 2) {
    i = 1;
    sfEvent = User1_event_send_data_call;
    User1_chartstep_c2_User1(&sfEvent);
  }

  if (rtb_inputevents[1U] == 2) {
    i = 1;
    sfEvent = User1_event_send_ACK_call;
    User1_chartstep_c2_User1(&sfEvent);
  }

  if (rtb_inputevents[2U] == 2) {
    i = 1;
    sfEvent = User1_event_receive_packet_call;
    User1_chartstep_c2_User1(&sfEvent);
  }

  if (rtb_inputevents[3U] == 2) {
    i = 1;
    sfEvent = User1_event_timeout;
    User1_chartstep_c2_User1(&sfEvent);
  }

  if (rtb_inputevents[4U] == 2) {
    i = 1;
    sfEvent = User1_event_reset_sender;
    User1_chartstep_c2_User1(&sfEvent);
  }

  if (rtb_inputevents[5U] == 2) {
    i = 1;
    sfEvent = User1_event_reset_receiver;
    User1_chartstep_c2_User1(&sfEvent);
  }

  if ((i != 0) && (User1_DW.send_readyEventCounter > 0U)) {
    // Outport: '<Root>/send_ready'
    User1_Y.send_ready = !User1_Y.send_ready;
    User1_DW.send_readyEventCounter--;
  }

  if ((i != 0) && (User1_DW.receive_readyEventCounter > 0U)) {
    // Outport: '<Root>/receive_ready'
    User1_Y.receive_ready = !User1_Y.receive_ready;
    User1_DW.receive_readyEventCounter--;
  }
}

// Model step function
void User1::receive_packet_call()
{
  // Chart: '<Root>/sw'

  // RootInportFunctionCallGenerator generated from: '<Root>/receive_packet_call' 

  // Chart: '<Root>/sw'
  User1_sw(2);

  // End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/receive_packet_call' 
}

// Model step function
void User1::reset_receiver()
{
  // Chart: '<Root>/sw'

  // RootInportFunctionCallGenerator generated from: '<Root>/reset_receiver'

  // Chart: '<Root>/sw'
  User1_sw(5);

  // End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/reset_receiver' 
}

// Model step function
void User1::reset_sender()
{
  // Chart: '<Root>/sw'

  // RootInportFunctionCallGenerator generated from: '<Root>/reset_sender'

  // Chart: '<Root>/sw'
  User1_sw(4);

  // End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/reset_sender' 
}

// Model step function
void User1::send_ACK_call()
{
  // Chart: '<Root>/sw'

  // RootInportFunctionCallGenerator generated from: '<Root>/send_ACK_call'

  // Chart: '<Root>/sw'
  User1_sw(1);

  // End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/send_ACK_call' 
}

// Model step function
void User1::send_data_call()
{
  // RootInportFunctionCallGenerator generated from: '<Root>/send_data_call' incorporates:
  //   Chart: '<Root>/sw'


  // Chart: '<Root>/sw'
  User1_sw(0);

  // End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/send_data_call' 
}

// Model step function
void User1::timeout()
{
  // Chart: '<Root>/sw'

  // RootInportFunctionCallGenerator generated from: '<Root>/timeout'

  // Chart: '<Root>/sw'
  User1_sw(3);

  // End of Outputs for RootInportFunctionCallGenerator generated from: '<Root>/timeout' 
}

// Model initialize function
void User1::initialize()
{
  // SystemInitialize for Chart: '<Root>/sw'

  // SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/reset_receiver' 

  // SystemInitialize for Chart: '<Root>/sw'
  User1_sw_Init();

  // End of SystemInitialize for RootInportFunctionCallGenerator generated from: '<Root>/reset_receiver' 
}

// Model terminate function
void User1::terminate()
{
  // (no terminate code required)
}

// Root inport: '<Root>/send_ACK' set method
void User1::setsend_ACK(uint8_t localArgInput)
{
  User1_U.send_ACK = localArgInput;
}

// Root inport: '<Root>/send_data' set method
void User1::setsend_data(uint8_t localArgInput)
{
  User1_U.send_data = localArgInput;
}

// Root inport: '<Root>/receive_packet' set method
void User1::setreceive_packet(uint16_t localArgInput)
{
  User1_U.receive_packet = localArgInput;
}

// Root outport: '<Root>/send_packet' get method
uint16_t User1::getsend_packet() const
{
  return User1_Y.send_packet;
}

// Root outport: '<Root>/receive_data' get method
uint8_t User1::getreceive_data() const
{
  return User1_Y.receive_data;
}

// Root outport: '<Root>/receive_ACK' get method
uint8_t User1::getreceive_ACK() const
{
  return User1_Y.receive_ACK;
}

// Root outport: '<Root>/receive_ready' get method
bool User1::getreceive_ready() const
{
  return User1_Y.receive_ready;
}

// Root outport: '<Root>/send_ready' get method
bool User1::getsend_ready() const
{
  return User1_Y.send_ready;
}

// Root outport: '<Root>/dequeue' get method
bool User1::getdequeue() const
{
  return User1_Y.dequeue;
}

// Constructor
User1::User1() :
  User1_U(),
  User1_Y(),
  User1_DW(),
  User1_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
User1::~User1() = default;

// Real-Time Model get method
RT_MODEL_User1_T * User1::getRTM()
{
  return (&User1_M);
}

const char* RT_MODEL_User1_T::getErrorStatus() const
{
  return (errorStatus);
}

void RT_MODEL_User1_T::setErrorStatus(const char* const volatile aErrorStatus)
{
  (errorStatus = aErrorStatus);
}

//
// File trailer for generated code.
//
// [EOF]
//

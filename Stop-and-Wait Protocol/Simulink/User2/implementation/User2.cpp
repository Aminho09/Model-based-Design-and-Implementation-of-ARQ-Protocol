//
// File: User2.cpp
//
// Code generated for Simulink model 'User2'.
//
// Model version                  : 1.11
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Sun Jan 26 17:41:32 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "User2.h"
#include <stdint.h>
#include <stdbool.h>
#include "zero_crossing_types.h"
#include <cmath>

// Named constants for Chart: '<Root>/sw'
const uint8_t User2_IN_Idle{ 1U };

const uint8_t User2_IN_Wait_for_ack{ 2U };

const int32_t User2_event_receive_packet_call{ 191 };

const int32_t User2_event_reset_receiver{ 199 };

const int32_t User2_event_reset_sender{ 197 };

const int32_t User2_event_send_ACK_call{ 192 };

const int32_t User2_event_send_data_call{ 193 };

const int32_t User2_event_timeout{ 185 };

extern float rt_roundf(float u);

// Function for Chart: '<Root>/sw'
uint8_t User2::User2_ack_crc(uint8_t b_value)
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
bool User2::User2_checkACK(uint8_t ack, uint8_t check)
{
  uint8_t ack_value;
  ack_value = static_cast<uint8_t>(ack & 15);
  return (ack >> 4 == User2_ack_crc(ack_value)) && (ack_value == check);
}

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
uint8_t User2::User2_crc4(uint8_t b_value, float t)
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
uint16_t User2::User2_calculation(uint8_t input, float c_tag)
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
    static_cast<uint16_t>(User2_crc4(input, c_tag) << 9));
  return output;
}

// Function for Chart: '<Root>/sw'
uint8_t User2::User2_reset_ACK(uint8_t ca)
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
bool User2::User2_check_packet(uint16_t p, float t)
{
  bool f;
  f = ((User2_crc4(static_cast<uint8_t>(p & 255U), User2_DW.receive_tag) == p >>
        9) && (((p & 256U) != 0U) == t));
  return f;
}

// Function for Chart: '<Root>/sw'
void User2::User2_send_data_ack(uint16_t p, uint8_t c, uint8_t *d, uint8_t *a,
  uint8_t *nc)
{
  int32_t tmp;
  *d = static_cast<uint8_t>(p & 255U);
  *a = static_cast<uint8_t>(static_cast<uint8_t>(User2_ack_crc(c) << 4) | c);
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
void User2::User2_chartstep_c2_User2(const int32_t *sfEvent)
{
  uint8_t tmp;
  bool out;
  if (User2_DW.is_send == User2_IN_Idle) {
    switch (*sfEvent) {
     case User2_event_send_data_call:
      // Outport: '<Root>/send_packet' incorporates:
      //   Inport: '<Root>/send_data'

      User2_Y.send_packet = User2_calculation(User2_U.send_data,
        User2_DW.send_tag);
      User2_DW.send_readyEventCounter++;

      // Outport: '<Root>/dequeue'
      User2_Y.dequeue = false;
      User2_DW.is_send = User2_IN_Wait_for_ack;
      break;

     case User2_event_reset_sender:
      User2_DW.c_ACK = 0U;
      User2_DW.send_tag = 1.0F;
      break;
    }
  } else {
    // case IN_Wait_for_ack:
    if (*sfEvent == User2_event_send_ACK_call) {
      // Inport: '<Root>/send_ACK'
      out = User2_checkACK(User2_U.send_ACK, User2_DW.c_ACK);
    } else {
      out = false;
    }

    if (out) {
      User2_DW.c_ACK = User2_reset_ACK(User2_DW.c_ACK);
      User2_DW.send_tag = 1.0F - User2_DW.send_tag;

      // Outport: '<Root>/dequeue'
      User2_Y.dequeue = true;
      User2_DW.is_send = User2_IN_Idle;
    } else if (*sfEvent == User2_event_timeout) {
      // Outport: '<Root>/send_packet' incorporates:
      //   Inport: '<Root>/send_data'

      User2_Y.send_packet = User2_calculation(User2_U.send_data,
        User2_DW.send_tag);
      User2_DW.send_readyEventCounter++;
      User2_DW.is_send = User2_IN_Wait_for_ack;
    }
  }

  if (*sfEvent == User2_event_receive_packet_call) {
    // Inport: '<Root>/receive_packet'
    out = User2_check_packet(User2_U.receive_packet, User2_DW.receive_tag);
  } else {
    out = false;
  }

  if (out) {
    // Inport: '<Root>/receive_packet' incorporates:
    //   Outport: '<Root>/receive_ACK'
    //   Outport: '<Root>/receive_data'

    User2_send_data_ack(User2_U.receive_packet, User2_DW.receive_c_ack,
                        &User2_Y.receive_data, &User2_Y.receive_ACK, &tmp);
    User2_DW.receive_c_ack = tmp;
    User2_DW.receive_tag = 1.0F - User2_DW.receive_tag;
    User2_DW.receive_readyEventCounter++;
  } else if (*sfEvent == User2_event_reset_receiver) {
    User2_DW.receive_c_ack = 0U;
    User2_DW.receive_tag = 1.0F;
  }
}

// Model step function
void User2::step()
{
  int32_t inputEventFiredFlag;
  int32_t sfEvent;
  bool zcEvent[6];
  bool zcEvent_0;
  ZCEventType tmp;

  // Chart: '<Root>/sw' incorporates:
  //   TriggerPort: '<S1>/input events'

  // Inport: '<Root>/send_data_call'
  zcEvent[0] = (((User2_PrevZCX.sw_Trig_ZCE[0] == POS_ZCSIG) !=
                 User2_U.send_data_call) && (User2_PrevZCX.sw_Trig_ZCE[0] !=
    UNINITIALIZED_ZCSIG));

  // Inport: '<Root>/send_ACK_call'
  zcEvent[1] = (((User2_PrevZCX.sw_Trig_ZCE[1] == POS_ZCSIG) !=
                 User2_U.send_ACK_call) && (User2_PrevZCX.sw_Trig_ZCE[1] !=
    UNINITIALIZED_ZCSIG));

  // Inport: '<Root>/receive_packet_call'
  zcEvent[2] = (((User2_PrevZCX.sw_Trig_ZCE[2] == POS_ZCSIG) !=
                 User2_U.receive_packet_call) && (User2_PrevZCX.sw_Trig_ZCE[2]
    != UNINITIALIZED_ZCSIG));

  // Inport: '<Root>/timeout'
  zcEvent[3] = (((User2_PrevZCX.sw_Trig_ZCE[3] == POS_ZCSIG) != User2_U.timeout)
                && (User2_PrevZCX.sw_Trig_ZCE[3] != UNINITIALIZED_ZCSIG));

  // Inport: '<Root>/reset_sender'
  zcEvent[4] = (((User2_PrevZCX.sw_Trig_ZCE[4] == POS_ZCSIG) !=
                 User2_U.reset_sender) && (User2_PrevZCX.sw_Trig_ZCE[4] !=
    UNINITIALIZED_ZCSIG));

  // Inport: '<Root>/reset_receiver'
  zcEvent[5] = (((User2_PrevZCX.sw_Trig_ZCE[5] == POS_ZCSIG) !=
                 User2_U.reset_receiver) && (User2_PrevZCX.sw_Trig_ZCE[5] !=
    UNINITIALIZED_ZCSIG));
  zcEvent_0 = false;
  for (inputEventFiredFlag = 0; inputEventFiredFlag < 6; inputEventFiredFlag++)
  {
    zcEvent_0 = (zcEvent_0 || zcEvent[inputEventFiredFlag]);
  }

  if (zcEvent_0) {
    inputEventFiredFlag = 0;
    if (zcEvent[0]) {
      // Inport: '<Root>/send_data_call'
      if (User2_U.send_data_call) {
        tmp = RISING_ZCEVENT;
      } else {
        tmp = FALLING_ZCEVENT;
      }
    } else {
      tmp = NO_ZCEVENT;
    }

    if (static_cast<int32_t>(tmp) != 0) {
      inputEventFiredFlag = 1;
      sfEvent = User2_event_send_data_call;
      User2_chartstep_c2_User2(&sfEvent);
    }

    if (zcEvent[1]) {
      // Inport: '<Root>/send_ACK_call'
      if (User2_U.send_ACK_call) {
        tmp = RISING_ZCEVENT;
      } else {
        tmp = FALLING_ZCEVENT;
      }
    } else {
      tmp = NO_ZCEVENT;
    }

    if (static_cast<int32_t>(tmp) != 0) {
      inputEventFiredFlag = 1;
      sfEvent = User2_event_send_ACK_call;
      User2_chartstep_c2_User2(&sfEvent);
    }

    if (zcEvent[2]) {
      // Inport: '<Root>/receive_packet_call'
      if (User2_U.receive_packet_call) {
        tmp = RISING_ZCEVENT;
      } else {
        tmp = FALLING_ZCEVENT;
      }
    } else {
      tmp = NO_ZCEVENT;
    }

    if (static_cast<int32_t>(tmp) != 0) {
      inputEventFiredFlag = 1;
      sfEvent = User2_event_receive_packet_call;
      User2_chartstep_c2_User2(&sfEvent);
    }

    if (zcEvent[3]) {
      // Inport: '<Root>/timeout'
      if (User2_U.timeout) {
        tmp = RISING_ZCEVENT;
      } else {
        tmp = FALLING_ZCEVENT;
      }
    } else {
      tmp = NO_ZCEVENT;
    }

    if (static_cast<int32_t>(tmp) != 0) {
      inputEventFiredFlag = 1;
      sfEvent = User2_event_timeout;
      User2_chartstep_c2_User2(&sfEvent);
    }

    if (zcEvent[4]) {
      // Inport: '<Root>/reset_sender'
      if (User2_U.reset_sender) {
        tmp = RISING_ZCEVENT;
      } else {
        tmp = FALLING_ZCEVENT;
      }
    } else {
      tmp = NO_ZCEVENT;
    }

    if (static_cast<int32_t>(tmp) != 0) {
      inputEventFiredFlag = 1;
      sfEvent = User2_event_reset_sender;
      User2_chartstep_c2_User2(&sfEvent);
    }

    if (zcEvent[5]) {
      // Inport: '<Root>/reset_receiver'
      if (User2_U.reset_receiver) {
        tmp = RISING_ZCEVENT;
      } else {
        tmp = FALLING_ZCEVENT;
      }
    } else {
      tmp = NO_ZCEVENT;
    }

    if (static_cast<int32_t>(tmp) != 0) {
      inputEventFiredFlag = 1;
      sfEvent = User2_event_reset_receiver;
      User2_chartstep_c2_User2(&sfEvent);
    }

    if ((inputEventFiredFlag != 0) && (User2_DW.send_readyEventCounter > 0U)) {
      // Outport: '<Root>/send_ready'
      User2_Y.send_ready = !User2_Y.send_ready;
      User2_DW.send_readyEventCounter--;
    }

    if ((inputEventFiredFlag != 0) && (User2_DW.receive_readyEventCounter > 0U))
    {
      // Outport: '<Root>/receive_ready'
      User2_Y.receive_ready = !User2_Y.receive_ready;
      User2_DW.receive_readyEventCounter--;
    }
  }

  // Inport: '<Root>/send_data_call'
  User2_PrevZCX.sw_Trig_ZCE[0] = User2_U.send_data_call;

  // Inport: '<Root>/send_ACK_call'
  User2_PrevZCX.sw_Trig_ZCE[1] = User2_U.send_ACK_call;

  // Inport: '<Root>/receive_packet_call'
  User2_PrevZCX.sw_Trig_ZCE[2] = User2_U.receive_packet_call;

  // Inport: '<Root>/timeout'
  User2_PrevZCX.sw_Trig_ZCE[3] = User2_U.timeout;

  // Inport: '<Root>/reset_sender'
  User2_PrevZCX.sw_Trig_ZCE[4] = User2_U.reset_sender;

  // Inport: '<Root>/reset_receiver'
  User2_PrevZCX.sw_Trig_ZCE[5] = User2_U.reset_receiver;
}

// Model initialize function
void User2::initialize()
{
  {
    int32_t i;
    for (i = 0; i < 6; i++) {
      User2_PrevZCX.sw_Trig_ZCE[i] = UNINITIALIZED_ZCSIG;
    }

    // SystemInitialize for Chart: '<Root>/sw'
    User2_DW.send_tag = 1.0F;

    // SystemInitialize for Outport: '<Root>/dequeue' incorporates:
    //   Chart: '<Root>/sw'

    User2_Y.dequeue = true;

    // SystemInitialize for Chart: '<Root>/sw'
    User2_DW.receive_tag = 1.0F;

    // Chart: '<Root>/sw'
    User2_DW.is_send = User2_IN_Idle;
  }
}

// Model terminate function
void User2::terminate()
{
  // (no terminate code required)
}

// Root inport: '<Root>/send_ACK' set method
void User2::setsend_ACK(uint8_t localArgInput)
{
  User2_U.send_ACK = localArgInput;
}

// Root inport: '<Root>/send_data' set method
void User2::setsend_data(uint8_t localArgInput)
{
  User2_U.send_data = localArgInput;
}

// Root inport: '<Root>/receive_packet' set method
void User2::setreceive_packet(uint16_t localArgInput)
{
  User2_U.receive_packet = localArgInput;
}

// Root inport: '<Root>/send_data_call' set method
void User2::setsend_data_call(bool localArgInput)
{
  User2_U.send_data_call = localArgInput;
}

// Root inport: '<Root>/send_ACK_call' set method
void User2::setsend_ACK_call(bool localArgInput)
{
  User2_U.send_ACK_call = localArgInput;
}

// Root inport: '<Root>/receive_packet_call' set method
void User2::setreceive_packet_call(bool localArgInput)
{
  User2_U.receive_packet_call = localArgInput;
}

// Root inport: '<Root>/timeout' set method
void User2::settimeout(bool localArgInput)
{
  User2_U.timeout = localArgInput;
}

// Root inport: '<Root>/reset_sender' set method
void User2::setreset_sender(bool localArgInput)
{
  User2_U.reset_sender = localArgInput;
}

// Root inport: '<Root>/reset_receiver' set method
void User2::setreset_receiver(bool localArgInput)
{
  User2_U.reset_receiver = localArgInput;
}

// Root outport: '<Root>/send_packet' get method
uint16_t User2::getsend_packet() const
{
  return User2_Y.send_packet;
}

// Root outport: '<Root>/receive_data' get method
uint8_t User2::getreceive_data() const
{
  return User2_Y.receive_data;
}

// Root outport: '<Root>/receive_ACK' get method
uint8_t User2::getreceive_ACK() const
{
  return User2_Y.receive_ACK;
}

// Root outport: '<Root>/receive_ready' get method
bool User2::getreceive_ready() const
{
  return User2_Y.receive_ready;
}

// Root outport: '<Root>/send_ready' get method
bool User2::getsend_ready() const
{
  return User2_Y.send_ready;
}

// Root outport: '<Root>/dequeue' get method
bool User2::getdequeue() const
{
  return User2_Y.dequeue;
}

const char* User2::RT_MODEL_User2_T::getErrorStatus() const
{
  return (errorStatus);
}

void User2::RT_MODEL_User2_T::setErrorStatus(const char* const volatile
  aErrorStatus)
{
  (errorStatus = aErrorStatus);
}

// Constructor
User2::User2() :
  User2_U(),
  User2_Y(),
  User2_DW(),
  User2_PrevZCX(),
  User2_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
User2::~User2() = default;

// Real-Time Model get method
User2::RT_MODEL_User2_T * User2::getRTM()
{
  return (&User2_M);
}

//
// File trailer for generated code.
//
// [EOF]
//

//
// File: User2.h
//
// Code generated for Simulink model 'User2'.
//
// Model version                  : 1.14
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Mon Apr 14 23:37:27 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef User2_h_
#define User2_h_
#include <stdbool.h>
#include <stdint.h>
#include "zero_crossing_types.h"

// Class declaration for model User2
class User2 final
{
  // public data and function members
 public:
  // Block states (default storage) for system '<Root>'
  struct DW_User2_T {
    float send_tag;                    // '<Root>/sw'
    float receive_tag;                 // '<Root>/sw'
    uint32_t send_readyEventCounter;   // '<Root>/sw'
    uint32_t receive_readyEventCounter;// '<Root>/sw'
    uint32_t previousTicks;            // '<Root>/sw'
    uint16_t temporalCounter_i1;       // '<Root>/sw'
    uint8_t c_ACK;                     // '<Root>/sw'
    uint8_t receive_c_ack;             // '<Root>/sw'
    uint8_t is_send;                   // '<Root>/sw'
  };

  // Zero-crossing (trigger) state
  struct PrevZCX_User2_T {
    ZCSigState sw_Trig_ZCE[5];         // '<Root>/sw'
  };

  // External inputs (root inport signals with default storage)
  struct ExtU_User2_T {
    uint8_t send_ACK;                  // '<Root>/send_ACK'
    uint8_t send_data;                 // '<Root>/send_data'
    uint16_t receive_packet;           // '<Root>/receive_packet'
    bool send_data_call;               // '<Root>/send_data_call'
    bool send_ACK_call;                // '<Root>/send_ACK_call'
    bool receive_packet_call;          // '<Root>/receive_packet_call'
    bool reset_sender;                 // '<Root>/reset_sender'
    bool reset_receiver;               // '<Root>/reset_receiver'
  };

  // External outputs (root outports fed by signals with default storage)
  struct ExtY_User2_T {
    uint16_t send_packet;              // '<Root>/send_packet'
    uint8_t receive_data;              // '<Root>/receive_data'
    uint8_t receive_ACK;               // '<Root>/receive_ACK'
    bool receive_ready;                // '<Root>/receive_ready'
    bool send_ready;                   // '<Root>/send_ready'
    bool dequeue;                      // '<Root>/dequeue'
  };

  // Real-time Model Data Structure
  struct RT_MODEL_User2_T {
    const char * volatile errorStatus;

    //
    //  Timing:
    //  The following substructure contains information regarding
    //  the timing information for the model.

    struct {
      uint32_t clockTick0;
    } Timing;

    const char* getErrorStatus() const;
    void setErrorStatus(const char* const volatile aErrorStatus);
  };

  // Copy Constructor
  User2(User2 const&) = delete;

  // Assignment Operator
  User2& operator= (User2 const&) & = delete;

  // Move Constructor
  User2(User2 &&) = delete;

  // Move Assignment Operator
  User2& operator= (User2 &&) = delete;

  // Real-Time Model get method
  User2::RT_MODEL_User2_T * getRTM();

  // Root inport: '<Root>/send_ACK' set method
  void setsend_ACK(uint8_t localArgInput);

  // Root inport: '<Root>/send_data' set method
  void setsend_data(uint8_t localArgInput);

  // Root inport: '<Root>/receive_packet' set method
  void setreceive_packet(uint16_t localArgInput);

  // Root inport: '<Root>/send_data_call' set method
  void setsend_data_call(bool localArgInput);

  // Root inport: '<Root>/send_ACK_call' set method
  void setsend_ACK_call(bool localArgInput);

  // Root inport: '<Root>/receive_packet_call' set method
  void setreceive_packet_call(bool localArgInput);

  // Root inport: '<Root>/reset_sender' set method
  void setreset_sender(bool localArgInput);

  // Root inport: '<Root>/reset_receiver' set method
  void setreset_receiver(bool localArgInput);

  // Root outport: '<Root>/send_packet' get method
  uint16_t getsend_packet() const;

  // Root outport: '<Root>/receive_data' get method
  uint8_t getreceive_data() const;

  // Root outport: '<Root>/receive_ACK' get method
  uint8_t getreceive_ACK() const;

  // Root outport: '<Root>/receive_ready' get method
  bool getreceive_ready() const;

  // Root outport: '<Root>/send_ready' get method
  bool getsend_ready() const;

  // Root outport: '<Root>/dequeue' get method
  bool getdequeue() const;

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  static void terminate();

  // Constructor
  User2();

  // Destructor
  ~User2();

  // private data and function members
 private:
  // External inputs
  ExtU_User2_T User2_U;

  // External outputs
  ExtY_User2_T User2_Y;

  // Block states
  DW_User2_T User2_DW;

  // Triggered events
  PrevZCX_User2_T User2_PrevZCX;

  // private member function(s) for subsystem '<Root>'
  uint8_t User2_ack_crc(uint8_t b_value);
  bool User2_checkACK(uint8_t ack, uint8_t check);
  uint8_t User2_crc4(uint8_t b_value, float t) const;
  uint16_t User2_calculation(uint8_t input, float c_tag);
  uint8_t User2_reset_ACK(uint8_t ca);
  bool User2_check_packet(uint16_t p, float t) const;
  void User2_send_data_ack(uint16_t p, uint8_t c, uint8_t *d, uint8_t *a,
    uint8_t *nc);
  void User2_chartstep_c2_User2(const int32_t *sfEvent);

  // Real-Time Model
  RT_MODEL_User2_T User2_M;
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'User2'
//  '<S1>'   : 'User2/sw'

#endif                                 // User2_h_

//
// File trailer for generated code.
//
// [EOF]
//

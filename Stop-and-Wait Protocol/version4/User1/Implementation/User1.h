//
// File: User1.h
//
// Code generated for Simulink model 'User1'.
//
// Model version                  : 1.14
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Thu Jan  9 15:42:49 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef User1_h_
#define User1_h_
#include <stdbool.h>
#include <stdint.h>

// Forward declaration for rtModel
typedef struct tag_RTM_User1_T RT_MODEL_User1_T;

// Block states (default storage) for system '<Root>'
struct DW_User1_T {
  float send_tag;                      // '<Root>/sw'
  float receive_tag;                   // '<Root>/sw'
  uint32_t send_readyEventCounter;     // '<Root>/sw'
  uint32_t receive_readyEventCounter;  // '<Root>/sw'
  uint8_t c_ACK;                       // '<Root>/sw'
  uint8_t receive_c_ack;               // '<Root>/sw'
  uint8_t is_send;                     // '<Root>/sw'
};

// External inputs (root inport signals with default storage)
struct ExtU_User1_T {
  uint8_t send_ACK;                    // '<Root>/send_ACK'
  uint8_t send_data;                   // '<Root>/send_data'
  uint16_t receive_packet;             // '<Root>/receive_packet'
};

// External outputs (root outports fed by signals with default storage)
struct ExtY_User1_T {
  uint16_t send_packet;                // '<Root>/send_packet'
  uint8_t receive_data;                // '<Root>/receive_data'
  uint8_t receive_ACK;                 // '<Root>/receive_ACK'
  bool receive_ready;                  // '<Root>/receive_ready'
  bool send_ready;                     // '<Root>/send_ready'
  bool dequeue;                        // '<Root>/dequeue'
};

// Real-time Model Data Structure
struct tag_RTM_User1_T {
  const char * volatile errorStatus;
  const char* getErrorStatus() const;
  void setErrorStatus(const char* const volatile aErrorStatus);
};

// Class declaration for model User1
class User1 final
{
  // public data and function members
 public:
  // Copy Constructor
  User1(User1 const&) = delete;

  // Assignment Operator
  User1& operator= (User1 const&) & = delete;

  // Move Constructor
  User1(User1 &&) = delete;

  // Move Assignment Operator
  User1& operator= (User1 &&) = delete;

  // Real-Time Model get method
  RT_MODEL_User1_T * getRTM();

  // Root inport: '<Root>/send_ACK' set method
  void setsend_ACK(uint8_t localArgInput);

  // Root inport: '<Root>/send_data' set method
  void setsend_data(uint8_t localArgInput);

  // Root inport: '<Root>/receive_packet' set method
  void setreceive_packet(uint16_t localArgInput);

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
  void receive_packet_call();

  // model step function
  void send_ACK_call();

  // model step function
  void send_data_call();

  // model step function
  void timeout();

  // model terminate function
  static void terminate();

  // Constructor
  User1();

  // Destructor
  ~User1();

  // private data and function members
 private:
  // External inputs
  ExtU_User1_T User1_U;

  // External outputs
  ExtY_User1_T User1_Y;

  // Block states
  DW_User1_T User1_DW;

  // private member function(s) for subsystem '<Root>/sw'
  void User1_sw_Init();
  void User1_sw(int32_t controlPortIdx);
  uint8_t User1_crc4(uint8_t b_value, float t);
  uint16_t User1_calculation(uint8_t input, float c_tag);
  uint8_t User1_ack_crc(uint8_t b_value);
  bool User1_checkACK(uint8_t ack, uint8_t check);
  bool User1_check_packet(uint16_t p, float t);
  uint8_t User1_reset_ACK(uint8_t ca);
  void User1_send_data_ack(uint16_t p, uint8_t c, uint8_t *d, uint8_t *a,
    uint8_t *nc);
  void User1_chartstep_c2_User1(const int32_t *sfEvent);

  // Real-Time Model
  RT_MODEL_User1_T User1_M;
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
//  '<Root>' : 'User1'
//  '<S1>'   : 'User1/sw'

#endif                                 // User1_h_

//
// File trailer for generated code.
//
// [EOF]
//

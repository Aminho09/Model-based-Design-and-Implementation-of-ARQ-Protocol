//
// File: receive.h
//
// Code generated for Simulink model 'receive'.
//
// Model version                  : 1.9
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Sun Jan  5 18:57:38 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef receive_h_
#define receive_h_
#include <stdbool.h>
#include <stdint.h>

// Class declaration for model receive
class receive final
{
  // public data and function members
 public:
  // Block states (default storage) for system '<Root>'
  struct DW_receive_T {
    float tag;                         // '<Root>/Chart'
    uint8_t c_ack;                     // '<Root>/Chart'
  };

  // External inputs (root inport signals with default storage)
  struct ExtU_receive_T {
    uint16_t packet;                   // '<Root>/packet'
  };

  // External outputs (root outports fed by signals with default storage)
  struct ExtY_receive_T {
    uint8_t data;                      // '<Root>/data'
    uint8_t ACK;                       // '<Root>/ACK'
    bool ready;                        // '<Root>/ready'
  };

  // Real-time Model Data Structure
  struct RT_MODEL_receive_T {
    const char * volatile errorStatus;
    const char* getErrorStatus() const;
    void setErrorStatus(const char* const volatile aErrorStatus);
  };

  // Copy Constructor
  receive(receive const&) = delete;

  // Assignment Operator
  receive& operator= (receive const&) & = delete;

  // Move Constructor
  receive(receive &&) = delete;

  // Move Assignment Operator
  receive& operator= (receive &&) = delete;

  // Real-Time Model get method
  receive::RT_MODEL_receive_T * getRTM();

  // Root inport: '<Root>/packet' set method
  void setpacket(uint16_t localArgInput);

  // Root outport: '<Root>/data' get method
  uint8_t getdata() const;

  // Root outport: '<Root>/ACK' get method
  uint8_t getACK() const;

  // Root outport: '<Root>/ready' get method
  bool getready() const;

  // model initialize function
  void initialize();

  // model step function
  void call();

  // model terminate function
  static void terminate();

  // Constructor
  receive();

  // Destructor
  ~receive();

  // private data and function members
 private:
  // External inputs
  ExtU_receive_T receive_U;

  // External outputs
  ExtY_receive_T receive_Y;

  // Block states
  DW_receive_T receive_DW;

  // private member function(s) for subsystem '<Root>'
  uint8_t receive_crc4(uint8_t b_value, float crc_tag);
  bool receive_check_packet(uint16_t p, float t) const;
  uint8_t receive_ack_crc(uint8_t b_value);
  void receive_send_data_ack(uint16_t p, uint8_t c, uint8_t *d, uint8_t *a,
    uint8_t *nc);

  // Real-Time Model
  RT_MODEL_receive_T receive_M;
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
//  '<Root>' : 'receive'
//  '<S1>'   : 'receive/Chart'

#endif                                 // receive_h_

//
// File trailer for generated code.
//
// [EOF]
//

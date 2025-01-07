//
// File: send.h
//
// Code generated for Simulink model 'send'.
//
// Model version                  : 1.29
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Tue Jan  7 10:56:05 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef send_h_
#define send_h_
#include <stdbool.h>
#include <stdint.h>
#include <cmath>

// user code (top of header file)
#include <QQueue>
#include <QDebug>

// Class declaration for model send
class send final
{
  // public data and function members
 public:
  // Block signals (default storage)
  struct B_send_T {
    uint8_t dequeuing;                 // '<S2>/dequeuing'
    bool emp;                          // '<S3>/emp'
  };

  // Block states (default storage) for system '<Root>'
  struct DW_send_T {
    float tag;                         // '<Root>/Chart1'
    uint16_t temporalCounter_i1;       // '<Root>/Chart1'
    uint8_t c_ACK;                     // '<Root>/Chart1'
    uint8_t data;                      // '<Root>/Chart1'
    uint8_t is_c2_send;                // '<Root>/Chart1'
    bool isNotInit;                    // '<Root>/Chart1'
  };

  // External inputs (root inport signals with default storage)
  struct ExtU_send_T {
    uint8_t ACK;                       // '<Root>/ACK'
  };

  // External outputs (root outports fed by signals with default storage)
  struct ExtY_send_T {
    uint16_t packet;                   // '<Root>/packet'
    bool ready;                        // '<Root>/ready'
    bool empty;                        // '<Root>/empty'
  };

  // Real-time Model Data Structure
  struct RT_MODEL_send_T {
    const char * volatile errorStatus;
    const char* getErrorStatus() const;
    void setErrorStatus(const char* const volatile aErrorStatus);
  };

  // Copy Constructor
  send(send const&) = delete;

  // Assignment Operator
  send& operator= (send const&) & = delete;

  // Move Constructor
  send(send &&) = delete;

  // Move Assignment Operator
  send& operator= (send &&) = delete;

  // Real-Time Model get method
  send::RT_MODEL_send_T * getRTM();

  // Root inport: '<Root>/ACK' set method
  void setACK(uint8_t localArgInput);

  // Root outport: '<Root>/packet' get method
  uint16_t getpacket() const;

  // Root outport: '<Root>/ready' get method
  bool getready() const;

  // Root outport: '<Root>/empty' get method
  bool getempty() const;

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  static void terminate();

  // Constructor
  send();

  // Destructor
  ~send();

  // private data and function members
 private:
  // External inputs
  ExtU_send_T send_U;

  // External outputs
  ExtY_send_T send_Y;

  // Block signals
  B_send_T send_B;

  // Block states
  DW_send_T send_DW;

  // private member function(s) for subsystem '<Root>/Simulink Function'
  void pop(uint8_t *rty_out);

  // private member function(s) for subsystem '<Root>/Simulink Function1'
  void is_empty(bool *rty_f);

  // private member function(s) for subsystem '<Root>'
  uint8_t send_ack_crc(uint8_t b_value);
  bool send_checkACK(uint8_t ack, uint8_t check);
  uint8_t send_crc4(uint8_t b_value, float t);
  uint16_t send_calculation(uint8_t input, float c_tag);
  uint8_t send_reset_ACK(uint8_t ca);

  // Real-Time Model
  RT_MODEL_send_T send_M;
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
//  '<Root>' : 'send'
//  '<S1>'   : 'send/Chart1'
//  '<S2>'   : 'send/Simulink Function'
//  '<S3>'   : 'send/Simulink Function1'

#endif                                 // send_h_

//
// File trailer for generated code.
//
// [EOF]
//

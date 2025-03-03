//
// File: rt_roundf_snf.cpp
//
// Code generated for Simulink model 'send'.
//
// Model version                  : 1.11
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Fri Dec 13 15:13:23 2024
//
#include "rt_roundf_snf.h"
#include <cmath>

float rt_roundf_snf(float u)
{
  float y;
  if (std::abs(u) < 8.388608E+6F) {
    if (u >= 0.5F) {
      y = std::floor(u + 0.5F);
    } else if (u > -0.5F) {
      y = u * 0.0F;
    } else {
      y = std::ceil(u - 0.5F);
    }
  } else {
    y = u;
  }

  return y;
}

//
// File trailer for generated code.
//
// [EOF]
//

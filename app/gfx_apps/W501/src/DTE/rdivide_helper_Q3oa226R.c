/*
 * File: rdivide_helper_Q3oa226R.c
 *
 * Code generated for Simulink model 'DTE_V_1'.
 *
 * Model version                  : 1.197
 * Simulink Coder version         : 9.0 (R2018b) 24-May-2018
 * C/C++ source code generated on : Mon Dec 30 16:43:09 2019
 */

#include "rtwtypes.h"
#include "rdivide_helper_Q3oa226R.h"

/* Function for Chart: '<S1>/Chart' */
uint32_T rdivide_helper_Q3oa226R(uint32_T x, uint32_T y)
{
  uint32_T z;
  uint32_T b_x;
  if (y == 0U) {
    if (x == 0U) {
      z = 0U;
    } else {
      z = MAX_uint32_T;
    }
  } else {
    z = x / y;
    b_x = x - (z * y);
    if ((b_x > 0U) && (b_x >= ((y >> 1U) + (y & 1U)))) {
      z++;
    }
  }

  return z;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

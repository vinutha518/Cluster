/*
 * File: IVN_Decryption.c
 *
 * Code generated for Simulink model 'IVN_SecurityAlgorithm'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 9.0 (R2018b) 24-May-2018
 * C/C++ source code generated on : Mon Aug 19 17:54:38 2019
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "IVN_Decryption.h"

/* Include model header file for global data */
#include "IVN_SecurityAlgorithm.h"
#include "IVN_SecurityAlgorithm_private.h"

/* Output and update for Simulink Function: '<S3>/Decryption_Func' */
uint32_T IVN_Decryption(uint32_T rtu_u)
{
  uint32_T rtb_BitwiseOR;

  /* S-Function (sfix_bitop): '<S8>/Bitwise OR2' incorporates:
   *  ArithShift: '<S8>/Shift Arithmetic2'
   *  ArithShift: '<S8>/Shift Arithmetic3'
   *  Constant: '<S8>/Constant3'
   *  Constant: '<S8>/Constant4'
   *  Constant: '<S8>/Constant5'
   *  Constant: '<S8>/Constant6'
   *  Constant: '<S8>/Constant7'
   *  S-Function (sfix_bitop): '<S8>/Bitwise AND1'
   *  S-Function (sfix_bitop): '<S8>/Bitwise AND2'
   *  S-Function (sfix_bitop): '<S8>/Bitwise AND3'
   *  S-Function (sfix_bitop): '<S8>/Bitwise OR1'
   *  SignalConversion: '<S8>/TmpSignal ConversionAtuOutport1'
   */
  rtb_BitwiseOR = (rtu_u & 4278190080U) >> 16U | (rtu_u & 16711935U) | (rtu_u &
    65280U) << 16U;

  /* SignalConversion: '<S8>/TmpSignal ConversionAtyInport1' incorporates:
   *  ArithShift: '<S8>/Shift Arithmetic'
   *  ArithShift: '<S8>/Shift Arithmetic1'
   *  Constant: '<S8>/Constant'
   *  Constant: '<S8>/Constant1'
   *  Constant: '<S8>/Constant2'
   *  S-Function (sfix_bitop): '<S8>/Bitwise AND'
   *  S-Function (sfix_bitop): '<S8>/Bitwise OR'
   */
  return (rtb_BitwiseOR & 1U) << 31U | rtb_BitwiseOR >> 1U;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

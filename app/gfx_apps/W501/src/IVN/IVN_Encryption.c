/*
 * File: IVN_Encryption.c
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

#include "IVN_Encryption.h"

/* Include model header file for global data */
#include "IVN_SecurityAlgorithm.h"
#include "IVN_SecurityAlgorithm_private.h"

/* Output and update for Simulink Function: '<S3>/Encryption_Func' */
uint32_T IVN_Encryption(uint32_T rtu_u)
{
  uint32_T rtb_BitwiseOR2;

  /* S-Function (sfix_bitop): '<S12>/Bitwise OR' incorporates:
   *  ArithShift: '<S12>/Shift Arithmetic'
   *  ArithShift: '<S12>/Shift Arithmetic1'
   *  Constant: '<S12>/Constant'
   *  Constant: '<S12>/Constant1'
   *  Constant: '<S12>/Constant2'
   *  S-Function (sfix_bitop): '<S12>/Bitwise AND'
   *  SignalConversion: '<S12>/TmpSignal ConversionAtuOutport1'
   */
  rtb_BitwiseOR2 = (rtu_u & 2147483648U) >> 31U | rtu_u << 1U;

  /* SignalConversion: '<S12>/TmpSignal ConversionAtyInport1' incorporates:
   *  ArithShift: '<S12>/Shift Arithmetic2'
   *  ArithShift: '<S12>/Shift Arithmetic3'
   *  Constant: '<S12>/Constant3'
   *  Constant: '<S12>/Constant4'
   *  Constant: '<S12>/Constant5'
   *  Constant: '<S12>/Constant6'
   *  Constant: '<S12>/Constant7'
   *  S-Function (sfix_bitop): '<S12>/Bitwise AND1'
   *  S-Function (sfix_bitop): '<S12>/Bitwise AND2'
   *  S-Function (sfix_bitop): '<S12>/Bitwise OR1'
   *  S-Function (sfix_bitop): '<S12>/Bitwise OR2'
   *  S-Function (sfix_bitop): '<S12>/Bitwise Operator'
   */
  return (rtb_BitwiseOR2 & 4278190080U) >> 16U | (rtb_BitwiseOR2 & 16711935U) |
    (rtb_BitwiseOR2 & 65280U) << 16U;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

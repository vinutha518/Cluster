/*
 * File: Encryption_0.c
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

#include "Encryption_0.h"

/* Include model header file for global data */
#include "IVN_SecurityAlgorithm.h"
#include "IVN_SecurityAlgorithm_private.h"

/* Output and update for Simulink Function: '<S3>/Encryption_0' */
void Encryption_0(uint32_T rtu_u, uint32_T rtu_u1, uint32_T *rty_y)
{
  /* SignalConversion: '<S9>/TmpSignal ConversionAtyInport1' incorporates:
   *  FunctionCaller: '<S9>/Function Caller'
   *  S-Function (sfix_bitop): '<S9>/Bitwise XOR'
   *  SignalConversion: '<S9>/TmpSignal ConversionAtu1Outport1'
   *  SignalConversion: '<S9>/TmpSignal ConversionAtuOutport1'
   */
  *rty_y = IVN_Encryption(rtu_u ^ rtu_u1);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

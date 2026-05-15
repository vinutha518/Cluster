/*
 * File: Encryption_1.c
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

#include "Encryption_1.h"

/* Include model header file for global data */
#include "IVN_SecurityAlgorithm.h"
#include "IVN_SecurityAlgorithm_private.h"

/* Output and update for Simulink Function: '<S3>/Encryption_1' */
void Encryption_1(uint32_T rtu_u, uint32_T rtu_u1, uint32_T rtu_u2, uint32_T
                  *rty_y)
{
  /* SignalConversion: '<S10>/TmpSignal ConversionAtyInport1' incorporates:
   *  FunctionCaller: '<S10>/Function Caller'
   *  S-Function (sfix_bitop): '<S10>/Bitwise XOR'
   *  SignalConversion: '<S10>/TmpSignal ConversionAtu1Outport1'
   *  SignalConversion: '<S10>/TmpSignal ConversionAtu2Outport1'
   *  SignalConversion: '<S10>/TmpSignal ConversionAtuOutport1'
   */
  *rty_y = IVN_Encryption(rtu_u ^ rtu_u1 ^ rtu_u2);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

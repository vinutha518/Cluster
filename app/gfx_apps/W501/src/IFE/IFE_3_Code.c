/*
 * File: IFE_3_Code.c
 *
 * Code generated for Simulink model 'IFE_3_Code'.
 *
 * Model version                  : 1.7
 * Simulink Coder version         : 9.0 (R2018b) 24-May-2018
 * C/C++ source code generated on : Tue Jan 14 12:12:27 2020
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Renesas->RH850
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "IFE_3_Code.h"
#include "IFE_3_Code_private.h"

/* Named constants for Chart: '<S3>/Cal_IFE' */
#define IFE_3_Code_B_FALSE             (false)
#define IFE_3_Code_B_TRUE              (true)

/* Block states (default storage) */
DW_IFE_3_Code_T IFE_3_Code_DW;

/* External inputs (root inport signals with default storage) */
ExtU_IFE_3_Code_T IFE_3_Code_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_IFE_3_Code_T IFE_3_Code_Y;

/* Real-time model */
RT_MODEL_IFE_3_Code_T IFE_3_Code_M_;
RT_MODEL_IFE_3_Code_T *const IFE_3_Code_M = &IFE_3_Code_M_;

/* Forward declaration for local functions */
static void IFE_3_Cod_inner_default_Display(void);
void mul_wide_u32(uint32_T in0, uint32_T in1, uint32_T *ptrOutBitsHi, uint32_T
                  *ptrOutBitsLo)
{
  uint32_T outBitsLo;
  uint32_T in0Lo;
  uint32_T in0Hi;
  uint32_T in1Lo;
  uint32_T in1Hi;
  uint32_T productHiLo;
  uint32_T productLoHi;
  in0Hi = in0 >> 16U;
  in0Lo = in0 & 65535U;
  in1Hi = in1 >> 16U;
  in1Lo = in1 & 65535U;
  productHiLo = in0Hi * in1Lo;
  productLoHi = in0Lo * in1Hi;
  in0Lo *= in1Lo;
  in1Lo = 0U;
  outBitsLo = (productLoHi << /*MW:OvBitwiseOk*/ 16U) + /*MW:OvCarryOk*/ in0Lo;
  if (outBitsLo < in0Lo) {
    in1Lo = 1U;
  }

  in0Lo = outBitsLo;
  outBitsLo += /*MW:OvCarryOk*/ productHiLo << /*MW:OvBitwiseOk*/ 16U;
  if (outBitsLo < in0Lo) {
    in1Lo++;
  }

  *ptrOutBitsHi = (((productLoHi >> 16U) + (productHiLo >> 16U)) + in0Hi * in1Hi)
    + in1Lo;
  *ptrOutBitsLo = outBitsLo;
}

uint32_T mul_u32_sat(uint32_T a, uint32_T b)
{
  uint32_T result;
  uint32_T u32_chi;
  mul_wide_u32(a, b, &u32_chi, &result);
  if (u32_chi) {
    result = MAX_uint32_T;
  }

  return result;
}

/* Function for Chart: '<S3>/Cal_IFE' */
static void IFE_3_Cod_inner_default_Display(void)
{
  real32_T z;
  uint8_T u8_IFESampSizeCnt;
  uint32_T qY;
  int32_T tmp;
  boolean_T guard1 = false;

  /* Inport: '<Root>/U8_IgnMode' */
  guard1 = false;
  if (IFE_3_Code_U.U8_IgnMode == 1) {
    if ((IFE_3_Code_U.U8_Eng_RPM != 0) && (IFE_3_Code_U.U16_VehSpeed != 0) &&
        (IFE_3_Code_U.U16_FuelConsRate != 0) && (IFE_3_Code_U.U8_STS_Eng == 2) &&
        (IFE_3_Code_U.U32_OdoDisp != 0U)) {
      /* Inport: '<Root>/U16_FuelConsRate' */
      qY = IFE_3_Code_DW.U32_IFE_FC_Acc + /*MW:OvSatOk*/
        IFE_3_Code_U.U16_FuelConsRate;
      if (qY < IFE_3_Code_DW.U32_IFE_FC_Acc) {
        qY = MAX_uint32_T;
      }

      IFE_3_Code_DW.U32_IFE_FC_Acc = qY;

      /* Inport: '<Root>/U16_VehSpeed' incorporates:
       *  Inport: '<Root>/T_stepTime'
       */
      IFE_3_Code_DW.s_IFE_DistAcc += (real32_T)IFE_3_Code_U.U16_VehSpeed *
        1000.0F * (real32_T)IFE_3_Code_U.T_stepTime / 3.6E+6F;
      if (!IFE_3_Code_DW.B_AccTimeFlag) {
        /* Inport: '<Root>/T_stepTime' */
        qY = (uint32_T)IFE_3_Code_DW.u16_IFEAccTimeCntr +
          IFE_3_Code_U.T_stepTime;
        if (qY > 65535U) {
          qY = 65535U;
        }

        IFE_3_Code_DW.u16_IFEAccTimeCntr = (uint16_T)qY;
      }

      /* Inport: '<Root>/U16_IFEAccTime' */
      if ((!IFE_3_Code_DW.B_AccTimeFlag) && (IFE_3_Code_DW.u16_IFEAccTimeCntr >=
           IFE_3_Code_U.U16_IFEAccTime)) {
        IFE_3_Code_DW.B_AccTimeFlag = IFE_3_Code_B_TRUE;
        IFE_3_Code_DW.s_IFE_DistAcc *= 1000.0F;

        /* Inport: '<Root>/U32_Diag_RW_Fuel_Bias' */
        z = IFE_3_Code_DW.s_IFE_DistAcc / (real32_T)mul_u32_sat
          (IFE_3_Code_DW.U32_IFE_FC_Acc, IFE_3_Code_U.U32_Diag_RW_Fuel_Bias);
        IFE_3_Code_DW.s_IFE_DistAcc = 0.0F;
        IFE_3_Code_DW.U32_IFE_FC_Acc = 0U;

        /* Inport: '<Root>/U8_RW_IFE_Unit' */
        z = roundf(z * (real32_T)IFE_3_Code_U.U8_RW_IFE_Unit);
        if (z < 4.2949673E+9F) {
          if (z >= 0.0F) {
            IFE_3_Code_DW.U32_IFE = (uint32_T)z;
          } else {
            IFE_3_Code_DW.U32_IFE = 0U;
          }
        } else {
          IFE_3_Code_DW.U32_IFE = MAX_uint32_T;
        }

        /* Inport: '<Root>/U8_IFEMax' */
        if (IFE_3_Code_DW.U32_IFE > IFE_3_Code_U.U8_IFEMax) {
          IFE_3_Code_DW.U32_IFE = IFE_3_Code_U.U8_IFEMax;
        }

        /* Outport: '<Root>/U32_IFE_Disp_Val' */
        IFE_3_Code_Y.U32_IFE_Disp_Val = IFE_3_Code_DW.U32_IFE;
      } else {
        if (IFE_3_Code_DW.B_AccTimeFlag) {
          /* Inport: '<Root>/T_stepTime' */
          qY = (uint32_T)IFE_3_Code_DW.u16_IFESampTimeCntr +
            IFE_3_Code_U.T_stepTime;
          if (qY > 65535U) {
            qY = 65535U;
          }

          IFE_3_Code_DW.u16_IFESampTimeCntr = (uint16_T)qY;

          /* Inport: '<Root>/U16_IFESampTime' */
          if (IFE_3_Code_DW.u16_IFESampTimeCntr >= IFE_3_Code_U.U16_IFESampTime)
          {
            /* Inport: '<Root>/U8_IFESampSize' */
            for (u8_IFESampSizeCnt = 1U; u8_IFESampSizeCnt <
                 IFE_3_Code_U.U8_IFESampSize; u8_IFESampSizeCnt++) {
              IFE_3_Code_DW.u32_RollFE[u8_IFESampSizeCnt - 1] =
                IFE_3_Code_DW.u32_RollFE[u8_IFESampSizeCnt];
            }

            IFE_3_Code_DW.s_IFE_DistAcc *= 1000.0F;

            /* Inport: '<Root>/U32_Diag_RW_Fuel_Bias' */
            z = IFE_3_Code_DW.s_IFE_DistAcc / (real32_T)mul_u32_sat
              (IFE_3_Code_DW.U32_IFE_FC_Acc, IFE_3_Code_U.U32_Diag_RW_Fuel_Bias);
            IFE_3_Code_DW.s_IFE_DistAcc = 0.0F;
            IFE_3_Code_DW.U32_IFE_FC_Acc = 0U;

            /* Inport: '<Root>/U8_RW_IFE_Unit' */
            z = roundf(z * (real32_T)IFE_3_Code_U.U8_RW_IFE_Unit);
            if (z < 4.2949673E+9F) {
              if (z >= 0.0F) {
                IFE_3_Code_DW.U32_IFE = (uint32_T)z;
              } else {
                IFE_3_Code_DW.U32_IFE = 0U;
              }
            } else {
              IFE_3_Code_DW.U32_IFE = MAX_uint32_T;
            }

            IFE_3_Code_DW.u16_IFESampTimeCntr = 0U;
            tmp = (int32_T)(IFE_3_Code_DW.u8_RolCnt + 1U);
            if ((uint32_T)tmp > 255U) {
              tmp = 255;
            }

            IFE_3_Code_DW.u8_RolCnt = (uint8_T)tmp;

            /* Inport: '<Root>/U8_IFEMax' */
            if (IFE_3_Code_DW.U32_IFE > IFE_3_Code_U.U8_IFEMax) {
              IFE_3_Code_DW.U32_IFE = IFE_3_Code_U.U8_IFEMax;
            }

            /* Inport: '<Root>/U8_IFESampSize' */
            IFE_3_Code_DW.u32_RollFE[IFE_3_Code_U.U8_IFESampSize - 1] =
              IFE_3_Code_DW.U32_IFE;
          }

          /* End of Inport: '<Root>/U16_IFESampTime' */

          /* Inport: '<Root>/B_IFEEnable' incorporates:
           *  Inport: '<Root>/U8_IFESampSize'
           */
          if (IFE_3_Code_U.B_IFEEnable && (IFE_3_Code_DW.u8_RolCnt >=
               IFE_3_Code_U.U8_IFESampSize)) {
            u8_IFESampSizeCnt = 1U;
            while (u8_IFESampSizeCnt <= IFE_3_Code_U.U8_IFESampSize) {
              qY = IFE_3_Code_DW.u32_RollFE[u8_IFESampSizeCnt - 1] +
                /*MW:OvSatOk*/ IFE_3_Code_DW.U32_IFEAccVal;
              if (qY < IFE_3_Code_DW.U32_IFEAccVal) {
                qY = MAX_uint32_T;
              }

              IFE_3_Code_DW.U32_IFEAccVal = qY;
              tmp = (int32_T)(u8_IFESampSizeCnt + 1U);
              if ((uint32_T)tmp > 255U) {
                tmp = 255;
              }

              u8_IFESampSizeCnt = (uint8_T)tmp;
            }

            z = roundf((real32_T)IFE_3_Code_DW.U32_IFEAccVal / (real32_T)
                       IFE_3_Code_U.U8_IFESampSize);
            if (z < 4.2949673E+9F) {
              if (z >= 0.0F) {
                /* Outport: '<Root>/U32_IFEAvgVal' */
                IFE_3_Code_Y.U32_IFEAvgVal = (uint32_T)z;
              } else {
                /* Outport: '<Root>/U32_IFEAvgVal' */
                IFE_3_Code_Y.U32_IFEAvgVal = 0U;
              }
            } else {
              /* Outport: '<Root>/U32_IFEAvgVal' */
              IFE_3_Code_Y.U32_IFEAvgVal = MAX_uint32_T;
            }

            /* Outport: '<Root>/U32_IFEAvgVal' */
            IFE_3_Code_DW.U32_IFE = IFE_3_Code_Y.U32_IFEAvgVal;
            IFE_3_Code_DW.U32_IFEAccVal = 0U;
          }

          /* End of Inport: '<Root>/B_IFEEnable' */
        }
      }

      /* End of Inport: '<Root>/U16_IFEAccTime' */
      guard1 = true;
    }
  } else {
    u8_IFESampSizeCnt = 1U;

    /* Inport: '<Root>/U8_IFESampSize' */
    while (u8_IFESampSizeCnt <= IFE_3_Code_U.U8_IFESampSize) {
      IFE_3_Code_DW.u32_RollFE[u8_IFESampSizeCnt - 1] = 0U;
      tmp = (int32_T)(u8_IFESampSizeCnt + 1U);
      if ((uint32_T)tmp > 255U) {
        tmp = 255;
      }

      u8_IFESampSizeCnt = (uint8_T)tmp;
    }

    IFE_3_Code_DW.U32_IFE_FC_Acc = 0U;
    IFE_3_Code_DW.U32_IFE = 0U;
    IFE_3_Code_DW.u8_RolCnt = 0U;
    IFE_3_Code_DW.B_AccTimeFlag = IFE_3_Code_B_FALSE;
    guard1 = true;
  }

  if (guard1) {
    /* Inport: '<Root>/U16_IFEDispTime' */
    if ((IFE_3_Code_DW.u16_IFEDispTimeCntr >= IFE_3_Code_U.U16_IFEDispTime) &&
        IFE_3_Code_DW.B_AccTimeFlag) {
      IFE_3_Code_DW.u16_IFEDispTimeCntr = 0U;

      /* Outport: '<Root>/U32_IFE_Disp_Val' */
      IFE_3_Code_Y.U32_IFE_Disp_Val = IFE_3_Code_DW.U32_IFE;

      /* Inport: '<Root>/T_stepTime' */
      qY = (uint32_T)IFE_3_Code_DW.u16_IFEDispTimeCntr + IFE_3_Code_U.T_stepTime;
      if (qY > 65535U) {
        qY = 65535U;
      }

      IFE_3_Code_DW.u16_IFEDispTimeCntr = (uint16_T)qY;
    } else {
      /* Inport: '<Root>/T_stepTime' */
      qY = (uint32_T)IFE_3_Code_DW.u16_IFEDispTimeCntr + IFE_3_Code_U.T_stepTime;
      if (qY > 65535U) {
        qY = 65535U;
      }

      IFE_3_Code_DW.u16_IFEDispTimeCntr = (uint16_T)qY;
    }

    /* End of Inport: '<Root>/U16_IFEDispTime' */
  }

  /* End of Inport: '<Root>/U8_IgnMode' */
}

/* Model step function */
void IFE_3_Code_step(void)
{
  /* Outputs for Enabled SubSystem: '<S1>/Enabled Subsystem' incorporates:
   *  EnablePort: '<S2>/Enable'
   */
  /* Outputs for Enabled SubSystem: '<S2>/Subsystem' incorporates:
   *  EnablePort: '<S3>/Enable'
   */
  /* Logic: '<S2>/Logical Operator' incorporates:
   *  Inport: '<Root>/B_EMSCode'
   *  Inport: '<Root>/U8_LHVSD'
   */
  if (IFE_3_Code_U.B_EMSCode && (!IFE_3_Code_U.U8_LHVSD)) {
    /* Chart: '<S3>/Cal_IFE' */
    if (IFE_3_Code_DW.is_active_c1_IFE_3_Code == 0U) {
      IFE_3_Code_DW.is_active_c1_IFE_3_Code = 1U;
      IFE_3_Code_DW.B_AccTimeFlag = IFE_3_Code_B_FALSE;
      IFE_3_Code_DW.u8_RolCnt = 0U;
      IFE_3_Cod_inner_default_Display();
    } else {
      IFE_3_Cod_inner_default_Display();
    }

    /* End of Chart: '<S3>/Cal_IFE' */
  }

  /* End of Logic: '<S2>/Logical Operator' */
  /* End of Outputs for SubSystem: '<S2>/Subsystem' */
  /* End of Outputs for SubSystem: '<S1>/Enabled Subsystem' */
}

/* Model initialize function */
void IFE_3_Code_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(IFE_3_Code_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&IFE_3_Code_DW, 0,
                sizeof(DW_IFE_3_Code_T));

  /* external inputs */
  (void)memset(&IFE_3_Code_U, 0, sizeof(ExtU_IFE_3_Code_T));

  /* external outputs */
  (void) memset((void *)&IFE_3_Code_Y, 0,
                sizeof(ExtY_IFE_3_Code_T));

  /* SystemInitialize for Enabled SubSystem: '<S1>/Enabled Subsystem' */
  /* SystemInitialize for Enabled SubSystem: '<S2>/Subsystem' */
  /* SystemInitialize for Chart: '<S3>/Cal_IFE' */
  IFE_3_Code_DW.is_active_c1_IFE_3_Code = 0U;
  IFE_3_Code_DW.u16_IFESampTimeCntr = 0U;
  IFE_3_Code_DW.U32_IFE_FC_Acc = 0U;
  IFE_3_Code_DW.U32_IFE = 0U;
  IFE_3_Code_DW.u32_RollFE[0] = 0U;
  IFE_3_Code_DW.u32_RollFE[1] = 0U;
  IFE_3_Code_DW.u32_RollFE[2] = 0U;
  IFE_3_Code_DW.u32_RollFE[3] = 0U;
  IFE_3_Code_DW.u32_RollFE[4] = 0U;
  IFE_3_Code_DW.u32_RollFE[5] = 0U;
  IFE_3_Code_DW.u32_RollFE[6] = 0U;
  IFE_3_Code_DW.u32_RollFE[7] = 0U;
  IFE_3_Code_DW.u32_RollFE[8] = 0U;
  IFE_3_Code_DW.u32_RollFE[9] = 0U;
  IFE_3_Code_DW.u8_RolCnt = 0U;
  IFE_3_Code_DW.u16_IFEDispTimeCntr = 0U;
  IFE_3_Code_DW.u16_IFEAccTimeCntr = 0U;
  IFE_3_Code_DW.U32_IFEAccVal = 0U;
  IFE_3_Code_DW.B_AccTimeFlag = false;
  IFE_3_Code_DW.s_IFE_DistAcc = 0.0F;

  /* InitializeConditions for Outport: '<Root>/U32_IFEAvgVal' incorporates:
   *  Chart: '<S3>/Cal_IFE'
   */
  IFE_3_Code_Y.U32_IFEAvgVal = 0U;

  /* InitializeConditions for Outport: '<Root>/U32_IFE_Disp_Val' incorporates:
   *  Chart: '<S3>/Cal_IFE'
   */
  IFE_3_Code_Y.U32_IFE_Disp_Val = 0U;

  /* End of SystemInitialize for SubSystem: '<S2>/Subsystem' */
  /* End of SystemInitialize for SubSystem: '<S1>/Enabled Subsystem' */
}

/* Model terminate function */
void IFE_3_Code_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

/*
 * File: AFE_3.c
 *
 * Code generated for Simulink model 'AFE_3'.
 *
 * Model version                  : 1.113
 * Simulink Coder version         : 9.0 (R2018b) 24-May-2018
 * C/C++ source code generated on : Sat Jan 11 20:43:00 2020
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Renesas->RH850
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "AFE_3.h"
#include "AFE_3_private.h"

/* Named constants for Chart: '<S3>/Cal_AFE' */
#define AFE_3_FALSE                    (false)
#define AFE_3_MAX_DIST_ACC             (4294967000U)
#define AFE_3_MAX_FUEL_ACC             (4294967000U)
#define AFE_3_TRUE                     (true)
#define AFE_3_U16_RES_AFE_DISP_INTER   ((uint16_T)1000U)
#define AFE_3_U8_RES_AFE_CALC_INTER    ((uint8_T)100U)

/* Block states (default storage) */
DW_AFE_3_T AFE_3_DW;

/* External inputs (root inport signals with default storage) */
ExtU_AFE_3_T AFE_3_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_AFE_3_T AFE_3_Y;

/* Real-time model */
RT_MODEL_AFE_3_T AFE_3_M_;
RT_MODEL_AFE_3_T *const AFE_3_M = &AFE_3_M_;

/* Forward declaration for local functions */
static uint32_T AFE_3_rdivide_helper(uint32_T x, uint32_T y);
static void AFE_3_enter_internal_AFE_GD_CAL(void);
static void AFE_3_enter_internal_c1_AFE_3(const boolean_T *LogicalOperator1);

/* Function for Chart: '<S3>/Cal_AFE' */
static uint32_T AFE_3_rdivide_helper(uint32_T x, uint32_T y)
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
    b_x = x - z * y;
    if ((b_x > 0U) && (b_x >= (y >> 1U) + (y & 1U))) {
      z++;
    }
  }

  return z;
}

real_T rt_roundd_snf(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/* Function for Chart: '<S3>/Cal_AFE' */
static void AFE_3_enter_internal_AFE_GD_CAL(void)
{
  real_T tmp;
  uint32_T qY;
  uint32_T qY_tmp;
  boolean_T guard1 = false;
  boolean_T guard2 = false;
  AFE_3_DW.U16_AFE_GD_CalTimer = 0U;

  /* Inport: '<Root>/U8_AfeCalcInterv' */
  AFE_3_DW.U16_AFE_CAL_INTERVAL = (uint16_T)(AFE_3_U.U8_AfeCalcInterv *
    AFE_3_U8_RES_AFE_CALC_INTER);

  /* Outport: '<Root>/U32_AccFuelConspGD' incorporates:
   *  Inport: '<Root>/U32_AccFuelConspGDEE'
   */
  AFE_3_Y.U32_AccFuelConspGD = AFE_3_U.U32_AccFuelConspGDEE;

  /* Outport: '<Root>/U32_OdoDispGDReset' incorporates:
   *  Inport: '<Root>/U32_OdoDispGDResetEE'
   */
  AFE_3_Y.U32_OdoDispGDReset = AFE_3_U.U32_OdoDispGDResetEE;

  /* Outport: '<Root>/U16_AvgKmplAFE_GD' incorporates:
   *  Inport: '<Root>/U16_AFE_GD_LastDispVal_EE'
   */
  AFE_3_Y.U16_AvgKmplAFE_GD = AFE_3_U.U16_AFE_GD_LastDispVal_EE;

  /* Inport: '<Root>/B_1Km_Dist_Flag_GD_EE' */
  AFE_3_DW.b_1kmover_GD = AFE_3_U.B_1Km_Dist_Flag_GD_EE;

  /* Inport: '<Root>/B_DispUpdated_GD_Flag_EE' */
  AFE_3_DW.b_dispUpdated_GD = AFE_3_U.B_DispUpdated_GD_Flag_EE;

  /* Inport: '<Root>/U8_AfeMIn' */
  AFE_3_DW.u16_AfeMIn = (uint16_T)(AFE_3_U.U8_AfeMIn * 10);

  /* Inport: '<Root>/U8_AfeMax' */
  AFE_3_DW.u16_AfeMax = (uint16_T)(AFE_3_U.U8_AfeMax * 10);

  /* Inport: '<Root>/U8_AfeDispFilterA' */
  AFE_3_DW.u16_AfeDispFilterA = AFE_3_U.U8_AfeDispFilterA;

  /* Inport: '<Root>/U8_AfeDispFilterB' */
  AFE_3_DW.u16_AfeDispFilterB = (uint16_T)(AFE_3_U.U8_AfeDispFilterB * 10);
  AFE_3_DW.counter = 0U;

  /* Inport: '<Root>/U8_AFE_DIST_TH' */
  AFE_3_DW.u16_AFE_DIST_TH = (uint16_T)(AFE_3_U.U8_AFE_DIST_TH * 100);

  /* Outport: '<Root>/U32_AccFuelConspGD' */
  AFE_3_DW.u32_AccFuelConspGDPrev = AFE_3_Y.U32_AccFuelConspGD;

  /* Inport: '<Root>/U8_AfeDispIntev' */
  qY_tmp = (uint32_T)AFE_3_U.U8_AfeDispIntev * AFE_3_U16_RES_AFE_DISP_INTER;
  if (qY_tmp > 65535U) {
    qY_tmp = 65535U;
  }

  AFE_3_DW.U16_AFEDISP_CNT = (uint16_T)qY_tmp;

  /* Inport: '<Root>/B_InfoResetAFE' */
  if (AFE_3_U.B_InfoResetAFE) {
    /* Outport: '<Root>/U32_AccFuelConspGD' */
    AFE_3_Y.U32_AccFuelConspGD = 0U;
    AFE_3_DW.u32_AccFuelConspGDPrev = 0U;

    /* Outport: '<Root>/U32_AccFuelConspGDReset' */
    AFE_3_Y.U32_AccFuelConspGDReset = 0U;

    /* Outport: '<Root>/U32_OdoDispGDReset' incorporates:
     *  Inport: '<Root>/U32_OdoDisp'
     */
    AFE_3_Y.U32_OdoDispGDReset = AFE_3_U.U32_OdoDisp;

    /* Outport: '<Root>/U16_AvgKmplAFE_GD' */
    AFE_3_Y.U16_AvgKmplAFE_GD = 0U;
    AFE_3_DW.b_dispUpdated_GD = AFE_3_FALSE;

    /* Outport: '<Root>/B_AFE_GD_ResetACK' */
    AFE_3_Y.B_AFE_GD_ResetACK = AFE_3_TRUE;

    /* Outport: '<Root>/U32_AccDistAFEGD' */
    AFE_3_Y.U32_AccDistAFEGD = 0U;
    AFE_3_DW.b_1kmover_GD = AFE_3_FALSE;
  } else {
    /* Outport: '<Root>/B_AFE_GD_ResetACK' */
    AFE_3_Y.B_AFE_GD_ResetACK = AFE_3_FALSE;
  }

  /* End of Inport: '<Root>/B_InfoResetAFE' */

  /* Update for Inport: '<Root>/U8_IgnMode' */
  if (AFE_3_U.U8_IgnMode == 1) {
    /* Inport: '<Root>/U8_STS_Eng' incorporates:
     *  Inport: '<Root>/B_CanRxFlag'
     */
    guard1 = false;
    guard2 = false;
    if ((AFE_3_U.U8_STS_Eng == 2) && AFE_3_U.B_CanRxFlag) {
      /* Inport: '<Root>/U16_FuelConsRate' */
      AFE_3_Y.U32_AccFuelConspGD = AFE_3_DW.u32_AccFuelConspGDPrev +
        /*MW:OvSatOk*/ AFE_3_U.U16_FuelConsRate;
      if (AFE_3_Y.U32_AccFuelConspGD < AFE_3_DW.u32_AccFuelConspGDPrev) {
        AFE_3_Y.U32_AccFuelConspGD = MAX_uint32_T;
      }

      /* Outport: '<Root>/U32_AccFuelConspGD' */
      AFE_3_DW.u32_AccFuelConspGDPrev = AFE_3_Y.U32_AccFuelConspGD;

      /* Inport: '<Root>/U32_OdoDisp' incorporates:
       *  Outport: '<Root>/U32_OdoDispGDReset'
       */
      qY_tmp = AFE_3_U.U32_OdoDisp - /*MW:OvSatOk*/ AFE_3_Y.U32_OdoDispGDReset;
      qY = qY_tmp;

      /* Inport: '<Root>/U32_OdoDisp' */
      if (qY_tmp > AFE_3_U.U32_OdoDisp) {
        qY = 0U;
      }

      /* Outport: '<Root>/U32_AccFuelConspGD' incorporates:
       *  Inport: '<Root>/U32_OdoDisp'
       *  Outport: '<Root>/U32_AccFuelConspGDReset'
       *  Outport: '<Root>/U32_OdoDispGDReset'
       */
      if ((AFE_3_Y.U32_AccFuelConspGD >= AFE_3_MAX_FUEL_ACC) || (qY >=
           AFE_3_MAX_DIST_ACC)) {
        AFE_3_Y.U32_AccFuelConspGD = AFE_3_rdivide_helper
          (AFE_3_Y.U32_AccFuelConspGD, 2U);
        AFE_3_Y.U32_AccFuelConspGDReset = AFE_3_rdivide_helper
          (AFE_3_Y.U32_AccFuelConspGDReset, 2U);
        if (qY_tmp > AFE_3_U.U32_OdoDisp) {
          qY_tmp = 0U;
        }

        qY_tmp = AFE_3_rdivide_helper(qY_tmp, 2U);
        AFE_3_Y.U32_OdoDispGDReset += /*MW:OvSatOk*/ qY_tmp;
        if (AFE_3_Y.U32_OdoDispGDReset < qY_tmp) {
          AFE_3_Y.U32_OdoDispGDReset = MAX_uint32_T;
        }

        AFE_3_DW.u32_AccFuelConspGDPrev = AFE_3_Y.U32_AccFuelConspGD;
      }

      guard2 = true;
    } else {
      if (AFE_3_U.U8_STS_Eng == 2) {
        guard2 = true;
      }
    }

    if (guard2) {
      /* Inport: '<Root>/U16_FuelConsRate' */
      if (AFE_3_U.U16_FuelConsRate >= 0) {
        /* Constant: '<S3>/Constant1' */
        qY_tmp = AFE_3_DW.U16_AFE_GD_CalTimer + 20U;
        if (qY_tmp > 65535U) {
          qY_tmp = 65535U;
        }

        AFE_3_DW.U16_AFE_GD_CalTimer = (uint16_T)qY_tmp;
      }

      if (AFE_3_DW.U16_AFE_GD_CalTimer >= AFE_3_DW.U16_AFE_CAL_INTERVAL) {
        AFE_3_DW.U16_AFE_GD_CalTimer = 0U;

        /* Inport: '<Root>/U32_OdoDisp' incorporates:
         *  Outport: '<Root>/U32_OdoDispGDReset'
         */
        qY_tmp = AFE_3_U.U32_OdoDisp - /*MW:OvSatOk*/ AFE_3_Y.U32_OdoDispGDReset;
        qY = qY_tmp;

        /* Inport: '<Root>/U32_OdoDisp' */
        if (qY_tmp > AFE_3_U.U32_OdoDisp) {
          qY = 0U;
        }

        if (AFE_3_U.B_CanRxFlag && (qY >= AFE_3_DW.u16_AFE_DIST_TH)) {
          AFE_3_Y.U32_AccDistAFEGD = qY_tmp;

          /* Inport: '<Root>/U32_OdoDisp' */
          if (qY_tmp > AFE_3_U.U32_OdoDisp) {
            AFE_3_Y.U32_AccDistAFEGD = 0U;
            qY_tmp = 0U;
          }

          /* Outport: '<Root>/U32_AccFuelConspGD' */
          tmp = rt_roundd_snf((real_T)qY_tmp * 10000.0 / (real_T)
                              AFE_3_Y.U32_AccFuelConspGD);
          if (tmp < 65536.0) {
            if (tmp >= 0.0) {
              /* Outport: '<Root>/U16_CalAvgKmplAFE_GD' */
              AFE_3_Y.U16_CalAvgKmplAFE_GD = (uint16_T)tmp;
            } else {
              /* Outport: '<Root>/U16_CalAvgKmplAFE_GD' */
              AFE_3_Y.U16_CalAvgKmplAFE_GD = 0U;
            }
          } else {
            /* Outport: '<Root>/U16_CalAvgKmplAFE_GD' */
            AFE_3_Y.U16_CalAvgKmplAFE_GD = MAX_uint16_T;
          }

          AFE_3_DW.b_1kmover_GD = AFE_3_TRUE;
        }
      }

      if (AFE_3_DW.b_dispUpdated_GD) {
        AFE_3_DW.b_1kmover_GD = AFE_3_TRUE;
      }

      if (AFE_3_U.B_EnbleAFE && AFE_3_DW.b_1kmover_GD) {
        if (!AFE_3_DW.b_dispUpdated_GD) {
          /* Outport: '<Root>/U16_AvgKmplAFE_GD' incorporates:
           *  Outport: '<Root>/U16_CalAvgKmplAFE_GD'
           */
          AFE_3_Y.U16_AvgKmplAFE_GD = AFE_3_Y.U16_CalAvgKmplAFE_GD;
          AFE_3_DW.b_dispUpdated_GD = AFE_3_TRUE;
          AFE_3_DW.U32_AFE_GD_DispCnter = 0U;

          /* Outport: '<Root>/U16_AvgKmplAFE_GD' */
          if (AFE_3_Y.U16_AvgKmplAFE_GD > AFE_3_DW.u16_AfeMax) {
            AFE_3_Y.U16_AvgKmplAFE_GD = AFE_3_DW.u16_AfeMax;
          } else {
            if (AFE_3_Y.U16_AvgKmplAFE_GD < AFE_3_DW.u16_AfeMIn) {
              AFE_3_Y.U16_AvgKmplAFE_GD = AFE_3_DW.u16_AfeMIn;
            }
          }
        } else {
          /* Constant: '<S3>/Constant1' */
          qY = AFE_3_DW.U32_AFE_GD_DispCnter + /*MW:OvSatOk*/ 20U;
          if (qY < AFE_3_DW.U32_AFE_GD_DispCnter) {
            qY = MAX_uint32_T;
          }

          AFE_3_DW.U32_AFE_GD_DispCnter = qY;
          guard1 = true;
        }
      } else {
        guard1 = true;
      }
    }

    if (guard1) {
      if (AFE_3_DW.U32_AFE_GD_DispCnter >= AFE_3_DW.U16_AFEDISP_CNT) {
        AFE_3_DW.U32_AFE_GD_DispCnter = 0U;

        /* Outport: '<Root>/U16_CalAvgKmplAFE_GD' incorporates:
         *  Outport: '<Root>/U16_AvgKmplAFE_GD'
         */
        if (AFE_3_Y.U16_CalAvgKmplAFE_GD > AFE_3_Y.U16_AvgKmplAFE_GD) {
          qY = (uint32_T)AFE_3_Y.U16_CalAvgKmplAFE_GD - /*MW:OvSatOk*/
            AFE_3_Y.U16_AvgKmplAFE_GD;
          if (qY > AFE_3_Y.U16_CalAvgKmplAFE_GD) {
            qY = 0U;
          }

          if ((uint16_T)qY != 0) {
            if ((uint16_T)qY < AFE_3_DW.u16_AfeDispFilterA) {
              qY_tmp = AFE_3_Y.U16_AvgKmplAFE_GD + 1U;
              if (qY_tmp > 65535U) {
                qY_tmp = 65535U;
              }

              AFE_3_Y.U16_AvgKmplAFE_GD = (uint16_T)qY_tmp;
            } else if ((uint16_T)qY > AFE_3_DW.u16_AfeDispFilterB) {
              qY_tmp = AFE_3_Y.U16_AvgKmplAFE_GD + 10U;
              if (qY_tmp > 65535U) {
                qY_tmp = 65535U;
              }

              AFE_3_Y.U16_AvgKmplAFE_GD = (uint16_T)qY_tmp;
            } else {
              qY_tmp = AFE_3_Y.U16_AvgKmplAFE_GD + 5U;
              if (qY_tmp > 65535U) {
                qY_tmp = 65535U;
              }

              AFE_3_Y.U16_AvgKmplAFE_GD = (uint16_T)qY_tmp;
            }
          }
        } else {
          qY = (uint32_T)AFE_3_Y.U16_AvgKmplAFE_GD - /*MW:OvSatOk*/
            AFE_3_Y.U16_CalAvgKmplAFE_GD;
          if (qY > AFE_3_Y.U16_AvgKmplAFE_GD) {
            qY = 0U;
          }

          if ((uint16_T)qY != 0) {
            if ((uint16_T)qY < AFE_3_DW.u16_AfeDispFilterA) {
              qY = AFE_3_Y.U16_AvgKmplAFE_GD - /*MW:OvSatOk*/ 1U;
              if (qY > AFE_3_Y.U16_AvgKmplAFE_GD) {
                qY = 0U;
              }

              AFE_3_Y.U16_AvgKmplAFE_GD = (uint16_T)qY;
            } else if ((uint16_T)qY > AFE_3_DW.u16_AfeDispFilterB) {
              qY = AFE_3_Y.U16_AvgKmplAFE_GD - /*MW:OvSatOk*/ 10U;
              if (qY > AFE_3_Y.U16_AvgKmplAFE_GD) {
                qY = 0U;
              }

              AFE_3_Y.U16_AvgKmplAFE_GD = (uint16_T)qY;
            } else {
              qY = AFE_3_Y.U16_AvgKmplAFE_GD - /*MW:OvSatOk*/ 5U;
              if (qY > AFE_3_Y.U16_AvgKmplAFE_GD) {
                qY = 0U;
              }

              AFE_3_Y.U16_AvgKmplAFE_GD = (uint16_T)qY;
            }
          }
        }

        /* Outport: '<Root>/U16_AvgKmplAFE_GD' */
        if (AFE_3_Y.U16_AvgKmplAFE_GD < AFE_3_DW.u16_AfeMIn) {
          AFE_3_Y.U16_AvgKmplAFE_GD = AFE_3_DW.u16_AfeMIn;
        } else {
          if (AFE_3_Y.U16_AvgKmplAFE_GD > AFE_3_DW.u16_AfeMax) {
            AFE_3_Y.U16_AvgKmplAFE_GD = AFE_3_DW.u16_AfeMax;
          }
        }
      }
    }

    /* End of Inport: '<Root>/U8_STS_Eng' */
  } else {
    AFE_3_DW.U16_AFE_GD_CalTimer = 0U;
    AFE_3_DW.U32_AFE_GD_DispCnter = 0U;
  }

  /* End of Update for Inport: '<Root>/U8_IgnMode' */

  /* Outport: '<Root>/B_1Km_Dist_Flag_GD' */
  AFE_3_Y.B_1Km_Dist_Flag_GD = AFE_3_DW.b_1kmover_GD;

  /* Outport: '<Root>/B_DispUpdated_GD_Flag' */
  AFE_3_Y.B_DispUpdated_GD_Flag = AFE_3_DW.b_dispUpdated_GD;
}

/* Function for Chart: '<S3>/Cal_AFE' */
static void AFE_3_enter_internal_c1_AFE_3(const boolean_T *LogicalOperator1)
{
  real_T tmp;
  uint32_T qY;
  uint32_T qY_tmp;
  boolean_T guard1 = false;
  boolean_T guard2 = false;
  AFE_3_DW.U16_AFECalTimer = 0U;

  /* Inport: '<Root>/U8_AfeCalcInterv' */
  AFE_3_DW.U16_AFE_CAL_INTERVAL = (uint16_T)(AFE_3_U.U8_AfeCalcInterv *
    AFE_3_U8_RES_AFE_CALC_INTER);

  /* Outport: '<Root>/U32_AccFuelConsp' incorporates:
   *  Inport: '<Root>/U32_AccFuelConspEE'
   */
  AFE_3_Y.U32_AccFuelConsp = AFE_3_U.U32_AccFuelConspEE;

  /* Outport: '<Root>/U32_OdoDispReset' incorporates:
   *  Inport: '<Root>/U32_OdoDispResetEE'
   */
  AFE_3_Y.U32_OdoDispReset = AFE_3_U.U32_OdoDispResetEE;

  /* Outport: '<Root>/U16_AFEDispVal' incorporates:
   *  Inport: '<Root>/U16_AFE_LastDispVal_EE'
   */
  AFE_3_Y.U16_AFEDispVal = AFE_3_U.U16_AFE_LastDispVal_EE;

  /* Inport: '<Root>/B_1Km_Dist_Flag_EE' */
  AFE_3_DW.b_1kmover = AFE_3_U.B_1Km_Dist_Flag_EE;

  /* Inport: '<Root>/B_DispUpdated_Flag_EE' */
  AFE_3_DW.b_dispUpdated = AFE_3_U.B_DispUpdated_Flag_EE;

  /* Inport: '<Root>/U8_AfeMIn' */
  AFE_3_DW.u16_AfeMIn = (uint16_T)(AFE_3_U.U8_AfeMIn * 10);

  /* Inport: '<Root>/U8_AfeMax' */
  AFE_3_DW.u16_AfeMax = (uint16_T)(AFE_3_U.U8_AfeMax * 10);

  /* Inport: '<Root>/U8_AfeDispFilterA' */
  AFE_3_DW.u16_AfeDispFilterA = AFE_3_U.U8_AfeDispFilterA;

  /* Inport: '<Root>/U8_AfeDispFilterB' */
  AFE_3_DW.u16_AfeDispFilterB = (uint16_T)(AFE_3_U.U8_AfeDispFilterB * 10);
  AFE_3_DW.counter = 0U;

  /* Inport: '<Root>/U8_AFE_DIST_TH' */
  AFE_3_DW.u16_AFE_DIST_TH = (uint16_T)(AFE_3_U.U8_AFE_DIST_TH * 100);

  /* Outport: '<Root>/U32_AccFuelConsp' */
  AFE_3_DW.U32_AccFuelConspPrev = AFE_3_Y.U32_AccFuelConsp;

  /* Inport: '<Root>/U8_AfeDispIntev' */
  qY_tmp = (uint32_T)AFE_3_U.U8_AfeDispIntev * AFE_3_U16_RES_AFE_DISP_INTER;
  if (qY_tmp > 65535U) {
    qY_tmp = 65535U;
  }

  AFE_3_DW.U16_AFEDISP_CNT = (uint16_T)qY_tmp;
  if (*LogicalOperator1) {
    /* Outport: '<Root>/U32_AccFuelConsp' */
    AFE_3_Y.U32_AccFuelConsp = 0U;
    AFE_3_DW.U32_AccFuelConspPrev = 0U;

    /* Outport: '<Root>/U32_AccFuelConspReset' */
    AFE_3_Y.U32_AccFuelConspReset = 0U;

    /* Outport: '<Root>/U32_OdoDispReset' incorporates:
     *  Inport: '<Root>/U32_OdoDisp'
     */
    AFE_3_Y.U32_OdoDispReset = AFE_3_U.U32_OdoDisp;

    /* Outport: '<Root>/U32_AccDistAFE' */
    AFE_3_Y.U32_AccDistAFE = 0U;

    /* Outport: '<Root>/U16_AFEDispVal' */
    AFE_3_Y.U16_AFEDispVal = 0U;

    /* Outport: '<Root>/B_AFEResetACK' */
    AFE_3_Y.B_AFEResetACK = AFE_3_TRUE;
    AFE_3_DW.b_dispUpdated = AFE_3_FALSE;
    AFE_3_DW.b_1kmover = AFE_3_FALSE;
  } else {
    /* Outport: '<Root>/B_AFEResetACK' */
    AFE_3_Y.B_AFEResetACK = AFE_3_FALSE;
  }

  /* Update for Inport: '<Root>/U8_IgnMode' */
  if (AFE_3_U.U8_IgnMode == 1) {
    /* Inport: '<Root>/U8_STS_Eng' incorporates:
     *  Inport: '<Root>/B_CanRxFlag'
     */
    guard1 = false;
    guard2 = false;
    if ((AFE_3_U.U8_STS_Eng == 2) && AFE_3_U.B_CanRxFlag) {
      /* Inport: '<Root>/U16_FuelConsRate' */
      AFE_3_Y.U32_AccFuelConsp = AFE_3_DW.U32_AccFuelConspPrev + /*MW:OvSatOk*/
        AFE_3_U.U16_FuelConsRate;
      if (AFE_3_Y.U32_AccFuelConsp < AFE_3_DW.U32_AccFuelConspPrev) {
        AFE_3_Y.U32_AccFuelConsp = MAX_uint32_T;
      }

      /* Outport: '<Root>/U32_AccFuelConsp' */
      AFE_3_DW.U32_AccFuelConspPrev = AFE_3_Y.U32_AccFuelConsp;
      qY_tmp = AFE_3_DW.counter + 1U;
      if (qY_tmp > 65535U) {
        qY_tmp = 65535U;
      }

      AFE_3_DW.counter = (uint16_T)qY_tmp;

      /* Inport: '<Root>/U32_OdoDisp' incorporates:
       *  Outport: '<Root>/U32_OdoDispReset'
       */
      qY_tmp = AFE_3_U.U32_OdoDisp - /*MW:OvSatOk*/ AFE_3_Y.U32_OdoDispReset;
      qY = qY_tmp;

      /* Inport: '<Root>/U32_OdoDisp' */
      if (qY_tmp > AFE_3_U.U32_OdoDisp) {
        qY = 0U;
      }

      /* Outport: '<Root>/U32_AccFuelConsp' incorporates:
       *  Inport: '<Root>/U32_OdoDisp'
       *  Outport: '<Root>/U32_AccFuelConspReset'
       *  Outport: '<Root>/U32_OdoDispReset'
       */
      if ((AFE_3_Y.U32_AccFuelConsp >= AFE_3_MAX_FUEL_ACC) || (qY >=
           AFE_3_MAX_DIST_ACC)) {
        AFE_3_Y.U32_AccFuelConsp = AFE_3_rdivide_helper(AFE_3_Y.U32_AccFuelConsp,
          2U);
        AFE_3_Y.U32_AccFuelConspReset = AFE_3_rdivide_helper
          (AFE_3_Y.U32_AccFuelConspReset, 2U);
        if (qY_tmp > AFE_3_U.U32_OdoDisp) {
          qY_tmp = 0U;
        }

        qY_tmp = AFE_3_rdivide_helper(qY_tmp, 2U);
        AFE_3_Y.U32_OdoDispReset += /*MW:OvSatOk*/ qY_tmp;
        if (AFE_3_Y.U32_OdoDispReset < qY_tmp) {
          AFE_3_Y.U32_OdoDispReset = MAX_uint32_T;
        }

        AFE_3_DW.U32_AccFuelConspPrev = AFE_3_Y.U32_AccFuelConsp;
      }

      guard2 = true;
    } else {
      if (AFE_3_U.U8_STS_Eng == 2) {
        guard2 = true;
      }
    }

    if (guard2) {
      /* Inport: '<Root>/U16_FuelConsRate' */
      if (AFE_3_U.U16_FuelConsRate >= 0) {
        /* Constant: '<S3>/Constant1' */
        qY_tmp = AFE_3_DW.U16_AFECalTimer + 20U;
        if (qY_tmp > 65535U) {
          qY_tmp = 65535U;
        }

        AFE_3_DW.U16_AFECalTimer = (uint16_T)qY_tmp;

        /* Outport: '<Root>/b_AFECalTimer' */
        AFE_3_Y.b_AFECalTimer = AFE_3_FALSE;
      }

      if (AFE_3_DW.U16_AFECalTimer >= AFE_3_DW.U16_AFE_CAL_INTERVAL) {
        AFE_3_DW.U16_AFECalTimer = 0U;

        /* Outport: '<Root>/b_AFECalTimer' */
        AFE_3_Y.b_AFECalTimer = AFE_3_TRUE;

        /* Inport: '<Root>/U32_OdoDisp' incorporates:
         *  Outport: '<Root>/U32_OdoDispReset'
         */
        qY_tmp = AFE_3_U.U32_OdoDisp - /*MW:OvSatOk*/ AFE_3_Y.U32_OdoDispReset;
        qY = qY_tmp;

        /* Inport: '<Root>/U32_OdoDisp' */
        if (qY_tmp > AFE_3_U.U32_OdoDisp) {
          qY = 0U;
        }

        if (AFE_3_U.B_CanRxFlag && (qY >= AFE_3_DW.u16_AFE_DIST_TH)) {
          AFE_3_Y.U32_AccDistAFE = qY_tmp;

          /* Inport: '<Root>/U32_OdoDisp' */
          if (qY_tmp > AFE_3_U.U32_OdoDisp) {
            AFE_3_Y.U32_AccDistAFE = 0U;
            qY_tmp = 0U;
          }

          /* Outport: '<Root>/U32_AccFuelConsp' */
          tmp = rt_roundd_snf((real_T)qY_tmp * 10000.0 / (real_T)
                              AFE_3_Y.U32_AccFuelConsp);
          if (tmp < 65536.0) {
            if (tmp >= 0.0) {
              /* Outport: '<Root>/U16_CalAvgKmplAFE' */
              AFE_3_Y.U16_CalAvgKmplAFE = (uint16_T)tmp;
            } else {
              /* Outport: '<Root>/U16_CalAvgKmplAFE' */
              AFE_3_Y.U16_CalAvgKmplAFE = 0U;
            }
          } else {
            /* Outport: '<Root>/U16_CalAvgKmplAFE' */
            AFE_3_Y.U16_CalAvgKmplAFE = MAX_uint16_T;
          }

          AFE_3_DW.b_1kmover = AFE_3_TRUE;
        }
      }

      if (AFE_3_U.B_EnbleAFE && AFE_3_DW.b_1kmover) {
        if (!AFE_3_DW.b_dispUpdated) {
          /* Outport: '<Root>/U16_AFEDispVal' incorporates:
           *  Outport: '<Root>/U16_CalAvgKmplAFE'
           */
          AFE_3_Y.U16_AFEDispVal = AFE_3_Y.U16_CalAvgKmplAFE;
          AFE_3_DW.b_dispUpdated = AFE_3_TRUE;
          AFE_3_DW.U32_AFEDispCnter = 0U;

          /* Outport: '<Root>/U16_AFEDispVal' */
          if (AFE_3_Y.U16_AFEDispVal > AFE_3_DW.u16_AfeMax) {
            AFE_3_Y.U16_AFEDispVal = AFE_3_DW.u16_AfeMax;
          } else {
            if (AFE_3_Y.U16_AFEDispVal < AFE_3_DW.u16_AfeMIn) {
              AFE_3_Y.U16_AFEDispVal = AFE_3_DW.u16_AfeMIn;
            }
          }
        } else {
          /* Constant: '<S3>/Constant1' */
          qY = AFE_3_DW.U32_AFEDispCnter + /*MW:OvSatOk*/ 20U;
          if (qY < AFE_3_DW.U32_AFEDispCnter) {
            qY = MAX_uint32_T;
          }

          AFE_3_DW.U32_AFEDispCnter = qY;
          guard1 = true;
        }
      } else {
        guard1 = true;
      }
    }

    if (guard1) {
      if (AFE_3_DW.U32_AFEDispCnter >= AFE_3_DW.U16_AFEDISP_CNT) {
        AFE_3_DW.U32_AFEDispCnter = 0U;

        /* Outport: '<Root>/U16_CalAvgKmplAFE' incorporates:
         *  Outport: '<Root>/U16_AFEDispVal'
         */
        if (AFE_3_Y.U16_CalAvgKmplAFE > AFE_3_Y.U16_AFEDispVal) {
          qY = (uint32_T)AFE_3_Y.U16_CalAvgKmplAFE - /*MW:OvSatOk*/
            AFE_3_Y.U16_AFEDispVal;
          if (qY > AFE_3_Y.U16_CalAvgKmplAFE) {
            qY = 0U;
          }

          if ((uint16_T)qY != 0) {
            if ((uint16_T)qY < AFE_3_DW.u16_AfeDispFilterA) {
              qY_tmp = AFE_3_Y.U16_AFEDispVal + 1U;
              if (qY_tmp > 65535U) {
                qY_tmp = 65535U;
              }

              AFE_3_Y.U16_AFEDispVal = (uint16_T)qY_tmp;
            } else if ((uint16_T)qY > AFE_3_DW.u16_AfeDispFilterB) {
              qY_tmp = AFE_3_Y.U16_AFEDispVal + 10U;
              if (qY_tmp > 65535U) {
                qY_tmp = 65535U;
              }

              AFE_3_Y.U16_AFEDispVal = (uint16_T)qY_tmp;
            } else {
              qY_tmp = AFE_3_Y.U16_AFEDispVal + 5U;
              if (qY_tmp > 65535U) {
                qY_tmp = 65535U;
              }

              AFE_3_Y.U16_AFEDispVal = (uint16_T)qY_tmp;
            }
          }
        } else {
          qY = (uint32_T)AFE_3_Y.U16_AFEDispVal - /*MW:OvSatOk*/
            AFE_3_Y.U16_CalAvgKmplAFE;
          if (qY > AFE_3_Y.U16_AFEDispVal) {
            qY = 0U;
          }

          if ((uint16_T)qY != 0) {
            if ((uint16_T)qY < AFE_3_DW.u16_AfeDispFilterA) {
              qY = AFE_3_Y.U16_AFEDispVal - /*MW:OvSatOk*/ 1U;
              if (qY > AFE_3_Y.U16_AFEDispVal) {
                qY = 0U;
              }

              AFE_3_Y.U16_AFEDispVal = (uint16_T)qY;
            } else if ((uint16_T)qY > AFE_3_DW.u16_AfeDispFilterB) {
              qY = AFE_3_Y.U16_AFEDispVal - /*MW:OvSatOk*/ 10U;
              if (qY > AFE_3_Y.U16_AFEDispVal) {
                qY = 0U;
              }

              AFE_3_Y.U16_AFEDispVal = (uint16_T)qY;
            } else {
              qY = AFE_3_Y.U16_AFEDispVal - /*MW:OvSatOk*/ 5U;
              if (qY > AFE_3_Y.U16_AFEDispVal) {
                qY = 0U;
              }

              AFE_3_Y.U16_AFEDispVal = (uint16_T)qY;
            }
          }
        }

        /* Outport: '<Root>/U16_AFEDispVal' */
        if (AFE_3_Y.U16_AFEDispVal < AFE_3_DW.u16_AfeMIn) {
          AFE_3_Y.U16_AFEDispVal = AFE_3_DW.u16_AfeMIn;
        } else {
          if (AFE_3_Y.U16_AFEDispVal > AFE_3_DW.u16_AfeMax) {
            AFE_3_Y.U16_AFEDispVal = AFE_3_DW.u16_AfeMax;
          }
        }
      }
    }
  } else {
    AFE_3_DW.U16_AFECalTimer = 0U;
    AFE_3_DW.U32_AFEDispCnter = 0U;
  }

  /* Outport: '<Root>/B_1Km_Dist_Flag' */
  AFE_3_Y.B_1Km_Dist_Flag = AFE_3_DW.b_1kmover;

  /* Outport: '<Root>/B_DispUpdated_Flag' */
  AFE_3_Y.B_DispUpdated_Flag = AFE_3_DW.b_dispUpdated;
  AFE_3_enter_internal_AFE_GD_CAL();
  AFE_3_DW.U32_DrvAccFuelConspPrev = 0U;

  /* Update for Inport: '<Root>/U8_IgnMode' incorporates:
   *  Inport: '<Root>/B_CanRxFlag'
   */
  if ((AFE_3_U.U8_IgnMode == 1) && AFE_3_U.B_CanRxFlag) {
    /* UnitDelay: '<S3>/Unit Delay' incorporates:
     *  Outport: '<Root>/U32_DrvAccFuelConsp'
     */
    if (AFE_3_DW.UnitDelay_DSTATE == 0) {
      AFE_3_Y.U32_DrvAccFuelConsp = 0U;
      AFE_3_DW.U32_DrvAccFuelConspPrev = 0U;
    }

    /* End of UnitDelay: '<S3>/Unit Delay' */

    /* Inport: '<Root>/U8_STS_Eng' */
    if (AFE_3_U.U8_STS_Eng == 2) {
      /* Inport: '<Root>/U16_FuelConsRate' */
      AFE_3_Y.U32_DrvAccFuelConsp = AFE_3_DW.U32_DrvAccFuelConspPrev +
        /*MW:OvSatOk*/ AFE_3_U.U16_FuelConsRate;
      if (AFE_3_Y.U32_DrvAccFuelConsp < AFE_3_DW.U32_DrvAccFuelConspPrev) {
        AFE_3_Y.U32_DrvAccFuelConsp = MAX_uint32_T;
      }

      /* Outport: '<Root>/U32_DrvAccFuelConsp' */
      AFE_3_DW.U32_DrvAccFuelConspPrev = AFE_3_Y.U32_DrvAccFuelConsp;
    }
  }
}

/* Model step function */
void AFE_3_step(void)
{
  boolean_T LogicalOperator1;
  real_T tmp;
  uint32_T qY;
  uint32_T qY_tmp;
  boolean_T guard1 = false;
  boolean_T guard2 = false;

  /* Outputs for Enabled SubSystem: '<S1>/Enabled Subsystem' incorporates:
   *  EnablePort: '<S2>/Enable'
   */
  /* Inport: '<Root>/B_EMSCode' */
  if (AFE_3_U.B_EMSCode) {
    /* Logic: '<S2>/Logical Operator1' incorporates:
     *  Inport: '<Root>/B_AFEResetTrg'
     *  Inport: '<Root>/B_ReqTripResetAFE'
     */
    LogicalOperator1 = (AFE_3_U.B_ReqTripResetAFE || AFE_3_U.B_AFEResetTrg);

    /* Outputs for Enabled SubSystem: '<S2>/Subsystem' incorporates:
     *  EnablePort: '<S3>/Enable'
     */
    /* Logic: '<S2>/Logical Operator' incorporates:
     *  Inport: '<Root>/U8_LHVSD'
     */
    if (!AFE_3_U.U8_LHVSD) {
      /* Chart: '<S3>/Cal_AFE' incorporates:
       *  Constant: '<S3>/Constant1'
       *  Inport: '<Root>/B_CanRxFlag'
       *  Inport: '<Root>/B_InfoResetAFE'
       *  Inport: '<Root>/U16_FuelConsRate'
       *  Inport: '<Root>/U32_OdoDisp'
       *  Inport: '<Root>/U8_IgnMode'
       *  Inport: '<Root>/U8_STS_Eng'
       *  Outport: '<Root>/U16_AFEDispVal'
       *  Outport: '<Root>/U16_AvgKmplAFE_GD'
       *  Outport: '<Root>/U16_CalAvgKmplAFE'
       *  Outport: '<Root>/U16_CalAvgKmplAFE_GD'
       *  Outport: '<Root>/U32_AccFuelConsp'
       *  Outport: '<Root>/U32_AccFuelConspGD'
       *  Outport: '<Root>/U32_DrvAccFuelConsp'
       *  Outport: '<Root>/U32_OdoDispGDReset'
       *  Outport: '<Root>/U32_OdoDispReset'
       *  UnitDelay: '<S3>/Unit Delay'
       */
      if (AFE_3_DW.is_active_c1_AFE_3 == 0U) {
        AFE_3_DW.is_active_c1_AFE_3 = 1U;
        AFE_3_enter_internal_c1_AFE_3(&LogicalOperator1);
      } else {
        if (LogicalOperator1) {
          /* Outport: '<Root>/U32_AccFuelConsp' */
          AFE_3_Y.U32_AccFuelConsp = 0U;
          AFE_3_DW.U32_AccFuelConspPrev = 0U;

          /* Outport: '<Root>/U32_AccFuelConspReset' */
          AFE_3_Y.U32_AccFuelConspReset = 0U;

          /* Outport: '<Root>/U32_OdoDispReset' incorporates:
           *  Inport: '<Root>/U32_OdoDisp'
           */
          AFE_3_Y.U32_OdoDispReset = AFE_3_U.U32_OdoDisp;

          /* Outport: '<Root>/U32_AccDistAFE' */
          AFE_3_Y.U32_AccDistAFE = 0U;

          /* Outport: '<Root>/U16_AFEDispVal' */
          AFE_3_Y.U16_AFEDispVal = 0U;

          /* Outport: '<Root>/B_AFEResetACK' */
          AFE_3_Y.B_AFEResetACK = AFE_3_TRUE;
          AFE_3_DW.b_dispUpdated = AFE_3_FALSE;
          AFE_3_DW.b_1kmover = AFE_3_FALSE;
        } else {
          /* Outport: '<Root>/B_AFEResetACK' */
          AFE_3_Y.B_AFEResetACK = AFE_3_FALSE;
        }

        if (AFE_3_U.U8_IgnMode == 1) {
          guard1 = false;
          guard2 = false;
          if ((AFE_3_U.U8_STS_Eng == 2) && AFE_3_U.B_CanRxFlag) {
            AFE_3_Y.U32_AccFuelConsp = AFE_3_DW.U32_AccFuelConspPrev +
              /*MW:OvSatOk*/ AFE_3_U.U16_FuelConsRate;
            if (AFE_3_Y.U32_AccFuelConsp < AFE_3_DW.U32_AccFuelConspPrev) {
              AFE_3_Y.U32_AccFuelConsp = MAX_uint32_T;
            }

            AFE_3_DW.U32_AccFuelConspPrev = AFE_3_Y.U32_AccFuelConsp;
            qY_tmp = AFE_3_DW.counter + 1U;
            if (qY_tmp > 65535U) {
              qY_tmp = 65535U;
            }

            AFE_3_DW.counter = (uint16_T)qY_tmp;
            qY_tmp = AFE_3_U.U32_OdoDisp - /*MW:OvSatOk*/
              AFE_3_Y.U32_OdoDispReset;
            qY = qY_tmp;
            if (qY_tmp > AFE_3_U.U32_OdoDisp) {
              qY = 0U;
            }

            if ((AFE_3_Y.U32_AccFuelConsp >= AFE_3_MAX_FUEL_ACC) || (qY >=
                 AFE_3_MAX_DIST_ACC)) {
              AFE_3_Y.U32_AccFuelConsp = AFE_3_rdivide_helper
                (AFE_3_Y.U32_AccFuelConsp, 2U);

              /* Outport: '<Root>/U32_AccFuelConspReset' */
              AFE_3_Y.U32_AccFuelConspReset = AFE_3_rdivide_helper
                (AFE_3_Y.U32_AccFuelConspReset, 2U);
              if (qY_tmp > AFE_3_U.U32_OdoDisp) {
                qY_tmp = 0U;
              }

              qY_tmp = AFE_3_rdivide_helper(qY_tmp, 2U);
              AFE_3_Y.U32_OdoDispReset += /*MW:OvSatOk*/ qY_tmp;
              if (AFE_3_Y.U32_OdoDispReset < qY_tmp) {
                AFE_3_Y.U32_OdoDispReset = MAX_uint32_T;
              }

              AFE_3_DW.U32_AccFuelConspPrev = AFE_3_Y.U32_AccFuelConsp;
            }

            guard2 = true;
          } else {
            if (AFE_3_U.U8_STS_Eng == 2) {
              guard2 = true;
            }
          }

          if (guard2) {
            if (AFE_3_U.U16_FuelConsRate >= 0) {
              qY_tmp = AFE_3_DW.U16_AFECalTimer + 20U;
              if (qY_tmp > 65535U) {
                qY_tmp = 65535U;
              }

              AFE_3_DW.U16_AFECalTimer = (uint16_T)qY_tmp;

              /* Outport: '<Root>/b_AFECalTimer' incorporates:
               *  Constant: '<S3>/Constant1'
               */
              AFE_3_Y.b_AFECalTimer = AFE_3_FALSE;
            }

            if (AFE_3_DW.U16_AFECalTimer >= AFE_3_DW.U16_AFE_CAL_INTERVAL) {
              AFE_3_DW.U16_AFECalTimer = 0U;

              /* Outport: '<Root>/b_AFECalTimer' */
              AFE_3_Y.b_AFECalTimer = AFE_3_TRUE;
              qY_tmp = AFE_3_U.U32_OdoDisp - /*MW:OvSatOk*/
                AFE_3_Y.U32_OdoDispReset;
              qY = qY_tmp;
              if (qY_tmp > AFE_3_U.U32_OdoDisp) {
                qY = 0U;
              }

              if (AFE_3_U.B_CanRxFlag && (qY >= AFE_3_DW.u16_AFE_DIST_TH)) {
                AFE_3_Y.U32_AccDistAFE = qY_tmp;
                if (qY_tmp > AFE_3_U.U32_OdoDisp) {
                  AFE_3_Y.U32_AccDistAFE = 0U;
                  qY_tmp = 0U;
                }

                tmp = rt_roundd_snf((real_T)qY_tmp * 10000.0 / (real_T)
                                    AFE_3_Y.U32_AccFuelConsp);
                if (tmp < 65536.0) {
                  if (tmp >= 0.0) {
                    /* Outport: '<Root>/U16_CalAvgKmplAFE' */
                    AFE_3_Y.U16_CalAvgKmplAFE = (uint16_T)tmp;
                  } else {
                    /* Outport: '<Root>/U16_CalAvgKmplAFE' */
                    AFE_3_Y.U16_CalAvgKmplAFE = 0U;
                  }
                } else {
                  /* Outport: '<Root>/U16_CalAvgKmplAFE' */
                  AFE_3_Y.U16_CalAvgKmplAFE = MAX_uint16_T;
                }

                AFE_3_DW.b_1kmover = AFE_3_TRUE;
              }
            }

            if (AFE_3_U.B_EnbleAFE && AFE_3_DW.b_1kmover) {
              if (!AFE_3_DW.b_dispUpdated) {
                /* Outport: '<Root>/U16_AFEDispVal' incorporates:
                 *  Outport: '<Root>/U16_CalAvgKmplAFE'
                 */
                AFE_3_Y.U16_AFEDispVal = AFE_3_Y.U16_CalAvgKmplAFE;
                AFE_3_DW.b_dispUpdated = AFE_3_TRUE;
                AFE_3_DW.U32_AFEDispCnter = 0U;
                if (AFE_3_Y.U16_AFEDispVal > AFE_3_DW.u16_AfeMax) {
                  AFE_3_Y.U16_AFEDispVal = AFE_3_DW.u16_AfeMax;
                } else {
                  if (AFE_3_Y.U16_AFEDispVal < AFE_3_DW.u16_AfeMIn) {
                    AFE_3_Y.U16_AFEDispVal = AFE_3_DW.u16_AfeMIn;
                  }
                }
              } else {
                qY = AFE_3_DW.U32_AFEDispCnter + /*MW:OvSatOk*/ 20U;
                if (qY < AFE_3_DW.U32_AFEDispCnter) {
                  qY = MAX_uint32_T;
                }

                AFE_3_DW.U32_AFEDispCnter = qY;
                guard1 = true;
              }
            } else {
              guard1 = true;
            }
          }

          if (guard1) {
            if (AFE_3_DW.U32_AFEDispCnter >= AFE_3_DW.U16_AFEDISP_CNT) {
              AFE_3_DW.U32_AFEDispCnter = 0U;
              if (AFE_3_Y.U16_CalAvgKmplAFE > AFE_3_Y.U16_AFEDispVal) {
                qY = (uint32_T)AFE_3_Y.U16_CalAvgKmplAFE - /*MW:OvSatOk*/
                  AFE_3_Y.U16_AFEDispVal;
                if (qY > AFE_3_Y.U16_CalAvgKmplAFE) {
                  qY = 0U;
                }

                if ((uint16_T)qY != 0) {
                  if ((uint16_T)qY < AFE_3_DW.u16_AfeDispFilterA) {
                    qY_tmp = AFE_3_Y.U16_AFEDispVal + 1U;
                    if (qY_tmp > 65535U) {
                      qY_tmp = 65535U;
                    }

                    AFE_3_Y.U16_AFEDispVal = (uint16_T)qY_tmp;
                  } else if ((uint16_T)qY > AFE_3_DW.u16_AfeDispFilterB) {
                    qY_tmp = AFE_3_Y.U16_AFEDispVal + 10U;
                    if (qY_tmp > 65535U) {
                      qY_tmp = 65535U;
                    }

                    AFE_3_Y.U16_AFEDispVal = (uint16_T)qY_tmp;
                  } else {
                    qY_tmp = AFE_3_Y.U16_AFEDispVal + 5U;
                    if (qY_tmp > 65535U) {
                      qY_tmp = 65535U;
                    }

                    AFE_3_Y.U16_AFEDispVal = (uint16_T)qY_tmp;
                  }
                }
              } else {
                qY = (uint32_T)AFE_3_Y.U16_AFEDispVal - /*MW:OvSatOk*/
                  AFE_3_Y.U16_CalAvgKmplAFE;
                if (qY > AFE_3_Y.U16_AFEDispVal) {
                  qY = 0U;
                }

                if ((uint16_T)qY != 0) {
                  if ((uint16_T)qY < AFE_3_DW.u16_AfeDispFilterA) {
                    qY = AFE_3_Y.U16_AFEDispVal - /*MW:OvSatOk*/ 1U;
                    if (qY > AFE_3_Y.U16_AFEDispVal) {
                      qY = 0U;
                    }

                    AFE_3_Y.U16_AFEDispVal = (uint16_T)qY;
                  } else if ((uint16_T)qY > AFE_3_DW.u16_AfeDispFilterB) {
                    qY = AFE_3_Y.U16_AFEDispVal - /*MW:OvSatOk*/ 10U;
                    if (qY > AFE_3_Y.U16_AFEDispVal) {
                      qY = 0U;
                    }

                    AFE_3_Y.U16_AFEDispVal = (uint16_T)qY;
                  } else {
                    qY = AFE_3_Y.U16_AFEDispVal - /*MW:OvSatOk*/ 5U;
                    if (qY > AFE_3_Y.U16_AFEDispVal) {
                      qY = 0U;
                    }

                    AFE_3_Y.U16_AFEDispVal = (uint16_T)qY;
                  }
                }
              }

              if (AFE_3_Y.U16_AFEDispVal < AFE_3_DW.u16_AfeMIn) {
                AFE_3_Y.U16_AFEDispVal = AFE_3_DW.u16_AfeMIn;
              } else {
                if (AFE_3_Y.U16_AFEDispVal > AFE_3_DW.u16_AfeMax) {
                  AFE_3_Y.U16_AFEDispVal = AFE_3_DW.u16_AfeMax;
                }
              }
            }
          }
        } else {
          AFE_3_DW.U16_AFECalTimer = 0U;
          AFE_3_DW.U32_AFEDispCnter = 0U;
        }

        /* Outport: '<Root>/B_1Km_Dist_Flag' incorporates:
         *  Constant: '<S3>/Constant1'
         *  Inport: '<Root>/B_CanRxFlag'
         *  Inport: '<Root>/U16_FuelConsRate'
         *  Inport: '<Root>/U32_OdoDisp'
         *  Inport: '<Root>/U8_IgnMode'
         *  Inport: '<Root>/U8_STS_Eng'
         *  Outport: '<Root>/U16_AFEDispVal'
         *  Outport: '<Root>/U16_CalAvgKmplAFE'
         *  Outport: '<Root>/U32_AccFuelConsp'
         *  Outport: '<Root>/U32_OdoDispReset'
         */
        AFE_3_Y.B_1Km_Dist_Flag = AFE_3_DW.b_1kmover;

        /* Outport: '<Root>/B_DispUpdated_Flag' */
        AFE_3_Y.B_DispUpdated_Flag = AFE_3_DW.b_dispUpdated;
        if (AFE_3_U.B_InfoResetAFE) {
          /* Outport: '<Root>/U32_AccFuelConspGD' */
          AFE_3_Y.U32_AccFuelConspGD = 0U;
          AFE_3_DW.u32_AccFuelConspGDPrev = 0U;

          /* Outport: '<Root>/U32_AccFuelConspGDReset' */
          AFE_3_Y.U32_AccFuelConspGDReset = 0U;

          /* Outport: '<Root>/U32_OdoDispGDReset' incorporates:
           *  Inport: '<Root>/U32_OdoDisp'
           */
          AFE_3_Y.U32_OdoDispGDReset = AFE_3_U.U32_OdoDisp;

          /* Outport: '<Root>/U16_AvgKmplAFE_GD' */
          AFE_3_Y.U16_AvgKmplAFE_GD = 0U;
          AFE_3_DW.b_dispUpdated_GD = AFE_3_FALSE;

          /* Outport: '<Root>/B_AFE_GD_ResetACK' */
          AFE_3_Y.B_AFE_GD_ResetACK = AFE_3_TRUE;

          /* Outport: '<Root>/U32_AccDistAFEGD' */
          AFE_3_Y.U32_AccDistAFEGD = 0U;
          AFE_3_DW.b_1kmover_GD = AFE_3_FALSE;
        } else {
          /* Outport: '<Root>/B_AFE_GD_ResetACK' */
          AFE_3_Y.B_AFE_GD_ResetACK = AFE_3_FALSE;
        }

        if (AFE_3_U.U8_IgnMode == 1) {
          guard1 = false;
          guard2 = false;
          if ((AFE_3_U.U8_STS_Eng == 2) && AFE_3_U.B_CanRxFlag) {
            AFE_3_Y.U32_AccFuelConspGD = AFE_3_DW.u32_AccFuelConspGDPrev +
              /*MW:OvSatOk*/ AFE_3_U.U16_FuelConsRate;
            if (AFE_3_Y.U32_AccFuelConspGD < AFE_3_DW.u32_AccFuelConspGDPrev) {
              AFE_3_Y.U32_AccFuelConspGD = MAX_uint32_T;
            }

            AFE_3_DW.u32_AccFuelConspGDPrev = AFE_3_Y.U32_AccFuelConspGD;
            qY = AFE_3_U.U32_OdoDisp - /*MW:OvSatOk*/ AFE_3_Y.U32_OdoDispGDReset;
            if (qY > AFE_3_U.U32_OdoDisp) {
              qY = 0U;
            }

            if ((AFE_3_Y.U32_AccFuelConspGD >= AFE_3_MAX_FUEL_ACC) || (qY >=
                 AFE_3_MAX_DIST_ACC)) {
              AFE_3_Y.U32_AccFuelConspGD = AFE_3_rdivide_helper
                (AFE_3_Y.U32_AccFuelConspGD, 2U);

              /* Outport: '<Root>/U32_AccFuelConspGDReset' */
              AFE_3_Y.U32_AccFuelConspGDReset = AFE_3_rdivide_helper
                (AFE_3_Y.U32_AccFuelConspGDReset, 2U);
              qY = AFE_3_U.U32_OdoDisp - /*MW:OvSatOk*/
                AFE_3_Y.U32_OdoDispGDReset;
              if (qY > AFE_3_U.U32_OdoDisp) {
                qY = 0U;
              }

              qY_tmp = AFE_3_rdivide_helper(qY, 2U);
              AFE_3_Y.U32_OdoDispGDReset += /*MW:OvSatOk*/ qY_tmp;
              if (AFE_3_Y.U32_OdoDispGDReset < qY_tmp) {
                AFE_3_Y.U32_OdoDispGDReset = MAX_uint32_T;
              }

              AFE_3_DW.u32_AccFuelConspGDPrev = AFE_3_Y.U32_AccFuelConspGD;
            }

            guard2 = true;
          } else {
            if (AFE_3_U.U8_STS_Eng == 2) {
              guard2 = true;
            }
          }

          if (guard2) {
            if (AFE_3_U.U16_FuelConsRate >= 0) {
              qY_tmp = AFE_3_DW.U16_AFE_GD_CalTimer + 20U;
              if (qY_tmp > 65535U) {
                qY_tmp = 65535U;
              }

              AFE_3_DW.U16_AFE_GD_CalTimer = (uint16_T)qY_tmp;
            }

            if (AFE_3_DW.U16_AFE_GD_CalTimer >= AFE_3_DW.U16_AFE_CAL_INTERVAL) {
              AFE_3_DW.U16_AFE_GD_CalTimer = 0U;
              qY_tmp = AFE_3_U.U32_OdoDisp - /*MW:OvSatOk*/
                AFE_3_Y.U32_OdoDispGDReset;
              qY = qY_tmp;
              if (qY_tmp > AFE_3_U.U32_OdoDisp) {
                qY = 0U;
              }

              if (AFE_3_U.B_CanRxFlag && (qY >= AFE_3_DW.u16_AFE_DIST_TH)) {
                AFE_3_Y.U32_AccDistAFEGD = qY_tmp;
                if (qY_tmp > AFE_3_U.U32_OdoDisp) {
                  AFE_3_Y.U32_AccDistAFEGD = 0U;
                  qY_tmp = 0U;
                }

                tmp = rt_roundd_snf((real_T)qY_tmp * 10000.0 / (real_T)
                                    AFE_3_Y.U32_AccFuelConspGD);
                if (tmp < 65536.0) {
                  if (tmp >= 0.0) {
                    /* Outport: '<Root>/U16_CalAvgKmplAFE_GD' */
                    AFE_3_Y.U16_CalAvgKmplAFE_GD = (uint16_T)tmp;
                  } else {
                    /* Outport: '<Root>/U16_CalAvgKmplAFE_GD' */
                    AFE_3_Y.U16_CalAvgKmplAFE_GD = 0U;
                  }
                } else {
                  /* Outport: '<Root>/U16_CalAvgKmplAFE_GD' */
                  AFE_3_Y.U16_CalAvgKmplAFE_GD = MAX_uint16_T;
                }

                AFE_3_DW.b_1kmover_GD = AFE_3_TRUE;
              }
            }

            if (AFE_3_DW.b_dispUpdated_GD) {
              AFE_3_DW.b_1kmover_GD = AFE_3_TRUE;
            }

            if (AFE_3_U.B_EnbleAFE && AFE_3_DW.b_1kmover_GD) {
              if (!AFE_3_DW.b_dispUpdated_GD) {
                /* Outport: '<Root>/U16_AvgKmplAFE_GD' incorporates:
                 *  Outport: '<Root>/U16_CalAvgKmplAFE_GD'
                 */
                AFE_3_Y.U16_AvgKmplAFE_GD = AFE_3_Y.U16_CalAvgKmplAFE_GD;
                AFE_3_DW.b_dispUpdated_GD = AFE_3_TRUE;
                AFE_3_DW.U32_AFE_GD_DispCnter = 0U;
                if (AFE_3_Y.U16_AvgKmplAFE_GD > AFE_3_DW.u16_AfeMax) {
                  AFE_3_Y.U16_AvgKmplAFE_GD = AFE_3_DW.u16_AfeMax;
                } else {
                  if (AFE_3_Y.U16_AvgKmplAFE_GD < AFE_3_DW.u16_AfeMIn) {
                    AFE_3_Y.U16_AvgKmplAFE_GD = AFE_3_DW.u16_AfeMIn;
                  }
                }
              } else {
                qY = AFE_3_DW.U32_AFE_GD_DispCnter + /*MW:OvSatOk*/ 20U;
                if (qY < AFE_3_DW.U32_AFE_GD_DispCnter) {
                  qY = MAX_uint32_T;
                }

                AFE_3_DW.U32_AFE_GD_DispCnter = qY;
                guard1 = true;
              }
            } else {
              guard1 = true;
            }
          }

          if (guard1) {
            if (AFE_3_DW.U32_AFE_GD_DispCnter >= AFE_3_DW.U16_AFEDISP_CNT) {
              AFE_3_DW.U32_AFE_GD_DispCnter = 0U;
              if (AFE_3_Y.U16_CalAvgKmplAFE_GD > AFE_3_Y.U16_AvgKmplAFE_GD) {
                qY = (uint32_T)AFE_3_Y.U16_CalAvgKmplAFE_GD - /*MW:OvSatOk*/
                  AFE_3_Y.U16_AvgKmplAFE_GD;
                if (qY > AFE_3_Y.U16_CalAvgKmplAFE_GD) {
                  qY = 0U;
                }

                if ((uint16_T)qY != 0) {
                  if ((uint16_T)qY < AFE_3_DW.u16_AfeDispFilterA) {
                    qY_tmp = AFE_3_Y.U16_AvgKmplAFE_GD + 1U;
                    if (qY_tmp > 65535U) {
                      qY_tmp = 65535U;
                    }

                    AFE_3_Y.U16_AvgKmplAFE_GD = (uint16_T)qY_tmp;
                  } else if ((uint16_T)qY > AFE_3_DW.u16_AfeDispFilterB) {
                    qY_tmp = AFE_3_Y.U16_AvgKmplAFE_GD + 10U;
                    if (qY_tmp > 65535U) {
                      qY_tmp = 65535U;
                    }

                    AFE_3_Y.U16_AvgKmplAFE_GD = (uint16_T)qY_tmp;
                  } else {
                    qY_tmp = AFE_3_Y.U16_AvgKmplAFE_GD + 5U;
                    if (qY_tmp > 65535U) {
                      qY_tmp = 65535U;
                    }

                    AFE_3_Y.U16_AvgKmplAFE_GD = (uint16_T)qY_tmp;
                  }
                }
              } else {
                qY = (uint32_T)AFE_3_Y.U16_AvgKmplAFE_GD - /*MW:OvSatOk*/
                  AFE_3_Y.U16_CalAvgKmplAFE_GD;
                if (qY > AFE_3_Y.U16_AvgKmplAFE_GD) {
                  qY = 0U;
                }

                if ((uint16_T)qY != 0) {
                  if ((uint16_T)qY < AFE_3_DW.u16_AfeDispFilterA) {
                    qY = AFE_3_Y.U16_AvgKmplAFE_GD - /*MW:OvSatOk*/ 1U;
                    if (qY > AFE_3_Y.U16_AvgKmplAFE_GD) {
                      qY = 0U;
                    }

                    AFE_3_Y.U16_AvgKmplAFE_GD = (uint16_T)qY;
                  } else if ((uint16_T)qY > AFE_3_DW.u16_AfeDispFilterB) {
                    qY = AFE_3_Y.U16_AvgKmplAFE_GD - /*MW:OvSatOk*/ 10U;
                    if (qY > AFE_3_Y.U16_AvgKmplAFE_GD) {
                      qY = 0U;
                    }

                    AFE_3_Y.U16_AvgKmplAFE_GD = (uint16_T)qY;
                  } else {
                    qY = AFE_3_Y.U16_AvgKmplAFE_GD - /*MW:OvSatOk*/ 5U;
                    if (qY > AFE_3_Y.U16_AvgKmplAFE_GD) {
                      qY = 0U;
                    }

                    AFE_3_Y.U16_AvgKmplAFE_GD = (uint16_T)qY;
                  }
                }
              }

              if (AFE_3_Y.U16_AvgKmplAFE_GD < AFE_3_DW.u16_AfeMIn) {
                AFE_3_Y.U16_AvgKmplAFE_GD = AFE_3_DW.u16_AfeMIn;
              } else {
                if (AFE_3_Y.U16_AvgKmplAFE_GD > AFE_3_DW.u16_AfeMax) {
                  AFE_3_Y.U16_AvgKmplAFE_GD = AFE_3_DW.u16_AfeMax;
                }
              }
            }
          }
        } else {
          AFE_3_DW.U16_AFE_GD_CalTimer = 0U;
          AFE_3_DW.U32_AFE_GD_DispCnter = 0U;
        }

        /* Outport: '<Root>/B_1Km_Dist_Flag_GD' incorporates:
         *  Constant: '<S3>/Constant1'
         *  Inport: '<Root>/B_CanRxFlag'
         *  Inport: '<Root>/B_InfoResetAFE'
         *  Inport: '<Root>/U16_FuelConsRate'
         *  Inport: '<Root>/U32_OdoDisp'
         *  Inport: '<Root>/U8_IgnMode'
         *  Inport: '<Root>/U8_STS_Eng'
         *  Outport: '<Root>/U16_AvgKmplAFE_GD'
         *  Outport: '<Root>/U16_CalAvgKmplAFE_GD'
         *  Outport: '<Root>/U32_AccFuelConspGD'
         *  Outport: '<Root>/U32_OdoDispGDReset'
         */
        AFE_3_Y.B_1Km_Dist_Flag_GD = AFE_3_DW.b_1kmover_GD;

        /* Outport: '<Root>/B_DispUpdated_GD_Flag' */
        AFE_3_Y.B_DispUpdated_GD_Flag = AFE_3_DW.b_dispUpdated_GD;
        if ((AFE_3_U.U8_IgnMode == 1) && AFE_3_U.B_CanRxFlag) {
          if (AFE_3_DW.UnitDelay_DSTATE == 0) {
            /* Outport: '<Root>/U32_DrvAccFuelConsp' */
            AFE_3_Y.U32_DrvAccFuelConsp = 0U;
            AFE_3_DW.U32_DrvAccFuelConspPrev = 0U;
          }

          if (AFE_3_U.U8_STS_Eng == 2) {
            AFE_3_Y.U32_DrvAccFuelConsp = AFE_3_DW.U32_DrvAccFuelConspPrev +
              /*MW:OvSatOk*/ AFE_3_U.U16_FuelConsRate;
            if (AFE_3_Y.U32_DrvAccFuelConsp < AFE_3_DW.U32_DrvAccFuelConspPrev)
            {
              AFE_3_Y.U32_DrvAccFuelConsp = MAX_uint32_T;
            }

            AFE_3_DW.U32_DrvAccFuelConspPrev = AFE_3_Y.U32_DrvAccFuelConsp;
          }
        }
      }

      /* End of Chart: '<S3>/Cal_AFE' */

      /* Update for UnitDelay: '<S3>/Unit Delay' incorporates:
       *  Inport: '<Root>/U8_IgnMode'
       */
      AFE_3_DW.UnitDelay_DSTATE = AFE_3_U.U8_IgnMode;
    }

    /* End of Logic: '<S2>/Logical Operator' */
    /* End of Outputs for SubSystem: '<S2>/Subsystem' */
  }

  /* End of Inport: '<Root>/B_EMSCode' */
  /* End of Outputs for SubSystem: '<S1>/Enabled Subsystem' */
}

/* Model initialize function */
void AFE_3_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(AFE_3_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&AFE_3_DW, 0,
                sizeof(DW_AFE_3_T));

  /* external inputs */
  (void)memset(&AFE_3_U, 0, sizeof(ExtU_AFE_3_T));

  /* external outputs */
  (void) memset((void *)&AFE_3_Y, 0,
                sizeof(ExtY_AFE_3_T));

  /* SystemInitialize for Enabled SubSystem: '<S1>/Enabled Subsystem' */
  /* SystemInitialize for Enabled SubSystem: '<S2>/Subsystem' */
  /* SystemInitialize for Chart: '<S3>/Cal_AFE' */
  AFE_3_DW.is_active_c1_AFE_3 = 0U;
  AFE_3_DW.U16_AFE_CAL_INTERVAL = 0U;
  AFE_3_DW.b_dispUpdated = true;
  AFE_3_DW.U32_AFEDispCnter = 0U;
  AFE_3_DW.U16_AFEDISP_CNT = 0U;
  AFE_3_DW.b_dispUpdated_GD = false;
  AFE_3_DW.U16_AFE_GD_CalTimer = 0U;
  AFE_3_DW.U32_AFE_GD_DispCnter = 0U;
  AFE_3_DW.b_1kmover_GD = false;

  /* InitializeConditions for Outport: '<Root>/U32_AccFuelConspReset' incorporates:
   *  Chart: '<S3>/Cal_AFE'
   */
  AFE_3_Y.U32_AccFuelConspReset = 0U;

  /* InitializeConditions for Outport: '<Root>/U32_OdoDispReset' incorporates:
   *  Chart: '<S3>/Cal_AFE'
   */
  AFE_3_Y.U32_OdoDispReset = 0U;

  /* InitializeConditions for Outport: '<Root>/U32_OdoDispGDReset' incorporates:
   *  Chart: '<S3>/Cal_AFE'
   */
  AFE_3_Y.U32_OdoDispGDReset = 0U;

  /* InitializeConditions for Outport: '<Root>/U32_AccFuelConspGDReset' incorporates:
   *  Chart: '<S3>/Cal_AFE'
   */
  AFE_3_Y.U32_AccFuelConspGDReset = 0U;

  /* InitializeConditions for Outport: '<Root>/B_1Km_Dist_Flag' incorporates:
   *  Chart: '<S3>/Cal_AFE'
   */
  AFE_3_Y.B_1Km_Dist_Flag = false;

  /* InitializeConditions for Outport: '<Root>/B_DispUpdated_Flag' incorporates:
   *  Chart: '<S3>/Cal_AFE'
   */
  AFE_3_Y.B_DispUpdated_Flag = false;

  /* InitializeConditions for Outport: '<Root>/B_DispUpdated_GD_Flag' incorporates:
   *  Chart: '<S3>/Cal_AFE'
   */
  AFE_3_Y.B_DispUpdated_GD_Flag = false;

  /* InitializeConditions for Outport: '<Root>/B_AFE_GD_ResetACK' incorporates:
   *  Chart: '<S3>/Cal_AFE'
   */
  AFE_3_Y.B_AFE_GD_ResetACK = false;

  /* InitializeConditions for Outport: '<Root>/B_1Km_Dist_Flag_GD' incorporates:
   *  Chart: '<S3>/Cal_AFE'
   */
  AFE_3_Y.B_1Km_Dist_Flag_GD = false;

  /* End of SystemInitialize for SubSystem: '<S2>/Subsystem' */
  /* End of SystemInitialize for SubSystem: '<S1>/Enabled Subsystem' */
}

/* Model terminate function */
void AFE_3_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

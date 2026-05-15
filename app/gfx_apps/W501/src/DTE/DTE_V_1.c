/*
 * File: DTE_V_1.c
 *
 * Code generated for Simulink model 'DTE_V_1'.
 *
 * Model version                  : 1.264
 * Simulink Coder version         : 9.0 (R2018b) 24-May-2018
 * C/C++ source code generated on : Fri Jul 17 16:16:29 2020
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Renesas->RH850
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 *    3. MISRA C:2012 guidelines
 *    4. Polyspace
 *    5. Debugging
 *    6. Safety precaution
 *    7. Traceability
 *    8. ROM efficiency
 * Validation result: Not run
 */

#include "DTE_V_1.h"
#include "rdivide_helper_Q3oa226R.h"

/* Named constants for Chart: '<S1>/Chart' */
#define B_FALSE                        (false)
#define B_TRUE                         (true)
#define S_CLEAR                        (0.0F)
#define U32_CLEAR                      (0U)
#define U8_CLEAR                       ((uint8_T)0U)
#define U8_FUEL_DMP_SAMP_DIST          ((uint8_T)100U)
#define U8_FUEL_SAMP_CNT               ((uint8_T)10U)

/* Block signals and states (default storage) */
DW rtDW;

/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Real-time model */
RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;

/* Forward declaration for local functions */
static void inner_default_DTE_Fuel_Damp_Mng(void);
static void inner_default_DTE_Cal6(void);

/* Function for Chart: '<S1>/Chart' */
static void inner_default_DTE_Fuel_Damp_Mng(void)
{
  int32_T tmp;
  uint32_T qY;

  /* Inport: '<Root>/U8_IgnMode' */
  if (((int32_T)rtU.U8_IgnMode) == 1) {
    /* Inport: '<Root>/B_LHVSD' */
    if (!rtU.B_LHVSD) {
      /* Inport: '<Root>/B_VAR_CODE_EMS' incorporates:
       *  Inport: '<Root>/B_FEATR_CODE_DTE_IN'
       */
      if ((rtU.B_VAR_CODE_EMS) && (rtU.B_FEATR_CODE_DTE_IN)) {
        /* Inport: '<Root>/B_DTE_Reset_Trig' */
        if (!rtU.B_DTE_Reset_Trig) {
          /* Inport: '<Root>/B_DTE_CAN_Msg_Misng' */
          if (!rtU.B_DTE_CAN_Msg_Misng) {
            /* Inport: '<Root>/U8_STS_Eng' */
            if (((int32_T)rtU.U8_STS_Eng) == 2) {
              /* Inport: '<Root>/U32_OdoDisp' */
              qY = rtU.U32_OdoDisp - /*MW:OvSatOk*/ rtDW.u32_PrevOdoDisp;
              if (qY > rtU.U32_OdoDisp) {
                qY = 0U;
              }

              /* Inport: '<Root>/U16_VehSpeed' */
              if ((qY != 0U) && (((int32_T)rtU.U16_VehSpeed) != 0)) {
                /* Inport: '<Root>/U32_OdoDisp' */
                qY = rtU.U32_OdoDisp - /*MW:OvSatOk*/ rtDW.u32_Fuel_PrevOdoDisp;
                if (qY > rtU.U32_OdoDisp) {
                  qY = 0U;
                }

                if (qY >= U8_FUEL_DMP_SAMP_DIST) {
                  /* Outport: '<Root>/S_Fuel_Level_Acc' incorporates:
                   *  Inport: '<Root>/S_Fuel_Level_Ind'
                   */
                  rtY.S_Fuel_Level_Acc += rtU.S_Fuel_Level_Ind;
                  tmp = (int32_T)((uint32_T)(((uint32_T)rtDW.u8_Fuel_Sample_Cnt)
                    + 1U));
                  if (((uint32_T)tmp) > 255U) {
                    tmp = 255;
                  }

                  rtDW.u8_Fuel_Sample_Cnt = (uint8_T)tmp;

                  /* Inport: '<Root>/U32_OdoDisp' */
                  rtDW.u32_Fuel_PrevOdoDisp = rtU.U32_OdoDisp;
                  if (((uint32_T)rtDW.u8_Fuel_Sample_Cnt) >= U8_FUEL_SAMP_CNT) {
                    /* Outport: '<Root>/S_Fuel_Level_Avg' incorporates:
                     *  Outport: '<Root>/S_Fuel_Level_Acc'
                     */
                    rtY.S_Fuel_Level_Avg = rtY.S_Fuel_Level_Acc / 10.0F;

                    /* Outport: '<Root>/S_Fuel_Level_Acc' */
                    rtY.S_Fuel_Level_Acc = S_CLEAR;
                    rtDW.u8_Fuel_Sample_Cnt = U8_CLEAR;

                    /* Outport: '<Root>/S_Fuel_Level_Avg' incorporates:
                     *  Inport: '<Root>/S_Fuel_Level_Ind'
                     */
                    if (rtY.S_Fuel_Level_Avg == 0.0F) {
                      rtY.S_Fuel_Level_Avg = rtU.S_Fuel_Level_Ind;
                    }
                  }
                } else {
                  /* Outport: '<Root>/S_Fuel_Level_Avg' incorporates:
                   *  Inport: '<Root>/S_Fuel_Level_Ind'
                   */
                  rtY.S_Fuel_Level_Avg = rtU.S_Fuel_Level_Ind;
                }
              } else {
                /* Outport: '<Root>/S_Fuel_Level_Avg' incorporates:
                 *  Inport: '<Root>/S_Fuel_Level_Ind'
                 */
                rtY.S_Fuel_Level_Avg = rtU.S_Fuel_Level_Ind;
              }

              /* End of Inport: '<Root>/U16_VehSpeed' */
            } else {
              /* Inport: '<Root>/U32_OdoDisp' */
              rtDW.u32_Fuel_PrevOdoDisp = rtU.U32_OdoDisp;
            }

            /* End of Inport: '<Root>/U8_STS_Eng' */
          }

          /* End of Inport: '<Root>/B_DTE_CAN_Msg_Misng' */
        } else {
          /* Inport: '<Root>/U32_OdoDisp' */
          rtDW.u32_Fuel_PrevOdoDisp = rtU.U32_OdoDisp;
          rtDW.u8_Fuel_Sample_Cnt = U8_CLEAR;

          /* Outport: '<Root>/S_Fuel_Level_Acc' */
          rtY.S_Fuel_Level_Acc = S_CLEAR;
        }

        /* End of Inport: '<Root>/B_DTE_Reset_Trig' */
      } else {
        /* Inport: '<Root>/U32_OdoDisp' */
        rtDW.u32_Fuel_PrevOdoDisp = rtU.U32_OdoDisp;
      }

      /* End of Inport: '<Root>/B_VAR_CODE_EMS' */
    } else {
      /* Inport: '<Root>/U32_OdoDisp' */
      rtDW.u32_Fuel_PrevOdoDisp = rtU.U32_OdoDisp;
    }

    /* End of Inport: '<Root>/B_LHVSD' */
  } else {
    /* Inport: '<Root>/U32_OdoDisp' */
    rtDW.u32_Fuel_PrevOdoDisp = rtU.U32_OdoDisp;
  }

  /* End of Inport: '<Root>/U8_IgnMode' */
}

/* Function for Chart: '<S1>/Chart' */
static void inner_default_DTE_Cal6(void)
{
  real32_T s_DTE_RAFE_Inst_1km_AvgVal;
  uint32_T u32_DTE_FC_Acc_2nd_500m;
  uint8_T u8_RAF_Cntr;
  uint32_T q0;
  int32_T tmp;
  int32_T tmp_0;
  uint32_T qY;
  int32_T s_LoDTE_RAFE_Inst_1km_Roll_tmp;
  uint32_T qY_tmp;
  real32_T S_Inst_1km_DTE_tmp;
  boolean_T guard1 = false;
  int32_T exitg1;

  /* Inport: '<Root>/U8_IgnMode' */
  guard1 = false;
  if (((int32_T)rtU.U8_IgnMode) == 1) {
    if (!rtDW.b_First_IgnON_State) {
      /* Outport: '<Root>/S_DTE_1km_Completion_Flag' incorporates:
       *  Inport: '<Root>/B_EEPROM_DTE_1km_Compn_Flag'
       *  Inport: '<Root>/B_EE_Odo_500m_Flag'
       *  Inport: '<Root>/B_EE_shift_RAFE_Roll'
       *  Inport: '<Root>/S_EEPROM_RAFE_Read'
       *  Inport: '<Root>/S_EE_Last_Caltd_DTE'
       *  Inport: '<Root>/U32_EE_500mRoll_FC_Acc'
       *  Inport: '<Root>/U32_EE_DTE_FC_Acc_1st_500m'
       *  Inport: '<Root>/U32_EE_DTE_Iddle_FC_Acc'
       *  Inport: '<Root>/U32_EE_DTE_PrevOdoDisp'
       *  Inport: '<Root>/U32_OdoDisp'
       *  Inport: '<Root>/U8_Default_RAFE'
       *  Inport: '<Root>/U8_EE_DTE_RAFE_Inst_1km_Cnt'
       *  Inport: '<Root>/U8_RAFE_MAX_ROL_DIST'
       *  Outport: '<Root>/S_DTE_1km'
       *  Outport: '<Root>/S_DTE_Disp_Val'
       *  Outport: '<Root>/S_DTE_RAFE_Avg_Val'
       *  Outport: '<Root>/S_DTE_RAFE_Inst_1km_Roll'
       *  Outport: '<Root>/S_Last_Caltd_DTE'
       *  Outport: '<Root>/S_Prev_1km_DTE'
       *  Outport: '<Root>/U8_DTE_RAFE_Inst_1km_Cnt'
       */
      rtY.S_DTE_1km_Completion_Flag = rtU.B_EEPROM_DTE_1km_Compn_Flag;
      if (rtY.S_DTE_1km_Completion_Flag) {
        rtY.S_DTE_1km = rtU.S_EE_Last_Caltd_DTE;
        rtY.S_Last_Caltd_DTE = rtU.S_EE_Last_Caltd_DTE;
        rtDW.u32_PrevOdoDisp = rtU.U32_EE_DTE_PrevOdoDisp;
        rtY.U8_DTE_RAFE_Inst_1km_Cnt = rtU.U8_EE_DTE_RAFE_Inst_1km_Cnt;
        rtDW.b_shift_RAFE_Roll = rtU.B_EE_shift_RAFE_Roll;
        rtDW.u32_DTE_Iddle_FC_Acc = rtU.U32_EE_DTE_Iddle_FC_Acc;
        rtDW.u32_DTE_FC_Acc_1st_500m = rtU.U32_EE_DTE_FC_Acc_1st_500m;
        rtDW.u32_500mRoll_FC_Acc = rtU.U32_EE_500mRoll_FC_Acc;
        rtDW.b_RAFE_500m = rtU.B_EE_Odo_500m_Flag;
        rtY.S_DTE_Disp_Val = rtU.S_EE_Last_Caltd_DTE;
        rtY.S_Prev_1km_DTE = rtU.S_EE_Last_Caltd_DTE;
        rtDW.b_First_IgnON_State = B_TRUE;
        if (!rtDW.b_EEPROM_Window_Read) {
          u8_RAF_Cntr = 1U;
          while ((((int32_T)u8_RAF_Cntr) == 1) || (u8_RAF_Cntr <=
                  rtU.U8_RAFE_MAX_ROL_DIST)) {
            s_LoDTE_RAFE_Inst_1km_Roll_tmp = ((int32_T)u8_RAF_Cntr) - 1;
            rtDW.s_LoDTE_RAFE_Inst_1km_Roll[s_LoDTE_RAFE_Inst_1km_Roll_tmp] =
              rtU.S_EEPROM_RAFE_Read[s_LoDTE_RAFE_Inst_1km_Roll_tmp];
            rtY.S_DTE_RAFE_Inst_1km_Roll[s_LoDTE_RAFE_Inst_1km_Roll_tmp] =
              rtU.S_EEPROM_RAFE_Read[((int32_T)u8_RAF_Cntr) - 1];
            rtDW.s_RAFE_EE_ACC +=
              rtDW.s_LoDTE_RAFE_Inst_1km_Roll[s_LoDTE_RAFE_Inst_1km_Roll_tmp];
            s_LoDTE_RAFE_Inst_1km_Roll_tmp = (int32_T)((uint32_T)(((uint32_T)
              u8_RAF_Cntr) + 1U));
            if (((uint32_T)s_LoDTE_RAFE_Inst_1km_Roll_tmp) > 255U) {
              s_LoDTE_RAFE_Inst_1km_Roll_tmp = 255;
            }

            u8_RAF_Cntr = (uint8_T)s_LoDTE_RAFE_Inst_1km_Roll_tmp;
          }

          rtY.S_DTE_RAFE_Avg_Val = rtDW.s_RAFE_EE_ACC / ((real32_T)
            rtU.U8_RAFE_MAX_ROL_DIST);
          rtDW.s_RAFE_EE_ACC = 0.0F;
          rtDW.b_EEPROM_Window_Read = B_TRUE;
        }
      } else {
        if (!rtDW.b_Default_RAFE_Load_Status) {
          u8_RAF_Cntr = 1U;
          while ((((int32_T)u8_RAF_Cntr) == 1) || (u8_RAF_Cntr <=
                  rtU.U8_RAFE_MAX_ROL_DIST)) {
            s_LoDTE_RAFE_Inst_1km_Roll_tmp = ((int32_T)u8_RAF_Cntr) - 1;
            rtDW.s_LoDTE_RAFE_Inst_1km_Roll[s_LoDTE_RAFE_Inst_1km_Roll_tmp] =
              (real32_T)rtU.U8_Default_RAFE;
            rtY.S_DTE_RAFE_Inst_1km_Roll[s_LoDTE_RAFE_Inst_1km_Roll_tmp] =
              (real32_T)rtU.U8_Default_RAFE;
            s_LoDTE_RAFE_Inst_1km_Roll_tmp = (int32_T)((uint32_T)(((uint32_T)
              u8_RAF_Cntr) + 1U));
            if (((uint32_T)s_LoDTE_RAFE_Inst_1km_Roll_tmp) > 255U) {
              s_LoDTE_RAFE_Inst_1km_Roll_tmp = 255;
            }

            u8_RAF_Cntr = (uint8_T)s_LoDTE_RAFE_Inst_1km_Roll_tmp;
          }

          rtDW.u32_PrevOdoDisp = rtU.U32_OdoDisp;
          rtDW.b_Default_RAFE_Load_Status = B_TRUE;
          rtY.S_DTE_RAFE_Avg_Val = (real32_T)rtU.U8_Default_RAFE;
        }
      }

      rtDW.b_First_IgnON_State = B_TRUE;
    }

    if (!rtU.B_LHVSD) {
      if ((rtU.B_VAR_CODE_EMS) && (rtU.B_FEATR_CODE_DTE_IN)) {
        if (!rtU.B_DTE_Reset_Trig) {
          /* Outport: '<Root>/S_DTE_1km_Completion_Flag' incorporates:
           *  Inport: '<Root>/S_Fuel_Dead_Vol'
           *  Inport: '<Root>/S_Fuel_Level_Ind'
           *  Inport: '<Root>/U32_OdoDisp'
           *  Inport: '<Root>/U8_Default_RAFE'
           *  Outport: '<Root>/S_DTE_1km'
           *  Outport: '<Root>/S_DTE_Disp_Val'
           *  Outport: '<Root>/S_Last_Caltd_DTE'
           */
          if ((!rtY.S_DTE_1km_Completion_Flag) && (!rtDW.b_Disp_Deflt_DTE)) {
            rtY.S_DTE_1km = (rtU.S_Fuel_Level_Ind - rtU.S_Fuel_Dead_Vol) *
              ((real32_T)rtU.U8_Default_RAFE);
            rtY.S_DTE_Disp_Val = rtY.S_DTE_1km;
            rtY.S_Last_Caltd_DTE = rtY.S_DTE_Disp_Val;
            rtDW.u32_PrevOdoDisp = rtU.U32_OdoDisp;
            rtDW.b_Disp_Deflt_DTE = B_TRUE;
          }

          /* Inport: '<Root>/B_DTE_CAN_Msg_Misng' incorporates:
           *  Inport: '<Root>/U16_FuelConsRate'
           *  Inport: '<Root>/U8_STS_Eng'
           */
          if ((!rtU.B_DTE_CAN_Msg_Misng) && ((((int32_T)rtU.U8_STS_Eng) == 2) &&
               (((int32_T)rtU.U16_FuelConsRate) != 0))) {
            /* Inport: '<Root>/U32_OdoDisp' */
            qY_tmp = rtU.U32_OdoDisp - /*MW:OvSatOk*/ rtDW.u32_PrevOdoDisp;
            qY = qY_tmp;

            /* Inport: '<Root>/U32_OdoDisp' */
            if (qY_tmp > rtU.U32_OdoDisp) {
              qY = 0U;
            }

            /* Inport: '<Root>/U16_VehSpeed' */
            if ((qY != 0U) && (((int32_T)rtU.U16_VehSpeed) != 0)) {
              qY = rtDW.u32_DTE_FC_Acc + /*MW:OvSatOk*/ ((uint32_T)
                rtU.U16_FuelConsRate);
              if (qY < rtDW.u32_DTE_FC_Acc) {
                qY = MAX_uint32_T;
              }

              rtDW.u32_DTE_FC_Acc = qY;
              qY = rtDW.u32_500mRoll_FC_Acc + /*MW:OvSatOk*/ ((uint32_T)
                rtU.U16_FuelConsRate);
              if (qY < rtDW.u32_500mRoll_FC_Acc) {
                qY = MAX_uint32_T;
              }

              rtDW.u32_500mRoll_FC_Acc = qY;

              /* Outport: '<Root>/U32_O_500mRoll_FC_Acc' */
              rtY.U32_O_500mRoll_FC_Acc = rtDW.u32_500mRoll_FC_Acc;
              qY = qY_tmp;

              /* Inport: '<Root>/U32_OdoDisp' */
              if (qY_tmp > rtU.U32_OdoDisp) {
                qY = 0U;
              }

              if (qY >= 1000U) {
                u32_DTE_FC_Acc_2nd_500m = rtDW.u32_500mRoll_FC_Acc;

                /* Inport: '<Root>/U32_OdoDisp' */
                if (qY_tmp > rtU.U32_OdoDisp) {
                  qY_tmp = 0U;
                }

                s_DTE_RAFE_Inst_1km_AvgVal = (((real32_T)qY_tmp) * 1000.0F) /
                  ((real32_T)rtDW.u32_500mRoll_FC_Acc);

                /* Outport: '<Root>/U8_DTE_RAFE_Inst_1km_Cnt' */
                s_LoDTE_RAFE_Inst_1km_Roll_tmp = (int32_T)((uint32_T)(((uint32_T)
                  rtY.U8_DTE_RAFE_Inst_1km_Cnt) + 1U));
                if (((uint32_T)s_LoDTE_RAFE_Inst_1km_Roll_tmp) > 255U) {
                  s_LoDTE_RAFE_Inst_1km_Roll_tmp = 255;
                }

                /* Outport: '<Root>/U8_DTE_RAFE_Inst_1km_Cnt' */
                rtY.U8_DTE_RAFE_Inst_1km_Cnt = (uint8_T)
                  s_LoDTE_RAFE_Inst_1km_Roll_tmp;

                /* Outport: '<Root>/S_Cur_1km_DTE_RAFE_Val' */
                rtY.S_Cur_1km_DTE_RAFE_Val = s_DTE_RAFE_Inst_1km_AvgVal;
                if ((s_DTE_RAFE_Inst_1km_AvgVal == 0.0F) ||
                    (s_DTE_RAFE_Inst_1km_AvgVal >= 50.0F)) {
                  /* Inport: '<Root>/U8_Default_RAFE' */
                  s_DTE_RAFE_Inst_1km_AvgVal = (real32_T)rtU.U8_Default_RAFE;
                }

                /* Outport: '<Root>/S_Fuel_Level_Avg' incorporates:
                 *  Inport: '<Root>/S_Fuel_Dead_Vol'
                 */
                S_Inst_1km_DTE_tmp = rtY.S_Fuel_Level_Avg - rtU.S_Fuel_Dead_Vol;

                /* Outport: '<Root>/S_Inst_1km_DTE' */
                rtY.S_Inst_1km_DTE = S_Inst_1km_DTE_tmp *
                  s_DTE_RAFE_Inst_1km_AvgVal;

                /* Outport: '<Root>/U8_DTE_RAFE_Inst_1km_Cnt' */
                rtDW.s_LoDTE_RAFE_Inst_1km_Roll[((int32_T)
                  rtY.U8_DTE_RAFE_Inst_1km_Cnt) - 1] =
                  s_DTE_RAFE_Inst_1km_AvgVal;

                /* Outport: '<Root>/S_DTE_1km_Completion_Flag' */
                rtY.S_DTE_1km_Completion_Flag = B_TRUE;
                rtDW.u32_500mRoll_FC_Acc = 0U;

                /* Inport: '<Root>/U32_OdoDisp' */
                rtDW.u32_PrevOdoDisp = rtU.U32_OdoDisp;

                /* Outport: '<Root>/S_DTE_1km_Completion_Flag' incorporates:
                 *  Inport: '<Root>/U8_DTE_FILTER_A'
                 *  Inport: '<Root>/U8_DTE_FILTER_B'
                 *  Inport: '<Root>/U8_DTE_FILTER_C'
                 *  Inport: '<Root>/U8_Default_RAFE'
                 *  Inport: '<Root>/U8_RAFE_MAX_ROL_DIST'
                 *  Outport: '<Root>/B_DTE_RAFE_Shift'
                 *  Outport: '<Root>/B_Odo_500m_Flag'
                 *  Outport: '<Root>/S_DTE_1km'
                 *  Outport: '<Root>/S_DTE_Disp_Val'
                 *  Outport: '<Root>/S_DTE_RAFE_Avg_Val'
                 *  Outport: '<Root>/S_DTE_RAFE_Inst_1km_Roll'
                 *  Outport: '<Root>/S_Last_Caltd_DTE'
                 *  Outport: '<Root>/S_Prev_1km_DTE'
                 *  Outport: '<Root>/U32_DTE_Prev_Odo'
                 *  Outport: '<Root>/U32_Idle_DTE_FC_Acc'
                 *  Outport: '<Root>/U32_O_500mRoll_FC_Acc'
                 *  Outport: '<Root>/U32_O_DTE_FC_Acc_1st_500m'
                 *  Outport: '<Root>/U32_O_DTE_FC_Acc_2nd_500m'
                 *  Outport: '<Root>/U8_DTE_RAFE_Inst_1km_Cnt'
                 */
                if (rtY.S_DTE_1km_Completion_Flag) {
                  if ((rtY.U8_DTE_RAFE_Inst_1km_Cnt == rtDW.u8_RAFE_MAX_ROL_DIST)
                      && (!rtDW.b_shift_RAFE_Roll)) {
                    rtDW.b_shift_RAFE_Roll = B_TRUE;
                  } else {
                    s_LoDTE_RAFE_Inst_1km_Roll_tmp = (int32_T)((uint32_T)
                      (((uint32_T)rtDW.u8_RAFE_MAX_ROL_DIST) + 1U));
                    tmp_0 = s_LoDTE_RAFE_Inst_1km_Roll_tmp;
                    if (((uint32_T)s_LoDTE_RAFE_Inst_1km_Roll_tmp) > 255U) {
                      tmp_0 = 255;
                    }

                    if ((rtDW.b_shift_RAFE_Roll) && (((int32_T)
                          rtY.U8_DTE_RAFE_Inst_1km_Cnt) >= tmp_0)) {
                      do {
                        exitg1 = 0;
                        tmp_0 = s_LoDTE_RAFE_Inst_1km_Roll_tmp;
                        if (((uint32_T)s_LoDTE_RAFE_Inst_1km_Roll_tmp) > 255U) {
                          tmp_0 = 255;
                        }

                        if ((((int32_T)rtDW.u8_RAF_Shift_Cntr) == 1) ||
                            (((int32_T)rtDW.u8_RAF_Shift_Cntr) <= tmp_0)) {
                          tmp_0 = (int32_T)((uint32_T)(((uint32_T)
                            rtDW.u8_RAF_Shift_Cntr) + 1U));
                          tmp = tmp_0;
                          if (((uint32_T)tmp_0) > 255U) {
                            tmp = 255;
                            tmp_0 = 255;
                          }

                          rtDW.s_LoDTE_RAFE_Inst_1km_Roll[((int32_T)
                            rtDW.u8_RAF_Shift_Cntr) - 1] =
                            rtDW.s_LoDTE_RAFE_Inst_1km_Roll[tmp - 1];
                          rtDW.u8_RAF_Shift_Cntr = (uint8_T)tmp_0;
                        } else {
                          exitg1 = 1;
                        }
                      } while (exitg1 == 0);

                      rtDW.u8_RAF_Shift_Cntr = 1U;
                      rtY.U8_DTE_RAFE_Inst_1km_Cnt = rtDW.u8_RAFE_MAX_ROL_DIST;
                    }
                  }

                  u8_RAF_Cntr = 1U;
                  while ((((int32_T)u8_RAF_Cntr) == 1) || (u8_RAF_Cntr <=
                          rtDW.u8_RAFE_MAX_ROL_DIST)) {
                    rtDW.s_DTE_RAFE_Acc_Val += rtDW.s_LoDTE_RAFE_Inst_1km_Roll
                      [((int32_T)u8_RAF_Cntr) - 1];
                    s_LoDTE_RAFE_Inst_1km_Roll_tmp = (int32_T)((uint32_T)
                      (((uint32_T)u8_RAF_Cntr) + 1U));
                    if (((uint32_T)s_LoDTE_RAFE_Inst_1km_Roll_tmp) > 255U) {
                      s_LoDTE_RAFE_Inst_1km_Roll_tmp = 255;
                    }

                    u8_RAF_Cntr = (uint8_T)s_LoDTE_RAFE_Inst_1km_Roll_tmp;
                  }

                  rtY.S_DTE_RAFE_Avg_Val = rtDW.s_DTE_RAFE_Acc_Val / ((real32_T)
                    rtDW.u8_RAFE_MAX_ROL_DIST);
                  rtDW.s_DTE_RAFE_Acc_Val = 0.0F;
                  if ((rtY.S_DTE_RAFE_Avg_Val == 0.0F) ||
                      (rtY.S_DTE_RAFE_Avg_Val >= 50.0F)) {
                    rtY.S_DTE_RAFE_Avg_Val = (real32_T)rtU.U8_Default_RAFE;
                  }

                  rtY.S_DTE_1km = S_Inst_1km_DTE_tmp * rtY.S_DTE_RAFE_Avg_Val;
                  if (((int32_T)rtY.U8_DTE_RAFE_Inst_1km_Cnt) > 1) {
                    rtY.S_Last_Caltd_DTE = rtY.S_DTE_1km;
                    s_DTE_RAFE_Inst_1km_AvgVal = roundf(rtY.S_Prev_1km_DTE);
                    S_Inst_1km_DTE_tmp = roundf(rtY.S_DTE_1km);
                    if (s_DTE_RAFE_Inst_1km_AvgVal < 4.2949673E+9F) {
                      if (s_DTE_RAFE_Inst_1km_AvgVal >= 0.0F) {
                        qY_tmp = (uint32_T)s_DTE_RAFE_Inst_1km_AvgVal;
                      } else {
                        qY_tmp = 0U;
                      }
                    } else {
                      qY_tmp = MAX_uint32_T;
                    }

                    if (S_Inst_1km_DTE_tmp < 4.2949673E+9F) {
                      if (S_Inst_1km_DTE_tmp >= 0.0F) {
                        qY = (uint32_T)S_Inst_1km_DTE_tmp;
                      } else {
                        qY = 0U;
                      }
                    } else {
                      qY = MAX_uint32_T;
                    }

                    if (qY_tmp > qY) {
                      if (s_DTE_RAFE_Inst_1km_AvgVal < 4.2949673E+9F) {
                        if (s_DTE_RAFE_Inst_1km_AvgVal >= 0.0F) {
                          q0 = (uint32_T)s_DTE_RAFE_Inst_1km_AvgVal;
                        } else {
                          q0 = 0U;
                        }
                      } else {
                        q0 = MAX_uint32_T;
                      }

                      S_Inst_1km_DTE_tmp = roundf(rtY.S_DTE_1km);
                      if (S_Inst_1km_DTE_tmp < 4.2949673E+9F) {
                        if (S_Inst_1km_DTE_tmp >= 0.0F) {
                          qY_tmp = (uint32_T)S_Inst_1km_DTE_tmp;
                        } else {
                          qY_tmp = 0U;
                        }
                      } else {
                        qY_tmp = MAX_uint32_T;
                      }

                      qY = q0 - /*MW:OvSatOk*/ qY_tmp;
                      if (qY > q0) {
                        qY = 0U;
                      }

                      if (qY != 0U) {
                        if (s_DTE_RAFE_Inst_1km_AvgVal < 4.2949673E+9F) {
                          if (s_DTE_RAFE_Inst_1km_AvgVal >= 0.0F) {
                            q0 = (uint32_T)s_DTE_RAFE_Inst_1km_AvgVal;
                          } else {
                            q0 = 0U;
                          }
                        } else {
                          q0 = MAX_uint32_T;
                        }

                        S_Inst_1km_DTE_tmp = roundf(rtY.S_DTE_1km);
                        if (S_Inst_1km_DTE_tmp < 4.2949673E+9F) {
                          if (S_Inst_1km_DTE_tmp >= 0.0F) {
                            qY_tmp = (uint32_T)S_Inst_1km_DTE_tmp;
                          } else {
                            qY_tmp = 0U;
                          }
                        } else {
                          qY_tmp = MAX_uint32_T;
                        }

                        qY = q0 - /*MW:OvSatOk*/ qY_tmp;
                        if (qY > q0) {
                          qY = 0U;
                        }

                        if (qY < ((uint32_T)rtU.U8_DTE_FILTER_A)) {
                          rtY.S_Prev_1km_DTE--;
                        } else {
                          if (s_DTE_RAFE_Inst_1km_AvgVal < 4.2949673E+9F) {
                            if (s_DTE_RAFE_Inst_1km_AvgVal >= 0.0F) {
                              q0 = (uint32_T)s_DTE_RAFE_Inst_1km_AvgVal;
                            } else {
                              q0 = 0U;
                            }
                          } else {
                            q0 = MAX_uint32_T;
                          }

                          S_Inst_1km_DTE_tmp = roundf(rtY.S_DTE_1km);
                          if (S_Inst_1km_DTE_tmp < 4.2949673E+9F) {
                            if (S_Inst_1km_DTE_tmp >= 0.0F) {
                              qY_tmp = (uint32_T)S_Inst_1km_DTE_tmp;
                            } else {
                              qY_tmp = 0U;
                            }
                          } else {
                            qY_tmp = MAX_uint32_T;
                          }

                          qY = q0 - /*MW:OvSatOk*/ qY_tmp;
                          if (qY > q0) {
                            qY = 0U;
                          }

                          if (s_DTE_RAFE_Inst_1km_AvgVal < 4.2949673E+9F) {
                            if (s_DTE_RAFE_Inst_1km_AvgVal >= 0.0F) {
                              q0 = (uint32_T)s_DTE_RAFE_Inst_1km_AvgVal;
                            } else {
                              q0 = 0U;
                            }
                          } else {
                            q0 = MAX_uint32_T;
                          }

                          S_Inst_1km_DTE_tmp = roundf(rtY.S_DTE_1km);
                          if (S_Inst_1km_DTE_tmp < 4.2949673E+9F) {
                            if (S_Inst_1km_DTE_tmp >= 0.0F) {
                              qY_tmp = (uint32_T)S_Inst_1km_DTE_tmp;
                            } else {
                              qY_tmp = 0U;
                            }
                          } else {
                            qY_tmp = MAX_uint32_T;
                          }

                          qY_tmp = q0 - /*MW:OvSatOk*/ qY_tmp;
                          if (qY_tmp > q0) {
                            qY_tmp = 0U;
                          }

                          if ((qY > ((uint32_T)rtU.U8_DTE_FILTER_A)) && (qY_tmp <
                               ((uint32_T)rtU.U8_DTE_FILTER_B))) {
                            rtY.S_Prev_1km_DTE -= 2.0F;
                          } else {
                            if (s_DTE_RAFE_Inst_1km_AvgVal < 4.2949673E+9F) {
                              if (s_DTE_RAFE_Inst_1km_AvgVal >= 0.0F) {
                                q0 = (uint32_T)s_DTE_RAFE_Inst_1km_AvgVal;
                              } else {
                                q0 = 0U;
                              }
                            } else {
                              q0 = MAX_uint32_T;
                            }

                            s_DTE_RAFE_Inst_1km_AvgVal = roundf(rtY.S_DTE_1km);
                            if (s_DTE_RAFE_Inst_1km_AvgVal < 4.2949673E+9F) {
                              if (s_DTE_RAFE_Inst_1km_AvgVal >= 0.0F) {
                                qY_tmp = (uint32_T)s_DTE_RAFE_Inst_1km_AvgVal;
                              } else {
                                qY_tmp = 0U;
                              }
                            } else {
                              qY_tmp = MAX_uint32_T;
                            }

                            qY = q0 - /*MW:OvSatOk*/ qY_tmp;
                            if (qY > q0) {
                              qY = 0U;
                            }

                            if (qY > ((uint32_T)rtU.U8_DTE_FILTER_B)) {
                              rtY.S_Prev_1km_DTE -= 4.0F;
                            }
                          }
                        }
                      }
                    } else {
                      S_Inst_1km_DTE_tmp = roundf(rtY.S_DTE_1km);
                      if (S_Inst_1km_DTE_tmp < 4.2949673E+9F) {
                        if (S_Inst_1km_DTE_tmp >= 0.0F) {
                          q0 = (uint32_T)S_Inst_1km_DTE_tmp;
                        } else {
                          q0 = 0U;
                        }
                      } else {
                        q0 = MAX_uint32_T;
                      }

                      if (s_DTE_RAFE_Inst_1km_AvgVal < 4.2949673E+9F) {
                        if (s_DTE_RAFE_Inst_1km_AvgVal >= 0.0F) {
                          qY_tmp = (uint32_T)s_DTE_RAFE_Inst_1km_AvgVal;
                        } else {
                          qY_tmp = 0U;
                        }
                      } else {
                        qY_tmp = MAX_uint32_T;
                      }

                      qY = q0 - /*MW:OvSatOk*/ qY_tmp;
                      if (qY > q0) {
                        qY = 0U;
                      }

                      if (qY != 0U) {
                        S_Inst_1km_DTE_tmp = roundf(rtY.S_DTE_1km);
                        if (S_Inst_1km_DTE_tmp < 4.2949673E+9F) {
                          if (S_Inst_1km_DTE_tmp >= 0.0F) {
                            q0 = (uint32_T)S_Inst_1km_DTE_tmp;
                          } else {
                            q0 = 0U;
                          }
                        } else {
                          q0 = MAX_uint32_T;
                        }

                        if (s_DTE_RAFE_Inst_1km_AvgVal < 4.2949673E+9F) {
                          if (s_DTE_RAFE_Inst_1km_AvgVal >= 0.0F) {
                            qY_tmp = (uint32_T)s_DTE_RAFE_Inst_1km_AvgVal;
                          } else {
                            qY_tmp = 0U;
                          }
                        } else {
                          qY_tmp = MAX_uint32_T;
                        }

                        qY = q0 - /*MW:OvSatOk*/ qY_tmp;
                        if (qY > q0) {
                          qY = 0U;
                        }

                        if (qY < ((uint32_T)rtU.U8_DTE_FILTER_A)) {
                          rtY.S_Prev_1km_DTE = rtY.S_DTE_1km;
                        } else {
                          S_Inst_1km_DTE_tmp = roundf(rtY.S_DTE_1km);
                          if (S_Inst_1km_DTE_tmp < 4.2949673E+9F) {
                            if (S_Inst_1km_DTE_tmp >= 0.0F) {
                              q0 = (uint32_T)S_Inst_1km_DTE_tmp;
                            } else {
                              q0 = 0U;
                            }
                          } else {
                            q0 = MAX_uint32_T;
                          }

                          if (s_DTE_RAFE_Inst_1km_AvgVal < 4.2949673E+9F) {
                            if (s_DTE_RAFE_Inst_1km_AvgVal >= 0.0F) {
                              qY_tmp = (uint32_T)s_DTE_RAFE_Inst_1km_AvgVal;
                            } else {
                              qY_tmp = 0U;
                            }
                          } else {
                            qY_tmp = MAX_uint32_T;
                          }

                          qY = q0 - /*MW:OvSatOk*/ qY_tmp;
                          if (qY > q0) {
                            qY = 0U;
                          }

                          S_Inst_1km_DTE_tmp = roundf(rtY.S_DTE_1km);
                          if (S_Inst_1km_DTE_tmp < 4.2949673E+9F) {
                            if (S_Inst_1km_DTE_tmp >= 0.0F) {
                              q0 = (uint32_T)S_Inst_1km_DTE_tmp;
                            } else {
                              q0 = 0U;
                            }
                          } else {
                            q0 = MAX_uint32_T;
                          }

                          if (s_DTE_RAFE_Inst_1km_AvgVal < 4.2949673E+9F) {
                            if (s_DTE_RAFE_Inst_1km_AvgVal >= 0.0F) {
                              qY_tmp = (uint32_T)s_DTE_RAFE_Inst_1km_AvgVal;
                            } else {
                              qY_tmp = 0U;
                            }
                          } else {
                            qY_tmp = MAX_uint32_T;
                          }

                          qY_tmp = q0 - /*MW:OvSatOk*/ qY_tmp;
                          if (qY_tmp > q0) {
                            qY_tmp = 0U;
                          }

                          if ((qY > ((uint32_T)rtU.U8_DTE_FILTER_A)) && (qY_tmp <
                               ((uint32_T)rtU.U8_DTE_FILTER_B))) {
                            rtY.S_Prev_1km_DTE++;
                          } else {
                            S_Inst_1km_DTE_tmp = roundf(rtY.S_DTE_1km);
                            if (S_Inst_1km_DTE_tmp < 4.2949673E+9F) {
                              if (S_Inst_1km_DTE_tmp >= 0.0F) {
                                q0 = (uint32_T)S_Inst_1km_DTE_tmp;
                              } else {
                                q0 = 0U;
                              }
                            } else {
                              q0 = MAX_uint32_T;
                            }

                            if (s_DTE_RAFE_Inst_1km_AvgVal < 4.2949673E+9F) {
                              if (s_DTE_RAFE_Inst_1km_AvgVal >= 0.0F) {
                                qY_tmp = (uint32_T)s_DTE_RAFE_Inst_1km_AvgVal;
                              } else {
                                qY_tmp = 0U;
                              }
                            } else {
                              qY_tmp = MAX_uint32_T;
                            }

                            qY = q0 - /*MW:OvSatOk*/ qY_tmp;
                            if (qY > q0) {
                              qY = 0U;
                            }

                            S_Inst_1km_DTE_tmp = roundf(rtY.S_DTE_1km);
                            if (S_Inst_1km_DTE_tmp < 4.2949673E+9F) {
                              if (S_Inst_1km_DTE_tmp >= 0.0F) {
                                q0 = (uint32_T)S_Inst_1km_DTE_tmp;
                              } else {
                                q0 = 0U;
                              }
                            } else {
                              q0 = MAX_uint32_T;
                            }

                            if (s_DTE_RAFE_Inst_1km_AvgVal < 4.2949673E+9F) {
                              if (s_DTE_RAFE_Inst_1km_AvgVal >= 0.0F) {
                                qY_tmp = (uint32_T)s_DTE_RAFE_Inst_1km_AvgVal;
                              } else {
                                qY_tmp = 0U;
                              }
                            } else {
                              qY_tmp = MAX_uint32_T;
                            }

                            qY_tmp = q0 - /*MW:OvSatOk*/ qY_tmp;
                            if (qY_tmp > q0) {
                              qY_tmp = 0U;
                            }

                            if ((qY > ((uint32_T)rtU.U8_DTE_FILTER_B)) &&
                                (qY_tmp < ((uint32_T)rtU.U8_DTE_FILTER_C))) {
                              rtY.S_Prev_1km_DTE += 2.0F;
                            } else {
                              S_Inst_1km_DTE_tmp = roundf(rtY.S_DTE_1km);
                              if (S_Inst_1km_DTE_tmp < 4.2949673E+9F) {
                                if (S_Inst_1km_DTE_tmp >= 0.0F) {
                                  q0 = (uint32_T)S_Inst_1km_DTE_tmp;
                                } else {
                                  q0 = 0U;
                                }
                              } else {
                                q0 = MAX_uint32_T;
                              }

                              if (s_DTE_RAFE_Inst_1km_AvgVal < 4.2949673E+9F) {
                                if (s_DTE_RAFE_Inst_1km_AvgVal >= 0.0F) {
                                  qY_tmp = (uint32_T)s_DTE_RAFE_Inst_1km_AvgVal;
                                } else {
                                  qY_tmp = 0U;
                                }
                              } else {
                                qY_tmp = MAX_uint32_T;
                              }

                              qY = q0 - /*MW:OvSatOk*/ qY_tmp;
                              if (qY > q0) {
                                qY = 0U;
                              }

                              if (qY > ((uint32_T)rtU.U8_DTE_FILTER_C)) {
                                rtY.S_Prev_1km_DTE += 4.0F;
                              }
                            }
                          }
                        }
                      }
                    }

                    rtY.S_DTE_Disp_Val = rtY.S_Prev_1km_DTE;
                  } else {
                    rtY.S_Prev_1km_DTE = rtY.S_DTE_1km;
                    rtY.S_DTE_Disp_Val = rtY.S_Prev_1km_DTE;
                    rtY.S_Last_Caltd_DTE = rtY.S_DTE_1km;
                  }

                  u8_RAF_Cntr = 1U;
                  while ((((int32_T)u8_RAF_Cntr) == 1) || (u8_RAF_Cntr <=
                          rtDW.u8_RAFE_MAX_ROL_DIST)) {
                    s_LoDTE_RAFE_Inst_1km_Roll_tmp = ((int32_T)u8_RAF_Cntr) - 1;
                    rtY.S_DTE_RAFE_Inst_1km_Roll[s_LoDTE_RAFE_Inst_1km_Roll_tmp]
                      =
                      rtDW.s_LoDTE_RAFE_Inst_1km_Roll[s_LoDTE_RAFE_Inst_1km_Roll_tmp];
                    s_LoDTE_RAFE_Inst_1km_Roll_tmp = (int32_T)((uint32_T)
                      (((uint32_T)u8_RAF_Cntr) + 1U));
                    if (((uint32_T)s_LoDTE_RAFE_Inst_1km_Roll_tmp) > 255U) {
                      s_LoDTE_RAFE_Inst_1km_Roll_tmp = 255;
                    }

                    u8_RAF_Cntr = (uint8_T)s_LoDTE_RAFE_Inst_1km_Roll_tmp;
                  }

                  rtDW.u8_RAFE_MAX_ROL_DIST = rtU.U8_RAFE_MAX_ROL_DIST;
                  rtY.U32_DTE_Prev_Odo = rtDW.u32_PrevOdoDisp;
                  rtY.U32_Idle_DTE_FC_Acc = rtDW.u32_DTE_Iddle_FC_Acc;
                  rtY.U32_O_DTE_FC_Acc_1st_500m = rtDW.u32_DTE_FC_Acc_1st_500m;
                  rtY.U32_O_DTE_FC_Acc_2nd_500m = u32_DTE_FC_Acc_2nd_500m;
                  rtY.U32_O_500mRoll_FC_Acc = rtDW.u32_500mRoll_FC_Acc;
                  rtY.B_Odo_500m_Flag = rtDW.b_RAFE_500m;
                  rtY.B_DTE_RAFE_Shift = rtDW.b_shift_RAFE_Roll;
                  rtY.S_Last_Caltd_DTE = rtY.S_Prev_1km_DTE;
                }
              }
            } else {
              qY = rtDW.u32_500mRoll_FC_Acc + /*MW:OvSatOk*/ ((uint32_T)
                rtU.U16_FuelConsRate);
              if (qY < rtDW.u32_500mRoll_FC_Acc) {
                qY = MAX_uint32_T;
              }

              rtDW.u32_500mRoll_FC_Acc = qY;

              /* Outport: '<Root>/U32_Idle_DTE_FC_Acc' */
              rtY.U32_Idle_DTE_FC_Acc = rtDW.u32_500mRoll_FC_Acc;

              /* Outport: '<Root>/U32_O_500mRoll_FC_Acc' */
              rtY.U32_O_500mRoll_FC_Acc = rtDW.u32_500mRoll_FC_Acc;

              /* Inport: '<Root>/U8_Default_RAFE' */
              if (((real32_T)rdivide_helper_Q3oa226R(rtDW.u32_500mRoll_FC_Acc,
                    1000U)) >= (1000.0F / ((real32_T)rtU.U8_Default_RAFE))) {
                /* Outport: '<Root>/S_DTE_1km_Completion_Flag' incorporates:
                 *  Inport: '<Root>/U32_OdoDisp'
                 *  Outport: '<Root>/S_DTE_1km'
                 *  Outport: '<Root>/S_DTE_Disp_Val'
                 *  Outport: '<Root>/S_Last_Caltd_DTE'
                 *  Outport: '<Root>/S_Prev_1km_DTE'
                 *  Outport: '<Root>/U32_DTE_Prev_Odo'
                 *  Outport: '<Root>/U32_Idle_DTE_FC_Acc'
                 *  Outport: '<Root>/U32_O_500mRoll_FC_Acc'
                 */
                if (!rtY.S_DTE_1km_Completion_Flag) {
                  rtY.S_Last_Caltd_DTE = rtY.S_DTE_Disp_Val;
                  rtDW.u32_500mRoll_FC_Acc = U32_CLEAR;
                  rtY.U32_Idle_DTE_FC_Acc = rtDW.u32_500mRoll_FC_Acc;
                  rtY.U32_O_500mRoll_FC_Acc = rtDW.u32_500mRoll_FC_Acc;
                } else {
                  rtDW.u32_PrevOdoDisp = rtU.U32_OdoDisp;
                  rtY.S_DTE_Disp_Val--;
                  rtY.S_Prev_1km_DTE = rtY.S_DTE_Disp_Val;
                  rtY.S_DTE_1km--;
                  rtDW.u32_500mRoll_FC_Acc = U32_CLEAR;
                  rtY.U32_Idle_DTE_FC_Acc = rtDW.u32_500mRoll_FC_Acc;
                  rtY.U32_DTE_Prev_Odo = rtDW.u32_PrevOdoDisp;
                  rtY.S_Last_Caltd_DTE = rtY.S_Prev_1km_DTE;
                  rtY.U32_O_500mRoll_FC_Acc = rtDW.u32_500mRoll_FC_Acc;
                }
              }
            }
          }

          /* End of Inport: '<Root>/B_DTE_CAN_Msg_Misng' */
          guard1 = true;
        } else {
          u8_RAF_Cntr = 1U;

          /* Inport: '<Root>/U8_RAFE_MAX_ROL_DIST' */
          while ((((int32_T)u8_RAF_Cntr) == 1) || (u8_RAF_Cntr <=
                  rtU.U8_RAFE_MAX_ROL_DIST)) {
            /* Inport: '<Root>/U8_Default_RAFE' */
            rtDW.s_LoDTE_RAFE_Inst_1km_Roll[((int32_T)u8_RAF_Cntr) - 1] =
              (real32_T)rtU.U8_Default_RAFE;
            s_LoDTE_RAFE_Inst_1km_Roll_tmp = (int32_T)((uint32_T)(((uint32_T)
              u8_RAF_Cntr) + 1U));
            if (((uint32_T)s_LoDTE_RAFE_Inst_1km_Roll_tmp) > 255U) {
              s_LoDTE_RAFE_Inst_1km_Roll_tmp = 255;
            }

            u8_RAF_Cntr = (uint8_T)s_LoDTE_RAFE_Inst_1km_Roll_tmp;
          }

          u8_RAF_Cntr = 1U;
          while ((((int32_T)u8_RAF_Cntr) == 1) || (u8_RAF_Cntr <=
                  rtDW.u8_RAFE_MAX_ROL_DIST)) {
            s_LoDTE_RAFE_Inst_1km_Roll_tmp = ((int32_T)u8_RAF_Cntr) - 1;

            /* Outport: '<Root>/S_DTE_RAFE_Inst_1km_Roll' */
            rtY.S_DTE_RAFE_Inst_1km_Roll[s_LoDTE_RAFE_Inst_1km_Roll_tmp] =
              rtDW.s_LoDTE_RAFE_Inst_1km_Roll[s_LoDTE_RAFE_Inst_1km_Roll_tmp];
            s_LoDTE_RAFE_Inst_1km_Roll_tmp = (int32_T)((uint32_T)(((uint32_T)
              u8_RAF_Cntr) + 1U));
            if (((uint32_T)s_LoDTE_RAFE_Inst_1km_Roll_tmp) > 255U) {
              s_LoDTE_RAFE_Inst_1km_Roll_tmp = 255;
            }

            u8_RAF_Cntr = (uint8_T)s_LoDTE_RAFE_Inst_1km_Roll_tmp;
          }

          rtDW.b_EEPROM_Window_Read = B_FALSE;
          rtDW.b_shift_RAFE_Roll = B_FALSE;

          /* Outport: '<Root>/S_DTE_1km_Completion_Flag' */
          rtY.S_DTE_1km_Completion_Flag = B_FALSE;

          /* Inport: '<Root>/U32_OdoDisp' */
          rtDW.u32_PrevOdoDisp = rtU.U32_OdoDisp;
          rtDW.b_Default_RAFE_Load_Status = B_FALSE;
          rtDW.u32_DTE_FC_Acc = 0U;
          rtDW.u32_500mRoll_FC_Acc = 0U;

          /* Outport: '<Root>/U8_DTE_RAFE_Inst_1km_Cnt' */
          rtY.U8_DTE_RAFE_Inst_1km_Cnt = 0U;

          /* Outport: '<Root>/S_DTE_Disp_Val' incorporates:
           *  Inport: '<Root>/S_Fuel_Dead_Vol'
           *  Inport: '<Root>/S_Fuel_Level_Ind'
           *  Inport: '<Root>/U8_Default_RAFE'
           */
          rtY.S_DTE_Disp_Val = (rtU.S_Fuel_Level_Ind - rtU.S_Fuel_Dead_Vol) *
            ((real32_T)rtU.U8_Default_RAFE);
          rtDW.u32_DTE_FC_Acc_1st_500m = 0U;

          /* Outport: '<Root>/S_Prev_1km_DTE' */
          rtY.S_Prev_1km_DTE = 0.0F;

          /* Outport: '<Root>/S_Last_Caltd_DTE' */
          rtY.S_Last_Caltd_DTE = 0.0F;

          /* Outport: '<Root>/S_DTE_1km' */
          rtY.S_DTE_1km = 0.0F;
        }
      } else {
        /* Inport: '<Root>/U32_OdoDisp' */
        rtDW.u32_PrevOdoDisp = rtU.U32_OdoDisp;
      }
    } else {
      /* Inport: '<Root>/U32_OdoDisp' */
      rtDW.u32_PrevOdoDisp = rtU.U32_OdoDisp;
    }
  } else {
    rtDW.b_First_IgnON_State = B_FALSE;
    guard1 = true;
  }

  if (guard1) {
    /* Inport: '<Root>/B_DTE_Fast_Flng_Mode' incorporates:
     *  Inport: '<Root>/U16_FAST_FIL_SPD_LMT'
     *  Inport: '<Root>/U16_VehSpeed'
     */
    if ((rtU.B_DTE_Fast_Flng_Mode) && (rtU.U16_VehSpeed <=
         rtU.U16_FAST_FIL_SPD_LMT)) {
      /* Outport: '<Root>/S_DTE_Disp_Val' incorporates:
       *  Inport: '<Root>/S_Fuel_Dead_Vol'
       *  Inport: '<Root>/S_Fuel_Level_Ind'
       *  Outport: '<Root>/S_DTE_RAFE_Avg_Val'
       */
      rtY.S_DTE_Disp_Val = (rtU.S_Fuel_Level_Ind - rtU.S_Fuel_Dead_Vol) *
        rtY.S_DTE_RAFE_Avg_Val;

      /* Outport: '<Root>/S_Prev_1km_DTE' incorporates:
       *  Outport: '<Root>/S_DTE_Disp_Val'
       */
      rtY.S_Prev_1km_DTE = rtY.S_DTE_Disp_Val;

      /* Outport: '<Root>/S_DTE_1km' incorporates:
       *  Outport: '<Root>/S_Prev_1km_DTE'
       */
      rtY.S_DTE_1km = rtY.S_Prev_1km_DTE;

      /* Outport: '<Root>/S_Last_Caltd_DTE' incorporates:
       *  Outport: '<Root>/S_DTE_1km'
       */
      rtY.S_Last_Caltd_DTE = rtY.S_DTE_1km;
    }

    /* End of Inport: '<Root>/B_DTE_Fast_Flng_Mode' */
  }

  /* End of Inport: '<Root>/U8_IgnMode' */
}

/* Model step function */
void DTE_V_1_step(void)
{
  /* Chart: '<S1>/Chart' incorporates:
   *  Inport: '<Root>/U32_OdoDisp'
   *  Inport: '<Root>/U8_RAFE_MAX_ROL_DIST'
   */
  if (((uint32_T)rtDW.is_active_c3_DTE_V_1) == 0U) {
    rtDW.is_active_c3_DTE_V_1 = 1U;
    rtDW.u32_Fuel_PrevOdoDisp = rtU.U32_OdoDisp;
    inner_default_DTE_Fuel_Damp_Mng();
    rtDW.b_EEPROM_Window_Read = B_FALSE;

    /* Outport: '<Root>/S_DTE_1km_Completion_Flag' incorporates:
     *  Inport: '<Root>/B_EEPROM_DTE_1km_Compn_Flag'
     *  Inport: '<Root>/U32_OdoDisp'
     */
    rtY.S_DTE_1km_Completion_Flag = rtU.B_EEPROM_DTE_1km_Compn_Flag;
    rtDW.u8_RAFE_MAX_ROL_DIST = rtU.U8_RAFE_MAX_ROL_DIST;
    inner_default_DTE_Cal6();
  } else {
    inner_default_DTE_Fuel_Damp_Mng();
    inner_default_DTE_Cal6();
  }

  /* End of Chart: '<S1>/Chart' */
}

/* Model initialize function */
void DTE_V_1_initialize(void)
{
  /* SystemInitialize for Chart: '<S1>/Chart' */
  rtDW.u8_RAF_Shift_Cntr = 1U;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

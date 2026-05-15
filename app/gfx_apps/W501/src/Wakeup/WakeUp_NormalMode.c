/*
 * File: WakeUp_NormalMode.c
 *
 * Code generated for Simulink model 'WakeUp_NormalMode'.
 *
 * Model version                  : 1.83
 * Simulink Coder version         : 9.0 (R2018b) 24-May-2018
 * C/C++ source code generated on : Mon Sep  2 09:03:29 2019
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Renesas->RH850
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "WakeUp_NormalMode.h"
#include "WakeUp_NormalMode_private.h"

/* Named constants for Chart: '<S1>/WakeUp_NormalMode' */
#define WakeUp_Norma_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define WakeUp_NormalMo_IN_NORMAL_STATE ((uint8_T)1U)
#define WakeUp_NormalMod_IN_SLEEP_STATE ((uint8_T)2U)
#define WakeUp_NormalMode_IN_SLEEP_WAIT ((uint8_T)3U)
#define WakeUp_NormalMode_NORMAL_MODE  ((uint8_T)2U)
#define WakeUp_NormalMode_RESET        (false)
#define WakeUp_NormalMode_SET          (true)
#define WakeUp_NormalMode_SLEEP_MODE   ((uint8_T)3U)
#define WakeUp_NormalMode_SLEEP_WAIT   ((uint8_T)4U)

/* Block states (default storage) */
DW_WakeUp_NormalMode_T WakeUp_NormalMode_DW;

/* External inputs (root inport signals with default storage) */
ExtU_WakeUp_NormalMode_T WakeUp_NormalMode_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_WakeUp_NormalMode_T WakeUp_NormalMode_Y;

/* Real-time model */
RT_MODEL_WakeUp_NormalMode_T WakeUp_NormalMode_M_;
RT_MODEL_WakeUp_NormalMode_T *const WakeUp_NormalMode_M = &WakeUp_NormalMode_M_;

/* Model step function */
void WakeUp_NormalMode_step(void)
{
  uint32_T tmp;

  /* Chart: '<S1>/WakeUp_NormalMode' incorporates:
   *  Inport: '<Root>/B_CtrlActiveFrameArrive'
   *  Inport: '<Root>/B_CtrlSleepFrameArrive'
   *  Inport: '<Root>/B_CtrlWakeUpFrameArrive'
   *  Inport: '<Root>/U16_ConfigStepTime'
   *  Inport: '<Root>/U16_KeepNormalTimeOutTH'
   *  Inport: '<Root>/U16_SleepWaitTimerTH'
   *  Inport: '<Root>/U8_ECUSleepToNormalState'
   *  Inport: '<Root>/U8_LocalSleepState'
   */
  if (WakeUp_NormalMode_DW.is_active_c3_WakeUp_NormalMode == 0U) {
    WakeUp_NormalMode_DW.is_active_c3_WakeUp_NormalMode = 1U;
    WakeUp_NormalMode_DW.is_WAKEUP_NORMAL_MODE = WakeUp_NormalMo_IN_NORMAL_STATE;

    /* Outport: '<Root>/B_TransmitCANFrame' */
    WakeUp_NormalMode_Y.B_TransmitCANFrame = WakeUp_NormalMode_SET;

    /* Outport: '<Root>/U8_ECUState' */
    WakeUp_NormalMode_Y.U8_ECUState = WakeUp_NormalMode_NORMAL_MODE;
    WakeUp_NormalMode_DW.u16_KeepNormalTimer = 0U;
    WakeUp_NormalMode_DW.b_KeepNormalTimerElaps = WakeUp_NormalMode_RESET;
  } else {
    switch (WakeUp_NormalMode_DW.is_WAKEUP_NORMAL_MODE) {
     case WakeUp_NormalMo_IN_NORMAL_STATE:
      /* Outport: '<Root>/U8_ECUState' */
      WakeUp_NormalMode_Y.U8_ECUState = WakeUp_NormalMode_NORMAL_MODE;
      if (WakeUp_NormalMode_DW.b_KeepNormalTimerElaps &&
          (WakeUp_NormalMode_U.U8_LocalSleepState ==
           WakeUp_NormalMode_SLEEP_MODE)) {
        WakeUp_NormalMode_DW.is_WAKEUP_NORMAL_MODE =
          WakeUp_NormalMode_IN_SLEEP_WAIT;

        /* Outport: '<Root>/B_TransmitCANFrame' */
        WakeUp_NormalMode_Y.B_TransmitCANFrame = WakeUp_NormalMode_RESET;

        /* Outport: '<Root>/U8_ECUState' */
        WakeUp_NormalMode_Y.U8_ECUState = WakeUp_NormalMode_SLEEP_WAIT;
        WakeUp_NormalMode_DW.u16_SleepWaitTimer = 0U;
        WakeUp_NormalMode_DW.b_SleepWaitTimerElaps = WakeUp_NormalMode_RESET;
      } else if (WakeUp_NormalMode_U.U8_LocalSleepState ==
                 WakeUp_NormalMode_SLEEP_MODE) {
        tmp = (uint32_T)WakeUp_NormalMode_DW.u16_KeepNormalTimer +
          WakeUp_NormalMode_U.U16_ConfigStepTime;
        if (tmp > 65535U) {
          tmp = 65535U;
        }

        WakeUp_NormalMode_DW.u16_KeepNormalTimer = (uint16_T)tmp;
        if (WakeUp_NormalMode_DW.u16_KeepNormalTimer >=
            WakeUp_NormalMode_U.U16_KeepNormalTimeOutTH) {
          WakeUp_NormalMode_DW.b_KeepNormalTimerElaps = WakeUp_NormalMode_SET;
          WakeUp_NormalMode_DW.u16_KeepNormalTimer = 0U;
        }
      } else {
        WakeUp_NormalMode_DW.u16_KeepNormalTimer = 0U;
      }
      break;

     case WakeUp_NormalMod_IN_SLEEP_STATE:
      /* Outport: '<Root>/U8_ECUState' */
      WakeUp_NormalMode_Y.U8_ECUState = WakeUp_NormalMode_SLEEP_MODE;
      if (WakeUp_NormalMode_U.U8_ECUSleepToNormalState ==
          WakeUp_NormalMode_NORMAL_MODE) {
        WakeUp_NormalMode_DW.is_WAKEUP_NORMAL_MODE =
          WakeUp_NormalMo_IN_NORMAL_STATE;

        /* Outport: '<Root>/B_TransmitCANFrame' */
        WakeUp_NormalMode_Y.B_TransmitCANFrame = WakeUp_NormalMode_SET;

        /* Outport: '<Root>/U8_ECUState' */
        WakeUp_NormalMode_Y.U8_ECUState = WakeUp_NormalMode_NORMAL_MODE;
        WakeUp_NormalMode_DW.u16_KeepNormalTimer = 0U;
        WakeUp_NormalMode_DW.b_KeepNormalTimerElaps = WakeUp_NormalMode_RESET;
      }
      break;

     default:
      /* Outport: '<Root>/U8_ECUState' */
      WakeUp_NormalMode_Y.U8_ECUState = WakeUp_NormalMode_SLEEP_WAIT;
      if (WakeUp_NormalMode_U.B_CtrlWakeUpFrameArrive ||
          (WakeUp_NormalMode_U.U8_LocalSleepState !=
           WakeUp_NormalMode_SLEEP_MODE)) {
        WakeUp_NormalMode_DW.is_WAKEUP_NORMAL_MODE =
          WakeUp_NormalMo_IN_NORMAL_STATE;

        /* Outport: '<Root>/B_TransmitCANFrame' */
        WakeUp_NormalMode_Y.B_TransmitCANFrame = WakeUp_NormalMode_SET;

        /* Outport: '<Root>/U8_ECUState' */
        WakeUp_NormalMode_Y.U8_ECUState = WakeUp_NormalMode_NORMAL_MODE;
        WakeUp_NormalMode_DW.u16_KeepNormalTimer = 0U;
        WakeUp_NormalMode_DW.b_KeepNormalTimerElaps = WakeUp_NormalMode_RESET;
      } else if (WakeUp_NormalMode_DW.b_SleepWaitTimerElaps ||
                 WakeUp_NormalMode_U.B_CtrlSleepFrameArrive) {
        WakeUp_NormalMode_DW.is_WAKEUP_NORMAL_MODE =
          WakeUp_NormalMod_IN_SLEEP_STATE;

        /* Outport: '<Root>/U8_ECUState' */
        WakeUp_NormalMode_Y.U8_ECUState = WakeUp_NormalMode_SLEEP_MODE;
      } else {
        if (WakeUp_NormalMode_U.B_CtrlActiveFrameArrive) {
          WakeUp_NormalMode_DW.u16_SleepWaitTimer = 0U;
        }

        tmp = (uint32_T)WakeUp_NormalMode_DW.u16_SleepWaitTimer +
          WakeUp_NormalMode_U.U16_ConfigStepTime;
        if (tmp > 65535U) {
          tmp = 65535U;
        }

        WakeUp_NormalMode_DW.u16_SleepWaitTimer = (uint16_T)tmp;
        if (WakeUp_NormalMode_DW.u16_SleepWaitTimer >=
            WakeUp_NormalMode_U.U16_SleepWaitTimerTH) {
          /* Outport: '<Root>/B_TransmitCANFrame' */
          WakeUp_NormalMode_Y.B_TransmitCANFrame = WakeUp_NormalMode_RESET;
          WakeUp_NormalMode_DW.b_SleepWaitTimerElaps = WakeUp_NormalMode_SET;
        }
      }
      break;
    }
  }

  /* End of Chart: '<S1>/WakeUp_NormalMode' */
}

/* Model initialize function */
void WakeUp_NormalMode_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(WakeUp_NormalMode_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&WakeUp_NormalMode_DW, 0,
                sizeof(DW_WakeUp_NormalMode_T));

  /* external inputs */
  (void)memset(&WakeUp_NormalMode_U, 0, sizeof(ExtU_WakeUp_NormalMode_T));

  /* external outputs */
  (void) memset((void *)&WakeUp_NormalMode_Y, 0,
                sizeof(ExtY_WakeUp_NormalMode_T));

  /* SystemInitialize for Chart: '<S1>/WakeUp_NormalMode' */
  WakeUp_NormalMode_DW.is_WAKEUP_NORMAL_MODE = WakeUp_Norma_IN_NO_ACTIVE_CHILD;
  WakeUp_NormalMode_DW.is_active_c3_WakeUp_NormalMode = 0U;
}

/* Model terminate function */
void WakeUp_NormalMode_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

/*
 * File: WakeUp_DeepMode.c
 *
 * Code generated for Simulink model 'WakeUp_DeepMode'.
 *
 * Model version                  : 1.32
 * Simulink Coder version         : 9.0 (R2018b) 24-May-2018
 * C/C++ source code generated on : Mon Sep  2 08:54:23 2019
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Renesas->RH850
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "WakeUp_DeepMode.h"
#include "WakeUp_DeepMode_private.h"

/* Named constants for Chart: '<S1>/WakeUp_SleepModel' */
#define WakeUp_DeepMode_NORMAL_MODE    ((uint8_T)2U)
#define WakeUp_DeepMode_RESET          (false)
#define WakeUp_DeepMode_SET            (true)
#define WakeUp_DeepMode_SLEEP_MODE     ((uint8_T)3U)
#define WakeUp_DeepMode_WAKEUP_MODE    ((uint8_T)1U)

/* Block states (default storage) */
DW_WakeUp_DeepMode_T WakeUp_DeepMode_DW;

/* External inputs (root inport signals with default storage) */
ExtU_WakeUp_DeepMode_T WakeUp_DeepMode_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_WakeUp_DeepMode_T WakeUp_DeepMode_Y;

/* Real-time model */
RT_MODEL_WakeUp_DeepMode_T WakeUp_DeepMode_M_;
RT_MODEL_WakeUp_DeepMode_T *const WakeUp_DeepMode_M = &WakeUp_DeepMode_M_;

/* Model step function */
void WakeUp_DeepMode_step(void)
{
  /* Chart: '<S1>/WakeUp_SleepModel' incorporates:
   *  Inport: '<Root>/B_CtrlActiveFrameArrive'
   *  Inport: '<Root>/B_CtrlWakeUpFrameArrive'
   *  Inport: '<Root>/B_MBFMWakeupFrame'
   *  Inport: '<Root>/B_WakeUpSynchTimerElaps'
   *  Inport: '<Root>/B_WakeUpTimerStartFlag'
   *  Outport: '<Root>/U8_ECUSleepToNormalState'
   */
  if (WakeUp_DeepMode_DW.is_active_c3_WakeUp_DeepMode == 0U) {
    WakeUp_DeepMode_DW.is_active_c3_WakeUp_DeepMode = 1U;

    /* Outport: '<Root>/U8_ECUSleepToNormalState' */
    WakeUp_DeepMode_Y.U8_ECUSleepToNormalState = 0U;

    /* Outport: '<Root>/B_ReloadSychTimerFlag' */
    WakeUp_DeepMode_Y.B_ReloadSychTimerFlag = WakeUp_DeepMode_RESET;
    WakeUp_DeepMode_DW.b_SynchTimerRunning = WakeUp_DeepMode_RESET;
  } else if (WakeUp_DeepMode_U.B_MBFMWakeupFrame) {
    /* Outport: '<Root>/U8_ECUSleepToNormalState' */
    WakeUp_DeepMode_Y.U8_ECUSleepToNormalState = WakeUp_DeepMode_NORMAL_MODE;
  } else if (WakeUp_DeepMode_U.B_WakeUpSynchTimerElaps) {
    if (WakeUp_DeepMode_DW.b_SynchTimerRunning) {
      /* Outport: '<Root>/U8_ECUSleepToNormalState' */
      WakeUp_DeepMode_Y.U8_ECUSleepToNormalState = WakeUp_DeepMode_NORMAL_MODE;
      WakeUp_DeepMode_DW.b_SynchTimerRunning = WakeUp_DeepMode_RESET;

      /* Outport: '<Root>/B_ReloadSychTimerFlag' */
      WakeUp_DeepMode_Y.B_ReloadSychTimerFlag = WakeUp_DeepMode_RESET;
    } else {
      /* Outport: '<Root>/U8_ECUSleepToNormalState' */
      WakeUp_DeepMode_Y.U8_ECUSleepToNormalState = WakeUp_DeepMode_SLEEP_MODE;
    }
  } else if (WakeUp_DeepMode_U.B_WakeUpTimerStartFlag &&
             (WakeUp_DeepMode_Y.U8_ECUSleepToNormalState ==
              WakeUp_DeepMode_WAKEUP_MODE) &&
             WakeUp_DeepMode_U.B_CtrlActiveFrameArrive) {
    /* Outport: '<Root>/U8_ECUSleepToNormalState' */
    WakeUp_DeepMode_Y.U8_ECUSleepToNormalState = WakeUp_DeepMode_NORMAL_MODE;

    /* Outport: '<Root>/B_ReloadSychTimerFlag' */
    WakeUp_DeepMode_Y.B_ReloadSychTimerFlag = WakeUp_DeepMode_RESET;
  } else {
    if (WakeUp_DeepMode_U.B_CtrlWakeUpFrameArrive) {
      /* Outport: '<Root>/U8_ECUSleepToNormalState' */
      WakeUp_DeepMode_Y.U8_ECUSleepToNormalState = WakeUp_DeepMode_WAKEUP_MODE;

      /* Outport: '<Root>/B_ReloadSychTimerFlag' */
      WakeUp_DeepMode_Y.B_ReloadSychTimerFlag = WakeUp_DeepMode_SET;
      WakeUp_DeepMode_DW.b_SynchTimerRunning = WakeUp_DeepMode_SET;
    }
  }

  /* End of Chart: '<S1>/WakeUp_SleepModel' */
}

/* Model initialize function */
void WakeUp_DeepMode_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(WakeUp_DeepMode_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&WakeUp_DeepMode_DW, 0,
                sizeof(DW_WakeUp_DeepMode_T));

  /* external inputs */
  (void)memset(&WakeUp_DeepMode_U, 0, sizeof(ExtU_WakeUp_DeepMode_T));

  /* external outputs */
  (void) memset((void *)&WakeUp_DeepMode_Y, 0,
                sizeof(ExtY_WakeUp_DeepMode_T));

  /* SystemInitialize for Chart: '<S1>/WakeUp_SleepModel' */
  WakeUp_DeepMode_DW.is_active_c3_WakeUp_DeepMode = 0U;
}

/* Model terminate function */
void WakeUp_DeepMode_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

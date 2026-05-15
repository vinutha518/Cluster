/*
 * File: WakeUp_NormalMode.h
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

#ifndef RTW_HEADER_WakeUp_NormalMode_h_
#define RTW_HEADER_WakeUp_NormalMode_h_
#include <string.h>
#include <stddef.h>
#ifndef WakeUp_NormalMode_COMMON_INCLUDES_
# define WakeUp_NormalMode_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* WakeUp_NormalMode_COMMON_INCLUDES_ */

#include "WakeUp_NormalMode_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  uint16_T u16_SleepWaitTimer;         /* '<S1>/WakeUp_NormalMode' */
  uint16_T u16_KeepNormalTimer;        /* '<S1>/WakeUp_NormalMode' */
  uint8_T is_active_c3_WakeUp_NormalMode;/* '<S1>/WakeUp_NormalMode' */
  uint8_T is_WAKEUP_NORMAL_MODE;       /* '<S1>/WakeUp_NormalMode' */
  boolean_T b_KeepNormalTimerElaps;    /* '<S1>/WakeUp_NormalMode' */
  boolean_T b_SleepWaitTimerElaps;     /* '<S1>/WakeUp_NormalMode' */
} DW_WakeUp_NormalMode_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  boolean_T B_CtrlActiveFrameArrive;   /* '<Root>/B_CtrlActiveFrameArrive' */
  boolean_T B_CtrlWakeUpFrameArrive;   /* '<Root>/B_CtrlWakeUpFrameArrive' */
  boolean_T B_CtrlSleepFrameArrive;    /* '<Root>/B_CtrlSleepFrameArrive' */
  uint8_T U8_ECUSleepToNormalState;    /* '<Root>/U8_ECUSleepToNormalState' */
  uint16_T U16_ConfigStepTime;         /* '<Root>/U16_ConfigStepTime' */
  uint16_T U16_SleepWaitTimerTH;       /* '<Root>/U16_SleepWaitTimerTH' */
  uint16_T U16_KeepNormalTimeOutTH;    /* '<Root>/U16_KeepNormalTimeOutTH' */
  uint8_T U8_LocalSleepState;          /* '<Root>/U8_LocalSleepState' */
} ExtU_WakeUp_NormalMode_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  uint8_T U8_ECUState;                 /* '<Root>/U8_ECUState' */
  boolean_T B_TransmitCANFrame;        /* '<Root>/B_TransmitCANFrame' */
} ExtY_WakeUp_NormalMode_T;

/* Real-time Model Data Structure */
struct tag_RTM_WakeUp_NormalMode_T {
  const char_T * volatile errorStatus;
};

/* Block states (default storage) */
extern DW_WakeUp_NormalMode_T WakeUp_NormalMode_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_WakeUp_NormalMode_T WakeUp_NormalMode_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_WakeUp_NormalMode_T WakeUp_NormalMode_Y;

/* Model entry point functions */
extern void WakeUp_NormalMode_initialize(void);
extern void WakeUp_NormalMode_step(void);
extern void WakeUp_NormalMode_terminate(void);

/* Real-time Model object */
extern RT_MODEL_WakeUp_NormalMode_T *const WakeUp_NormalMode_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'WakeUp_NormalMode'
 * '<S1>'   : 'WakeUp_NormalMode/NormalMode'
 * '<S2>'   : 'WakeUp_NormalMode/NormalMode/WakeUp_NormalMode'
 */
#endif                                 /* RTW_HEADER_WakeUp_NormalMode_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

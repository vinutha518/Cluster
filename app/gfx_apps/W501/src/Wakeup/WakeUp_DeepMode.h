/*
 * File: WakeUp_DeepMode.h
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

#ifndef RTW_HEADER_WakeUp_DeepMode_h_
#define RTW_HEADER_WakeUp_DeepMode_h_
#include <string.h>
#include <stddef.h>
#ifndef WakeUp_DeepMode_COMMON_INCLUDES_
# define WakeUp_DeepMode_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* WakeUp_DeepMode_COMMON_INCLUDES_ */

#include "WakeUp_DeepMode_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  uint8_T is_active_c3_WakeUp_DeepMode;/* '<S1>/WakeUp_SleepModel' */
  boolean_T b_SynchTimerRunning;       /* '<S1>/WakeUp_SleepModel' */
} DW_WakeUp_DeepMode_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  boolean_T B_CtrlActiveFrameArrive;   /* '<Root>/B_CtrlActiveFrameArrive' */
  boolean_T B_WakeUpSynchTimerElaps;   /* '<Root>/B_WakeUpSynchTimerElaps' */
  boolean_T B_CtrlWakeUpFrameArrive;   /* '<Root>/B_CtrlWakeUpFrameArrive' */
  boolean_T B_WakeUpTimerStartFlag;    /* '<Root>/B_WakeUpTimerStartFlag' */
  boolean_T B_MBFMWakeupFrame;         /* '<Root>/B_MBFMWakeupFrame' */
} ExtU_WakeUp_DeepMode_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  uint8_T U8_ECUSleepToNormalState;    /* '<Root>/U8_ECUSleepToNormalState' */
  boolean_T B_ReloadSychTimerFlag;     /* '<Root>/B_ReloadSychTimerFlag' */
} ExtY_WakeUp_DeepMode_T;

/* Real-time Model Data Structure */
struct tag_RTM_WakeUp_DeepMode_T {
  const char_T * volatile errorStatus;
};

/* Block states (default storage) */
extern DW_WakeUp_DeepMode_T WakeUp_DeepMode_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_WakeUp_DeepMode_T WakeUp_DeepMode_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_WakeUp_DeepMode_T WakeUp_DeepMode_Y;

/* Model entry point functions */
extern void WakeUp_DeepMode_initialize(void);
extern void WakeUp_DeepMode_step(void);
extern void WakeUp_DeepMode_terminate(void);

/* Real-time Model object */
extern RT_MODEL_WakeUp_DeepMode_T *const WakeUp_DeepMode_M;

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
 * '<Root>' : 'WakeUp_DeepMode'
 * '<S1>'   : 'WakeUp_DeepMode/Subsystem'
 * '<S2>'   : 'WakeUp_DeepMode/Subsystem/WakeUp_SleepModel'
 */
#endif                                 /* RTW_HEADER_WakeUp_DeepMode_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

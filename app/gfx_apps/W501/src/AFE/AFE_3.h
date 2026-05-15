/*
 * File: AFE_3.h
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

#ifndef RTW_HEADER_AFE_3_h_
#define RTW_HEADER_AFE_3_h_
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef AFE_3_COMMON_INCLUDES_
# define AFE_3_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* AFE_3_COMMON_INCLUDES_ */

#include "AFE_3_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  uint32_T U32_DrvAccFuelConspPrev;    /* '<S3>/Cal_AFE' */
  uint32_T u32_AccFuelConspGDPrev;     /* '<S3>/Cal_AFE' */
  uint32_T U32_AccFuelConspPrev;       /* '<S3>/Cal_AFE' */
  uint32_T U32_AFEDispCnter;           /* '<S3>/Cal_AFE' */
  uint32_T U32_AFE_GD_DispCnter;       /* '<S3>/Cal_AFE' */
  uint16_T U16_AFECalTimer;            /* '<S3>/Cal_AFE' */
  uint16_T U16_AFE_CAL_INTERVAL;       /* '<S3>/Cal_AFE' */
  uint16_T u16_AfeMax;                 /* '<S3>/Cal_AFE' */
  uint16_T u16_AfeDispFilterA;         /* '<S3>/Cal_AFE' */
  uint16_T u16_AfeDispFilterB;         /* '<S3>/Cal_AFE' */
  uint16_T u16_AfeMIn;                 /* '<S3>/Cal_AFE' */
  uint16_T counter;                    /* '<S3>/Cal_AFE' */
  uint16_T u16_AFE_DIST_TH;            /* '<S3>/Cal_AFE' */
  uint16_T U16_AFEDISP_CNT;            /* '<S3>/Cal_AFE' */
  uint16_T U16_AFE_GD_CalTimer;        /* '<S3>/Cal_AFE' */
  uint8_T UnitDelay_DSTATE;            /* '<S3>/Unit Delay' */
  uint8_T is_active_c1_AFE_3;          /* '<S3>/Cal_AFE' */
  boolean_T b_1kmover;                 /* '<S3>/Cal_AFE' */
  boolean_T b_dispUpdated;             /* '<S3>/Cal_AFE' */
  boolean_T b_dispUpdated_GD;          /* '<S3>/Cal_AFE' */
  boolean_T b_1kmover_GD;              /* '<S3>/Cal_AFE' */
} DW_AFE_3_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  boolean_T B_EMSCode;                 /* '<Root>/B_EMSCode' */
  uint16_T U16_FuelConsRate;           /* '<Root>/U16_FuelConsRate' */
  uint32_T U32_OdoDisp;                /* '<Root>/U32_OdoDisp' */
  boolean_T U8_LHVSD;                  /* '<Root>/U8_LHVSD' */
  uint8_T U8_AfeCalcInterv;            /* '<Root>/U8_AfeCalcInterv' */
  uint8_T U8_AfeMax;                   /* '<Root>/U8_AfeMax' */
  uint8_T U8_AfeDispFilterA;           /* '<Root>/U8_AfeDispFilterA' */
  uint8_T U8_AfeDispFilterB;           /* '<Root>/U8_AfeDispFilterB' */
  uint8_T U8_AfeMIn;                   /* '<Root>/U8_AfeMIn' */
  boolean_T B_EnbleAFE;                /* '<Root>/B_EnbleAFE' */
  uint8_T U8_IgnMode;                  /* '<Root>/U8_IgnMode' */
  uint8_T U8_STS_Eng;                  /* '<Root>/U8_STS_Eng' */
  boolean_T B_InfoResetAFE;            /* '<Root>/B_InfoResetAFE' */
  uint32_T U32_AccFuelConspEE;         /* '<Root>/U32_AccFuelConspEE' */
  uint32_T U32_AccFuelConspGDEE;       /* '<Root>/U32_AccFuelConspGDEE' */
  uint32_T U32_OdoDispResetEE;         /* '<Root>/U32_OdoDispResetEE' */
  uint32_T U32_OdoDispGDResetEE;       /* '<Root>/U32_OdoDispGDResetEE' */
  uint8_T U8_AFE_DIST_TH;              /* '<Root>/U8_AFE_DIST_TH' */
  uint8_T U8_AfeDispIntev;             /* '<Root>/U8_AfeDispIntev' */
  boolean_T B_CanRxFlag;               /* '<Root>/B_CanRxFlag' */
  uint16_T U16_AFE_LastDispVal_EE;     /* '<Root>/U16_AFE_LastDispVal_EE' */
  boolean_T B_1Km_Dist_Flag_EE;        /* '<Root>/B_1Km_Dist_Flag_EE' */
  boolean_T B_DispUpdated_GD_Flag_EE;  /* '<Root>/B_DispUpdated_GD_Flag_EE' */
  uint16_T U16_AFE_GD_LastDispVal_EE;  /* '<Root>/U16_AFE_GD_LastDispVal_EE' */
  boolean_T B_DispUpdated_Flag_EE;     /* '<Root>/B_DispUpdated_Flag_EE' */
  boolean_T B_1Km_Dist_Flag_GD_EE;     /* '<Root>/B_1Km_Dist_Flag_GD_EE' */
  uint16_T U16_VehSpeed;               /* '<Root>/U16_VehSpeed' */
  boolean_T B_ReqTripResetAFE;         /* '<Root>/B_ReqTripResetAFE' */
  boolean_T B_AFEResetTrg;             /* '<Root>/B_AFEResetTrg' */
} ExtU_AFE_3_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  uint32_T U32_AccFuelConsp;           /* '<Root>/U32_AccFuelConsp' */
  uint32_T U32_AccFuelConspReset;      /* '<Root>/U32_AccFuelConspReset' */
  boolean_T b_AFECalTimer;             /* '<Root>/b_AFECalTimer' */
  uint32_T U32_OdoDispReset;           /* '<Root>/U32_OdoDispReset' */
  boolean_T B_AFEResetACK;             /* '<Root>/B_AFEResetACK' */
  uint32_T U32_AccDistAFE;             /* '<Root>/U32_AccDistAFE' */
  uint16_T U16_CalAvgKmplAFE;          /* '<Root>/U16_CalAvgKmplAFE' */
  uint16_T U16_AFEDispVal;             /* '<Root>/U16_AFEDispVal' */
  uint32_T U32_DrvAccFuelConsp;        /* '<Root>/U32_DrvAccFuelConsp' */
  uint32_T U32_OdoDispGDReset;         /* '<Root>/U32_OdoDispGDReset' */
  uint32_T U32_AccFuelConspGDReset;    /* '<Root>/U32_AccFuelConspGDReset' */
  uint32_T U32_AccFuelConspGD;         /* '<Root>/U32_AccFuelConspGD' */
  uint32_T U32_AccDistAFEGD;           /* '<Root>/U32_AccDistAFEGD' */
  uint16_T U16_CalAvgKmplAFE_GD;       /* '<Root>/U16_CalAvgKmplAFE_GD' */
  uint16_T U16_AvgKmplAFE_GD;          /* '<Root>/U16_AvgKmplAFE_GD' */
  boolean_T B_DispUpdated_Flag;        /* '<Root>/B_DispUpdated_Flag' */
  boolean_T B_DispUpdated_GD_Flag;     /* '<Root>/B_DispUpdated_GD_Flag' */
  boolean_T B_AFE_GD_ResetACK;         /* '<Root>/B_AFE_GD_ResetACK' */
  boolean_T B_1Km_Dist_Flag;           /* '<Root>/B_1Km_Dist_Flag' */
  boolean_T B_1Km_Dist_Flag_GD;        /* '<Root>/B_1Km_Dist_Flag_GD' */
} ExtY_AFE_3_T;

/* Real-time Model Data Structure */
struct tag_RTM_AFE_3_T {
  const char_T * volatile errorStatus;
};

/* Block states (default storage) */
extern DW_AFE_3_T AFE_3_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_AFE_3_T AFE_3_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_AFE_3_T AFE_3_Y;

/* Model entry point functions */
extern void AFE_3_initialize(void);
extern void AFE_3_step(void);
extern void AFE_3_terminate(void);

/* Real-time Model object */
extern RT_MODEL_AFE_3_T *const AFE_3_M;

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
 * '<Root>' : 'AFE_3'
 * '<S1>'   : 'AFE_3/AFE'
 * '<S2>'   : 'AFE_3/AFE/Enabled Subsystem'
 * '<S3>'   : 'AFE_3/AFE/Enabled Subsystem/Subsystem'
 * '<S4>'   : 'AFE_3/AFE/Enabled Subsystem/Subsystem/Cal_AFE'
 */
#endif                                 /* RTW_HEADER_AFE_3_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

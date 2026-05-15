/*
 * File: IFE_3_Code.h
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

#ifndef RTW_HEADER_IFE_3_Code_h_
#define RTW_HEADER_IFE_3_Code_h_
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef IFE_3_Code_COMMON_INCLUDES_
# define IFE_3_Code_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* IFE_3_Code_COMMON_INCLUDES_ */

#include "IFE_3_Code_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real32_T s_IFE_DistAcc;              /* '<S3>/Cal_IFE' */
  uint32_T U32_IFE_FC_Acc;             /* '<S3>/Cal_IFE' */
  uint32_T U32_IFE;                    /* '<S3>/Cal_IFE' */
  uint32_T u32_RollFE[10];             /* '<S3>/Cal_IFE' */
  uint32_T U32_IFEAccVal;              /* '<S3>/Cal_IFE' */
  uint16_T u16_IFESampTimeCntr;        /* '<S3>/Cal_IFE' */
  uint16_T u16_IFEDispTimeCntr;        /* '<S3>/Cal_IFE' */
  uint16_T u16_IFEAccTimeCntr;         /* '<S3>/Cal_IFE' */
  uint8_T is_active_c1_IFE_3_Code;     /* '<S3>/Cal_IFE' */
  uint8_T u8_RolCnt;                   /* '<S3>/Cal_IFE' */
  boolean_T B_AccTimeFlag;             /* '<S3>/Cal_IFE' */
} DW_IFE_3_Code_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  boolean_T U8_LHVSD;                  /* '<Root>/U8_LHVSD' */
  uint8_T U8_IgnMode;                  /* '<Root>/U8_IgnMode' */
  uint16_T U16_FuelConsRate;           /* '<Root>/U16_FuelConsRate' */
  uint32_T U32_OdoDisp;                /* '<Root>/U32_OdoDisp' */
  uint16_T U16_VehSpeed;               /* '<Root>/U16_VehSpeed' */
  uint8_T U8_STS_Eng;                  /* '<Root>/U8_STS_Eng' */
  uint16_T U16_IFECalTIme;             /* '<Root>/U16_IFECalTIme' */
  uint16_T U16_IFESampTime;            /* '<Root>/U16_IFESampTime' */
  uint16_T U16_IFEAccTime;             /* '<Root>/U16_IFEAccTime' */
  uint8_T U8_IFEUnit;                  /* '<Root>/U8_IFEUnit' */
  uint8_T U8_IFEMax;                   /* '<Root>/U8_IFEMax' */
  uint8_T U8_IFESampSize;              /* '<Root>/U8_IFESampSize' */
  uint16_T U16_IFEDispTime;            /* '<Root>/U16_IFEDispTime' */
  boolean_T B_IFEEnable;               /* '<Root>/B_IFEEnable' */
  uint8_T T_stepTime;                  /* '<Root>/T_stepTime' */
  uint8_T U8_RW_IFE_Unit;              /* '<Root>/U8_RW_IFE_Unit' */
  uint32_T U32_Diag_RW_Fuel_Bias;      /* '<Root>/U32_Diag_RW_Fuel_Bias' */
  uint8_T U8_Eng_RPM;                  /* '<Root>/U8_Eng_RPM' */
  boolean_T B_EMSCode;                 /* '<Root>/B_EMSCode' */
} ExtU_IFE_3_Code_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  uint32_T U32_IFEAvgVal;              /* '<Root>/U32_IFEAvgVal' */
  uint32_T U32_IFE_Disp_Val;           /* '<Root>/U32_IFE_Disp_Val' */
} ExtY_IFE_3_Code_T;

/* Real-time Model Data Structure */
struct tag_RTM_IFE_3_Code_T {
  const char_T * volatile errorStatus;
};

/* Block states (default storage) */
extern DW_IFE_3_Code_T IFE_3_Code_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_IFE_3_Code_T IFE_3_Code_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_IFE_3_Code_T IFE_3_Code_Y;

/* Model entry point functions */
extern void IFE_3_Code_initialize(void);
extern void IFE_3_Code_step(void);
extern void IFE_3_Code_terminate(void);

/* Real-time Model object */
extern RT_MODEL_IFE_3_Code_T *const IFE_3_Code_M;

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
 * '<Root>' : 'IFE_3_Code'
 * '<S1>'   : 'IFE_3_Code/IFE'
 * '<S2>'   : 'IFE_3_Code/IFE/Enabled Subsystem'
 * '<S3>'   : 'IFE_3_Code/IFE/Enabled Subsystem/Subsystem'
 * '<S4>'   : 'IFE_3_Code/IFE/Enabled Subsystem/Subsystem/Cal_IFE'
 */
#endif                                 /* RTW_HEADER_IFE_3_Code_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

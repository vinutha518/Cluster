/*
 * File: DTE_V_1.h
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

#ifndef RTW_HEADER_DTE_V_1_h_
#define RTW_HEADER_DTE_V_1_h_
#include <math.h>
#ifndef DTE_V_1_COMMON_INCLUDES_
# define DTE_V_1_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* DTE_V_1_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  real32_T s_LoDTE_RAFE_Inst_1km_Roll[60];/* '<S1>/Chart' */
  real32_T s_DTE_RAFE_Acc_Val;         /* '<S1>/Chart' */
  real32_T s_RAFE_EE_ACC;              /* '<S1>/Chart' */
  uint32_T u32_PrevOdoDisp;            /* '<S1>/Chart' */
  uint32_T u32_500mRoll_FC_Acc;        /* '<S1>/Chart' */
  uint32_T u32_DTE_FC_Acc;             /* '<S1>/Chart' */
  uint32_T u32_DTE_Iddle_FC_Acc;       /* '<S1>/Chart' */
  uint32_T u32_DTE_FC_Acc_1st_500m;    /* '<S1>/Chart' */
  uint32_T u32_Fuel_PrevOdoDisp;       /* '<S1>/Chart' */
  uint8_T is_active_c3_DTE_V_1;        /* '<S1>/Chart' */
  uint8_T u8_RAFE_MAX_ROL_DIST;        /* '<S1>/Chart' */
  uint8_T u8_RAF_Shift_Cntr;           /* '<S1>/Chart' */
  uint8_T u8_Fuel_Sample_Cnt;          /* '<S1>/Chart' */
  boolean_T b_Default_RAFE_Load_Status;/* '<S1>/Chart' */
  boolean_T b_RAFE_500m;               /* '<S1>/Chart' */
  boolean_T b_shift_RAFE_Roll;         /* '<S1>/Chart' */
  boolean_T b_First_IgnON_State;       /* '<S1>/Chart' */
  boolean_T b_EEPROM_Window_Read;      /* '<S1>/Chart' */
  boolean_T b_Disp_Deflt_DTE;          /* '<S1>/Chart' */
} DW;

/* External inputs (root inport signals with default storage) */
typedef struct {
  uint16_T U16_FuelConsRate;           /* '<Root>/U16_FuelConsRate' */
  uint32_T U32_OdoDisp;                /* '<Root>/U32_OdoDisp' */
  uint8_T U8_IgnMode;                  /* '<Root>/U8_IgnMode' */
  uint8_T U8_STS_Eng;                  /* '<Root>/U8_STS_Eng' */
  uint16_T U16_VehSpeed;               /* '<Root>/U16_VehSpeed' */
  uint8_T U8_Default_RAFE;             /* '<Root>/U8_Default_RAFE' */
  uint8_T U8_RAFE_MAX_ROL_DIST;        /* '<Root>/U8_RAFE_MAX_ROL_DIST' */
  uint8_T U8_DTE_FILTER_C;             /* '<Root>/U8_DTE_FILTER_C' */
  uint8_T U8_DTE_FILTER_B;             /* '<Root>/U8_DTE_FILTER_B' */
  uint8_T U8_DTE_FILTER_A;             /* '<Root>/U8_DTE_FILTER_A' */
  boolean_T B_LHVSD;                   /* '<Root>/B_LHVSD' */
  boolean_T B_VAR_CODE_EMS;            /* '<Root>/B_VAR_CODE_EMS' */
  boolean_T B_FEATR_CODE_DTE_IN;       /* '<Root>/B_FEATR_CODE_DTE_IN' */
  uint8_T U8_Fuel_Sendr_AD_In;         /* '<Root>/U8_Fuel_Sendr_AD_In' */
  boolean_T B_DTE_Reset_Trig;          /* '<Root>/B_DTE_Reset_Trig' */
  uint8_T U8_Fuel_Used_Bias;           /* '<Root>/U8_Fuel_Used_Bias' */
  real32_T S_Fuel_Level_Ind;           /* '<Root>/S_Fuel_Level_Ind' */
  real32_T S_Fuel_Dead_Vol;            /* '<Root>/S_Fuel_Dead_Vol' */
  boolean_T B_DTE_Fast_Flng_Mode;      /* '<Root>/B_DTE_Fast_Flng_Mode' */
  boolean_T B_DTE_CAN_Msg_Misng;       /* '<Root>/B_DTE_CAN_Msg_Misng' */
  uint32_T U32_DTE_StepTimeContr;      /* '<Root>/U32_DTE_StepTimeContr' */
  uint8_T U8_EE_DTE_RAFE_Inst_1km_Cnt; /* '<Root>/U8_EE_DTE_RAFE_Inst_1km_Cnt' */
  boolean_T B_EE_shift_RAFE_Roll;      /* '<Root>/B_EE_shift_RAFE_Roll' */
  real32_T S_EEPROM_RAFE_Read[60];     /* '<Root>/S_EEPROM_RAFE_Read' */
  boolean_T B_EEPROM_DTE_1km_Compn_Flag;/* '<Root>/B_EEPROM_DTE_1km_Compn_Flag' */
  uint32_T U32_EE_DTE_PrevOdoDisp;     /* '<Root>/U32_EE_DTE_PrevOdoDisp' */
  boolean_T B_EE_Odo_500m_Flag;        /* '<Root>/B_EE_Odo_500m_Flag' */
  uint32_T U32_EE_DTE_Iddle_FC_Acc;    /* '<Root>/U32_EE_DTE_Iddle_FC_Acc' */
  uint32_T U32_EE_500mRoll_FC_Acc;     /* '<Root>/U32_EE_500mRoll_FC_Acc' */
  real32_T S_EE_Last_Caltd_DTE;        /* '<Root>/S_EE_Last_Caltd_DTE' */
  uint32_T U32_EE_DTE_FC_Acc_2nd_500m; /* '<Root>/U32_EE_DTE_FC_Acc_2nd_500m' */
  uint32_T U32_EE_DTE_FC_Acc_1st_500m; /* '<Root>/U32_EE_DTE_FC_Acc_1st_500m' */
  uint16_T U16_FAST_FIL_SPD_LMT;       /* '<Root>/U16_FAST_FIL_SPD_LMT' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real32_T S_Cur_1km_DTE_RAFE_Val;     /* '<Root>/S_Cur_1km_DTE_RAFE_Val' */
  real32_T S_DTE_1km;                  /* '<Root>/S_DTE_1km' */
  real32_T S_DTE_RAFE_Avg_Val;         /* '<Root>/S_DTE_RAFE_Avg_Val' */
  real32_T S_DTE_Disp_Val;             /* '<Root>/S_DTE_Disp_Val' */
  boolean_T S_DTE_1km_Completion_Flag; /* '<Root>/S_DTE_1km_Completion_Flag' */
  real32_T S_Prev_DTE_Disp_Val;        /* '<Root>/S_Prev_DTE_Disp_Val' */
  real32_T S_Prev_1km_DTE;             /* '<Root>/S_Prev_1km_DTE' */
  real32_T S_Inst_1km_DTE;             /* '<Root>/S_Inst_1km_DTE' */
  real32_T S_Fuel_Level_Avg;           /* '<Root>/S_Fuel_Level_Avg' */
  real32_T S_Fuel_Level_Acc;           /* '<Root>/S_Fuel_Level_Acc' */
  uint8_T U8_DTE_RAFE_Inst_1km_Cnt;    /* '<Root>/U8_DTE_RAFE_Inst_1km_Cnt' */
  real32_T S_DTE_RAFE_Inst_1km_Roll[60];/* '<Root>/S_DTE_RAFE_Inst_1km_Roll' */
  uint32_T U32_Idle_DTE_FC_Acc;        /* '<Root>/U32_Idle_DTE_FC_Acc' */
  uint32_T U32_DTE_Prev_Odo;           /* '<Root>/U32_DTE_Prev_Odo' */
  boolean_T B_Odo_500m_Flag;           /* '<Root>/B_Odo_500m_Flag' */
  real32_T S_Last_Caltd_DTE;           /* '<Root>/S_Last_Caltd_DTE' */
  uint32_T U32_O_500mRoll_FC_Acc;      /* '<Root>/U32_O_500mRoll_FC_Acc' */
  uint32_T U32_O_DTE_FC_Acc_1st_500m;  /* '<Root>/U32_O_DTE_FC_Acc_1st_500m' */
  uint32_T U32_O_DTE_FC_Acc_2nd_500m;  /* '<Root>/U32_O_DTE_FC_Acc_2nd_500m' */
  boolean_T B_DTE_RAFE_Shift;          /* '<Root>/B_DTE_RAFE_Shift' */
} ExtY;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T * volatile errorStatus;
};

/* Block signals and states (default storage) */
extern DW rtDW;

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY rtY;

/* Model entry point functions */
extern void DTE_V_1_initialize(void);
extern void DTE_V_1_step(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

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
 * '<Root>' : 'DTE_V_1'
 * '<S1>'   : 'DTE_V_1/Subsystem'
 * '<S2>'   : 'DTE_V_1/Subsystem/Chart'
 */

/*-
 * Requirements for '<Root>': DTE_V_1
 */
#endif                                 /* RTW_HEADER_DTE_V_1_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

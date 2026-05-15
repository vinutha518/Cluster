/*
 * File: IVN_SecurityAlgorithm.h
 *
 * Code generated for Simulink model 'IVN_SecurityAlgorithm'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 9.0 (R2018b) 24-May-2018
 * C/C++ source code generated on : Mon Aug 19 17:54:38 2019
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_IVN_SecurityAlgorithm_h_
#define RTW_HEADER_IVN_SecurityAlgorithm_h_
#include <string.h>
#include <stddef.h>
#ifndef IVN_SecurityAlgorithm_COMMON_INCLUDES_
# define IVN_SecurityAlgorithm_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* IVN_SecurityAlgorithm_COMMON_INCLUDES_ */

#include "IVN_SecurityAlgorithm_types.h"

/* Child system includes */
#include "Decryption_0_private.h"
#include "Decryption_1_private.h"
#include "Decryption_1.h"
#include "Decryption_2_private.h"
#include "Encryption_0_private.h"
#include "Encryption_0.h"
#include "Encryption_1_private.h"
#include "Encryption_1.h"
#include "Encryption_2_private.h"
#include "Encryption_2.h"
#include "IVN_Decryption_private.h"
#include "IVN_Decryption.h"
#include "IVN_Encryption_private.h"
#include "IVN_Encryption.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  uint16_T u16_InternalTimer_T2;       /* '<S2>/IVN_SecurityLayer' */
  uint16_T u16_SynchWindowTimer;       /* '<S2>/IVN_SecurityLayer' */
  uint16_T u16_FCIncCntrlTiTh_C;       /* '<S2>/IVN_SecurityLayer' */
  uint16_T u16_SynchWndTiTh_c;         /* '<S2>/IVN_SecurityLayer' */
  uint8_T UnitDelay_DSTATE;            /* '<S2>/Unit Delay' */
  uint8_T UnitDelay1_DSTATE;           /* '<S2>/Unit Delay1' */
  uint8_T is_active_c3_IVN_SecurityAlgori;/* '<S2>/IVN_SecurityLayer' */
  uint8_T is_IVN_FRESHNESS_COUNTER;    /* '<S2>/IVN_SecurityLayer' */
  uint8_T u8_FcErrReptCntr;            /* '<S2>/IVN_SecurityLayer' */
  uint8_T U8_FcErrHealCntr;            /* '<S2>/IVN_SecurityLayer' */
  boolean_T b_SynchWndTimerStartFlag;  /* '<S2>/IVN_SecurityLayer' */
  boolean_T b_TxMAC_Indicator;         /* '<S2>/IVN_SecurityLayer' */
} DW_IVN_SecurityAlgorithm_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  boolean_T B_RxMAC_Indicator;         /* '<Root>/B_RxMAC_Indicator' */
  uint32_T U32_SecurityKeyConst;       /* '<Root>/U32_SecurityKeyConst' */
  uint32_T U32_SecurityKey_1;          /* '<Root>/U32_SecurityKey_1' */
  uint32_T U32_SecurityKey_2;          /* '<Root>/U32_SecurityKey_2' */
  boolean_T U8_IgnMode;                /* '<Root>/U8_IgnMode' */
  uint32_T U32_EEFreshnessCounter;     /* '<Root>/U32_EEFreshnessCounter' */
  uint32_T U32_MacRxBuffer;            /* '<Root>/U32_MacRxBuffer' */
  boolean_T B_SecurityEnableFlag;      /* '<Root>/B_SecurityEnableFlag' */
  uint8_T U8_RxNSMMsgArrive;           /* '<Root>/U8_RxNSMMsgArrive' */
  uint8_T U8_RxSynchMsgArrive;         /* '<Root>/U8_RxSynchMsgArrive' */
  uint16_T U8_ConfigStepTime;          /* '<Root>/U8_ConfigStepTime' */
  uint8_T U8_cfg_FCIncCntrTiTh_C;      /* '<Root>/U8_cfg_FCIncCntrTiTh_C' */
  uint8_T U8_cfg_SynchWndTiTh_C;       /* '<Root>/U8_cfg_SynchWndTiTh_C' */
  uint8_T U8_cfg_FCDevTh_C;            /* '<Root>/U8_cfg_FCDevTh_C' */
  uint8_T U8_cfg_FcErrReptDec_C;       /* '<Root>/U8_cfg_FcErrReptDec_C' */
  uint8_T U8_cfg_FcErrHealDec_C;       /* '<Root>/U8_cfg_FcErrHealDec_C' */
  uint8_T U8_cfg_FcDifErrReptTh_C;     /* '<Root>/U8_cfg_FcDifErrReptTh_C' */
  uint8_T U8_cfg_FcErrReptInc_C;       /* '<Root>/U8_cfg_FcErrReptInc_C' */
  uint32_T U32_MAC0RxBuffer;           /* '<Root>/U32_MAC0RxBuffer' */
} ExtU_IVN_SecurityAlgorithm_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  uint32_T U32_FreshnessCounter;       /* '<Root>/U32_FreshnessCounter' */
  boolean_T B_TxMAC_Indicator;         /* '<Root>/B_TxMAC_Indicator' */
  uint8_T U8_CANSignal_State;          /* '<Root>/U8_CANSignal_State' */
  boolean_T B_TransmitCANFrame;        /* '<Root>/B_TransmitCANFrame' */
  uint32_T U32_MacTxBuffer;            /* '<Root>/U32_MacTxBuffer' */
  boolean_T B_Ivn_LimpHome;            /* '<Root>/B_Ivn_LimpHome' */
  uint32_T U32_Mac2TxBuffer;           /* '<Root>/U32_Mac2TxBuffer' */
  boolean_T B_FcErrorStatusBit;        /* '<Root>/B_FcErrorStatusBit' */
  boolean_T B_NsmDTCStatus;            /* '<Root>/B_NsmDTCStatus' */
} ExtY_IVN_SecurityAlgorithm_T;

/* Real-time Model Data Structure */
struct tag_RTM_IVN_SecurityAlgorithm_T {
  const char_T * volatile errorStatus;
};

/* Block states (default storage) */
extern DW_IVN_SecurityAlgorithm_T IVN_SecurityAlgorithm_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_IVN_SecurityAlgorithm_T IVN_SecurityAlgorithm_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_IVN_SecurityAlgorithm_T IVN_SecurityAlgorithm_Y;

/* Model entry point functions */
extern void IVN_SecurityAlgorithm_initialize(void);
extern void IVN_SecurityAlgorithm_step(void);
extern void IVN_SecurityAlgorithm_terminate(void);

/* Real-time Model object */
extern RT_MODEL_IVN_SecurityAlgorith_T *const IVN_SecurityAlgorithm_M;

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
 * '<Root>' : 'IVN_SecurityAlgorithm'
 * '<S1>'   : 'IVN_SecurityAlgorithm/Subsystem'
 * '<S2>'   : 'IVN_SecurityAlgorithm/Subsystem/IVN_SecurityLayer'
 * '<S3>'   : 'IVN_SecurityAlgorithm/Subsystem/Security_Algorithms'
 * '<S4>'   : 'IVN_SecurityAlgorithm/Subsystem/IVN_SecurityLayer/IVN_SecurityLayer'
 * '<S5>'   : 'IVN_SecurityAlgorithm/Subsystem/Security_Algorithms/Decryption_0'
 * '<S6>'   : 'IVN_SecurityAlgorithm/Subsystem/Security_Algorithms/Decryption_1'
 * '<S7>'   : 'IVN_SecurityAlgorithm/Subsystem/Security_Algorithms/Decryption_2'
 * '<S8>'   : 'IVN_SecurityAlgorithm/Subsystem/Security_Algorithms/Decryption_Func'
 * '<S9>'   : 'IVN_SecurityAlgorithm/Subsystem/Security_Algorithms/Encryption_0'
 * '<S10>'  : 'IVN_SecurityAlgorithm/Subsystem/Security_Algorithms/Encryption_1'
 * '<S11>'  : 'IVN_SecurityAlgorithm/Subsystem/Security_Algorithms/Encryption_2'
 * '<S12>'  : 'IVN_SecurityAlgorithm/Subsystem/Security_Algorithms/Encryption_Func'
 */
#endif                                 /* RTW_HEADER_IVN_SecurityAlgorithm_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

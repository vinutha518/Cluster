/*
 * File: IVN_SecurityAlgorithm.c
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

#include "IVN_SecurityAlgorithm.h"
#include "IVN_SecurityAlgorithm_private.h"

/* Named constants for Chart: '<S2>/IVN_SecurityLayer' */
#define IVN_Sec_U8_FCIncCntrlTiTh_C_Res ((uint8_T)10U)
#define IVN_Secur_U8_SynchWndTiTh_c_Res ((uint8_T)100U)
#define IVN_SecurityAlgori_U32_OVERFLOW (MAX_uint32_T)
#define IVN_SecurityAlgorith_IN_IGN_OFF ((uint8_T)2U)
#define IVN_SecurityAlgorithm_E_OK     ((uint8_T)1U)
#define IVN_SecurityAlgorithm_MAC_1    (false)
#define IVN_SecurityAlgorithm_MAC_2    (true)
#define IVN_SecurityAlgorithm_NOT_OK   ((uint8_T)2U)
#define IVN_SecurityAlgorithm_RESET    (false)
#define IVN_SecurityAlgorithm_SEND_OK  (true)
#define IVN_SecurityAlgorithm_SET      (true)
#define IVN_Security_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define IVN__IN_FC_IncrementState_IGNON ((uint8_T)1U)

/* Block states (default storage) */
DW_IVN_SecurityAlgorithm_T IVN_SecurityAlgorithm_DW;

/* External inputs (root inport signals with default storage) */
ExtU_IVN_SecurityAlgorithm_T IVN_SecurityAlgorithm_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_IVN_SecurityAlgorithm_T IVN_SecurityAlgorithm_Y;

/* Real-time model */
RT_MODEL_IVN_SecurityAlgorith_T IVN_SecurityAlgorithm_M_;
RT_MODEL_IVN_SecurityAlgorith_T *const IVN_SecurityAlgorithm_M =
  &IVN_SecurityAlgorithm_M_;

/* Model step function */
void IVN_SecurityAlgorithm_step(void)
{
  uint32_T u32_DecrptedFCounter;
  uint32_T u32_InternalCalculatedMAC0;
  int32_T tmp;

  /* Outputs for Enabled SubSystem: '<S1>/IVN_SecurityLayer' incorporates:
   *  EnablePort: '<S2>/Enable'
   */
  /* Inport: '<Root>/B_SecurityEnableFlag' */
  if (IVN_SecurityAlgorithm_U.B_SecurityEnableFlag) {
    /* Chart: '<S2>/IVN_SecurityLayer' incorporates:
     *  Inport: '<Root>/B_RxMAC_Indicator'
     *  Inport: '<Root>/U32_MAC0RxBuffer'
     *  Inport: '<Root>/U32_MacRxBuffer'
     *  Inport: '<Root>/U32_SecurityKeyConst'
     *  Inport: '<Root>/U32_SecurityKey_1'
     *  Inport: '<Root>/U32_SecurityKey_2'
     *  Inport: '<Root>/U8_ConfigStepTime'
     *  Inport: '<Root>/U8_IgnMode'
     *  Inport: '<Root>/U8_RxNSMMsgArrive'
     *  Inport: '<Root>/U8_RxSynchMsgArrive'
     *  Inport: '<Root>/U8_cfg_FCDevTh_C'
     *  Inport: '<Root>/U8_cfg_FCIncCntrTiTh_C'
     *  Inport: '<Root>/U8_cfg_FcDifErrReptTh_C'
     *  Inport: '<Root>/U8_cfg_FcErrHealDec_C'
     *  Inport: '<Root>/U8_cfg_FcErrReptDec_C'
     *  Inport: '<Root>/U8_cfg_FcErrReptInc_C'
     *  Inport: '<Root>/U8_cfg_SynchWndTiTh_C'
     *  Outport: '<Root>/U32_FreshnessCounter'
     *  UnitDelay: '<S2>/Unit Delay'
     *  UnitDelay: '<S2>/Unit Delay1'
     */
    if (IVN_SecurityAlgorithm_DW.is_active_c3_IVN_SecurityAlgori == 0U) {
      IVN_SecurityAlgorithm_DW.is_active_c3_IVN_SecurityAlgori = 1U;
      IVN_SecurityAlgorithm_DW.u16_FCIncCntrlTiTh_C = (uint16_T)
        (IVN_SecurityAlgorithm_U.U8_cfg_FCIncCntrTiTh_C *
         IVN_Sec_U8_FCIncCntrlTiTh_C_Res);
      IVN_SecurityAlgorithm_DW.u16_SynchWndTiTh_c = (uint16_T)
        (IVN_SecurityAlgorithm_U.U8_cfg_SynchWndTiTh_C *
         IVN_Secur_U8_SynchWndTiTh_c_Res);
      IVN_SecurityAlgorithm_DW.is_IVN_FRESHNESS_COUNTER =
        IVN_SecurityAlgorith_IN_IGN_OFF;
      IVN_SecurityAlgorithm_DW.u16_InternalTimer_T2 = 0U;
      IVN_SecurityAlgorithm_DW.u16_SynchWindowTimer = 0U;
      IVN_SecurityAlgorithm_DW.b_SynchWndTimerStartFlag =
        IVN_SecurityAlgorithm_RESET;
      IVN_SecurityAlgorithm_DW.u8_FcErrReptCntr = 0U;
      IVN_SecurityAlgorithm_DW.U8_FcErrHealCntr = 0U;

      /* Outport: '<Root>/U32_MacTxBuffer' incorporates:
       *  Inport: '<Root>/U8_cfg_FCIncCntrTiTh_C'
       *  Inport: '<Root>/U8_cfg_SynchWndTiTh_C'
       */
      IVN_SecurityAlgorithm_Y.U32_MacTxBuffer = 0U;
    } else {
      if (IVN_SecurityAlgorithm_DW.is_IVN_FRESHNESS_COUNTER ==
          IVN__IN_FC_IncrementState_IGNON) {
        if (!IVN_SecurityAlgorithm_U.U8_IgnMode) {
          IVN_SecurityAlgorithm_DW.is_IVN_FRESHNESS_COUNTER =
            IVN_SecurityAlgorith_IN_IGN_OFF;
          IVN_SecurityAlgorithm_DW.u16_InternalTimer_T2 = 0U;
          IVN_SecurityAlgorithm_DW.u16_SynchWindowTimer = 0U;
          IVN_SecurityAlgorithm_DW.b_SynchWndTimerStartFlag =
            IVN_SecurityAlgorithm_RESET;
        } else {
          u32_InternalCalculatedMAC0 = (uint32_T)
            IVN_SecurityAlgorithm_DW.u16_InternalTimer_T2 +
            IVN_SecurityAlgorithm_U.U8_ConfigStepTime;
          if (u32_InternalCalculatedMAC0 > 65535U) {
            u32_InternalCalculatedMAC0 = 65535U;
          }

          IVN_SecurityAlgorithm_DW.u16_InternalTimer_T2 = (uint16_T)
            u32_InternalCalculatedMAC0;
          if (IVN_SecurityAlgorithm_DW.u16_InternalTimer_T2 >=
              IVN_SecurityAlgorithm_DW.u16_FCIncCntrlTiTh_C) {
            if (IVN_SecurityAlgorithm_Y.U32_FreshnessCounter ==
                IVN_SecurityAlgori_U32_OVERFLOW) {
              IVN_SecurityAlgorithm_Y.U32_FreshnessCounter = 0U;
              IVN_SecurityAlgorithm_DW.u16_InternalTimer_T2 = 0U;
            } else {
              u32_InternalCalculatedMAC0 =
                IVN_SecurityAlgorithm_Y.U32_FreshnessCounter + /*MW:OvSatOk*/ 1U;
              if (u32_InternalCalculatedMAC0 <
                  IVN_SecurityAlgorithm_Y.U32_FreshnessCounter) {
                u32_InternalCalculatedMAC0 = MAX_uint32_T;
              }

              IVN_SecurityAlgorithm_Y.U32_FreshnessCounter =
                u32_InternalCalculatedMAC0;
              IVN_SecurityAlgorithm_DW.u16_InternalTimer_T2 = 0U;
            }
          }

          if (IVN_SecurityAlgorithm_DW.b_SynchWndTimerStartFlag) {
            u32_InternalCalculatedMAC0 = (uint32_T)
              IVN_SecurityAlgorithm_DW.u16_SynchWindowTimer +
              IVN_SecurityAlgorithm_U.U8_ConfigStepTime;
            if (u32_InternalCalculatedMAC0 > 65535U) {
              u32_InternalCalculatedMAC0 = 65535U;
            }

            IVN_SecurityAlgorithm_DW.u16_SynchWindowTimer = (uint16_T)
              u32_InternalCalculatedMAC0;
            if (IVN_SecurityAlgorithm_DW.u16_SynchWindowTimer >=
                IVN_SecurityAlgorithm_DW.u16_SynchWndTiTh_c) {
              IVN_SecurityAlgorithm_DW.b_SynchWndTimerStartFlag =
                IVN_SecurityAlgorithm_RESET;
              IVN_SecurityAlgorithm_DW.u16_SynchWindowTimer = 0U;

              /* Outport: '<Root>/B_Ivn_LimpHome' */
              IVN_SecurityAlgorithm_Y.B_Ivn_LimpHome = IVN_SecurityAlgorithm_SET;
            }
          } else {
            IVN_SecurityAlgorithm_DW.u16_SynchWindowTimer = 0U;
          }
        }
      } else {
        if (IVN_SecurityAlgorithm_U.U8_IgnMode) {
          IVN_SecurityAlgorithm_DW.is_IVN_FRESHNESS_COUNTER =
            IVN__IN_FC_IncrementState_IGNON;
          IVN_SecurityAlgorithm_DW.b_TxMAC_Indicator =
            IVN_SecurityAlgorithm_MAC_1;

          /* Outport: '<Root>/U32_FreshnessCounter' incorporates:
           *  Inport: '<Root>/U32_EEFreshnessCounter'
           */
          IVN_SecurityAlgorithm_Y.U32_FreshnessCounter =
            IVN_SecurityAlgorithm_U.U32_EEFreshnessCounter;
        }
      }

      if (IVN_SecurityAlgorithm_U.U8_RxNSMMsgArrive !=
          IVN_SecurityAlgorithm_DW.UnitDelay_DSTATE) {
        if (!IVN_SecurityAlgorithm_U.B_RxMAC_Indicator) {
          Decryption_1(IVN_SecurityAlgorithm_U.U32_MacRxBuffer,
                       IVN_SecurityAlgorithm_U.U32_SecurityKeyConst,
                       IVN_SecurityAlgorithm_U.U32_SecurityKey_1,
                       &u32_DecrptedFCounter);
          if (u32_DecrptedFCounter >
              IVN_SecurityAlgorithm_Y.U32_FreshnessCounter) {
            u32_InternalCalculatedMAC0 = u32_DecrptedFCounter - /*MW:OvSatOk*/
              IVN_SecurityAlgorithm_Y.U32_FreshnessCounter;
            if (u32_InternalCalculatedMAC0 > u32_DecrptedFCounter) {
              u32_InternalCalculatedMAC0 = 0U;
            }
          } else {
            u32_InternalCalculatedMAC0 =
              IVN_SecurityAlgorithm_Y.U32_FreshnessCounter - /*MW:OvSatOk*/
              u32_DecrptedFCounter;
            if (u32_InternalCalculatedMAC0 >
                IVN_SecurityAlgorithm_Y.U32_FreshnessCounter) {
              u32_InternalCalculatedMAC0 = 0U;
            }
          }

          if (u32_InternalCalculatedMAC0 <=
              IVN_SecurityAlgorithm_U.U8_cfg_FCDevTh_C) {
            if (IVN_SecurityAlgorithm_DW.u8_FcErrReptCntr >
                IVN_SecurityAlgorithm_U.U8_cfg_FcErrReptDec_C) {
              u32_InternalCalculatedMAC0 = (uint32_T)
                IVN_SecurityAlgorithm_DW.u8_FcErrReptCntr - /*MW:OvSatOk*/
                IVN_SecurityAlgorithm_U.U8_cfg_FcErrReptDec_C;
              if (u32_InternalCalculatedMAC0 >
                  IVN_SecurityAlgorithm_DW.u8_FcErrReptCntr) {
                u32_InternalCalculatedMAC0 = 0U;
              }

              IVN_SecurityAlgorithm_DW.u8_FcErrReptCntr = (uint8_T)
                u32_InternalCalculatedMAC0;
            } else {
              IVN_SecurityAlgorithm_DW.u8_FcErrReptCntr = 0U;
            }

            if (IVN_SecurityAlgorithm_DW.U8_FcErrHealCntr > 0) {
              if (IVN_SecurityAlgorithm_DW.U8_FcErrHealCntr >
                  IVN_SecurityAlgorithm_U.U8_cfg_FcErrHealDec_C) {
                /* Outport: '<Root>/U8_CANSignal_State' */
                IVN_SecurityAlgorithm_Y.U8_CANSignal_State =
                  IVN_SecurityAlgorithm_NOT_OK;
                u32_InternalCalculatedMAC0 = (uint32_T)
                  IVN_SecurityAlgorithm_DW.U8_FcErrHealCntr - /*MW:OvSatOk*/
                  IVN_SecurityAlgorithm_U.U8_cfg_FcErrHealDec_C;
                if (u32_InternalCalculatedMAC0 >
                    IVN_SecurityAlgorithm_DW.U8_FcErrHealCntr) {
                  u32_InternalCalculatedMAC0 = 0U;
                }

                IVN_SecurityAlgorithm_DW.U8_FcErrHealCntr = (uint8_T)
                  u32_InternalCalculatedMAC0;
              } else {
                IVN_SecurityAlgorithm_DW.u8_FcErrReptCntr = 0U;
              }
            } else {
              /* Outport: '<Root>/U8_CANSignal_State' */
              IVN_SecurityAlgorithm_Y.U8_CANSignal_State =
                IVN_SecurityAlgorithm_E_OK;
              IVN_SecurityAlgorithm_DW.u8_FcErrReptCntr = 0U;
            }
          } else {
            /* Outport: '<Root>/U8_CANSignal_State' */
            IVN_SecurityAlgorithm_Y.U8_CANSignal_State =
              IVN_SecurityAlgorithm_NOT_OK;
            if (IVN_SecurityAlgorithm_DW.u8_FcErrReptCntr >=
                IVN_SecurityAlgorithm_U.U8_cfg_FcDifErrReptTh_C) {
              IVN_SecurityAlgorithm_DW.U8_FcErrHealCntr = 10U;
              IVN_SecurityAlgorithm_DW.b_TxMAC_Indicator =
                IVN_SecurityAlgorithm_MAC_2;
              IVN_SecurityAlgorithm_DW.b_SynchWndTimerStartFlag =
                IVN_SecurityAlgorithm_SET;

              /* Outport: '<Root>/B_FcErrorStatusBit' */
              IVN_SecurityAlgorithm_Y.B_FcErrorStatusBit =
                IVN_SecurityAlgorithm_SET;

              /* Outport: '<Root>/B_NsmDTCStatus' */
              IVN_SecurityAlgorithm_Y.B_NsmDTCStatus = IVN_SecurityAlgorithm_SET;
            } else {
              tmp = (int32_T)((uint32_T)
                              IVN_SecurityAlgorithm_DW.u8_FcErrReptCntr +
                              IVN_SecurityAlgorithm_U.U8_cfg_FcErrReptInc_C);
              if ((uint32_T)tmp > 255U) {
                tmp = 255;
              }

              IVN_SecurityAlgorithm_DW.u8_FcErrReptCntr = (uint8_T)tmp;
            }
          }
        } else {
          /* Outport: '<Root>/U8_CANSignal_State' */
          IVN_SecurityAlgorithm_Y.U8_CANSignal_State =
            IVN_SecurityAlgorithm_NOT_OK;
        }
      } else {
        /* Outport: '<Root>/B_FcErrorStatusBit' */
        IVN_SecurityAlgorithm_Y.B_FcErrorStatusBit = IVN_SecurityAlgorithm_RESET;

        /* Outport: '<Root>/B_NsmDTCStatus' */
        IVN_SecurityAlgorithm_Y.B_NsmDTCStatus = IVN_SecurityAlgorithm_RESET;
      }

      if (IVN_SecurityAlgorithm_U.U8_RxSynchMsgArrive !=
          IVN_SecurityAlgorithm_DW.UnitDelay1_DSTATE) {
        Encryption_0(IVN_SecurityAlgorithm_U.U32_SecurityKeyConst,
                     IVN_SecurityAlgorithm_U.U32_SecurityKey_1,
                     &u32_InternalCalculatedMAC0);
        if (IVN_SecurityAlgorithm_U.U32_MAC0RxBuffer ==
            u32_InternalCalculatedMAC0) {
          IVN_SecurityAlgorithm_DW.b_SynchWndTimerStartFlag =
            IVN_SecurityAlgorithm_RESET;
          Decryption_1(IVN_SecurityAlgorithm_U.U32_MacRxBuffer,
                       IVN_SecurityAlgorithm_U.U32_SecurityKeyConst,
                       IVN_SecurityAlgorithm_U.U32_SecurityKey_1,
                       &u32_DecrptedFCounter);
          IVN_SecurityAlgorithm_Y.U32_FreshnessCounter = u32_DecrptedFCounter;

          /* Outport: '<Root>/B_TxMAC_Indicator' */
          IVN_SecurityAlgorithm_Y.B_TxMAC_Indicator =
            IVN_SecurityAlgorithm_MAC_1;
        } else {
          IVN_SecurityAlgorithm_DW.b_SynchWndTimerStartFlag =
            IVN_SecurityAlgorithm_SET;
        }
      }

      if (IVN_SecurityAlgorithm_U.U8_IgnMode) {
        if (!IVN_SecurityAlgorithm_DW.b_TxMAC_Indicator) {
          Encryption_1(IVN_SecurityAlgorithm_U.U32_SecurityKeyConst,
                       IVN_SecurityAlgorithm_U.U32_SecurityKey_1,
                       IVN_SecurityAlgorithm_Y.U32_FreshnessCounter,
                       &u32_InternalCalculatedMAC0);

          /* Outport: '<Root>/U32_MacTxBuffer' */
          IVN_SecurityAlgorithm_Y.U32_MacTxBuffer = u32_InternalCalculatedMAC0;

          /* Outport: '<Root>/B_TxMAC_Indicator' */
          IVN_SecurityAlgorithm_Y.B_TxMAC_Indicator =
            IVN_SecurityAlgorithm_MAC_1;

          /* Outport: '<Root>/B_TransmitCANFrame' */
          IVN_SecurityAlgorithm_Y.B_TransmitCANFrame =
            IVN_SecurityAlgorithm_SEND_OK;
        } else {
          Encryption_2(IVN_SecurityAlgorithm_U.U32_SecurityKeyConst,
                       IVN_SecurityAlgorithm_U.U32_SecurityKey_1,
                       IVN_SecurityAlgorithm_U.U32_SecurityKey_2,
                       &u32_InternalCalculatedMAC0);

          /* Outport: '<Root>/U32_Mac2TxBuffer' incorporates:
           *  Inport: '<Root>/U32_SecurityKey_2'
           */
          IVN_SecurityAlgorithm_Y.U32_Mac2TxBuffer = u32_InternalCalculatedMAC0;

          /* Outport: '<Root>/B_TxMAC_Indicator' */
          IVN_SecurityAlgorithm_Y.B_TxMAC_Indicator =
            IVN_SecurityAlgorithm_MAC_2;

          /* Outport: '<Root>/B_TransmitCANFrame' */
          IVN_SecurityAlgorithm_Y.B_TransmitCANFrame =
            IVN_SecurityAlgorithm_SEND_OK;
        }
      } else {
        /* Outport: '<Root>/B_TransmitCANFrame' */
        IVN_SecurityAlgorithm_Y.B_TransmitCANFrame = IVN_SecurityAlgorithm_RESET;
      }
    }

    /* End of Chart: '<S2>/IVN_SecurityLayer' */

    /* Update for UnitDelay: '<S2>/Unit Delay' incorporates:
     *  Inport: '<Root>/U8_RxNSMMsgArrive'
     */
    IVN_SecurityAlgorithm_DW.UnitDelay_DSTATE =
      IVN_SecurityAlgorithm_U.U8_RxNSMMsgArrive;

    /* Update for UnitDelay: '<S2>/Unit Delay1' incorporates:
     *  Inport: '<Root>/U8_RxSynchMsgArrive'
     */
    IVN_SecurityAlgorithm_DW.UnitDelay1_DSTATE =
      IVN_SecurityAlgorithm_U.U8_RxSynchMsgArrive;
  }

  /* End of Inport: '<Root>/B_SecurityEnableFlag' */
  /* End of Outputs for SubSystem: '<S1>/IVN_SecurityLayer' */
}

/* Model initialize function */
void IVN_SecurityAlgorithm_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(IVN_SecurityAlgorithm_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&IVN_SecurityAlgorithm_DW, 0,
                sizeof(DW_IVN_SecurityAlgorithm_T));

  /* external inputs */
  (void)memset(&IVN_SecurityAlgorithm_U, 0, sizeof(ExtU_IVN_SecurityAlgorithm_T));

  /* external outputs */
  (void) memset((void *)&IVN_SecurityAlgorithm_Y, 0,
                sizeof(ExtY_IVN_SecurityAlgorithm_T));

  /* SystemInitialize for Enabled SubSystem: '<S1>/IVN_SecurityLayer' */
  /* SystemInitialize for Chart: '<S2>/IVN_SecurityLayer' */
  IVN_SecurityAlgorithm_DW.is_IVN_FRESHNESS_COUNTER =
    IVN_Security_IN_NO_ACTIVE_CHILD;
  IVN_SecurityAlgorithm_DW.is_active_c3_IVN_SecurityAlgori = 0U;

  /* End of SystemInitialize for SubSystem: '<S1>/IVN_SecurityLayer' */
}

/* Model terminate function */
void IVN_SecurityAlgorithm_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */

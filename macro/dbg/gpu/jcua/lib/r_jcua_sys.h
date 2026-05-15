/*
**************************************************************************** 
PROJECT : JCUA driver
FILE    : $Id: r_jcua_sys.h 14751 2017-09-25 04:27:33Z shinya.tomari $
============================================================================ 
DESCRIPTION
JCUA support functions
============================================================================
                            C O P Y R I G H T
============================================================================
                           Copyright (c) 2013 - 2017
                                  by
                       Renesas Electronics (Europe) GmbH.
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing, not for mass production

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

SAMPLE CODE is not part of the licensed software, as such it must not be used in
mass-production applications. It can only be used for evaluation and
demonstration purposes at customer's premises listed in the signed license
agreement.

****************************************************************************
*/


/***************************************************************************
  Title: JCUA Driver support functions

  JCUA (JPEG Codec Unit A) driver support functions

  The generic JCUA driver uses these functions. They have to be implemented 
  within the driver library for a concrete device. (e.g. D1L, D1M)

  Support functions are functions that are not part of the driver itself 
  but they must be provided to integrate the driver on a particular board.
*/

#ifndef R_JCUA_SYS_H
#define R_JCUA_SYS_H

#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************************
  Section: Global Functions
*/
/***************************************************************************
  Group: JCUA driver Basic interface functions
*/

/***************************************************************************
  Function: R_JCUA_Sys_Init

  Description:
  This function initializes environment-dependent part.
  This function is called from <R_JCUA_Init>.

  This function executes the following processing.

  * Bus reset for JCUA H/W macro (refer to OPC(Operating Precautions) item #12 "JCUA reset operation").
  >         -> JCCMD.BRST must not be used. (JCU macro internal reset not sufficent)
  >         -> JCSWRST.JCUA0RES must be used instead. (JCU macro external reset)
  >            Refer to D1x UM section 44.6 "Software Reset Processing".
  >              Use the following software reset procedure to reset the JCUA unit:
  >              0. Wait for 30us.
  >              1. Set JCSWRST.JCUA0RES bit to 1, to generate the software reset condition.
  >              2. Read JCSWRST.JCUA0RES bit until bit is set to 1, to wait for software reset is active.
  >              3. Set JCSWRST.JCUA0RES bit to 0, to release software reset condition.
  >              By this sequennce all JCUA registers with exception of the JCSWRST register are initialized by a software reset.

  * Enable interrupt for JCUA H/W macro.
  > Call R_JCUA_Sys_InterruptEnable(Unit)

  Customizing Points:
  It is NOT necessary to modify this function in general use-case.
  But this function is using the TICK driver or OSTM driver for wait process. 
  If you don't want to use TICK driver or OSTM driver, please modify wait process.

  Parameter:
  Unit     - Instance number.
  
  Return value:
  R_JCUA_ERR_OK               - No error has occurred.
  R_JCUA_ERR_RANGE_UNIT       - Unit number is out of range.
  R_JCUA_ERR_FATAL_HW         - Fatal error has occurred at H/W.

  See <r_jcua_Error_t>. 
*/

r_jcua_Error_t R_JCUA_Sys_Init(const uint32_t Unit);


/***************************************************************************
  Function: R_JCUA_Sys_DeInit

  Description:
  This function de-initializes environment-dependent part.
  This function is called from <R_JCUA_DeInit>.

  This function executes the following processing.

  * Disable interrupt for JCUA H/W macro.
  > Call R_JCUA_Sys_InterruptDisable(Unit)

  * Close the H/W timer for the wait process. (if necessary)

  Customizing Points:
  It is NOT necessary to modify this function in general use-case.
  But this function is using the TICK driver or OSTM driver for wait process. 
  If you don't want to use TICK driver or OSTM driver, please modify wait process.

  Parameter:
  Unit     - Instance number.
  
  Return value:
  R_JCUA_ERR_OK               - No error has occurred.
  R_JCUA_ERR_RANGE_UNIT       - Unit number is out of range.
  R_JCUA_ERR_FATAL_HW         - Fatal error has occurred at H/W.

  See <r_jcua_Error_t>. 
*/

r_jcua_Error_t R_JCUA_Sys_DeInit(const uint32_t Unit);


/***************************************************************************
  Function: R_JCUA_Sys_InterruptEnable

  Description:
  This function is enable interrupt request.
  This function is called from <R_JCUA_DecoderStart> and <R_JCUA_DecoderContinue>.

  This function executes the following processing.
  * Enables the JEDI and JDTI interrupt.

  Customizing Points:
  It is NOT necessary to modify this function in general use-case.

  Parameter:
  Unit     - Instance number.
  
  Return value:
  None. 
*/
void R_JCUA_Sys_InterruptEnable(const uint32_t Unit);


/***************************************************************************
  Function: R_JCUA_Sys_InterruptDisable

  Description:
  This function is disable interrupt request.
  This function is called from several JCUA APIs.

  This function executes the following processing.
  * Disables the JEDI and JDTI interrupt.

  Customizing Points:
  It is NOT necessary to modify this function in general use-case.

  Parameter:
  Unit     - Instance number.
  
  Return value:
  None.
*/
void R_JCUA_Sys_InterruptDisable(const uint32_t Unit);


/***************************************************************************
  Function: R_JCUA_Sys_BaseAddrGet

  Description:
  This function gives back the base address of JCUA H/W macro's register.
  This function is called from several JCUA APIs.

  This function executes the following processing.
  * Returns the base address of JCUA H/W macro's register.

  Customizing Points:
  It is NOT necessary to modify this function in general use-case.

  Parameter:
  Unit     - Instance number.

  Return value:
  uint32_t - base address of macro instance (Unit).
*/
uint32_t R_JCUA_Sys_BaseAddrGet(const uint32_t Unit);

    
/***************************************************************************
  Function: R_JCUA_Sys_TimerStart

  Description:
  This function starts timeout measurement.
  This function is called from <R_JCUA_DecoderStart> and <R_JCUA_IsrStop>.
  
  This function executes the following processing.
  * Starts the timer for timeout measurement.
  * Calls <R_JCUA_IsrTimeOut> calllback function when timeout occurs. (This process is executed 
    by loc_OstmCallback.)

  Note:
  Please note that this function is called from <R_JCUA_IsrStop>.
  This may be called from ISR depending on the system.

  Customizing Points:
  It is NOT necessary to modify this function in general use-case.
  But this function is using the OSTM driver for timeout measurement. 
  If you don't want to use OSTM driver, please modify timeout measurement process.

  Parameter:
  Unit     - Instance number.
  IsHeader - The parameter specified the decoding part. 
              R_TRUE  : JPEG Header part 
              R_FALSE : JPEG Image data part

  Return value:
  R_JCUA_ERR_OK               - No error has occurred.
  R_JCUA_ERR_RANGE_UNIT       - Unit number is out of range.
  R_JCUA_ERR_PARAM_INCORRECT  - A parameter is incorrect.
  R_JCUA_ERR_FATAL_HW         - Fatal error has occurred at H/W.

  See <r_jcua_Error_t>. 
*/
r_jcua_Error_t R_JCUA_Sys_TimerStart(const uint32_t Unit, const uint32_t IsHeader);

    
/***************************************************************************
  Function: R_JCUA_Sys_TimerStop

  Description:
  This function stops timeout measurement.
  This function is called from <R_JCUA_IsrFinish> and <R_JCUA_IsrStop>.

  This function executes the following processing.
  * Stops the timer for timeout measurement.

  Note:
  Please note that this function is called from <R_JCUA_IsrFinish> and <R_JCUA_IsrStop>.
  These may be called from ISR depending on the system.
  
  Customizing Points:
  It is NOT necessary to modify this function in general use-case.
  But this function is using the OSTM driver for timeout measurement. 
  If you don't want to use OSTM driver, please modify timeout measurement process.

  Parameter:
  Unit     - Instance number.

  Return value:
  R_JCUA_ERR_OK               - No error has occurred.
  R_JCUA_ERR_RANGE_UNIT       - Unit number is out of range.
  R_JCUA_ERR_FATAL_HW         - Fatal error has occurred at H/W.

  See <r_jcua_Error_t>. 
*/
r_jcua_Error_t R_JCUA_Sys_TimerStop(const uint32_t Unit);

    
/***************************************************************************
  Function: R_JCUA_Sys_TimerPause

  Description:
  This function suspends timeout measurement (pause).
  This function stores the remaining counter until timeout.
  This function is called from <R_JCUA_IsrFinish>.

  This function executes the following processing.
  * Pauses the timer for timeout measurement.

  Note:
  Please note that this function is called from <R_JCUA_IsrFinish>.
  This may be called from ISR depending on the system.
  
  Customizing Points:
  It is NOT necessary to modify this function in general use-case.
  But this function is using the OSTM driver for timeout measurement. 
  If you don't want to use OSTM driver, please modify timeout measurement process.

  Parameter:
  Unit     - Instance number.

  Return value:
  R_JCUA_ERR_OK               - No error has occurred.
  R_JCUA_ERR_RANGE_UNIT       - Unit number is out of range.
  R_JCUA_ERR_FATAL_HW         - Fatal error has occurred at H/W.

  See <r_jcua_Error_t>. 
*/
r_jcua_Error_t R_JCUA_Sys_TimerPause(const uint32_t Unit);

    
/***************************************************************************
  Function: R_JCUA_Sys_TimerResume

  Description:
  This function resumes timeout measurement.
  The timeout value is the remaining count value stored at <R_JCUA_Sys_TimerPause>.
  This function is called from <R_JCUA_DecoderContinue>.

  This function executes the following processing.
  * Resumes the timer for timeout measurement.

  Customizing Points:
  It is NOT necessary to modify this function in general use-case.
  But this function is using the OSTM driver for timeout measurement. 
  If you don't want to use OSTM driver, please modify timeout measurement process.

  Parameter:
  Unit     - Instance number.

  Return value:
  R_JCUA_ERR_OK               - No error has occurred.
  R_JCUA_ERR_RANGE_UNIT       - Unit number is out of range.
  R_JCUA_ERR_FATAL_HW         - Fatal error has occurred at H/W.

  See <r_jcua_Error_t>. 
*/
r_jcua_Error_t R_JCUA_Sys_TimerResume(const uint32_t Unit);


/***************************************************************************
  Group: JCUA driver OS interface functions
*/

/***************************************************************************
  Function: R_JCUA_Sys_Lock

  Description:
  This function locks the JCUA driver access to the specified unit from other threads.
  This function is called from several JCUA APIs.
  
  This function is empty as default.

  Customizing Points:
  Please implement the lock process by mutex or semaphore if JCUA API is called from 
  multi-thread.

  Parameter:
  Unit     - Instance number.
  
  Return value:
  R_JCUA_ERR_OK               - No error has occurred.
  R_JCUA_ERR_RANGE_UNIT       - Unit number is out of range.
  R_JCUA_ERR_FATAL_OS         - Fatal error has occurred at OS interface.

  See <r_jcua_Error_t>. 
*/

r_jcua_Error_t R_JCUA_Sys_Lock(const uint32_t Unit);


/***************************************************************************
  Function: R_JCUA_Sys_Unlock

  Description:
  This function unlocks the JCUA driver access to the specified unit from other threads.
  This function is called from several JCUA APIs.

  This function is empty as default.

  Customizing Points:
  Please implement the unlock process depending on <R_JCUA_Sys_Lock>.

  Parameter:
  Unit     - Instance number.
  
  Return value:
  R_JCUA_ERR_OK               - No error has occurred.
  R_JCUA_ERR_RANGE_UNIT       - Unit number is out of range.
  R_JCUA_ERR_FATAL_OS         - Fatal error has occurred at OS interface.

  See <r_jcua_Error_t>. 
*/

r_jcua_Error_t R_JCUA_Sys_Unlock(const uint32_t Unit);

#ifdef __cplusplus
}
#endif

#endif /* R_JCUA_SYS_H */


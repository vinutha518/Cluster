/*
****************************************************************************
PROJECT : Sprite engine
FILE    : $Id: r_spea_sys.h 15442 2017-12-19 11:33:55Z florian.zimmermann $
============================================================================ 
DESCRIPTION
SPEA support functions
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

/*******************************************************************************
  Title: SPEA Driver support functions

  SPEA (Sprite Engine A) driver support functions

  The generic SPEA driver uses these functions. They have to be implemented 
  within the driver library for a concrete device. (e.g. D1L, D1M)

  Support functions are functions that are not part of the driver itself 
  but they must be provided to integrate the driver on a particular board.
*/

#ifndef R_SPEA_SYS_H_
#define R_SPEA_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Section: Global Functions
*/

/***************************************************************************
  Group: SPEA driver Basic interface functions
*/

/*******************************************************************************
  Function: R_SPEA_SYS_HardwareInit

  Description:
  Setup the H/W for the generic SPEA.
  This function is called from <R_SPEA_Init>.

  This function executes the following processing.  
  * Initialize the sprite engine update timing control register (SPEAUPDEN).

  Customizing Points:
  It is NOT necessary to modify this function in general use-case.

  Parameter:
  Unit       - Instance number
  
  Return Value:
  R_SPEA_ERR_OK               - No error has occurred.
  R_SPEA_ERR_NG               - Unspecified error

  see: <r_spea_Error_t>
*/
r_spea_Error_t R_SPEA_SYS_HardwareInit(uint32_t Unit);


/*******************************************************************************
  Function: R_SPEA_SYS_HardwareDeInit

  Description:
  Deinit of the HW for the generic SPEA.
  This function is called from <R_SPEA_DeInit>.

  This function executes the following processing.  
  * Deinitialize the variables.

  Customizing Points:
  It is NOT necessary to modify this function in general use-case.
  
  Parameter:
  Unit       - Instance number
  
  Return Value:
  R_SPEA_ERR_OK               - No error has occurred.
  R_SPEA_ERR_NG               - Unspecified error

  see: <r_spea_Error_t>
*/
r_spea_Error_t R_SPEA_SYS_HardwareDeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_SPEA_SYS_BaseAddr

  Description:
  This function gives back the base address of SPEA H/W macro's register.
  This function is called from several SPEA APIs.
  
  This function executes the following processing.
  * Returns the base address of SPEA H/W macro's register.

  Customizing Points:
  It is NOT necessary to modify this function in general use-case.

  Parameter:
  Unit       - Instance number
  
  Return Value:
  Macro base address
*/
uint32_t R_SPEA_SYS_BaseAddr(uint32_t Unit);

    
/*******************************************************************************
  Function: R_SPEA_SYS_ErrorHandler

  Description:
  Low level error handler, called in case there was no user error handle
  assigned for this macro.
  This function is called from callback function.
  
  This function executes the following processing.
  * This function will just call R_DEV_ErrrorHandler (avoid having to assign
    a dedicated low level error handler for all the macros).
    
  Customizing Points:
  It is NOT necessary to modify this function in general use-case.

  Parameter:
  Unit       - Instance number
  Error      - detected error    
  
  Return Value:
  void  
*/
void R_SPEA_SYS_ErrorHandler(uint32_t Unit, r_spea_Error_t Error);  


/*******************************************************************************
  Function: R_SPEA_SYS_IsD1M1A

  Description:
  This function returns the flag of RH850/D1M1A or otherwise.
  This function is called from several SPEA APIs.

  This function executes the following processing.
  * If LSI is RH850/D1M1A this function should return 1.

  Customizing Points:
  It is NOT necessary to modify this function in general use-case.

  Parameter:
  None

  Return Value:
  1 if D1M1A, 0 otherwise
*/
int8_t R_SPEA_SYS_IsD1M1A(void);

/*******************************************************************************
  Function: R_SPEA_SYS_IsD1M1v2

  Description:
  This function returns the flag of RH850/D1M1-V2 or otherwise.
  This function is called from several SPEA APIs.

  This function executes the following processing.
  * If LSI is RH850/D1M1-V2, this function should return 1.

  Customizing Points:
  It is NOT necessary to modify this function in general use-case.

  Parameter:
  None

  Return Value:
  1 if D1M1-V2, 0 otherwise
*/
int8_t R_SPEA_SYS_IsD1M1v2(void);

#ifdef __cplusplus
}
#endif


#endif /* R_SPEA_SYS_H_ */

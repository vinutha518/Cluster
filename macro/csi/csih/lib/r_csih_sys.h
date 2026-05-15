/*
****************************************************************************
PROJECT : CSIH driver
FILE ID : $Id: r_csih_sys.h 7165 2016-01-06 12:33:52Z matthias.nippert $
============================================================================ 
DESCRIPTION
Driver for CSIH macro
============================================================================
                            C O P Y R I G H T                            
============================================================================
                           Copyright (c) 2012
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing

Warranty Disclaimer

Because the Product(s) is licensed free of charge, there is no warranty 
of any kind whatsoever and expressly disclaimed and excluded by Renesas, 
either expressed or implied, including but not limited to those for 
non-infringement of intellectual property, merchantability and/or 
fitness for the particular purpose. 
Renesas shall not have any obligation to maintain, service or provide bug 
fixes for the supplied Product(s) and/or the Application.

Each User is solely responsible for determining the appropriateness of 
using the Product(s) and assumes all risks associated with its exercise 
of rights under this Agreement, including, but not limited to the risks 
and costs of program errors, compliance with applicable laws, damage to 
or loss of data, programs or equipment, and unavailability or 
interruption of operations.

Limitation of Liability

In no event shall Renesas be liable to the User for any incidental, 
consequential, indirect, or punitive damage (including but not limited 
to lost profits) regardless of whether such liability is based on breach 
of contract, tort, strict liability, breach of warranties, failure of 
essential purpose or otherwise and even if advised of the possibility of 
such damages. Renesas shall not be liable for any services or products 
provided by third party vendors, developers or consultants identified or
referred to the User by Renesas in connection with the Product(s) and/or the 
Application.

****************************************************************************

*/

#ifndef  CSIH_SYS_H_
#define  CSIH_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: CSIH Support Functions

  The generic CSIH driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/*******************************************************************************
  Section: Global Functions
*/


/*******************************************************************************
  Function: R_SYS_CSIH_GetCsihBaseAddr

  Gives back base address of dedicated csih macro channel.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/

uint32_t R_SYS_CSIH_GetCsihBaseAddr(uint32_t Unit);


/*******************************************************************************
  Function: R_SYS_CSIH_GetChbaBaseAddr

  Gives back base address of dedicated chba macro channel.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/

uint32_t R_SYS_CSIH_GetChbaBaseAddr(uint32_t Unit);


/*******************************************************************************
  Function: R_SYS_CSIH_GetCsihHwVariant

  Gives back the CSIH variant of the respective unit.
  For example: In D1x two different variants are available
	  		   CSIH0 is the same as also used by F1x.
  	  		   CSIH1 has some slight variances, as described in the D1x UM.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/

uint32_t R_SYS_CSIH_GetCsihHwVariant(uint32_t Unit);

/*******************************************************************************
  Function: R_SYS_CSIH_GetClock

  Gives back clock frequency of dedicated clock input. 
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Clock frequency
*/

uint32_t R_SYS_CSIH_GetClock(uint32_t Unit);


/*******************************************************************************
  Function: R_SYS_CSIH_IntCInit

  Init of CSIH Interrupt for dedicated channel
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_SYS_CSIH_IntCInit(uint32_t Unit);


/*******************************************************************************
  Function: R_SYS_CSIH_EnableInt

  Enable the interrupt controller for 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_csih_Interrupt_t>
  
  Returns:
  void
*/

void R_SYS_CSIH_EnableInt(uint32_t Unit, r_csih_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_SYS_CSIH_DisableInt

  Disable the interrupt controller for 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_csih_Interrupt_t>
  
  Returns:
  void
*/

void R_SYS_CSIH_DisableInt(uint32_t Unit, r_csih_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_SYS_CSIH_GetIntMask

  Returns the value of the masked interrupt mask.
    - TRUE: interrupt masked
    - FALSE: Interrupt is not masked
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_csih_Interrupt_t>
  
  Returns:
  Boolean
*/
uint8_t R_SYS_CSIH_GetIntMask(uint32_t Unit, r_csih_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_SYS_CSIH_GetNbChipSelect

  Gives back the number of available CS outputs of macro channel.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Number of Chip Selects (-1 is error)
*/

int32_t R_SYS_CSIH_GetNbChipSelect(uint32_t Unit);


/*******************************************************************************
  Function: R_SYS_CSIH_ForceClearInt

  Clear interrupt forcefully
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_csih_Interrupt_t>
  Set        - clear with LOC_CSIH_FALSE or LOC_CSIH_TRUE
  Returns:
  nothing
*/
void R_SYS_CSIH_ForceClearInt(uint32_t Unit, r_csih_Interrupt_t Interrupt, uint8_t Set);



#ifdef __cplusplus
}
#endif


#endif /* _CSIH_SYS_H_ */

/*
****************************************************************************
PROJECT : Stand-by Controller Api
FILE    : $Id: stbc_main.c 7262 2016-01-15 13:34:37Z michael.golczewski $
============================================================================ 
DESCRIPTION
Driver for STBC macro
============================================================================
                            C O P Y R I G H T                                    
============================================================================
                           Copyright (c) 2011
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: template for STBC API driver

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

#ifndef _STBC_API_H_
#define _STBC_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "stbc_api.h"


/***********************************************************
  Title: Standby controller API

  An application using standby controller should include this header file.

*/

/***********************************************************
  Section: local defines
*/

/* control registers */ 
#define STBC_WUF0     (0xFFF80400u) /* enable, active high */
#define STBC_WUFMSK0  (0xFFF80404u) /* mask, active low */
#define STBC_WUFC0    (0xFFF80408u) /* wakeup factor clear register */
#define STBC_STBC0PSC (0xFFF80100u) /* standby register */
#define STBC_IOHOLD   (0xFFF80B00u) /* IO hold */

/* options */
#define STBC_DEEPSTOP (0x00000002u) /* Deepstop mode */

/* protected registers */
#define STBC_PROTCMD0 (0xfff80000u)



/***********************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_STBC_SetIoHold

*/
r_Error_t R_STBC_SetIoHold(uint32_t Events)
{
uint32_t val;

    /* set the port to hold the IO state in deep stop 
    */ 
    val = Events;
    R_DEV_WRITE_PROT_REG(STBC_PROTCMD0, STBC_IOHOLD, val);

    return R_ERR_OK;      
}


/*******************************************************************************
  Function: R_STBC_EnableWakeUpFactor

*/
r_Error_t R_STBC_EnableWakeUpFactor(uint32_t WakeupEvents)
{
uint32_t val;

    /* clear the WUFMSK0 — Wake-up factor mask register
       for each wakeup set the bit to 0 
    */ 
    val = (uint32_t) (~WakeupEvents & 0xFFFFFFFF);
    R_DEV_WRITE_REG(32, STBC_WUFMSK0, val);

    return R_ERR_OK;      
}


/*******************************************************************************
  Function: R_STBC_EnableStandby

*/
r_Error_t R_STBC_EnableStandby(void)
{
    /* clear any current wakeups just before before we enter deepstop
       otherwise we will just wakeup again straight away
    */
    R_DEV_WRITE_REG(32, STBC_WUFC0, 0xFFFFFFFFu);

    /* set deepstop */
    R_DEV_WRITE_PROT_REG(STBC_PROTCMD0, STBC_STBC0PSC, STBC_DEEPSTOP);

    return R_ERR_OK;      
}


/*******************************************************************************
  Function: R_STBC_GetWakeUpEvent

*/
uint32_t R_STBC_GetWakeUpEvent(void)
{
uint32_t val;

    /* This register informs of a wake-up event */
    val = R_DEV_READ_REG(32, STBC_WUF0);
   
    return val;      
}


#endif /* _STBC_API_H_  */

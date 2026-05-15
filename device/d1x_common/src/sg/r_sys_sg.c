/*
****************************************************************************
PROJECT : SG driver
FILE    : $Id: r_sys_sg.c 3952 2014-09-24 09:52:47Z golczewskim $
============================================================================ 
DESCRIPTION
SG system functions for D1M2(H) device (r7f701412)
============================================================================
                            C O P Y R I G H T                                    
============================================================================
                           Copyright (c) 2014
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


/*******************************************************************************
  Title: device specific PCMP Support Functions

  Implementation of the PCMP support functions device specific.
*/

/***********************************************************
  Section: Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */

#include "r_config_sg.h"
#include "r_sg_api.h"
#include "r_sg_sys.h"

#include "r_dev_api.h"

/***********************************************************
  Section: Local Defines
*/

static r_dev_IntSel_t loc_IntAddress[R_SG_MACRO_NUM] = 
{
/*              R_SG_INT_SGTIO */
/* unit 0 */    R_DEV_INT_SG0TI, 
/* unit 1 */    R_DEV_INT_SG1TI,   
/* unit 2 */    R_DEV_INT_SG2TI,   
/* unit 3 */    R_DEV_INT_SG3TI,
/* unit 4 */    R_DEV_INT_SG4TI,
};

/*******************************************************************************
  Constant: loc_ClkSelId

  Clock selector ID for each UART unit  
*/
static const r_dev_ClkSel_t loc_ClkSelId[R_SG_MACRO_NUM] = 
{
/*sg0*/    R_DEV_CKS_CLKFIX,
/*sg1*/    R_DEV_CKS_CLKFIX,
/*sg2*/    R_DEV_CKS_CLKFIX,
/*sg3*/    R_DEV_CKS_CLKFIX,
/*sg4*/    R_DEV_CKS_CLKFIX,
};


/***********************************************************
  Section: Global Functions

  Comments see: <r_pcmp_sys.h>
*/

/*******************************************************************************
  Function: R_SYS_CSIG_IntCInit

*/
void R_SYS_SG_IntCInit(uint32_t Unit) 
{
    if (Unit >= R_SG_MACRO_NUM)
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "SG: We do not have this Unit"); 
    }

    /* Enable the interrupts */
    R_DEV_IntEnable(loc_IntAddress[Unit],  1);

}

/***********************************************************
  Function: R_SYS_SG_GetBaseAddr

*/

uint32_t R_SYS_SG_GetBaseAddr(uint32_t Unit)
{
    uint32_t addr;
    if (Unit >= R_SG_MACRO_NUM)
    {
        addr = R_SG_ERR_INSTANCE;
    } else {                                      
        if (Unit == 0) {
            addr = R_SG0_BASE_ADDRESS;
        }
        if (Unit == 1) {
            addr = R_SG1_BASE_ADDRESS;
        }
        if (Unit == 2) {
            addr = R_SG2_BASE_ADDRESS;
        }
        if (Unit == 3) {
            addr = R_SG3_BASE_ADDRESS;
        }
        if (Unit == 4) {
            addr = R_SG4_BASE_ADDRESS;
        }
    }
    return addr;
}


/***********************************************************
  Function: R_SYS_SG_GetClock
*/
uint32_t R_SYS_SG_GetClock(uint32_t Unit)
{
    uint32_t val;
    
    val = R_DEV_ClkFrequencyHz(loc_ClkSelId[Unit]); 
    return val;
}


/***********************************************************
  Function: R_SYS_SG_EnableInt

*/

void R_SYS_SG_EnableInt(uint32_t Unit, r_sg_Interrupt_t Interrupt)
{
    switch (Unit)
    {
    case 0:
        R_DEV_IntEnable(R_DEV_INT_SG0TI, 1);
         
        break;
    default:
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "SG: We do not have this Unit");      
        break; 
    }
}

/***********************************************************
  Function: R_SYS_PCMP_DisableInt

*/

void R_SYS_SG_DisableInt(uint32_t Unit, r_sg_Interrupt_t Interrupt)
{
    switch (Unit)
    {
    case 0:
        R_DEV_IntEnable(R_DEV_INT_SG0TI, 0);
         
        break;
    default:
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "SG: We do not have this Unit"); 
        break;      
    }
}


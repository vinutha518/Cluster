/*
****************************************************************************
PROJECT : CSIH driver
FILE    : $Id: r_csih_isr.c 3935 2014-09-23 08:52:32Z golczewskim $
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


/*******************************************************************************
  Title: CSIH Interrupt Functions

  Handle the CSIH interrupts.
*/

/*******************************************************************************
  Section: Includes
*/
#if defined (__ghs__)
#pragma ghs startnomisra
#endif

#include "r_typedefs.h"
#include "r_cdi_api.h"
#include "r_csih_api.h"
#include "r_csih_sys.h"
#include "r_config_csih.h"
#include "r_csih_regs.h"
#include "r_csih_main.h"

#define LOC_CSIH_NULL (void*)0
/*******************************************************************************
  Section: Global Functions
*/
  
/*******************************************************************************
  Section: Global API Functions
  
  For comments see file r_csih_api.h
*/


/*******************************************************************************
  Function: R_CSIH_SetIsrCallback
  
*/
r_csih_Error_t R_CSIH_SetIsrCallback( uint32_t Unit, r_csih_Interrupt_t IntType, void (*Isr)(void))
{                                    
    if (Unit >= R_CSIH_MACRO_NUM) 
    {
        return R_CSIH_ERR_PARAM; 
    } 
    else 
    {
        r_csih_Dev[Unit].IsrCallback[IntType] = Isr;
    }
    return R_CSIH_ERR_OK;
}


/*******************************************************************************
  Function: R_CSIH_EnableInt
*/

r_csih_Error_t R_CSIH_EnableInt(uint32_t Unit, r_csih_Interrupt_t Interrupt) 
{
    /* Check range */
    if (Unit >= R_CSIH_MACRO_NUM || Interrupt >=R_CSIH_INT_NUM)
    {
        return R_CSIH_ERR_PARAM;
    }
    
    R_SYS_CSIH_EnableInt(Unit, Interrupt); 

    return R_CSIH_ERR_OK; 
}


/*******************************************************************************
  Function: R_CSIH_DisableInt
*/

r_csih_Error_t R_CSIH_DisableInt(uint32_t Unit, r_csih_Interrupt_t Interrupt) 
{
    /* Check range */
    if (Unit >= R_CSIH_MACRO_NUM || Interrupt >= R_CSIH_INT_NUM)
    {
        return R_CSIH_ERR_PARAM; 
    }
    
    R_SYS_CSIH_DisableInt(Unit, Interrupt); 

    return R_CSIH_ERR_OK; 
}


/*******************************************************************************
  Function: R_CSIH_GetIntMask
*/
uint8_t R_CSIH_GetIntMask(uint32_t Unit, r_csih_Interrupt_t Interrupt) 
{
    uint8_t ret;
    ret = R_SYS_CSIH_GetIntMask(Unit, Interrupt); 
    return ret; 
}


/*******************************************************************************
  Function: R_CSIH_FifoCfgInt

*/
r_csih_Error_t R_CSIH_FifoCfgInt(uint32_t Unit, r_csih_Interrupt_t Interrupt, uint8_t Num) 
{    
    uint32_t basecsih;
    r_csih_RegMCtl1_t mctl1;
    
    /* Check parameters */
    if (  Unit >= R_CSIH_MACRO_NUM     || \
          Interrupt >= R_CSIH_INT_NUM  || \
          Num == 0                      || \
          Num > 0xef                       ) 
    {
        R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_PARAM);
        return R_CSIH_ERR_PARAM;
    }
    
    /* Read current value */
    basecsih    = r_csih_Dev[Unit].BaseAddrCsih;
    mctl1.Word  = R_CSIH_READ_REG(32, basecsih + R_CSIH_MCTL_1);
    
    /* Prepare the new value */
    switch(Interrupt) 
    {
        case R_CSIH_INT_TIC:
            mctl1.Bit.Fes = Num;
        break;
        case R_CSIH_INT_TIR:
            mctl1.Bit.Ffs = Num;
        break;
        default:
            R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_PARAM);
            return R_CSIH_ERR_PARAM;
    }
    
    /* Write the new value */
    R_CSIH_WRITE_REG(32, basecsih + R_CSIH_MCTL_1, mctl1.Word);

    return R_CSIH_ERR_OK; 
}


/*******************************************************************************
  Function: R_CSIH_IsrTic
*/

void R_CSIH_IsrTic(uint32_t Unit) 
{
    /* Macro internal Interrupt processing */
    /* Applications interrupt handling */
    if (r_csih_Dev[Unit].IsrCallback[R_CSIH_INT_TIC]  != LOC_CSIH_NULL)
    {
        r_csih_Dev[Unit].IsrCallback[R_CSIH_INT_TIC](); 
    }
}

/*******************************************************************************
  Function: R_CSIH_IsrTijc
*/

void R_CSIH_IsrTijc(uint32_t Unit) 
{
    /* Macro internal Interrupt processing */
    /* Applications interrupt handling */
    if (r_csih_Dev[Unit].IsrCallback[R_CSIH_INT_TIJC] != LOC_CSIH_NULL)
    {
        r_csih_Dev[Unit].IsrCallback[R_CSIH_INT_TIJC](); 
    }
 }

/*******************************************************************************
  Function: R_CSIH_IsrTir
*/

void R_CSIH_IsrTir(uint32_t Unit) 
{
    /* Applications interrupt handling */
    if (r_csih_Dev[Unit].IsrCallback[R_CSIH_INT_TIR] != LOC_CSIH_NULL)
    {
        r_csih_Dev[Unit].IsrCallback[R_CSIH_INT_TIR](); 
    }
}

/*******************************************************************************
  Function: R_CSIH_IsrTire
*/

void R_CSIH_IsrTire(uint32_t Unit) 
{
    /* Macro internal Interrupt processing */
    /* Applications interrupt handling */
    if (r_csih_Dev[Unit].IsrCallback[R_CSIH_INT_TIRE] != LOC_CSIH_NULL)
    {
        r_csih_Dev[Unit].IsrCallback[R_CSIH_INT_TIRE](); 
    }
}    


#if defined (__ghs__)
#pragma ghs endnomisra
#endif

/*
****************************************************************************
PROJECT : VLIB
FILE    : $Id: r_ssif_isr.c 4630 2015-01-13 16:48:47Z golczewskim $
============================================================================ 
DESCRIPTION
Generic SSIF driver
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

DISCLAIMER                                                                   
This software is supplied by Renesas Electronics Corporation and is only     
intended for use with Renesas products. No other uses are authorized. This   
software is owned by Renesas Electronics Corporation and is protected under  
all applicable laws, including copyright laws.                               
THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING  
THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT      
LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE   
AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.          
TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS       
ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR   
ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE  
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                             
Renesas reserves the right, without notice, to make changes to this software 
and to discontinue the availability of this software. By using this software,
you agree to the additional terms and conditions found by accessing the      
following link:                                                              
http://www.renesas.com/disclaimer *                                          
Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.     

****************************************************************************
*/


/*******************************************************************************
  Title: SSIF Interrupt Functions

  Handle the SSIF interrupts.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_ssif.h"
#include "r_ssif_api.h"
#include "r_ssif_sys.h"
#include "r_ssif_regs.h"
#include "r_ssif_main.h"


/*******************************************************************************
  Section: Global API Functions
  
  For comments see: <ssif_api.h>
*/


/*******************************************************************************
  Function: R_SSIF_EnableInt
*/

void R_SSIF_EnableInt(uint32_t Unit, r_ssif_Interrupt_t Interrupt) 
{
    r_ssif_Error_t err;
    uint32_t base;
    uint32_t regval32;
    err = R_SSIF_CheckMacroParams(Unit);

    if ((R_SSIF_ERR_OK != err) || (Interrupt >= R_SSIF_INT_NUM)) 
    {
        R_SSIF_ErrorHandler(Unit, R_SSIF_ERR_RANGE); 
        return; 
    }

    base = r_ssif_Dev[Unit].BaseAddr;

    /* register SSICR */
    regval32 = R_SSIF_READ_REG(32, base + R_SSIF_REG_CR);

    if (Interrupt == R_SSIF_INT_IRQ)
    {
        /* all causes of multi purpose interrupt are enabled. */
        regval32 |= (0x1uL << 29u); /* Tx underflow */
        regval32 |= (0x1uL << 28u); /* Tx overflow  */
        
        regval32 |= (0x1uL << 27u); /* Rx underflow */
        regval32 |= (0x1uL << 26u); /* Rx overflow  */
        
        regval32 |= (0x1uL << 25u); /* idle mode */
    }

    if (Interrupt == R_SSIF_INT_IRQ_IDLE)
    {
        regval32 |= (0x1uL << 25u); /* idle mode */
    }

    if (Interrupt == R_SSIF_INT_IRQ_RXOVER)
    {
        regval32 |= (0x1uL << 26u); /* Rx overflow  */
    }

    if (Interrupt == R_SSIF_INT_IRQ_RXUNDER)
    {
        regval32 |= (0x1uL << 27u); /* Rx underflow */
    }

    if (Interrupt == R_SSIF_INT_IRQ_TXOVER)
    {
        regval32 |= (0x1uL << 28u); /* Tx overflow  */
    }

    if (Interrupt == R_SSIF_INT_IRQ_TXUNDER)
    {
        regval32 |= (0x1uL << 29u); /* Tx underflow */
    }

    R_SSIF_WRITE_REG(32, base + R_SSIF_REG_CR, regval32);

    /* register SSIFCR */
    regval32 = R_SSIF_READ_REG(32, base + R_SSIF_REG_FCR);

    if (Interrupt == R_SSIF_INT_TXI)
    {
        regval32 |= (0x1uL << 3u); /* Tx int enable */
    }

    if (Interrupt == R_SSIF_INT_RXI)
    {
        regval32 |= (0x1uL << 2u); /* Rx int enable */
    }

    R_SSIF_WRITE_REG(32, base + R_SSIF_REG_FCR, regval32);

    /* concentrate IRQ sub-types */
    if ((Interrupt == R_SSIF_INT_TXI) || (Interrupt == R_SSIF_INT_RXI))
    {
        R_SSIF_SYS_EnableInt(Unit, Interrupt); 
    }
    else
    {
        R_SSIF_SYS_EnableInt(Unit, R_SSIF_INT_IRQ); 
    }
}


/*******************************************************************************
  Function: R_SSIF_DisableInt
*/

void R_SSIF_DisableInt(uint32_t Unit, r_ssif_Interrupt_t Interrupt) 
{
    r_ssif_Error_t err;
    uint32_t base;
    uint32_t regval32;

    err = R_SSIF_CheckMacroParams(Unit);

    if ((R_SSIF_ERR_OK != err) || (Interrupt >= R_SSIF_INT_NUM)) 
    {
        R_SSIF_ErrorHandler(Unit, R_SSIF_ERR_RANGE); 
        return; 
    } 
    
    base = r_ssif_Dev[Unit].BaseAddr;

    /* register SSICR */
    regval32 = R_SSIF_READ_REG(32, base + R_SSIF_REG_CR);

    if (Interrupt == R_SSIF_INT_IRQ)
    {
        /* all causes of multi purpose interrupt are enabled. */
        regval32 &= ~(0x1uL << 29u); /* Tx underflow */
        regval32 &= ~(0x1uL << 28u); /* Tx overflow  */
        
        regval32 &= ~(0x1uL << 27u); /* Rx underflow */
        regval32 &= ~(0x1uL << 26u); /* Rx overflow  */
        
        regval32 &= ~(0x1uL << 25u); /* idle mode */
    }

    if (Interrupt == R_SSIF_INT_IRQ_IDLE)
    {
        regval32 &= ~(0x1uL << 25u); /* idle mode */
    }

    if (Interrupt == R_SSIF_INT_IRQ_RXOVER)
    {
        regval32 &= ~(0x1uL << 26u); /* Rx overflow  */
    }

    if (Interrupt == R_SSIF_INT_IRQ_RXUNDER)
    {
        regval32 &= ~(0x1uL << 27u); /* Rx underflow */
    }

    if (Interrupt == R_SSIF_INT_IRQ_TXOVER)
    {
        regval32 &= ~(0x1uL << 28u); /* Tx overflow  */
    }

    if (Interrupt == R_SSIF_INT_IRQ_TXUNDER)
    {
        regval32 &= ~(0x1uL << 29u); /* Tx underflow */
    }

    R_SSIF_WRITE_REG(32, base + R_SSIF_REG_CR, regval32);

    /* register SSIFCR */  
    regval32 = R_SSIF_READ_REG(32, base + R_SSIF_REG_FCR);

    if (Interrupt == R_SSIF_INT_TXI)
    {
        regval32 &= ~(0x1uL << 3u); /* Tx int disable */
    }

    if (Interrupt == R_SSIF_INT_RXI)
    {
        regval32 &= ~(0x1uL << 2u); /* Rx int disable */
    }

    R_SSIF_WRITE_REG(32, base + R_SSIF_REG_FCR, regval32);
  
    R_SSIF_SYS_DisableInt(Unit, Interrupt); 
}

/*******************************************************************************
  Function: R_SSIF_SetIsrCallback
*/

 r_ssif_Error_t R_SSIF_SetIsrCallback(uint32_t Unit, 
                                              r_ssif_Interrupt_t Interrupt,     
                                              void (*Isr)(void)) 
{
    r_ssif_Error_t err; 
    err = R_SSIF_CheckMacroParams(Unit);
    if ((R_SSIF_ERR_OK != err) || (Interrupt >= R_SSIF_INT_NUM)) 
    {
        R_SSIF_ErrorHandler(Unit, R_SSIF_ERR_RANGE); 
        return R_SSIF_ERR_RANGE; 
    } 
        
    r_ssif_Dev[Unit].IsrCallback[Interrupt] = Isr;
    return R_SSIF_ERR_OK;
}

/*******************************************************************************
  Function: R_SSIF_IsrIRQ/RX/TX
*/

void R_SSIF_IsrIRQ(uint32_t Unit)
{
    if (r_ssif_Dev[Unit].IsrCallback[R_SSIF_INT_IRQ] != R_NULL)
    {
        r_ssif_Dev[Unit].IsrCallback[R_SSIF_INT_IRQ]();
    }
    if (r_ssif_Dev[Unit].IsrCallback[R_SSIF_INT_IRQ_IDLE] != R_NULL)
    {
        r_ssif_Dev[Unit].IsrCallback[R_SSIF_INT_IRQ_IDLE]();
    }
    if (r_ssif_Dev[Unit].IsrCallback[R_SSIF_INT_IRQ_RXOVER] != R_NULL)
    {
        r_ssif_Dev[Unit].IsrCallback[R_SSIF_INT_IRQ_RXOVER]();
    }
    if (r_ssif_Dev[Unit].IsrCallback[R_SSIF_INT_IRQ_RXUNDER] != R_NULL)
    {
        r_ssif_Dev[Unit].IsrCallback[R_SSIF_INT_IRQ_RXUNDER]();
    }
    if (r_ssif_Dev[Unit].IsrCallback[R_SSIF_INT_IRQ_TXOVER] != R_NULL)
    {
        r_ssif_Dev[Unit].IsrCallback[R_SSIF_INT_IRQ_TXOVER]();
    }
    if (r_ssif_Dev[Unit].IsrCallback[R_SSIF_INT_IRQ_TXUNDER] != R_NULL)
    {
        r_ssif_Dev[Unit].IsrCallback[R_SSIF_INT_IRQ_TXUNDER]();
    }
}

void R_SSIF_IsrRX(uint32_t Unit)
{
    if (r_ssif_Dev[Unit].IsrCallback[R_SSIF_INT_RXI] != R_NULL)
    {
        r_ssif_Dev[Unit].IsrCallback[R_SSIF_INT_RXI]();
    }
}

void R_SSIF_IsrTX(uint32_t Unit)
{
    if (r_ssif_Dev[Unit].IsrCallback[R_SSIF_INT_TXI] != R_NULL)
    {
        r_ssif_Dev[Unit].IsrCallback[R_SSIF_INT_TXI]();
    }
}


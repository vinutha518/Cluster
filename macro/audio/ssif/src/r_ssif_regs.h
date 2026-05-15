/*
****************************************************************************
PROJECT : SSIF driver
FILE    : $Id: r_ssif_regs.h 4366 2014-11-19 09:43:51Z golczewskim $
============================================================================ 
DESCRIPTION
Register declaration for SSIF macro driver 
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

#ifndef SSIF_REGS_H_
#define SSIF_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: SSIF Register Offsets

  Only the driver modules shall include this header.
*/

/*******************************************************************************
  Section: Global Defines
*/

/***********************************************************
  Constants: Register (offsets) of SSIF0 and SSIF1 registers

  R_SSIF_REG_xxx         - some register
*/

#define R_SSIF_REG_CR       0x00u
#define R_SSIF_REG_SR       0x04u
#define R_SSIF_REG_FCR      0x10u
#define R_SSIF_REG_FSR      0x14u
#define R_SSIF_REG_FTDR     0x18u
#define R_SSIF_REG_FRDR     0x1Cu
#define R_SSIF_REG_TDMR     0x20u
#define R_SSIF_REG_FCCR     0x24u
#define R_SSIF_REG_FCMR     0x28u
#define R_SSIF_REG_FCSR     0x2Cu


/***********************************************************
  Constants: Register of common SSIF0 and SSIF1 register

  R_SSIF_REG_FCLKCFG        - SSIF clock setting register
*/

#define R_SSIF_REG_FCLKCFG      (R_SSIF_COMMON_BASE + 0x2Cu)


/*******************************************************************************
  Section: Global Types
  
  SSIF Register Definitions
*/
/* historical reasons?! */
#if 0  
/***********************************************************
  Type: r_ssif_RegOpmodeBits_t

  Content of the operation mode control register EE_LCDBIF_OPMODE

  Members:
  Opm_ode   - operation mode
  Opm_fast  - cycle speed
  Resn      - reserved (n=1,2)

*/

typedef struct
{
    uint32_t  Opm_ode  : 2;
    uint32_t  Res1     : 2;
    uint32_t  Opm_fast : 1;
    uint32_t Res2     : 27;
} r_ssif_RegOpmodeBits_t;


/***********************************************************
  Union: r_ssif_RegOpmode_t
  
  Simplifies Bit and double word access to register EE_LCDBIF_OPMODE  
  
  Members:
  Bits   - Allow bit access.
  DWord  - Allow access of the whole double word.
*/

typedef union
{
    r_ssif_RegOpmodeBits_t Bits;
    uint32_t                 DWord;
} r_ssif_RegOpmode_t;
#endif

#ifdef __cplusplus
}
#endif

#endif /* SSIF_REGS_H_  */

/*
****************************************************************************
PROJECT : DCRA driver
FILE    : $Id: r_dcra_regs.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Register declaration for DCRA macro driver 
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

#ifndef DCRA_REGS_H_
#define DCRA_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: DCRA Register Offsets

  Only the driver modules shall include this header.
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constants: Register Offsets
    R_DCRA_REG_CIN      - DCRA CRC input register   
    R_DCRA_REG_COUT     - DCRA CRC data register   
    R_DCRA_REG_CTL      - DCRA CRC control register   

*/

#define R_DCRA_REG_CIN          0x00u
#define R_DCRA_REG_COUT         0x04u
#define R_DCRA_REG_CTL          0x20u


/*******************************************************************************
  Section: Global Types
  
  DCRA Register Definitions
*/

/*******************************************************************************
  Type: r_dcra_RegCtlBit_t
  
  Content of control register DCRAnCTL
  
  Members:
  UrteClov      - Clears overrunn error flag
  UrteClf       - Clears framing error detection flag
  UrteClp       - Clears parity error detection flag
  UrteCldc      - Clears consistency error flag
  UrteClbs      - Clears BF reception OK flag 
  Reserved      - Reserved bits  
*/

typedef struct {
    uint8_t DcraPol      : 1; 
    uint8_t DcraIsz      : 2;
    uint8_t Reserved     : 5;  
} r_dcra_RegCtlBit_t;


/*******************************************************************************
  Union: r_dcra_RegCtl_t
  
  Simplifies Bit and Byte access to register DCRAnCTL
  
  Members:
  Bit      - Allow bit access; see: <r_dcra_RegCtl_t>
  Byte     - Allow access ofthe whole byte
*/

typedef union {
    r_dcra_RegCtlBit_t   Bit;
    uint8_t              Byte;
} r_dcra_RegCtl_t;


#ifdef __cplusplus
}
#endif

#endif /* DCRA_REGS_H_  */

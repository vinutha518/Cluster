/*
****************************************************************************
PROJECT : AWOT driver
FILE    : $Id: r_awot_regs.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Driver for AWOT macro
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

#ifndef AWOT_REGS_H_
#define AWOT_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: AWOT Register Offsets

  Only the driver modules shall include this header.
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constants: AWOT Register Offsets
  
    R_AWOT_CCR     - Capture Compare register
    R_AWOT_CMP0    - Compare register 0
    R_AWOT_CMP1    - Compare register 1
    R_AWOT_FLG     - Flag register
    R_AWOT_STC     - Status clear register
    R_AWOT_CTL     - Control register
    R_AWOT_TOE     - Output enable register
    R_AWOT_EMU     - Emulation register
  
*/
  
#define R_AWOT_CCR    (0x00)
#define R_AWOT_CMP0   (0x04)
#define R_AWOT_CMP1   (0x08)
#define R_AWOT_FLG    (0x0c)
#define R_AWOT_STC    (0x10)
#define R_AWOT_CTL    (0x20)
#define R_AWOT_TOE    (0x24)
#define R_AWOT_EMU    (0x28)




/*******************************************************************************
  Type: R_AWOT_Reg_CtlBit_t
  
  Content of control register CTL
  
  Members:
    MD          - Operation mode selection
    Reserved0   - Reserved bits 
    TIS         - Capture input valid edge selection
    Reserved1   - Reserved bits 
    TIS         - Timer enable / disable

*/

typedef struct {
    uint8_t MD          : 2;  
    uint8_t Reserved0   : 1;
    uint8_t TIS         : 1;  
    uint8_t Reserved1   : 3;
    uint8_t TE          : 1;  
} R_AWOT_RegCtlBit_t;


/*******************************************************************************
  Union: R_AWOT_RegCtl_t
  
  Simplifies Bit and Byte access to register CTL
  
  Members:
  Bit      - Allow bit access - see: <R_AWOT_RegCtlBit_t>
  Byte     - Allow access ofthe whole byte
*/

typedef union {
    R_AWOT_RegCtlBit_t  Bit;
    uint8_t             Byte;
} R_AWOT_RegCtl_t;



#ifdef __cplusplus
}
#endif

#endif /* AWOT_REGS_H_  */

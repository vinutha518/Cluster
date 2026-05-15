/*
****************************************************************************
PROJECT : PCMP driver
FILE ID : $Id: r_pcmp_regs.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Driver for PCMP macro
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


#ifndef PCMP_REGS_H_
#define PCMP_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: Pcm Register Offsets

  Only the driver modules shall include this header.
*/

/***********************************************************
  Section: Global Defines
*/

/***********************************************************
  Constants: Register Offsets

  R_PCMP_PCMP0CTL       - Control register
  R_PCMP_PCMP0TPWM      - Counter Period register 
  R_PCMP_PCMP0INV       - Inverter Control Register
  R_PCMP_PCMP0STLV      - Static Level Selection Register
  R_PCMP_PCMP0STEN      - Static Level Enable Register
  R_PCMP_PCMP0OFFS      - Conversion Offset Register
  R_PCMP_PCMP0AUSA      - Audio Sample Input Register
  R_PCMP_PCMP0STR       - Status Register
  R_PCMP_PCMP0STC       - Status Clear Register
  R_PCMP_PCMP0CKSEL     - Clock Selection Register
  R_PCMP_PCMP0EMU       - Emulation Register
*/

#define R_PCMP_PCMP0CTL     0x00
#define R_PCMP_PCMP0TPWM    0x04
#define R_PCMP_PCMP0INV     0x08
#define R_PCMP_PCMP0STLV    0x0C
#define R_PCMP_PCMP0STEN    0x10
#define R_PCMP_PCMP0OFFS    0x14
#define R_PCMP_PCMP0AUSA    0x18
#define R_PCMP_PCMP0STR     0x1C
#define R_PCMP_PCMP0STC     0x20
#define R_PCMP_PCMP0CKSEL   0x24
#define R_PCMP_PCMP0EMU     0x28


/***********************************************************
  Type: eePcmReg_Ctl_t
  
  Control Register
  Content of the PCM PWM control register PCMP0CTL
  
  Members:
  
    Res         - Reserved
    Dom         - Double output mode 
    Stereo      - Stereo mode 
    Mode        - PCM output mode 
    Enab        - Enable PCM-PWM function 
*/  
  
typedef struct {
    uint32_t   Enab     : 1;
    uint32_t   Mode     : 1;        
    uint32_t   Stereo   : 1;       
    uint32_t   Dom      : 1;        
    uint32_t   Res      : 28;
} R_PCMP_RegCtl_t;



/***********************************************************
  Type: R_PCMP_RegTPwm_t
  
  Content of the PCM PWM control register PCMP0TPWM
  
  Members:
  
    Res         - Reserved 
    TPwm        - Period value for PWM generator
*/  
  
typedef struct {
    uint32_t   TPwm     : 16;
    uint32_t   Res      : 16;         
} R_PCMP_RegTPwm_t;


/***********************************************************
  Type: R_PCMP_RegInv_t
  
  Content of the PCM PWM control register PCMP0INV
  
  Members:
  
    Res         - Reserved 
    InvBN1      - Invert B Negativ 1
    InvBN1      - Invert B Positiv 1
    InvBN1      - Invert A Negativ 1
    InvBN1      - Invert A Positiv 1
    Res2        - Reserved
    InvBN0      - Invert B Negativ 0
    InvBN0      - Invert B Positiv 0
    InvBN0      - Invert A Negativ 0
    InvBN0      - Invert A Positiv 0
*/  
  
typedef struct {
    uint32_t   InvAP0   : 1; 
    uint32_t   InvAN0   : 1;        
    uint32_t   InvBP0   : 1;        
    uint32_t   InvBN0   : 1;        
    uint32_t   Res2     : 4;        
    uint32_t   InvAP1   : 1;  
    uint32_t   InvAN1   : 1;        
    uint32_t   InvBP1   : 1;        
    uint32_t   InvBN1   : 1;        
    uint32_t   Res      : 20;              
} R_PCMP_RegInv_t;


/***********************************************************
  Type: R_PCMP_RegStLv_t
  
  Content of the PCM PWM control register PCMP0STLV
  
  Members:
  
    Res         - Reserved 
    StLvBN1     - Static Level B Negativ 1
    StLvBN1     - Static Level B Positiv 1
    StLvBN1     - Static Level A Negativ 1
    StLvBN1     - Static Level A Positiv 1
    Res2        - Reserved
    StLvBN0     - Static Level B Negativ 0
    StLvBN0     - Static Level B Positiv 0
    StLvBN0     - Static Level A Negativ 0
    StLvBN0     - Static Level A Positiv 0
*/  
  
typedef struct {
    uint32_t   StLvAP0  : 1;
    uint32_t   StLvAN0  : 1;        
    uint32_t   StLvBP0  : 1;        
    uint32_t   StLvBN0  : 1;        
    uint32_t   Res2     : 4;     
    uint32_t   StLvAP1  : 1;        
    uint32_t   StLvAN1  : 1;        
    uint32_t   StLvBP1  : 1;        
    uint32_t   StLvBN1  : 1;        
    uint32_t   Res      : 20;                   
} R_PCMP_RegStLv_t;


/***********************************************************
  Type: R_PCMP_RegStEn_t
  
  Content of the PCM PWM control register PCMP0STLV
  
  Members:
  
    Res         - Reserved 
    StEnBN1     - Static Level Enable B Negativ 1
    StEnBN1     - Static Level Enable B Positiv 1
    StEnBN1     - Static Level Enable A Negativ 1
    StEnBN1     - Static Level Enable A Positiv 1
    Res2        - Reserved
    StEnBN0     - Static Level Enable B Negativ 0
    StEnBN0     - Static Level Enable B Positiv 0
    StEnBN0     - Static Level Enable A Negativ 0
    StEnBN0     - Static Level Enable A Positiv 0
*/  
  
typedef struct {
    uint32_t   StEnAP0  : 1;
    uint32_t   StEnAN0  : 1;        
    uint32_t   StEnBP0  : 1;        
    uint32_t   StEnBN0  : 1;        
    uint32_t   Res2     : 4;         
    uint32_t   StEnAP1  : 1;        
    uint32_t   StEnAN1  : 1;        
    uint32_t   StEnBP1  : 1;
    uint32_t   StEnBN1  : 1;        
    uint32_t   Res      : 20;        
} R_PCMP_RegStEn_t;


/***********************************************************
  Type: R_PCMP_RegOffs_t
  
  Content of the PCM PWM control register PCMP0OFFS
  
  Members:
  
    Res         - Reserved 
    Offs        - Offset that is used for the signed to unsigned 
                  conversion in Mode 0
*/  
  
typedef struct {
    uint32_t   Offs     : 16;
    uint32_t   Res      : 16;         
} R_PCMP_RegOffs_t;


/***********************************************************
  Type: R_PCMP_RegAuSA_t
  
  Audio Sample Input Register
  Content of the PCM PWM control register PCMP0AUSA
  
  
  Members:
  
    AuSL        - 16 bit Audio Sample Left 
    AuSR        - 16 bit Audio Sample Right
*/  
  
typedef struct {
    uint32_t   AuSR     : 16;
    uint32_t   AuSL     : 16;         
} R_PCMP_RegAuSA_t;


/***********************************************************
  Type: R_PCMP_RegStR_t
  
  status Register
  Content of the PCM PWM control register PCMP0STR
  
  
  Members:
  
    Res         - Reserved
    Fifo        - Number of samples in FIFO (0~2)
    Res2        - Reserved
    Ov          - FIFO Overflow occured
    Ur          - FIFO Underrun occured
*/  
  
typedef struct {
    uint32_t   Ur       : 1;
    uint32_t   Ov       : 1;        
    uint32_t   Res2     : 12;        
    uint32_t   Fifo     : 2;         
    uint32_t   Res      : 16;         
} R_PCMP_RegStR_t;


/***********************************************************
  Type: R_PCMP_RegStC_t
  
  Clear Status Register
  Content of the PCM PWM control register PCMP0STC
  
  
  Members:
  
    Res         - Reserved
    Ov          - Clear FIFO Overflow
    Ur          - Clear FIFO Underrun
*/  
  
typedef struct {
    uint32_t   Ur       : 1;
    uint32_t   Ov       : 1;        
    uint32_t   Res      : 30;        
} R_PCMP_RegStC_t;


/***********************************************************
  Type: R_PCMP_RegCkSel_t
  
  Clock Selection Register
  Content of the PCM PWM control register PCMP0CKSEL
    
  Members:
  
    Res         - Reserved 
    CkSel       - Clock Selection Register stores the divider
                  value for the Sync Clock Enable Generator
                
*/  
  
typedef struct {
    uint32_t   CkSel    : 3;
    uint32_t   Res      : 29;
} R_PCMP_RegCkSel_t;


/***********************************************************
  Type: R_PCMP_RegEmu_t
  
  Emulation Register
  Content of the PCM PWM control register PCMP0EMU
    
  Members:
  
    Res         - Reserved 
    SvDis       - SVSTOP disable
                
*/  
  
typedef struct {
    uint32_t   Res2     : 7;
    uint32_t   SvDis    : 1;        
    uint32_t   Res      : 24;         
} R_PCMP_RegEmu_t;

                         
/***********************************************************
  Union: R_PCMP_RegCtlBit_t
  
  Simplifies Bit and Byte access to register CTL    
  
  Members:
  Bit       - Allow bit access; see: <void R_PCMP_RegCtl_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_PCMP_RegCtl_t      Bit;
    uint32_t            Word;
} R_PCMP_RegCtlBit_t;
                         

/***********************************************************
  Union: R_PCMP_RegTPwmBit_t _t
  
  Simplifies Bit and Byte access to register TPWM
  
  Members:
  Bit       - Allow bit access; see: <void R_PCMP_RegTPwm_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_PCMP_RegCtl_t      Bit;
    uint32_t            Word;
} R_PCMP_RegTPwmBit_t;
                         

/***********************************************************
  Union: R_PCMP_RegInvBit_t
  
  Simplifies Bit and Byte access to register INV
  
  Members:
  Bit       - Allow bit access; see: <void R_PCMP_RegInv_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_PCMP_RegInv_t      Bit;
    uint32_t            Word;
} R_PCMP_RegInvBit_t;
                         

/***********************************************************
  Union: R_PCMP_RegStLvBit_t
  
  Simplifies Bit and Byte access to register STLV
  
  Members:
  Bit       - Allow bit access; see: <void R_PCMP_RegStLv_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_PCMP_RegStLv_t     Bit;
    uint32_t            Word;
} R_PCMP_RegStLvBit_t;
                         

/***********************************************************
  Union: R_PCMP_RegStEnBit_t
  
  Simplifies Bit and Byte access to register STEN
  
  Members:
  Bit       - Allow bit access; see: <void R_PCMP_RegStEn_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_PCMP_RegStEn_t     Bit;
    uint32_t            Word;
} R_PCMP_RegStEnBit_t;
                         


/***********************************************************
  Union: R_PCMP_RegOffsBit_t
  
  Simplifies Bit and Byte access to register OFFS
  
  Members:
  Bit       - Allow bit access; see: <void R_PCMP_RegOffs_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_PCMP_RegOffs_t     Bit;
    uint32_t            Word;
} R_PCMP_RegOffsBit_t;
                         

/***********************************************************
  Union: R_PCMP_RegAuSABit_t
  
  Simplifies Bit and Byte access to register AUSA
  
  Members:
  Bit       - Allow bit access; see: <void R_PCMP_RegAuSA_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_PCMP_RegAuSA_t     Bit;
    uint32_t            Word;
} R_PCMP_RegAuSABit_t;
                         

/***********************************************************
  Union: R_PCMP_RegStRBit_t
  
  Simplifies Bit and Byte access to register STR
  
  Members:
  Bit       - Allow bit access; see: <void R_PCMP_RegStR_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_PCMP_RegStR_t      Bit;
    uint32_t            Word;
} R_PCMP_RegStRBit_t;
                         

/***********************************************************
  Union: R_PCMP_RegStCBit_t
  
  Simplifies Bit and Byte access to register STC
  
  Members:
  Bit       - Allow bit access; see: <void R_PCMP_RegStC_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_PCMP_RegStC_t      Bit;
    uint32_t            Word;
} R_PCMP_RegStCBit_t;
                         

/***********************************************************
  Union: R_PCMP_RegCkSelBit_t
  
  Simplifies Bit and Byte access to register CKSEL
  
  Members:
  Bit       - Allow bit access; see: <void R_PCMP_RegCkSel_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_PCMP_RegCkSel_t    Bit;
    uint32_t            Word;
} R_PCMP_RegCkSelBit_t;
                         

/***********************************************************
  Union: R_PCMP_RegEmuBit_t
  
  Simplifies Bit and Byte access to register EMU
  
  Members:
  Bit       - Allow bit access; see: <void R_PCMP_RegEmu_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_PCMP_RegEmu_t      Bit;
    uint32_t            Word;
} R_PCMP_RegEmuBit_t;



#ifdef __cplusplus
}
#endif

#endif /* PCMP_REGS_H_  */


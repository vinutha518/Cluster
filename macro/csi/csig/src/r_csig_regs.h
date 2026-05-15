/*
****************************************************************************
PROJECT : CSIG driver
FILE    : $Id: r_csig_regs.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Driver for CSIG macro
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

#ifndef CSIG_REGS_H_
#define CSIG_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: CSIG Register Offsets

  Only the driver modules shall include this header.
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constants: Register Offsets

  R_CSIG_CTL_0     - Control register 0 
  R_CSIG_CTL_1     - Control register 1
  R_CSIG_CTL_2     - Control register 2
  R_CSIG_STR_0     - Status register 0
  R_CSIG_STC_0     - Status clear register 0
  R_CSIG_EMU       - Emulation register
  
*/

#ifndef R_CSIG_USER_OFFSET
  #define R_CSIG_USER_OFFSET(x)  (0x00000000)
#endif
#ifndef R_CSIG_OS_OFFSET
  #define R_CSIG_OS_OFFSET(x) (0x00000000)
#endif

#ifndef R_CSIG_BUFFER_OFFSET_USER
#define R_CSIG_BUFFER_OFFSET_USER 0x0
#endif
#ifndef R_CSIG_BUFFER_OFFSET_OS
#define R_CSIG_BUFFER_OFFSET_OS   0x0
#endif


/* User register */
#define R_CSIG_CTL_0   (R_CSIG_USER_OFFSET(Unit) + 0x00)
#define R_CSIG_STR_0   (R_CSIG_USER_OFFSET(Unit) + 0x04)
#define R_CSIG_STC_0   (R_CSIG_USER_OFFSET(Unit) + 0x08)

/* User register */
#define R_CSIG_BCTL_0  (R_CSIG_USER_OFFSET(Unit) + 0x00 + R_CSIG_BUFFER_OFFSET_USER)
#define R_CSIG_TX0W    (R_CSIG_USER_OFFSET(Unit) + 0x04 + R_CSIG_BUFFER_OFFSET_USER)
#define R_CSIG_TX0H    (R_CSIG_USER_OFFSET(Unit) + 0x08 + R_CSIG_BUFFER_OFFSET_USER)
#define R_CSIG_RX0     (R_CSIG_USER_OFFSET(Unit) + 0x0c + R_CSIG_BUFFER_OFFSET_USER)
#define R_CSIG_CFG0    (R_CSIG_USER_OFFSET(Unit) + 0x10 + R_CSIG_BUFFER_OFFSET_USER)
#define R_CSIG_ERX0    (R_CSIG_USER_OFFSET(Unit) + 0x14 + R_CSIG_BUFFER_OFFSET_USER)

/* OS register */
#define R_CSIG_CTL_1   (R_CSIG_OS_OFFSET(Unit) + 0x10)
#define R_CSIG_CTL_2   (R_CSIG_OS_OFFSET(Unit) + 0x14)
#define R_CSIG_EMU     (R_CSIG_OS_OFFSET(Unit) + 0x18)



/*******************************************************************************
  Section: Global Types
  
  CSIG Register Definitions
*/
  
/*******************************************************************************
  Type: r_csig_RegCtl0Bit_t
  
  Content of control register CTL0
  
  Members:
  Rxe       - Reception enable 
  Txe       - Transmission enable
  Pwr       - Operation control
*/

typedef struct
{
    uint8_t Mbs     : 1;
    uint8_t Unused  : 4;
    uint8_t Rxe     : 1;  
    uint8_t Txe     : 1;  
    uint8_t Pwr     : 1;  
} r_csig_RegCtl0Bit_t;


/*******************************************************************************
  Union: r_csig_RegCtl0_t
  
  Simplifies Bit and Byte access to register CTL0    
  
  Members:
  Bit       - Allow bit access; see: <r_csig_RegCtl0Bit_t>
  Byte      - Allow access to the whole byte
*/

typedef union
{
    r_csig_RegCtl0Bit_t Bit;
    uint8_t             Byte;
} r_csig_RegCtl0_t;

  
/*******************************************************************************
  Type: r_csig_RegCtl1Bit_t
  
  Content of control register CTL1
  
  Members:
  Sse       - Slave Select function Enable
  Hse       - Hand-Shake function Mode Selection
  Sit       - Interrupt Delay Mode Selection
  Lbm       - Loop-Back Mode
  Dcs       - Data Consistency Check Enable/Disable
  Edle      - Extended Data Length Mode Enable
  Slit      - Select CSIGTIC Interrupt timing
  
  
*/

typedef struct
{
    uint8_t Sse     : 1;  
    uint8_t Hse     : 1;  
    uint8_t Sit     : 1; 
    uint8_t Lbm     : 1; 
    uint8_t Unused0 : 1; 
    uint8_t Dcs     : 1; 
    uint8_t Unused1 : 1; 
    uint8_t Edle    : 1; 
    uint8_t Unused2 : 8; 
    uint8_t Slit    : 1;  
    uint8_t Ckr     : 1;  
    uint8_t Unused3 : 6; 
    uint8_t Unused4 : 8;  
} r_csig_RegCtl1Bit_t;


/*******************************************************************************
  Union: r_csig_RegCtl1_t
  
  Simplifies Bit and Byte access to register CTL1    
  
  Members:
  Bit       - Allow bit access; see: <r_csig_RegCtl1Bit_t>
  Word      - Allow access to the whole word
*/

typedef union
{
    r_csig_RegCtl1Bit_t Bit;
    uint32_t            Word;
} r_csig_RegCtl1_t;

  
/*******************************************************************************
  Type: r_csig_RegCtl2Bit_t
  
  Content of control register CTL2
  
  Members:
  Brs       - Baud Rate Selection Bits
  Prs       - Prescaler Section Bits
  
*/

typedef struct
{
    uint16_t Brs    : 12;
    uint16_t Unused : 1;
    uint16_t Prs    : 3;
} r_csig_RegCtl2Bit_t;


/*******************************************************************************
  Union: r_csig_RegCtl2_t
  
  Simplifies Bit and Byte access to register CTL2    
  
  Members:
  Bit       - Allow bit access; see: <r_csig_RegCtl2Bit_t>
  Hwrod     - Allow access to the half word (16 bits)
*/

typedef union
{
    r_csig_RegCtl2Bit_t Bit;
    uint16_t            Hword;
} r_csig_RegCtl2_t;

  
/*******************************************************************************
  Type: r_csig_RegBCtl0Bit_t
  
  Content of control register MCTL0
  
  Members:
  To        - Time-out Setting Selection
  Mms       - Memory Mode Selection
  
*/

typedef struct
{
    uint8_t Sce    : 1;
    uint8_t Unused : 7;
} r_csig_RegBCtl0Bit_t;


/*******************************************************************************
  Union: r_csig_RegBCtl0_t
  
  Simplifies Bit and Byte access to register MCTL0    
  
  Members:
  Bit       - Allow bit access; see: <r_csig_RegBCtl0Bit_t>
  Hwrod     - Allow access to the half word (16 bits)
*/

typedef union
{
    r_csig_RegBCtl0Bit_t    Bit;
    uint8_t                 Byte;
} r_csig_RegBCtl0_t;

  
/*******************************************************************************
  Type: r_csig_RegCfgBit_t
  
  Content of control register MCFGm
  
  Members:
  Dap       - Chip Select n Idle Enforcement Configuration
  Ckp       - Chip Select n Clock And Data Phase Selection
  Dir       - Chip Select n Serial Data Direction Selection
  Dls       - Chip Select n Data Length Selection
  Ps        - Chip Select n Parity Selection
  
*/

typedef struct
{
    uint8_t Unused1 : 8;
    uint8_t Unused2 : 8;
    uint8_t Dap     : 1;
    uint8_t Ckp     : 1;
    uint8_t Dir     : 1;
    uint8_t Unused3 : 5;
    uint8_t Dls     : 4;
    uint8_t Ps      : 2;
    uint8_t Unused4 : 2;
} r_csig_RegCfgBit_t;


/*******************************************************************************
  Union: r_csig_RegCfg_t
  
  Simplifies Bit and Byte access to register MCFGm    
  
  Members:
  Bit       - Allow bit access; see: <r_csig_RegCfgBit_t>
  Word      - Allow access to the word (32 bits)
*/

typedef union
{
    r_csig_RegCfgBit_t      Bit;
    uint32_t                Word;
} r_csig_RegCfg_t;

  
/*******************************************************************************
  Type: r_csig_RegTxwBit_t
  
  Content of control register TXW
  
  Members:
  Tx        - Transmission data
  Edl       - Extended Data Length Configuration
  
*/

typedef struct
{
    uint16_t Tx     : 16;
    uint8_t Unused0 : 8;
    uint8_t Unused1 : 5;
    uint8_t Edl     : 1;
    uint8_t Unused2 : 2;
} r_csig_RegTxwBit_t;


/*******************************************************************************
  Union: r_csig_RegTxw_t
  
  Simplifies Bit and Byte access to register TXW    
  
  Members:
  Bit       - Allow bit access; see: <r_csig_RegTxwBit_t>
  Word      - Allow access to the word (32 bits)
*/

typedef union
{
    r_csig_RegTxwBit_t      Bit;
    uint32_t                Word;
} r_csig_RegTxw_t;

  
/*******************************************************************************
  Type: r_csig_RegTxh_t
  
*/

typedef uint16_t r_csig_RegTxh_t;

  
/*******************************************************************************
  Type: r_csig_RegRx_t
  
*/

typedef uint16_t r_csig_RegRx_t;

  
/*******************************************************************************
  Type: r_csig_RegERx_t
  
*/

typedef uint16_t r_csig_RegERx_t;

  
/*******************************************************************************
  Type: r_csig_RegStr0Bit_t
  
  Content of status register STR 0
  
  Members:
  Ove       - Overrun Error Flag
  Pe        - Parity Error Flag
  Dce       - Data Consistency Check Error Flag
  Tsf       - Transfer Status Flag
  Ofe       - Overflow Error Flag
  Tmoe      - Time Out Error Flag
  
*/

typedef struct
{
    uint8_t Ove      : 1;
    uint8_t Pe       : 1;
    uint8_t Unused0  : 1;
    uint8_t Dce      : 1;
    uint8_t Unused1  : 3;
    uint8_t Tsf      : 1;
    uint8_t Unused2  : 8;
    uint16_t Unused3 : 16;
} r_csig_RegStr0Bit_t;


/*******************************************************************************
  Union: r_csig_RegStr0_t
  
  Simplifies Bit and Byte access to register STR0    
  
  Members:
  Bit       - Allow bit access; see: <r_csig_RegStr0Bit_t>
  Word      - Allow access to the word (32 bits)
*/

typedef union
{
    r_csig_RegStr0Bit_t Bit;
    uint32_t            Word;
} r_csig_RegStr0_t;


/*******************************************************************************
  Type: r_csig_RegStc0Bit_t
  
  Content of status register STR 0
  
  Members:
  Ovec      - Overrun Error Flag Clear Command
  Pec       - Parity Error Flag Clear Command
  Dcec      - Data Consistency Check Error Flag Clear Command
  Pct       - Pointer Clear Command
  
*/

typedef struct
{
    uint8_t Ovec    : 1;
    uint8_t Pec     : 1;
    uint8_t Unused0 : 1;
    uint8_t Dcec    : 1;
    uint8_t Unused1 : 4;
    uint8_t Unused2 : 8;
} r_csig_RegStc0Bit_t;


/*******************************************************************************
  Union: r_csig_RegStc0_t
  
  Simplifies Bit and Byte access to register STCR 0    
  
  Members:
  Bit       - Allow bit access; see: <r_csig_RegStc0Bit_t>
  Word      - Allow access to the half word (16 bits)
*/

typedef union
{
    r_csig_RegStc0Bit_t Bit;
    uint16_t            Hword;
} r_csig_RegStc0_t;

  
/*******************************************************************************
  Type: r_csig_RegEmuBit_t
  
  Content of status register EMU
  
  Members:
  svsdis        - SVSTOP disable
  
*/

typedef struct
{
    uint8_t Unused0 : 7;
    uint8_t svsdis  : 1;
} r_csig_RegEmuBit_t;


/*******************************************************************************
  Union: r_csig_RegEmu_t
  
  Simplifies Bit and Byte access to register EMU    
  
  Members:
  Bit       - Allow bit access; see: <r_csig_RegEmuBit_t>
  Word      - Allow access to the Byte (8 bits)
*/

typedef union
{
    r_csig_RegEmuBit_t  Bit;
    uint16_t            Byte;
} r_csig_RegEmu_t;



#ifdef __cplusplus
}
#endif

#endif /* CSIG_REGS_H_  */

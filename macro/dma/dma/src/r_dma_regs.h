/*
****************************************************************************
PROJECT : Vlib DMA driver
FILE    : $Id: r_dma_regs.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Register definition for DMA driver
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

#ifndef DMA_REGS_H_
#define DMA_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: DMA Register Offsets

  Only the driver modules shall include this header.
*/

/***********************************************************
  Section: Global Defines
*/

/***********************************************************
  Constants: Register Offsets

*/

/* DMA register offsets */
#define R_DMA_DTRC0_OFFSET       0x0u
#define R_DMA_DMCM0_OFFSET       0x4u
#define R_DMA_DSA_OFFSET         0x14u
#define R_DMA_DSC_OFFSET         0x18u
#define R_DMA_DDA_OFFSET         0x24u
#define R_DMA_DDC_OFFSET         0x28u
#define R_DMA_DTC_OFFSET         0x32u
#define R_DMA_DTCT_OFFSET        0x38u
#define R_DMA_DTS_OFFSET         0x3Au
#define R_DMA_DTFR_OFFSET        0x0u
#define R_DMA_DRQCLR_OFFSET      0xFFFF8B40u

/* DMA register default values */
#define R_DMA_DTRC0_DEFAULT      0
#define R_DMA_DMCM0_DEFAULT      0
#define R_DMA_DSA_DEFAULT        0
#define R_DMA_DSC_DEFAULT        1u
#define R_DMA_DDA_DEFAULT        0
#define R_DMA_DDC_DEFAULT        1u
#define R_DMA_DTC_DEFAULT        0
#define R_DMA_DTCT_DEFAULT       0
#define R_DMA_DTS_DEFAULT        0
#define R_DMA_DTFR_DEFAULT       0
#define R_DMA_DRQCLR_DEFAULT     0

/***********************************************************
  Type: eeDma_RegDtsBit_t
  
  Content of DMA transfer status register
  
  Members:
    Dte         - DMA transfer enable
    Sr          - Software DMA transfer request
    Dr          - Hardware DMA transfer request flag
    Er          - DMA transfer error flag
    Res1        - Reserved
    Dt          - DMA transfer status
    Tc          - DMA transfer end status
*/

typedef struct 
{
    uint8_t Dte :    1;
    uint8_t Sr  :    1;
    uint8_t Dr  :    1;
    uint8_t Er  :    1;
    uint8_t Res1:    2;
    uint8_t Dt  :    1;
    uint8_t Tc  :    1;
} eeDma_RegDtsBit_t;


/***********************************************************
  Union: eeDma_RegDts_t
  
  Simplifies Bit and Byte access to register DTS
  
  Members:
  Bit       - Allow bit access; see: <eeDma_RegDtsBit_t>
  Byte      - Allow access to the 8-bit word
*/

typedef union 
{
    eeDma_RegDtsBit_t  Bit;
    uint8_t            Byte;
} eeDma_RegDts_t;


/***********************************************************
  Type: eeDma_RegDtctBit_t
  
  Content of DMA Transfer Control Register DTCTn
  
  Members:
    Res1        - Reserved
    DacM        - Destination Address Count Mode (Direction) 
    SacM        - Source Address Count Mode (Direction) 
                  see also eeDma_AddressDirection_t
    Res2        - Reserved
    le          - Multi Link Enable / DTE stays ON after transfer /
    Ds          - Transfer Data Size, 
    Res3        - Reserved
*/

typedef struct 
{
    uint16_t Res1 :    4;
    uint16_t DacM :    2;
    uint16_t SacM :    2;
    uint16_t Res2 :    4;
    uint16_t le   :    1;
    uint16_t Ds   :    2;
    uint16_t Res3 :    1;
} eeDma_RegDtctBit_t;


/***********************************************************
  Union: eeDma_RegDtct_t
  
  Simplifies Bit and Byte access to register DTCT
  
  Members:
  Bit       - Allow bit access; see: <eeDma_RegDtctBit_t>
  Word      - Allow access to the 16-bit word
*/

typedef union 
{
    eeDma_RegDtctBit_t  Bit;
    uint16_t            Word;
} eeDma_RegDtct_t;



#ifdef __cplusplus
}
#endif

#endif /* DMA_REGS_H_  */

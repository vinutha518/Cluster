/*
****************************************************************************
PROJECT : Vlib DMA driver
FILE    : $Id: r_g3dma_regs.h 2296 2014-04-08 09:07:36Z michael.golczewski $
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
#define R_DMA_DMACTL_OFFSET    0x0000u
#define R_DMA_DMACER_OFFSET    0x0020u
#define R_DMA_DM0CMV_OFFSET    0x0030u
#define R_DMA_DM1CMV_OFFSET    0x0034u
#define R_DMA_CMVC_OFFSET      0x003Cu
#define R_DMA_DM00CM_OFFSET    0x0100u
#define R_DMA_DM01CM_OFFSET    0x0104u
#define R_DMA_DM02CM_OFFSET    0x0108u
#define R_DMA_DM03CM_OFFSET    0x010Cu
#define R_DMA_DM04CM_OFFSET    0x0110u
#define R_DMA_DM05CM_OFFSET    0x0114u
#define R_DMA_DM06CM_OFFSET    0x0118u
#define R_DMA_DM07CM_OFFSET    0x011Cu
#define R_DMA_DM10CM_OFFSET    0x0120u
#define R_DMA_DM11CM_OFFSET    0x0124u
#define R_DMA_DM12CM_OFFSET    0x0128u
#define R_DMA_DM13CM_OFFSET    0x012Cu
#define R_DMA_DM14CM_OFFSET    0x0130u
#define R_DMA_DM15CM_OFFSET    0x0134u
#define R_DMA_DM16CM_OFFSET    0x0138u
#define R_DMA_DM17CM_OFFSET    0x013Cu

#define R_DMA_DSA_OFFSET       0x0400u
#define R_DMA_DDA_OFFSET       0x0404u
#define R_DMA_DTC_OFFSET       0x0408u
#define R_DMA_DTCT_OFFSET      0x040Cu
#define R_DMA_DRSA_OFFSET      0x0410u
#define R_DMA_DRDA_OFFSET      0x0414u
#define R_DMA_DRTC_OFFSET      0x0418u
#define R_DMA_DTCC_OFFSET      0x041Cu
#define R_DMA_DCEN_OFFSET      0x0420u
#define R_DMA_DCST_OFFSET      0x0424u
#define R_DMA_DCSTS_OFFSET     0x0428u
#define R_DMA_DCSTC_OFFSET     0x042Cu
#define R_DMA_DTFR_OFFSET      0x0430u
#define R_DMA_DTFRRQ_OFFSET    0x0434u
#define R_DMA_DTFRRQC_OFFSET   0x0438u

/* old stuff
 #define R_DMA_DRQCLR_OFFSET      0xFFFF8B40u */

/* DMA register default values */
#define R_DMA_DMACTL_DEFAULT    0
#define R_DMA_DMACER_DEFAULT    0
#define R_DMA_DM0CMV_DEFAULT    0
#define R_DMA_DM1CMV_DEFAULT    0
#define R_DMA_CMVC_DEFAULT      3
#define R_DMA_DM00CM_DEFAULT    0
#define R_DMA_DM01CM_DEFAULT    0
#define R_DMA_DM02CM_DEFAULT    0
#define R_DMA_DM03CM_DEFAULT    0
#define R_DMA_DM04CM_DEFAULT    0
#define R_DMA_DM05CM_DEFAULT    0
#define R_DMA_DM06CM_DEFAULT    0
#define R_DMA_DM07CM_DEFAULT    0
#define R_DMA_DM10CM_DEFAULT    0
#define R_DMA_DM11CM_DEFAULT    0
#define R_DMA_DM12CM_DEFAULT    0
#define R_DMA_DM13CM_DEFAULT    0
#define R_DMA_DM14CM_DEFAULT    0
#define R_DMA_DM15CM_DEFAULT    0
#define R_DMA_DM16CM_DEFAULT    0
#define R_DMA_DM17CM_DEFAULT    0

#define R_DMA_DSA_DEFAULT       0
#define R_DMA_DDA_DEFAULT       0
#define R_DMA_DTC_DEFAULT       0
#define R_DMA_DTCT_DEFAULT      0
#define R_DMA_DRSA_DEFAULT      0
#define R_DMA_DRDA_DEFAULT      0
#define R_DMA_DRTC_DEFAULT      0
#define R_DMA_DTCC_DEFAULT      0
#define R_DMA_DCEN_DEFAULT      0
#define R_DMA_DCST_DEFAULT      0
#define R_DMA_DCSTS_DEFAULT     0
#define R_DMA_DCSTC_DEFAULT     0xB1u
#define R_DMA_DTFR_DEFAULT      0
#define R_DMA_DTFRRQ_DEFAULT    0
#define R_DMA_DTFRRQC_DEFAULT   1u

#if 0
 old stuff
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

 no longer supported
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

no longer supported
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

no longer supported
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

no longer supported
typedef union 
{
    eeDma_RegDtctBit_t  Bit;
    uint16_t            Word;
} eeDma_RegDtct_t;  
#endif 


#ifdef __cplusplus
}
#endif

#endif /* DMA_REGS_H_  */

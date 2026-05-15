/*
****************************************************************************
PROJECT : CSIG driver
FILE    : $Id: r_csig_main.h 3935 2014-09-23 08:52:32Z golczewskim $
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


#ifndef CSIG_MAIN_H_
#define CSIG_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Main Module Internal Interface

  Only the driver modules shall include this header.
*/

/*******************************************************************************
  Section: Global Defines
*/

#undef DEF_EXTERN_
#ifndef CSIG_MAIN_GLOBALS_
#define DEF_EXTERN_ extern 
#else
#define DEF_EXTERN_
#endif



/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constants: 
  
  R_CSIG_RXBUF_SIZE    - Rx buffer size (in data)
  R_CSIG_TXBUF_SIZE    - Tx buffer size (in data)
  
*/

#define R_CSIG_RXBUF_SIZE  (R_CSIG_RXBUF_BYTE_SIZE/r_csig_Dev[Unit].DataByteSize)
#define R_CSIG_TXBUF_SIZE  (R_CSIG_TXBUF_BYTE_SIZE/r_csig_Dev[Unit].DataByteSize)


/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Type: r_csig_Dev_t

  Device instance specific data.
  All instance specific data is kept within that structure.

  Members:
  BaseAddrCsig  -   Base address of CSIH macro
  BaseAddrChbb  -   Base address of CSIH macro
  IsrCallback   -   Pointer to Isr callback functions
  DataByteSize      -   Data size in Bytes (1 or 2)
  RxBuf         -   Buffer for Rx data
  TxBuf         -   Buffer for Tx data
  RxRBuf        -   Ring Buffer structure for Rx data
  TxRemain        -   Tx register unavailable for new data
*/

typedef struct
{
    uint32_t          BaseAddrCsig;
    uint32_t          BaseAddrChbb;
    void              (*IsrCallback[R_CSIG_INT_NUM])(void);
    uint8_t           DataByteSize;
    uint8_t           DataBitSize;
    uint8_t           RxBuf[R_CSIG_RXBUF_BYTE_SIZE];
    uint8_t           TxBuf[R_CSIG_TXBUF_BYTE_SIZE];
    r_cdi_RBuf_t      RxRBuf;
    r_cdi_RBuf_t      TxRBuf;
    uint8_t           TxRemainBytes;
    uint8_t           OngoingBytes;
    uint8_t           TxOngoing;
    uint8_t           RxRemain;
    uint32_t          RxOnlyData;
    r_csig_Mode_t     Mode;
    r_csig_TxRxMode_t TxRxMode;  
    uint8_t         LastRxRead;
    uint8_t           SavedRxSize;
} r_csig_Dev_t;


/*******************************************************************************
  Section: Global Variables
*/

/*******************************************************************************
  Variable: eeCsiG_Dev

  Array for device instance specific data.
*/

DEF_EXTERN_ r_csig_Dev_t r_csig_Dev[R_CSIG_MACRO_NUM];



/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_CSIG_ErrorHandler

  Central error function. 
  
  When the driver detects any error, it will call this function.
  In case there is a registered error callback user function, 
  it  will be called. If a user handler is missing an endless loop is entered. 
     
  Parameters:
  Unit       - Instance number
  Error      - Error code, see: <r_csig_Error_t>
  
  Returns:
  void
*/

void R_CSIG_ErrorHandler(uint32_t Unit, r_csig_Error_t Error);


/*******************************************************************************
  Function: R_CSIG_ReadData

  Read received data out of the CSIG RX register.
  
  This function simply returns the RX register value, it does
  not check for availability of data.

  Parameters:
  Unit       - Instance number

  Returns:
  Received data
*/

uint16_t R_CSIG_ReadData(uint32_t Unit);


/*******************************************************************************
  Function: R_CSIG_WriteData

  Write one value to the CSIG TX register.

  The function writes a data to the TX register.

  Parameters:
  Unit         - Instance number
  TxData       - Value to be transmitted

  Returns:
  see: <r_csig_Error_t>
*/

r_csig_Error_t R_CSIG_WriteData(uint32_t Unit, uint16_t TxData, uint8_t Edl); 




#ifdef __cplusplus
}
#endif

#endif /* CSIG_MAIN_H_  */

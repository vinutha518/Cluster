/*
****************************************************************************
PROJECT : DMA driver
FILE ID : $Id: r_dma_sys.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Driver for DMA macro
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

#ifndef DMA_SYS_H_
#define DMA_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: DMA Support Functions

  The generic DMA driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/***********************************************************
  Section: Global Functions
*/


/***********************************************************
  Function: R_SYS_DMA_GetDmaBaseAddr

  Gives back base address of DMA unit.
  
  Parameters:
  Unit       - Instance number (DMA channel ID)
  
  Returns:
  Macro base address
*/
uint32_t R_SYS_DMA_GetDmaBaseAddr(uint32_t Unit, uint32_t ChId);


/***********************************************************
  Function: R_SYS_DMA_GetNoOfChannels

  Gives back No of channels available for the given macro.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  number of DMA channels for the given macro  
*/
uint32_t R_SYS_DMA_GetNoOfChannels(uint32_t Unit);

/***********************************************************
  Function: R_SYS_DMA_GetDtfrBaseAddr

  Gives back the base address of DTFR .
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/
uint32_t R_SYS_DMA_GetDtfrBaseAddr(uint32_t Unit);


/***********************************************************
  Function: R_SYS_DMA_DisableInt

  Enable the interrupt for the given unit and channel  
  
  Parameters:
  Unit       - Instance number
  ChId       - DMA Channel ID (number)
  
  Returns:
  void
*/
void R_SYS_DMA_DisableInt(uint32_t Unit, int32_t ChId);

/***********************************************************
  Function: R_SYS_DMA_EnableInt

  Disable the interrupt for the given unit and channel  

  Parameters:
  Unit       - Instance number
  ChId       - DMA Channel ID (number)
  
  
  Returns:
  void
*/
void R_SYS_DMA_EnableInt(uint32_t Unit, int32_t ChId);

#ifdef __cplusplus
}
#endif


#endif /* DMA_SYS_H_ */


/*
****************************************************************************
PROJECT : VLIB
FILE ID : $Id: r_g3dma_sys.h 2218 2014-03-27 16:29:52Z michael.golczewski $
============================================================================ 
DESCRIPTION
Generic G3DMA driver
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

#ifndef G3DMA_SYS_H_
#define G3DMA_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Generic G3DMA Support Functions

  The generic G3DMA driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_G3DMA_SYS_HardwareInit

  Setup the HW part for the generic G3DMA.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_g3dma_Error_t>
*/

r_g3dma_Error_t R_G3DMA_SYS_HardwareInit(uint32_t Unit);


/*******************************************************************************
  Function: R_G3DMA_SYS_HardwareDeInit

  Deinit of the HW part for the generic G3DMA.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_g3dma_Error_t>
*/

r_g3dma_Error_t R_G3DMA_SYS_HardwareDeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_G3DMA_SYS_BaseAddr

  Returns the base address of dedicated macro unit.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/

uint32_t R_G3DMA_SYS_BaseAddr(uint32_t Unit);


/***********************************************************
  Function: R_G3DMA_SYS_GetNoOfChannels

  Gives back No of channels available for the given macro.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  number of DMA channels for the given macro  
*/

uint32_t R_G3DMA_SYS_GetNoOfChannels(uint32_t Unit);


/***********************************************************
  Function: R_G3DMA_SYS_GetDtfrBaseAddr

  Gives back the base address of DTFR .
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Macro base address
*/

/*uint32_t R_G3DMA_SYS_GetDtfrBaseAddr(uint32_t Unit);*/


/***********************************************************
  Function: R_G3DMA_SYS_GetDmaBaseAddr

  Gives back base address of DMA unit.
  
  Parameters:
  Unit       - Instance number (DMA channel ID)
  
  Returns:
  Macro base address
*/
uint32_t R_G3DMA_SYS_GetDmaBaseAddr(uint32_t Unit, uint32_t ChId);



/*******************************************************************************
  Function: R_G3DMA_SYS_EnableInt

  Enable the interrupt for the given unit and channel  
  
  Parameters:
  Unit       - Instance number
  ChId       - DMA Channel ID (number)
  
  Returns:
  void
*/

void R_G3DMA_SYS_EnableInt(uint32_t Unit, uint32_t ChId);


/*******************************************************************************
  Function: R_G3DMA_SYS_DisableInt

  Disable the interrupt for the given unit and channel  
  
  Parameters:
  Unit       - Instance number
  ChId       - DMA Channel ID (number)
  
  Returns:
  void
*/
void R_G3DMA_SYS_DisableInt(uint32_t Unit, uint32_t ChId);


#ifdef __cplusplus
}
#endif


#endif /* G3DMA_SYS_H_ */

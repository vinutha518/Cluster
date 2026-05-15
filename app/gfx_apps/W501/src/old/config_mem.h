/***********************************************************************************************
PROJECT : dave hd static drawinglist app
FILE   : $Id: config_mem.h 11006 2016-11-07 16:32:44Z michael.golczewski $
============================================================================ 
DESCRIPTION
Main functions of the dave hd static drawinglist application
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
#ifndef CONFIG_MEM_H
#define CONFIG_MEM_H


#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Define: CPU_HP_BLOCK_NB

  Number of blocks the CPU heap is divided into
  Heap Memory is allocated blockwise.
*/
#define CPU_HP_BLOCK_NB     0x800


/*******************************************************************************
  Define: CPU_HP_BLOCK_SIZE

  Size of one CPU heap block 
*/
#define CPU_HP_BLOCK_SIZE   0x40


/*******************************************************************************
  Define: HEAP_SIZE

  Drawing engine driver CPU heap memory.

  Byte size of the drawing engine driver local heap. It
  is used for internal structures and for preparation of
  the drawing engine command lists. Its minimum size depends 
  on  the size of local command lists, number of drawing
  contexts and number of video memory blocks.
*/
#define HEAP_SIZE           (CPU_HP_BLOCK_NB * CPU_HP_BLOCK_SIZE)



#ifdef __cplusplus
}
#endif

#endif /* CONFIG_MEM_H */

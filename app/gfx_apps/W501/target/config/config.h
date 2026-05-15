/***********************************************************************************************
PROJECT : Template Test APP
FILE   : $Id: config.h 4763 2015-02-09 09:02:08Z golczewskim $
============================================================================ 
DESCRIPTION
device config of the dave hd test application
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
#ifndef CONFIG_H_
#define CONFIG_H_


#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************
  Section: Includes
*/
#include "r_dev_api.h"


/*******************************************************************************
  Section: Type Definitions
*/

/*******************************************************************************
*/

/*******************************************************************************
  Section: Global Defines
*/

/* if you change this port then update the pin reference locations in the
   loc_TestPinSetup()
*/
#define TEST_PORT (44)
#define TEST_PIN0 (0)
#define TEST_PIN1 (1)
#define TEST_PIN2 (2)
#define TEST_PIN3 (3)
#define TEST_PIN4 (4)
#define TEST_PIN5 (5)
#define TEST_PIN6 (6)
#define TEST_PIN7 (7)

/* registers for the ISMnEMU (SVSTOP) */
#define ISM_EMU_REG                 (0xFFF00000u + 0x0260u)
#define ISM_SVDIS_NOT_ALLOW_STOP    (0x00000080u)
#define ISM_SVDIS_ALLOW_STOP        (0x00000000u)
/*******************************************************************************
  Define: ERRN_NG
  
  Error Constant set to -1
  Is used as return value when an error occurs.
*/
#define ERR_NG      (-1)


/*******************************************************************************
  Macro: RTCA_UNIT

  Define the unit to use.
*/
#define RTCA_UNIT 0


/*******************************************************************************
  Macro: Clock

  Reference values for the RTC.
*/

#define EE_PCLK_CKS_MOSC_8     (8000000uL)
#define EE_CLK_SOSC_FRQ        (32768uL)

/***********************************************************
  Define: PCM_UNIT

  Define the instance number of the PCM used for this test
*/
#define PCM_UNIT 0
#define PCMP0AUSA (0xFFF10000 + 0x18)


/*******************************************************************************
  Macro: TAUB_UNIT

  Define the unit to use.
*/
#define TAUB_UNIT 0


/*******************************************************************************
  Macro: TAUB_UNIT

  Define the unit to use.
*/
#define TAUJ_UNIT 0



/*******************************************************************************
  Macro: Standby control
  
  Cotrol for the RTC in deep stop mode
*/

#define CKSC_ARTCAD_STPM    0xFFF82418
#define RTCAD_STP_MSK       0x00000003



/*******************************************************************************
  Define: USE_HMI_TERMINAL

  Option to use the HMI terminal for user input  
*/

//#define USE_HMI_TERMINAL


/*******************************************************************************
  Define: USE_RS232_TERMINAL

  Option to use the RS232 terminal for user input  
*/

//#define USE_RS232_TERMINAL


/*******************************************************************************
  Define: NO_HMI_TIMEOUT

  Option to stop HMI timeout occuring  
*/

#define NO_HMI_TIMEOUT

#define LOC_DISPLAY_NAME	"TRULY_240x320"
//#define LOC_DISPLAY_NAME	"TRULY_QVGA_TOUCH_240x320"
//#define LOC_DISPLAY_NAME	"HITACHI_QVGA_TFT_240x320"


#define LOC_DISPLAY_WIDTH  240
#define LOC_DISPLAY_STRIDE 256
#define LOC_DISPLAY_HEIGHT 320
#define LOC_DISPLAY_BPP    4

/*******************************************************************************
  Define: LOC_RING_BUFFER_MODE

  0 : VOWE frame buffer mode.
  1 : VOWE ring buffer mode.
*/
#define LOC_RING_BUFFER_MODE  (1)

/*******************************************************************************
  Define: LOC_DL_CHANGE

  0 : not change display list in executing.
  1 : change display list in executing.
*/
#define LOC_DL_CHANGE         (1)



  extern uint32_t loc_VRAM0;
  extern uint32_t loc_VRAM0_SIZE;

/*******************************************************************************
  Define: HEAP_SIZE

  Drawing engine driver CPU heap memory.

  Byte size of the drawing engine driver local heap. It
  is used for internal structures and for preparation of
  the drawing engine command lists. Its minimum size depends 
  on  the size of local command lists, number of drawing
  contexts and number of video memory blocks.
*/
#define LOC_HEAP_SIZE           (LOC_CPU_HP_BLOCK_NB * LOC_CPU_HP_BLOCK_SIZE)

/*******************************************************************************
  Define: CPU_HP_BLOCK_NB

  Number of blocks the CPU heap is divided into
  Heap Memory is allocated blockwise.
*/
#define LOC_CPU_HP_BLOCK_NB     (0x800)

/*******************************************************************************
  Define: CPU_HP_BLOCK_SIZE

  Size of one CPU heap block 
*/
#define LOC_CPU_HP_BLOCK_SIZE   (0x40)


/*******************************************************************************
  Define: ERRN_NG
  
  Error Constant set to -1
  Is used as return value when an error occurs.
*/
#define ERR_NG -1

/* VOWE work buffer is allocated before VOWE DL buffer */
#define LOC_VOWE_FRAME_BUF_SIZE  (LOC_DISPLAY_STRIDE * LOC_DISPLAY_HEIGHT * 2 * 4)
#define LOC_VOWE_RING_BUF_SIZE   (0x00020000)
#define LOC_VOWE_RING_BUFF_DELAY (50)

/* VOWE DL buffer(x2) is allocated in bottom of VRAM */
#define LOC_VOWE_DL_BUF_SIZE     (0x00010000)

#define LOC_WM_UNIT     0
#define LOC_DHD_UNIT    0
#define LOC_DRW2D_UNIT  0
#define LOC_VOWE_UNIT   0

#define LOC_VOWE_ENABLE 1


#ifdef __cplusplus
}
#endif

#endif /* CONFIG_H_ */

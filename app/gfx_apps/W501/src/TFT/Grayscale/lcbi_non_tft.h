/*
****************************************************************************
Project : d1l1_display_connectivity
FILE    : $Id: hmi.c 5069 2015-05-04 15:40:54Z golczewskim $
============================================================================
DESCRIPTION

D1L1 display connectivity demo
============================================================================
                            C O P Y R I G H T
============================================================================
                           Copyright (c) 2016
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
Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.

****************************************************************************
*/


/*#ifndef __LCBI_NON_TFT_H
#define __LCBI_NON_TFT_H
*/
/***********************************************************
  Section: Functions
*/


/***********************************************************
  Function: Lcbi_Non_Tft_Init

  Initialisation for the LCBI

  Parameters:
  none

  Returns:
  none
*/
void Lcbi_Non_Tft_Init(void);


/***********************************************************
  Function: Lcbi_Non_Tft_DeInit

  DeInitialisation for the LCBI

  Parameters:
  none

  Returns:
  none
*/
void Lcbi_Non_Tft_DeInit(void);


/***********************************************************
  Function: Lcbi_Non_Tft_Processing

  Round robin processing for the LCBI for general housekeeping

  Parameters:
  none

  Returns:
  none
*/
void Lcbi_Non_Tft_Processing(void);

void clear_screen(void);
/***********************************************************
  Function: Lcbi_Non_Tft_Image

  Selects the image to be displayeed on the LCD

  Parameters:
    uint8_t Img, Image ID to be displayed  

  Returns:
  none
*/
void Lcbi_Non_Tft_Image( uint8_t Img);


void loc_WriteImage(uint16_t *ptrImage);
void loc_WriteImage_fast(uint32_t y,uint32_t x,uint16_t *ptrImage);
enum
{
	COMPLETE_SCREEN=1,
	DYNAMIC_AREA,
	TRIP_A_B,
	TRIP_AC,
	CLOCK_SPEEDO,
	AFE_DISP,
};
//#endif /* __LCBI_NON_TFT_H */

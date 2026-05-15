/*
****************************************************************************
PROJECT : lcd display driver for dogm displays (DX4 applboard)
FILE    : $Id: r_lcddogm_cmd.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
LCD Display Commands
============================================================================
                            C O P Y R I G H T                            
============================================================================
                           Copyright (c) 2013
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

#ifndef  LCDDOGM_CMD_H_
#define  LCDDOGM_CMD_H_

/********* LCD-Instruction set *******/

#define  LCD_SET_CLEAR    0x01 /* Clear Display */

#define  LCD_SET_RETURN   0x02 /* Return home */

#define  LCD_SET_ENTRY    0x04 /* Entry mode set */
 #define LCD_ID           0x02 /* Increment/Decrement */
 #define LCD_S            0x01 /* Shift on/off */

#define  LCD_SET_DISPLAY  0x08 /* Display ON/OFF control */
 #define LCD_D            0x04 /* Display on/off */
 #define LCD_C            0x02 /* Cursor on/off */
 #define LCD_B            0x01 /* Blink on/off */

#define  LCD_SET_SHIFT    0x10 /* Cursor or display shift */
 #define LCD_SC           0x08 /* Display shift/cursor move */
 #define LCD_RL           0x04 /* Shift right/left */

#define  LCD_SET_FUNCTION 0x20 /* Function set */
 #define LCD_DL           0x10 /* Set 8-Bit/4-Bit interface */
 #define LCD_N            0x08 /* 2 line display */
 #define LCD_F            0x04 /* 5x8 / 5x11 dots */

#define LCD_SET_BIAS      0x14  /* Bias set */
 #define LCD_BS1_4        0x08  /* set 1/4 bias */
 #define LCD_BS1_5        0x00  /* set 1/5 bias */

#define LCD_SET_POWER     0x50  /* set Power/ICON Control/Contrast */
 #define LCD_ICON         0x08  /* ICON display on */
 #define LCD_BON          0x04  /* Booster on */
 #define LCD_CONTRHI_0    0x00  /* Contrast set for internal follower (upper two bits) */
 #define LCD_CONTRHI_1    0x01  /* Contrast set for internal follower (upper two bits) */
 #define LCD_CONTRHI_2    0x02  /* Contrast set for internal follower (upper two bits) */
 #define LCD_CONTRHI_3    0x03  /* Contrast set for internal follower (upper two bits) */

#define LCD_SET_FOLLOW    0x60 /* set Follower Control */
 #define LCD_FOLLOW_ON    0x08 /* Follower Control on */
 #define LCD_FOLLOW_RAB0  0x00 /* Follower amplified ratio 0..7*/
 #define LCD_FOLLOW_RAB1  0x01
 #define LCD_FOLLOW_RAB2  0x02
 #define LCD_FOLLOW_RAB3  0x03
 #define LCD_FOLLOW_RAB4  0x04
 #define LCD_FOLLOW_RAB5  0x05
 #define LCD_FOLLOW_RAB6  0x06
 #define LCD_FOLLOW_RAB7  0x07
 
#define LCD_SET_CONTR     0x70  /* set Contrast (lower four bits) */
 #define LCD_CONTRLO_0    0x00  /* Contrast set for internal follower (lower four bits) */
 #define LCD_CONTRLO_1    0x01  /* Contrast set for internal follower (lower four bits) */
 #define LCD_CONTRLO_2    0x02  /* Contrast set for internal follower (lower four bits) */
 #define LCD_CONTRLO_3    0x03  /* Contrast set for internal follower (lower four bits) */
 #define LCD_CONTRLO_4    0x04  /* Contrast set for internal follower (lower four bits) */
 #define LCD_CONTRLO_5    0x05  /* Contrast set for internal follower (lower four bits) */
 #define LCD_CONTRLO_6    0x06  /* Contrast set for internal follower (lower four bits) */
 #define LCD_CONTRLO_7    0x07  /* Contrast set for internal follower (lower four bits) */
 #define LCD_CONTRLO_8    0x08  /* Contrast set for internal follower (lower four bits) */
 #define LCD_CONTRLO_9    0x09  /* Contrast set for internal follower (lower four bits) */
 #define LCD_CONTRLO_10   0x0A  /* Contrast set for internal follower (lower four bits) */
 #define LCD_CONTRLO_11   0x0B  /* Contrast set for internal follower (lower four bits) */
 #define LCD_CONTRLO_12   0x0C  /* Contrast set for internal follower (lower four bits) */
 #define LCD_CONTRLO_13   0x0D  /* Contrast set for internal follower (lower four bits) */
 #define LCD_CONTRLO_14   0x0E  /* Contrast set for internal follower (lower four bits) */
 #define LCD_CONTRLO_15   0x0F  /* Contrast set for internal follower (lower four bits) */

 
#define  LCD_SET_CG       0x40 /* Set character RAM address */

#define  LCD_SET_DD       0x80 /* Set display RAM address */

#define  LCD_IS0          0x00  /* instruction set 0 */
#define  LCD_IS1          0x01  /* instruction set 1 */
#define  LCD_IS2          0x10  /* instruction set 2 */

/********* LCD-Cursor position *******/

#define  LCD_BAD_X        0xF0 /* Cursor x-position out of sight */
#define  LCD_BAD_Y        0x0F /* Cursor Y-position out of sight */

#define  LCD_DIR_GET      0xFF /* Data lines in input mode */
#define  LCD_DIR_PUT      0x00 /* Data lines in output mode */

#define  LCD_CTRL_BIT	  0x80


#endif /* _LCDDOGM_CMD_H_ */


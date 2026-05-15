/*
****************************************************************************
PROJECT : lcd display driver for dogm displays (DX4 applboard)
FILE    : $Id: r_lcddogm_main.c 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Main Functions for LCD Dogm Display
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

#include "r_typedefs.h"
#include "r_lcddogm_api.h"
#include "r_lcddogm_sys.h"
#include "r_lcddogm_cmd.h"
#include "r_tick_api.h"

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

#define R_LCDDOGM_WAIT_CYCLES 5000

r_lcdDogm_LcdState_t lcdInfo;

/*******************************************************************************
  Variable: loc_ErrorHandler

  A function pointer to a user callback (Applications error handler)  
  
  The value is assigned by the application using the driver 
*/

static void (*loc_ErrorHandler )(uint32_t Unit, r_lcdDogm_Error_t Error) = 0;

/*******************************************************************************
  Function: R_LCDDOGM_ErrorHandler
  
  Central error function. 
  
  When the driver detects any error, it will call this function.
  In case there is a registered error callback user function, 
  it  will be called. If a user handler is missing an endless loop is entered. 
     
  Parameters:
  Unit       - Instance number
  Error      - Error code, see: <r_lcdDogm_Error_t>
  
  Returns:
  void
*/

void R_LCDDOGM_ErrorHandler(uint32_t Unit, r_lcdDogm_Error_t Error) 
{
    if (loc_ErrorHandler != 0) 
    {      /* we've got a registered error handler here ...*/
        loc_ErrorHandler(Unit, Error);      /* ... and we gonna use it!*/
    } 
    else 
    {
        for (;;) 
        {                          /* we don't have a handler */
        }                          /* so, let's wait here */
    }
}

void R_LCDDOGM_Init(r_lcdDogm_Data_t lcdData, r_lcdDogm_PortConfig_t pconf) {
  
  uint8_t rows_dots = 0;
  
  R_LCDDOGM_Sys_PortInit(pconf);
      
  lcdInfo.cols = lcdData.cols;   /* Store number of columns (characters per line) */
  lcdInfo.rows = lcdData.rows;   /* Store number of rows (character lines) */
  lcdInfo.dots = lcdData.dots;   /* Store number of vertical dots per character */
  lcdInfo.mode = 0;      /* Display,Cursor,Blink and Backlight switched off */
  lcdInfo.xpos = 1;      /* Initial x-position of cursor will be 1. column */
  lcdInfo.ypos = 1;      /* Initial y-position of cursor will be 1. line */
  
  lcdInfo.miss = 0;      /* Valid write position */
  lcdInfo.hide = 0;      /* Do not force cursor/blink to be invisible */

  if (lcdInfo.rows > 1) {    /* Set "N" for display with more than 1 line     */
      rows_dots |= LCD_N;
  }
  if (lcdInfo.dots > 8) {    /* Set "F" for displays with more than 8 pixel   */
      rows_dots |= LCD_F;
  }

                                /* Function Set, 8 bit Databus, number of rows */
  R_LCDDOGM_WriteReg(LCD_SET_FUNCTION | LCD_DL | rows_dots | LCD_IS0);

                                /* Function Set, 8 bit Databus, number of rows */    
  R_LCDDOGM_WriteReg(LCD_SET_FUNCTION | LCD_DL | rows_dots | LCD_IS0);

                                /* Function Set, 8 bit Databus, number of rows */
                                /* switch to Instruction Table 1 */
  R_LCDDOGM_WriteReg(LCD_SET_FUNCTION | LCD_DL | rows_dots | LCD_IS1);
  R_LCDDOGM_WriteReg(LCD_SET_BIAS | LCD_BS1_5);            /* Set Bias 1/5 */    
                                /* Power control set, booster on, */
                                /* set contrast (higher bits) */
  R_LCDDOGM_WriteReg(LCD_SET_POWER | LCD_BON | LCD_CONTRHI_1);
                                /* set follower control, voltage fol. on, */
                                /* set follower gain (amp.) to '6' */
  R_LCDDOGM_WriteReg(LCD_SET_FOLLOW | LCD_FOLLOW_ON | LCD_FOLLOW_RAB6);
                                /* set contrast (lower bits) */
  R_LCDDOGM_WriteReg(LCD_SET_CONTR | LCD_CONTRLO_2);
                                /* Function Set, switch to Instruction Table 0 */
  R_LCDDOGM_WriteReg(LCD_SET_FUNCTION | LCD_DL | rows_dots | LCD_IS0);
  
}

uint8_t R_LCDDOGM_ReadRegister(void) {
    
    uint8_t regval;
    
    R_LCDDOGM_Sys_SetDirection(R_LCDDOGM_DIR_INPUT);        /* Set direction to input   */
    R_LCDDOGM_Sys_SetControl(R_LCDDOGM_READ_CTRL);          /* Set enable + R/W         */
    regval = R_LCDDOGM_Sys_GetByte();                        /* Read data from module    */
    R_LCDDOGM_Sys_SetControl(R_LCDDOGM_FALL_EDGE);          /* Create falling edge on E */
    return (regval);                                         /* Return value             */
}

r_lcdDogm_Error_t R_LCDDOGM_Wait(uint32_t cycles) {
  
  /* The time required for the display's operations is defined in the datasheet
     alternative to read the wait flag, the driver can wait for the time defined in the DS. 
     This value is dependant to the executed command!
     
     The longest command takes 1.08ms on the slowest oscillator setting */
  
  uint32_t timeout;
  timeout = cycles; 
  
  while (R_LCDDOGM_ReadRegister() & LCD_CTRL_BIT) { // Check busy flag
    timeout--;                                      // One attempt less to go
    if (timeout == 0)
    {
      return R_LCDDOGM_ERR_WAIT;
    }
  }

  /*    R_TICK_WaitUS(0, 1500);   */
  
  return R_LCDDOGM_OK;
}

void R_LCDDOGM_WriteReg(uint8_t value) {
    
    R_LCDDOGM_Wait(R_LCDDOGM_WAIT_CYCLES);
    R_LCDDOGM_Sys_SetDirection(R_LCDDOGM_DIR_OUTPUT);       /* Set direction to output   */
    R_LCDDOGM_Sys_SetControl(R_LCDDOGM_WRITE_CTRL);           /* Set enable (E) line  */
    R_LCDDOGM_Sys_PutByte(value);                            /* Write data to module */
   /*     R_TICK_WaitUS(0, 2);  */
    R_LCDDOGM_Sys_SetControl(R_LCDDOGM_FALL_EDGE);              /* Create falling edge  */
   /*     R_TICK_WaitUS(0, 2);  */
}

void R_LCDDOGM_WriteData(uint8_t value) {
    
    R_LCDDOGM_Wait(R_LCDDOGM_WAIT_CYCLES);
    R_LCDDOGM_Sys_SetDirection(R_LCDDOGM_DIR_OUTPUT);       /* Set direction to output   */
    R_LCDDOGM_Sys_SetControl(R_LCDDOGM_WRITE_DATA);           /* Set enable (E) line  */
    R_LCDDOGM_Sys_PutByte(value);                            /* Write data to module */
    /*    R_TICK_WaitUS(0, 2);  */
    R_LCDDOGM_Sys_SetControl(R_LCDDOGM_FALL_EDGE);              /* Create falling edge  */
    /*    R_TICK_WaitUS(0, 2);  */
}

															
void R_LCDDOGM_CheckXPos(void) {
  /* x_pos within correct range ? */
  if ((lcdInfo.xpos > 0) && (lcdInfo.xpos <= lcdInfo.cols))
  {
  /* Clear LCD_BAD_X flag, because x-position inside range */
    lcdInfo.miss &= ~LCD_BAD_X;
  }
  else 
  {
  /* Set LCD_BAD_X flag, because x-position is outside of range */
    lcdInfo.miss |= LCD_BAD_X;
  }
}	
					   
void R_LCDDOGM_CheckYPos(void) {
  /* y_pos within correct range ? */
  if ((lcdInfo.ypos > 0) && (lcdInfo.ypos <= lcdInfo.rows))   
  /* Clear LCD_BAD_Y flag, because y-position inside range */
  {
    lcdInfo.miss &= ~LCD_BAD_Y;   
  }
  else 
  {
  /* Set LCD_BAD_Y flag, because y-position is outside of range */
    lcdInfo.miss |= LCD_BAD_Y;
  }
}		  

void R_LCDDOGM_SetMode(void) {
  if (lcdInfo.miss || lcdInfo.hide) /* Cursor out of range or invisible ? */
  {
    R_LCDDOGM_WriteReg(LCD_SET_DISPLAY | (lcdInfo.mode & ~(LCD_C | LCD_B))); 
  }
  else
  {
    R_LCDDOGM_WriteReg(LCD_SET_DISPLAY | lcdInfo.mode); /* Set mode */
  }
}  
											




/*----------------------------------**
**  higher layer prototypes         **
**----------------------------------*/
/*-------------------------**
** R_LCDDOGM_ActivateDisplay  **
** 						   **
**-------------------------*/					   
void R_LCDDOGM_ActivateDisplay(void)
{
  lcdInfo.mode |= LCD_D;    /* Set Display-flag in display mode byte */
  R_LCDDOGM_SetMode();          /* Set new mode */
}



/*----------------------**
** R_LCDDOGM_ActivateCursor**
**                      **
**----------------------*/
void R_LCDDOGM_ActivateCursor(void)
{
  lcdInfo.mode |= LCD_C;    /* Set Cursor-flag in display mode byte */
  R_LCDDOGM_SetMode();          /* Set new mode */
}



/*----------------------**
** R_LCDDOGM_ActivateBlink **
** 					    **
**----------------------*/					
void R_LCDDOGM_ActivateBlink(void)
{
  lcdInfo.mode |= LCD_B;    /* Set Blink-flag in display mode byte */
  R_LCDDOGM_SetMode();          /* Set new mode */
}



/*--------------------------**
** R_LCDDOGM_DeactivateDisplay **
** 						    **
**--------------------------*/						
void R_LCDDOGM_DeactivateDisplay(void)
{
  lcdInfo.mode &= ~LCD_D;   /* Clear Display-flag in display mode byte */
  R_LCDDOGM_SetMode();          /* Set new mode */
}



/*-------------------------**
** R_LCDDOGM_DeactivateCursor **
** 						   **
**-------------------------*/					   
void R_LCDDOGM_DeactivateCursor(void)
{
  lcdInfo.mode &= ~LCD_C;   /* Clear Cursor-flag in display mode byte */
  R_LCDDOGM_SetMode();          /* Set new mode */
}



/*------------------------**
** R_LCDDOGM_DeactivateBlink **
** 						  **
**------------------------*/					  
void R_LCDDOGM_DeactivateBlink(void)
{
  lcdInfo.mode &= ~LCD_B;   /* Clear Blink-flag in display mode byte */
  R_LCDDOGM_SetMode();          /* Set new mode */
}



/*---------------------**
** R_LCDDOGM_ClearDisplay **
** 					   **
**---------------------*/				   
void R_LCDDOGM_ClearDisplay(void)
{
  R_LCDDOGM_WriteReg(LCD_SET_CLEAR);         /* Clear display */
  R_LCDDOGM_GotoXY(lcdInfo.xpos, lcdInfo.ypos); /* Restore cursor position */
}



/*------------------**
** R_LCDDOGM_ClearLine **
** 				    **
**------------------*/				
void R_LCDDOGM_ClearLine(uint8_t line)
{
  uint8_t x_old, y_old;
  x_old = lcdInfo.xpos;     /* Remember current cursor x position  */
  y_old = lcdInfo.ypos;     /* Remember current cursor y position  */
  lcdInfo.hide = 1;         /* Hide cursor although being in correct borders */
  R_LCDDOGM_GotoXY(1, line);       /* Set cursor to first row of selected line */
  R_LCDDOGM_ClearEOL();            /* Clear to end of line */
  lcdInfo.hide = 0;         /* Enable cursor & blink to be displayed */
  R_LCDDOGM_GotoXY(x_old, y_old);  /* Restore former cursor position */
}



/*-----------------**
** R_LCDDOGM_ClearEOL **
** 				   **
**-----------------*/			   
void R_LCDDOGM_ClearEOL(void)
{
  uint8_t cnt, x_old;
  x_old = lcdInfo.xpos;        /* Remember current cursor x position  */
  for(cnt=lcdInfo.xpos;            /* From current curor position ... */
      cnt <= lcdInfo.cols; cnt++)  /* ... to end of line */
  {
    R_LCDDOGM_WriteChar(' ');         /* Write blanks */
  };
  R_LCDDOGM_GotoX(x_old);             /* Restore former cursor position */
}



/*--------------------------------------**
** R_LCDDOGM_GotoXY                        **
**                                      **
** x_pos : Next column for text output  **
** y_pos : Next line for text output    **
** 									    **
**--------------------------------------*/									
void R_LCDDOGM_GotoXY(uint8_t x_pos, uint8_t y_pos)
{
  uint8_t dd_address;
  
  if ( (x_pos > lcdInfo.cols) || (y_pos > lcdInfo.rows) )
  {
      /* GotoXY out of range! */
      R_LCDDOGM_ErrorHandler(0, R_LCDDOGM_ERR_RANGE); 
  }

  lcdInfo.xpos = x_pos;      /* Store new column as x-position of cursor */
  lcdInfo.ypos = y_pos;      /* Store new line as y-position of cursor */

  R_LCDDOGM_CheckXPos();            /* Is given x-position inside display ? */
  R_LCDDOGM_CheckYPos();            /* Is given y-position inside display ? */
  R_LCDDOGM_SetMode();              /* Set new mode */

  switch (y_pos)              /* Distinguish line for address calculation */
  {
    case 1:                   /* DD-RAM adress 0x00 refers to xpos 1, ypos 1 */
      dd_address = (x_pos - 1);  
    break;
    case 2:                   /* Second line begins at 0x40 */
      dd_address = (x_pos - 1 + 0x40);
    break;
    case 3:                   /* Third line begins directly after first line */
      dd_address = (x_pos - 1 + lcdInfo.cols);
    break;
    case 4:                   /* Fourth line begins directly after second */
      dd_address = (x_pos - 1 + 0x40 + lcdInfo.cols);
    break;
    default: {}
  }
  R_LCDDOGM_WriteReg(LCD_SET_DD | (dd_address & 0x7F)); /* Set DD RAM address */
}



/*--------------**
** R_LCDDOGM_GotoX **
** 			    **
**--------------*/			
void R_LCDDOGM_GotoX(uint8_t x_pos)
{
  R_LCDDOGM_GotoXY(x_pos, lcdInfo.ypos);  /* Set write position to given column */
}



/*--------------**
** R_LCDDOGM_GotoY **
** 			    **
**--------------*/			
void R_LCDDOGM_GotoY(uint8_t y_pos)
{
  R_LCDDOGM_GotoXY(lcdInfo.xpos, y_pos);  /* Set write position to given line */
}



/*-------------------**
** R_LCDDOGM_GetCurPosX  **
** 					 **
**-------------------*/				 
uint8_t R_LCDDOGM_GetCurPosX(void)
{
  return(lcdInfo.xpos);        /* Return current column */
}



/*-------------------**
** R_LCDDOGM_GetCurPosY  **
** 					 **
**-------------------*/				 
uint8_t R_LCDDOGM_GetCurPosY(void)
{
  return(lcdInfo.ypos);        /* Return current line */
}



/*-------------------**
** R_LCDDOGM_GetMaxPosX  **
** 				 	 **
**-------------------*/				 
uint8_t R_LCDDOGM_GetMaxPosX(void)
{
 return(lcdInfo.cols);         /* Return maximum column number */
}



/*-------------------**
** R_LCDDOGM_GetMaxPosY  **
** 					 **
**-------------------*/				 
uint8_t R_LCDDOGM_GetMaxPosY(void)
{
 return(lcdInfo.rows);         /* Return maximum line number */
}



/*-----------------------**
** R_LCDDOGM_WriteString    **
**                       **
** out_string  : string  **
** 						 **
**-----------------------*/					 
void R_LCDDOGM_WriteString(char * out_string)
{
  char tmp=0;
  
  while (*out_string != 0)         /* Until end of string (null terminated) */
  {
    if (*out_string == '\n') {
        *out_string = ' '; 
    }   
    R_LCDDOGM_WriteChar(*out_string);  /* Write character to current position */
    out_string++;
    tmp++;
    if (tmp==16)
    {
       return; /* return if string is larger than 16 characters */
    }
  }
  /* in case the string is shorter, fill the rest with blanks */
  while (tmp < 16) { 
    R_LCDDOGM_WriteChar(' ');  /* Write character to current position */
    tmp++; 
  }  
}



/*---------------------------------------------------**
** R_LCDDOGM_WriteChar                                  **
**                                                   **
** character :           character to be displayed   **
** 													 **
**---------------------------------------------------*/												 
void R_LCDDOGM_WriteChar(uint8_t character)
{
  if (!lcdInfo.miss)        /* Disable output outside borders */
  {
    R_LCDDOGM_WriteData(character);/* Write character to current write position */
    lcdInfo.xpos++;         /* New x-position */  
    R_LCDDOGM_CheckXPos();         /* Should coursor/blink now be invisible ? */
  }
  else                       /* Write position was invalid before */
  {
    lcdInfo.xpos++;         /* New x-position */  
    R_LCDDOGM_CheckXPos();         /* Is write position still invalid? */
    if (!lcdInfo.miss)      /* If it is now ok */
    {
      R_LCDDOGM_GotoXY(lcdInfo.xpos, lcdInfo.ypos); /* Go to valid position */
    }
  }
  R_LCDDOGM_SetMode();             /* Set new mode */
}



/*--------------------------------------**
** R_LCDDOGM_SetUserChar                   **
**                                      **
** charnum    : User defined character  **
** user_char  : Pointer to a bitmap     **
** 									    **
**--------------------------------------*/									
void R_LCDDOGM_SetUserChar(uint8_t charnum, unsigned char * user_char)
{
  uint8_t cnt, x_old, y_old;  

  x_old = lcdInfo.xpos;             /* Remember current cursor x position  */
  y_old = lcdInfo.ypos;             /* Remember current cursor y position  */

  for (cnt=0; cnt<lcdInfo.dots; cnt++) /* For all dot lines within character */
  {
    R_LCDDOGM_WriteReg(LCD_SET_CG | cnt | (charnum<<3)); /* Set CG RAM address */
    R_LCDDOGM_WriteData(*(user_char+cnt)); /* Write bitmap data into CG RAM */
  };
  R_LCDDOGM_GotoXY(x_old, y_old);          /* Restore former cursor position */
}



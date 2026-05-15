/*
****************************************************************************
PROJECT : lcd display driver for dogm displays (DX4 applboard)
FILE    : $Id: r_lcddogm_api.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
API Header for the LCD Dogm Display
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

#ifndef  LCDDOGM_API_H_
#define  LCDDOGM_API_H_

/***********************************************************

  Title: LCD API

  Driver for the embedded LCD display
*/

#define R_LCDDOGM_DWIDTH    8

/***********************************************************
  Section: Global Types
*/
  
/***********************************************************
  Enum: r_lcdDogm_Direction_t
  
  List of pin direction 
  
  Values:
  R_LCDDOGM_DIR_INPUT  - input
  R_LCDDOGM_DIR_OUTPUT - output
  
*/
typedef enum {
  R_LCDDOGM_DIR_INPUT = 0x00,
  R_LCDDOGM_DIR_OUTPUT

} r_lcdDogm_Direction_t;



/***********************************************************
  Enum: r_lcdDogm_Control_t
  
  List of control mode 
  
  Values:
  R_LCDDOGM_READ_DATA  -  Read Data
  R_LCDDOGM_READ_CTRL  -  Read control register
  R_LCDDOGM_WRITE_DATA -  Write data
  R_LCDDOGM_WRITE_CTRL -  Write control register
  R_LCDDOGM_FALL_EDGE  -  Fall Edge
  R_LCDDOGM_END        -  Delimiter 
  
*/
typedef enum {
  R_LCDDOGM_READ_DATA = 0x00,
  R_LCDDOGM_READ_CTRL,
  R_LCDDOGM_WRITE_DATA,
  R_LCDDOGM_WRITE_CTRL,
  R_LCDDOGM_FALL_EDGE,
  R_LCDDOGM_END
  
} r_lcdDogm_Control_t;


/***********************************************************
  Enum: r_lcdDogm_Error_t
  
  List of error 
  
  Values:
  R_LCDDOGM_OK             - No error
  R_LCDDOGM_ERR_PARA       - Parameter error
  R_LCDDOGM_ERR_WAIT       - Error while waiting
  R_LCDDOGM_ERR_RANGE      - Parameter out of range
  R_LCDDOGM_ERR_UNKNOWN    - Unknown error
  
*/
typedef enum {
  R_LCDDOGM_OK = 0x00u,
  R_LCDDOGM_ERR_PARA,
  R_LCDDOGM_ERR_WAIT,
  R_LCDDOGM_ERR_RANGE, 
  R_LCDDOGM_ERR_UNKNOWN
  
} r_lcdDogm_Error_t;


/***********************************************************
  Type: r_lcdDogm_Data_t
  
  Structure describing the LCD.
  
  Members:
  cols  - number of columns
  rows  - number of rows
  dots  - number of vertical dots per character
  
*/
typedef struct {
   uint8_t cols;
   uint8_t rows;
   uint8_t dots;

} r_lcdDogm_Data_t;

/***********************************************************
  Type: r_lcdDogm_LcdState_t
  
  Structure describing the current state of the LCD.
  
  Members:
  cols  - number of columns
  rows  - number of rows
  dots  - number of dots
  mode  - current mode
  xpos  - position on x-axis
  ypos  - position on y-axis
  csel  - not used
  miss  - valid position
  hide  - hide cursor
  
*/
typedef struct {
  uint8_t cols;
  uint8_t rows;
  uint8_t dots;
  uint8_t mode;
  uint8_t xpos;
  uint8_t ypos;
  uint8_t csel;
  uint8_t miss;
  uint8_t hide;
  
} r_lcdDogm_LcdState_t;



/***********************************************************
  Type: r_lcdDogm_PortConfig_t
  
  Port configuration
  
  Members:
  rw_port   - 
  rw_pin    - 
  cs_port   - 
  cs_pin    - 
  en_port   - 
  en_pin    - 
  rs_port   - 
  rs_pin    - 
  data_port - 
  data_pin  -
*/
typedef struct {
  uint8_t rw_port;
  uint8_t rw_pin;
  
  uint8_t cs_port;
  uint8_t cs_pin;
  
  uint8_t en_port;
  uint8_t en_pin;
  
  uint8_t rs_port;
  uint8_t rs_pin;
  
  uint8_t data_port[R_LCDDOGM_DWIDTH];
  uint8_t data_pin[R_LCDDOGM_DWIDTH];

} r_lcdDogm_PortConfig_t;



/***********************************************************
  Section: Global API Functions
*/


/***********************************************************
  Function: R_LCDDOGM_Init
  
  Initialises LCDDogm
  
  Parameters:
  lcdData         - LCD data, see <r_lcdDogm_Data_t>
  pconf           - port configuration, see <r_lcdDogm_PortConfig_t>
                                           
  Returns:             
  void

*/
void R_LCDDOGM_Init(r_lcdDogm_Data_t lcdData, r_lcdDogm_PortConfig_t pconf);

/***********************************************************
  Function: R_LCDDOGM_ReadRegister
  
  Read Register 
  
  Parameters:
  void
                                           
  Returns:             
  Byte - register value

*/
uint8_t R_LCDDOGM_ReadRegister(void);

/***********************************************************
  Function: R_LCDDOGM_Wait
  
  Wait for a given number of cycles
  
  Parameters:
  cycles         - number of cycles to wait for
                                           
  Returns:             
  error, see <r_lcdDogm_Error_t>

*/
r_lcdDogm_Error_t R_LCDDOGM_Wait(uint32_t cycles);

/***********************************************************
  Function: R_LCDDOGM_WriteReg
  
  Write to register 
  
  Parameters:
  value         - value to be written
                                           
  Returns:             
  void

*/
void R_LCDDOGM_WriteReg(uint8_t value);

/***********************************************************
  Function: R_LCDDOGM_WriteData
  
  Write Data
  
  Parameters:
  value         - value to be written
                                           
  Returns:             
  void

*/
void R_LCDDOGM_WriteData(uint8_t value);


/***********************************************************
  Function: R_LCDDOGM_CheckXPos
  
  Check X position
  
  Parameters:
  void         
                                           
  Returns:             
  void

*/

void R_LCDDOGM_CheckXPos(void);

/***********************************************************
  Function: R_LCDDOGM_CheckYPos
  
  Check X position
  
  Parameters:
  void         
                                           
  Returns:             
  void

*/
void R_LCDDOGM_CheckYPos(void);		  

/***********************************************************
  Function: R_LCDDOGM_SetMode
  
  Set LCDdogm control mode 
  
  Parameters:
  void
                                           
  Returns:             
  void

*/
void R_LCDDOGM_SetMode(void);  


/***********************************************************
  Function: R_LCDDOGM_ActivateDisplay
  
  Activates the display
  
  Parameters:
  void
                                           
  Returns:             
  void

*/
void R_LCDDOGM_ActivateDisplay(void);

/***********************************************************
  Function: R_LCDDOGM_ActivateCursor
  
  Activates the cursor
  
  Parameters:
  void
                                           
  Returns:             
  void

*/
void R_LCDDOGM_ActivateCursor(void);

/***********************************************************
  Function: R_LCDDOGM_ActivateBlink
  
  Activates blinks
  
  Parameters:
  void
                                           
  Returns:             
  void

*/
void R_LCDDOGM_ActivateBlink(void);

/***********************************************************
  Function: R_LCDDOGM_ActivateBacklight
  
  Activates backlight
  
  Parameters:
  void
                                           
  Returns:             
  void

*/
void R_LCDDOGM_ActivateBacklight(void);

/***********************************************************
  Function: R_LCDDOGM_DeactivateDisplay
  
  Deactivates the display
  
  Parameters:
  void
                                           
  Returns:             
  void

*/
void R_LCDDOGM_DeactivateDisplay(void);

/***********************************************************
  Function: R_LCDDOGM_DeactivateCursor
  
  Deactivates the cursor
  
  Parameters:
  void
                                           
  Returns:             
  void

*/
void R_LCDDOGM_DeactivateCursor(void);

/***********************************************************
  Function: R_LCDDOGM_DeactivateBlink
  
  Deactivates the Blink
  
  Parameters:
  void
                                           
  Returns:             
  void

*/
void R_LCDDOGM_DeactivateBlink(void);

/***********************************************************
  Function: R_LCDDOGM_DeactivateBacklight
  
  Deactivates the backlight
  
  Parameters:
  void
                                           
  Returns:             
  void

*/
void R_LCDDOGM_DeactivateBacklight(void);


/***********************************************************
  Function: R_LCDDOGM_ClearDisplay
  
  Clear the display
  
  Parameters:
  void
                                           
  Returns:             
  void

*/
void R_LCDDOGM_ClearDisplay(void);

/***********************************************************
  Function: R_LCDDOGM_ClearLine
  
  Clear one given line
  
  Parameters:
  Line      - Line number to be clear
                                           
  Returns:             
  void

*/
void R_LCDDOGM_ClearLine(uint8_t line);

/***********************************************************
  Function: R_LCDDOGM_ClearEOL
  
  Clear EOL
  
  Parameters:
  void
                                           
  Returns:             
  void

*/
void R_LCDDOGM_ClearEOL(void);


/***********************************************************
  Function: R_LCDDOGM_GotoXY
  
  Go to a given position XY
  
  Parameters:
  x_pos         - position on the x-axis
  y_pos         - position on the y-axis
                                           
  Returns:             
  void

*/
void R_LCDDOGM_GotoXY(uint8_t x_pos, uint8_t y_pos);

/***********************************************************
  Function: R_LCDDOGM_GotoX
  
  Go to a given position X
  
  Parameters:
  x_pos         - position on the x-axis
                                           
  Returns:             
  void

*/
void R_LCDDOGM_GotoX(uint8_t x_pos);

/***********************************************************
  Function: R_LCDDOGM_GotoY
  
  Go to a given position Y
  
  Parameters:
  y_pos         - position on the y-axis
                                           
  Returns:             
  void

*/
void R_LCDDOGM_GotoY(uint8_t y_pos);

/***********************************************************
  Function: R_LCDDOGM_GetCurPosX
  
  Get the current X position 
  
  Parameters:
  void
                                           
  Returns:             
  position on the x-axis

*/
uint8_t R_LCDDOGM_GetCurPosX(void);

/***********************************************************
  Function: R_LCDDOGM_GetCurPosY
  
  Get the current Y position 
  
  Parameters:
  void
                                           
  Returns:             
  position on the y-axis

*/
uint8_t R_LCDDOGM_GetCurPosY(void);

/***********************************************************
  Function: R_LCDDOGM_GetMaxPosX
  
  Get the max limit on x-axis 
  
  Parameters:
  void
                                           
  Returns:             
  Max position on x-axis

*/
uint8_t R_LCDDOGM_GetMaxPosX(void);

/***********************************************************
  Function: R_LCDDOGM_GetMaxPosY
  
  Get the max limit on y-axis 
  
  Parameters:
  void
                                           
  Returns:             
  Max position on y-axis

*/
uint8_t R_LCDDOGM_GetMaxPosY(void);

/***********************************************************
  Function: R_LCDDOGM_WriteString
  
  Write Stringto LCD 
  
  Parameters:
  out_string    - String to be written out.
                                           
  Returns:             
  void

*/
void R_LCDDOGM_WriteString(char * out_string);

/***********************************************************
  Function: R_LCDDOGM_WriteChar
  
  Write character 
  
  Parameters:
  character     - character to be displayed
                                           
  Returns:             
  void

*/
void R_LCDDOGM_WriteChar(uint8_t character);


/***********************************************************
  Function: R_LCDDOGM_SetUserChar
  
  Set user character 
  
  Parameters:
  charnum       - number of character
  user_char     - pointer to an array of character
                                           
  Returns:             
  void

*/
void R_LCDDOGM_SetUserChar(uint8_t charnum, unsigned char * user_char);


#endif /* _LCDDOGM_API_H_  */



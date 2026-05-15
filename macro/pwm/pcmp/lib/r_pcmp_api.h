/*
****************************************************************************
PROJECT : PCMP driver
FILE ID : $Id: r_pcmp_api.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Driver for PCMP macro
============================================================================
                            C O P Y R I G H T                                    
============================================================================
                           Copyright (c) 2014
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

#ifndef PCMP_API_H_
#define PCMP_API_H_

#ifdef __cplusplus
extern "C" {
#endif


#define R_PCMP_SIGNAL_NUMBER   8u


/***********************************************************
  Title: Hi Speed (PCM) PWM  API

  outputs PCM audio samples as PWM to drive speakers.
    
*/


/***********************************************************
  Section: Global Types
*/


/***********************************************************
  Enum: r_pcmp_Signal_t

  PCMP macro signals.

  Values:

*/

typedef enum {
    R_PCMP_SIGNAL_AN0 = 0,
    R_PCMP_SIGNAL_AP0,
    R_PCMP_SIGNAL_BN0,
    R_PCMP_SIGNAL_BP0,
    R_PCMP_SIGNAL_AN1,
    R_PCMP_SIGNAL_AP1,
    R_PCMP_SIGNAL_BN1,
    R_PCMP_SIGNAL_BP1                
} r_pcmp_Signal_t;


/***********************************************************
  Enum: r_pcmp_Error_t

  PCM PWM macro driver error code.

  Values:
  R_PCMP_ERR_OK             - No error
  R_PCMP_ERR_INVALID_FMT    - this format is not valid
  R_PCMP_ERR_RANGE          - Parameter out of range
  R_PCMP_ERR_INSTANCE       - Unit, instance not supported
  R_PCMP_ERR_PORTINIT       - port init failed
  R_PCMP_ERR_LAST           - Delimiter 
*/

typedef enum {
    R_PCMP_ERR_OK           = 0x00,
    R_PCMP_ERR_INVALID_FMT  = 0x01,
    R_PCMP_ERR_INSTANCE     = 0x02,
    R_PCMP_ERR_PORTINIT,
    R_PCMP_ERR_RANGE,
    R_PCMP_ERR_LAST
} r_pcmp_Error_t;

/***********************************************************
  Enum: r_pcmp_ClockDivider_t

  Macro clock selection.

  Values:
  R_PCMP_CLOCK_DIV_1    - Divide clock by 1
  R_PCMP_CLOCK_DIV_2    - Divide clock by 2
  R_PCMP_CLOCK_DIV_4    - Divide clock by 4
  R_PCMP_CLOCK_DIV_8    - Divide clock by 8
  R_PCMP_CLOCK_DIV_16   - Divide clock by 16
  R_PCMP_CLOCK_DIV_32   - Divide clock by 32
  R_PCMP_CLOCK_DIV_64   - Divide clock by 64
  R_PCMP_CLOCK_DIV_128  - Divide clock by 128
*/

typedef enum {
    R_PCMP_CLOCK_DIV_1 = 0,
    R_PCMP_CLOCK_DIV_2,
    R_PCMP_CLOCK_DIV_4,
    R_PCMP_CLOCK_DIV_8,
    R_PCMP_CLOCK_DIV_16,
    R_PCMP_CLOCK_DIV_32,
    R_PCMP_CLOCK_DIV_64,
    R_PCMP_CLOCK_DIV_128,
} r_pcmp_ClockDivider_t;


/***********************************************************
  Enum: R_PCMP_StaticLevel_t

  Macro usage type.

  Values:
    R_PCMP_8K       - Audio Sample Rate = 8000,
    R_PCMP_11K      - Audio Sample Rate = 11000,
    R_PCMP_16K      - Audio Sample Rate = 16000,
    R_PCMP_22K      - Audio Sample Rate = 22050,
    R_PCMP_44K      - Audio Sample Rate = 44000,
    R_PCMP_48K      - Audio Sample Rate = 48000     
*/

typedef enum {
    R_PCMP_8K = 8000,
    R_PCMP_11K = 11025,
    R_PCMP_16K = 16000,
    R_PCMP_22K = 22050,
    R_PCMP_44K = 44100,
    R_PCMP_48K = 48000           
} r_pcmp_SampleRate_t;


/***********************************************************
  Enum: r_pcmp_OperationMode_t

  Macro usage type.

  Values:
  R_PCMP_OPERATION_MODE0    - Mode 0 (e.g. emitter folower)
  R_PCMP_OPERATION_MODE1    - Mode 1 (full H-bridge)
*/

typedef enum {
    R_PCMP_OPERATION_MODE0 = 0,
    R_PCMP_OPERATION_MODE1
} r_pcmp_OperationMode_t;


/***********************************************************
  Enum: r_pcmp_PcmFormat_t

  Macro usage type.

  Values:
  R_PCMP_FORMAT_2KOMP    - PCM Data is 2-Complement
  R_PCMP_FORMAT_WAVE     - PCM DATA is WAV format
*/

typedef enum {
    R_PCMP_FORMAT_2KOMP = 0,
    R_PCMP_FORMAT_WAVE
} r_pcmp_PcmFormat_t;


/***********************************************************
  Enum: r_pcmp_BitLength_t

  Macro usage type.

  Values:
    Bit length of PCM data from 8 to 16 Bits
*/

typedef enum {
    R_PCMP_16_BITS = 0,
    R_PCMP_15_BITS,
    R_PCMP_14_BITS,        
    R_PCMP_13_BITS,        
    R_PCMP_12_BITS,        
    R_PCMP_11_BITS,        
    R_PCMP_10_BITS,        
    R_PCMP_9_BITS,        
    R_PCMP_8_BITS                       
} r_pcmp_BitLength_t;

/***********************************************************
  Enum: r_pcmp_StereoMode_t

  Macro usage type.

  Values:
  R_PCMP_MODE_MONO      - Mono
  R_PCMP_MODE_STEREO    - Stereo
*/

typedef enum {
    R_PCMP_MODE_MONO = 0,
    R_PCMP_MODE_STEREO,
} r_pcmp_StereoMode_t;


/***********************************************************
  Enum: r_pcmp_DoubleMode_t

  Macro usage type.

  Values:
  R_PCMP_DOUBLE_MODE_OFF    - outputs every audio sample once
  R_PCMP_DOUBLE_MODE_ON     - outputs every audio sample twice
*/

typedef enum {
    R_PCMP_DOUBLE_MODE_OFF = 0,
    R_PCMP_DOUBLE_MODE_ON,
} r_pcmp_DoubleMode_t;


/***********************************************************
  Enum: eePCM_ActiveLevel_t

  Macro usage type.

  Values:
  R_PCMP_ACTIVE_LEVEL_HIGH  - Active level high
  R_PCMP_ACTIVE_LEVEL_LOW   - Active level low
*/

typedef enum {
    R_PCMP_ACTIVE_LEVEL_HIGH,
    R_PCMP_ACTIVE_LEVEL_LOW,
} r_pcmp_ActiveOutput_t;


/***********************************************************
  Enum: R_PCMP_StaticLevel_t

  Macro usage type.

  Values:
  R_PCMP_STATIC_LEVEL_HIGH  - Static level high
  R_PCMP_STATIC_LEVEL_LOW   - Static level low
*/

typedef enum {
    R_PCMP_STATIC_LEVEL_HIGH,
    R_PCMP_STATIC_LEVEL_LOW,
} r_pcmp_StaticOutput_t;


/***********************************************************
  Enum: eePCM_OutputPins_t

  Macro usage type.

  Values:
  R_PCMP_OUTPUT_AP0 - Pin A Positiv 0
  R_PCMP_OUTPUT_AN0 - Pin A Negativ 0
  R_PCMP_OUTPUT_BP0 - Pin B Positiv 0
  R_PCMP_OUTPUT_BN0 - Pin B Negativ 0

  R_PCMP_OUTPUT_AP1 - Pin A Positiv 1
  R_PCMP_OUTPUT_AN1 - Pin A Negativ 1
  R_PCMP_OUTPUT_BP1 - Pin B Positiv 1
  R_PCMP_OUTPUT_BN1 - Pin B Negativ 1
*/

typedef enum {
    R_PCMP_OUTPUT_AP0 = 1u,
    R_PCMP_OUTPUT_AN0 = 2u,
    R_PCMP_OUTPUT_BP0 = 4u,
    R_PCMP_OUTPUT_BN0 = 8u,

    R_PCMP_OUTPUT_AP1 = 256u,
    R_PCMP_OUTPUT_AN1 = 512u,
    R_PCMP_OUTPUT_BP1 = 1024u,
    R_PCMP_OUTPUT_BN1 = 2048u
} r_pcmp_PinMask_t;


/***********************************************************
  Enum: r_pcmp_BufferFlag_t

  FIFO buffer flag types

  Values:
  R_PCMP_BUFFER_UNDERRUN    - FIFO buffer underrun
  R_PCMP_BUFFER_OVERFLOW    - FIFO buffer overflow
*/

typedef enum {
    R_PCMP_BUFFER_UNDERRUN = 0,
    R_PCMP_BUFFER_OVERFLOW,
} r_pcmp_BufferFlag_t;


/***********************************************************
  Enum: r_pcmp_Interrupt_t

  Pcm macro interrupt type. List of all available interrupts for this macro

  Values:
    INTPCMP0TFFIL - FIFO fill request interrupt
    INTPCMP0TFERR - error interrupt
*/
typedef enum {
    R_PCMP_INT_FFIL = 0,
    R_PCMP_INT_ERR,
    R_PCMP_INT_NUM   /* delimiter */
} r_pcmp_Interrupt_t;


/***********************************************************
  Enum: eePCM_Channel_t

  PCM channel selcetion 0 or 1

  Values:
  R_PCMP_CHANNEL_0        - Channel 0
  R_PCMP_CHANNEL_1        - Channel 1
*/

typedef enum {
  R_PCMP_CHANNEL_0 = 0,
  R_PCMP_CHANNEL_1
} r_pcmp_Channel_t;

/* 2 channels */
#define R_PCMP_NUM_CHANNELS    (2u)


/***********************************************************
  Enum: r_pcmp_Parameters_t

  PCM PWM macro configuration data
  
  PinMasks (Outputs, Inverted, StaticLevelActive) are 
  or-linked PinMask enum members
  (e.g. : R_PCMP_OUTPUT_AP0 | R_PCMP_OUTPUT_AN1)
  
    r_pcmp_ClockDivider_t    ClockDiv                    - divider value see: <r_pcmp_ClockDivider_t>
    r_pcmp_OperationMode_t   OperationMode               - Output mode 0,1 see: <r_pcmp_OperationMode_t>
    r_pcmp_StereoMode_t      StereoMode                  - Mono or stereo see: <r_pcmp_StereoMode_t>
    r_pcmp_DoubleMode_t      DoubleMode                  - Double output see: <r_pcmp_DoubleMode_t>
    uint16_t                PinMaskOutputs              - static output level
    uint16_t                PinMaskInverted             - inverted level
    uint16_t                PinMaskStaticLevelActive    - enable static output

*/     

typedef struct {
    r_pcmp_ClockDivider_t    ClockDiv;
    r_pcmp_OperationMode_t   OperationMode;
    r_pcmp_StereoMode_t      StereoMode;
    r_pcmp_DoubleMode_t      DoubleMode;
    uint16_t                PinMaskOutputs;
    uint16_t                PinMaskInverted;
    uint16_t                PinMaskStaticLevelActive;
} r_pcmp_Parameters_t;
 
 
/***********************************************************
  Enum: r_pcmp_Format_t

  PCM data info
  
    BitLength   - Bit length of PCM data; see: <r_pcmp_BitLength_t>
    Format      - Bit Format of PCM data; see: <r_pcmp_PcmFormat_t>

*/   

typedef struct {
    r_pcmp_BitLength_t   BitLength;
    r_pcmp_PcmFormat_t   Format;    
} r_pcmp_Format_t;


/***********************************************************
  Section: Global API Functions
*/

/***********************************************************
  Function: R_PCMP_Init

  Driver init function. Provides a complete initialization of the the PWM.
  Sets the configuration options as defined in the parameters structure.

  Parameters:
  Unit      - Instance number
  Config    - see: <r_pcmp_Parameters_t>
 
  Returns:
  see: <eePwm_Pcm_Error_t>
*/

r_pcmp_Error_t R_PCMP_Init(uint32_t Unit, r_pcmp_Parameters_t  *Config);


/***********************************************************
  Function: R_PCMP_Enable

  
  Parameters:
  Unit      - Instance number
  Enable    - true: Enable, false: Disable
  
  Returns:
  see: <r_pcmp_Error_t>
*/

r_pcmp_Error_t R_PCMP_Enable(uint32_t Unit, uint32_t Enable);


uint32_t R_PCMP_SetBestClock(uint32_t Unit, r_pcmp_SampleRate_t Rate, r_pcmp_BitLength_t Bits);

/***********************************************************
  Function: R_PCMP_SetSvstop

  disable or enable SVSTOP signal
  
  Parameters:
  Unit      - Instance number
  Enable    - true: Enable, false: Disable
  
  Returns:
  see: <r_pcmp_Error_t>
*/

r_pcmp_Error_t R_PCMP_SetSvstop(uint32_t Unit, uint32_t Enable);


/***********************************************************
  Function: R_PCMP_GetBufferCount

  
  Parameters:
  Unit          - Instance number
  
  Returns:
  number of Samples in buffer 0~2
  see: <r_pcmp_Error_t>
  
*/

r_pcmp_Error_t R_PCMP_GetBufferCount(uint32_t Unit, uint8_t *SampleCnt);


/***********************************************************
  Function: R_PCMP_GetBufferFlag

  
  Parameters:
  Unit          - Instance number
  Flag          - Select flag to read
  
  Returns:
  false: flag not ser, true: flag is set
  
*/
uint32_t R_PCMP_GetBufferFlag(uint32_t Unit, r_pcmp_BufferFlag_t Flag);


/***********************************************************
  Function: R_PCMP_ClearBufferFlag

  
  Parameters:
  Unit          - Instance number
  Flag          - Select flag to read
  
  Returns:
  see: <r_pcmp_Error_t>

  
*/

r_pcmp_Error_t R_PCMP_ClearBufferFlag(uint32_t Unit, r_pcmp_BufferFlag_t Flag);


/***********************************************************
  Function: R_PCMP_SetOffset

  
  Parameters:
  Unit          - Instance number
  Offset        - This value will be added to every sample
                  during the conversion
  
  Returns:
  see: <r_pcmp_Error_t>
  
*/

r_pcmp_Error_t R_PCMP_SetOffset(uint32_t Unit, uint16_t Offset);


/***********************************************************
  Function: R_PCMP_WriteSamples

  
  Parameters:
  Unit          - Instance number
  Sample1       - mono: First, stereo: Left channel sample
  Sample2       - mono: Second, stereo: Right channel sample
  
  Returns:
  see: <r_pcmp_Error_t>

*/

r_pcmp_Error_t R_PCMP_WriteSamples(uint32_t Unit, uint16_t Sample1, uint16_t Sample2);


r_pcmp_Error_t R_PCMP_Write2Buffer(uint32_t Unit, uint16_t Sample1, uint16_t Sample2,
                                    r_pcmp_Format_t DataInfo);
    
    
/***********************************************************
  Function: R_PCMP_SetClockDivider

  
  Parameters:
  Unit          - Instance number
  ClockDiv      - Clock Divider, PWM clock = PCLK / ClockDiv
  
  Returns:
  see: <r_pcmp_Error_t>

  
*/

r_pcmp_Error_t R_PCMP_SetClockDivider(
    uint32_t Unit, 
    r_pcmp_ClockDivider_t    ClockDiv);


/***********************************************************
  Function: R_PCMP_SetPwmPeriod

  
  Parameters:
  Unit          - Instance number
  Period        - new value for TPwm - PWM period counter
  
  Returns:
  see: <r_pcmp_Error_t>

  
*/

r_pcmp_Error_t R_PCMP_SetPwmPeriod(
    uint32_t Unit, 
    uint16_t     Period);



/***********************************************************
  Function: R_PCMP_SetErrorCallback

  Set a user function as an the error handler.

  Parameters:
  *ErrorHandler - a pointer to a user function (callback)
                  with two paramerters (unit and error number)

  Returns:
  void

*/

void R_PCMP_SetErrorCallback(void ( *ErrorCallback )(uint32_t Unit, uint32_t Error));


/***********************************************************
  Function: R_PCMP_SetIsrCallback

  Assign a user callback for the given interrupt of a unit.

  The function will be called after the driver ISR has
  processed the interrupt.

  Parameters:
  Unit          - Instance number
  IntType       - What kind of interrupt; see: <r_pcmp_Interrupt_t>
  Isr           - Isr function pointer

  Returns:
  see: <r_pcmp_Error_t>
*/

r_pcmp_Error_t R_PCMP_SetIsrCallback(uint32_t Unit, r_pcmp_Interrupt_t IntType,
                                 void (*Isr)(void));


/***********************************************************
  Function: R_PCMP_EnableInt

  Enables specific interrupt

  Parameters:
  Unit          - Instance number
  Interrupt     - What kind of interrupt; see: <r_pcmp_Interrupt_t>

  Returns:
  see: <r_pcmp_Error_t>
*/

r_pcmp_Error_t R_PCMP_EnableInt(uint32_t Unit, r_pcmp_Interrupt_t Interrupt) ;


/***********************************************************
  Function: R_PCMP_DisableInt

  Disables specific interrupt

  Parameters:
  Unit          - Instance number
  Interrupt     - What kind of interrupt; see: <r_pcmp_Interrupt_t>

  Returns:
  see: <r_pcmp_Error_t>
*/

r_pcmp_Error_t R_PCMP_DisableInt(uint32_t Unit, r_pcmp_Interrupt_t Interrupt);


/***********************************************************
  Function: R_PCMP_Isr

  Driver interrupt service routine. 
  
  This function shall be called by sevice interrupt handler 

  Parameters:
  Unit       - Instance number
  Interrupt  - channel interrupt
  
  Returns:
  void
*/

void R_PCMP_Isr(uint32_t Unit, r_pcmp_Interrupt_t Interrupt);

/***********************************************************
  Function: R_PCMP_GetClockOption

  Gets the clock option for the selected Pcm unit

  Parameters:
  Unit          Pcm unit
  uint8_t       ID

  Returns:
  uint32_t      clock frequency

*/
uint32_t R_PCMP_GetClockOption(uint32_t Unit, uint8_t ID);


/***********************************************************
  Function: eeIic_GetClock

  Gets the clock for the selected Pcm unit

  Parameters:
  Unit          Pcm unit

  Returns:
  uint32_t      clock frequency

*/
uint32_t R_PCMP_GetClock(uint32_t Unit);


#ifdef __cplusplus
}
#endif

#endif /* PCMP_API_H_  */


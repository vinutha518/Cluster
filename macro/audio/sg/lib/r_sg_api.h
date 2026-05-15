/*
****************************************************************************
PROJECT : VLIB macro driver
FILE    : $Id: r_sg_api.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Generic part of the macro driver
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

#ifndef SG_API_H_
#define SG_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: SG Driver API 
  
  An application using SG should only need to include this r_sg_api.h
*/

/*******************************************************************************
  Section: Global Constants 
*/


/*******************************************************************************
  Constant:  R_SG_VERSION_HI and R_SG_VERSION_LO

  Driver version information  
*/

#define R_SG_VERSION_HI 0
#define R_SG_VERSION_LO 1



/***********************************************************
  Title: Sound Generator API

  An application using SG should include this header file.
  
  The Sound Generator provides a sound output signal that may be
  used to drive a speaker circuit.
  The output signal is a square wave signal with selectable 
  frequency and amplitude.
  The sound signal amplitude is defined by the pulse width 
  of a PWM signal.
  
Programmable tone frequency (100 Hz to 6 kHz with a minimum
  step size of 20 Hz)
• Programmable volume level (9 bit resolution)
• Wide range of PWM signal frequency (32 kHz to 64 kHz)
• Automatic logarithmic volume decrement function (ALD):
 – Volume reduction without CPU interaction
 – Programmable time offset between amplitude changes (up to
   256 sound frequency periods)
 – Sound duration associated with tone frequency (gong effect)
 – Interrupt generation when programmable volume low level is 
   reached
• Automatic Duration Control function (ADC)
 – Programmable sound duration with interrupt generation at
   the end of the sound
• Sound can be stopped or retriggered (even if the 
  ALD / ADC function is ongoing)
• Composite or separated frequency/volume output for 
  external circuitry variation
• Hardware-optimized update of frequency and volume to avoid
  audible artifacts
*/

/***********************************************************
  Section: Global Defines
*/

/***********************************************************
  frequencies of scale of c - major (rounded)
*/

#define R_SG_C1 262
#define R_SG_D1 294
#define R_SG_E1 330
#define R_SG_F1 349
#define R_SG_G1 392
#define R_SG_A1 440
#define R_SG_B1 494
#define R_SG_C2 523
#define R_SG_D2 587
#define R_SG_E2 659
#define R_SG_F2 698
#define R_SG_G2 784
#define R_SG_A2 880
#define R_SG_B2 988
#define R_SG_C3 1046
#define R_SG_D3 1175
#define R_SG_E3 1318
#define R_SG_F3 1397
#define R_SG_G3 1568
#define R_SG_A3 1760
#define R_SG_B3 1975                       
#define R_SG_SIGNAL_NUMBER 3
#define R_SG_SIGNAL_MAXPOS 2


/*******************************************************************************
  Section: Global Types
*/


/***********************************************************
  Enum: r_sg_Interrupt_t

  SG macro interrupt type. List of all available interrupts 
  for this macro

  Values:
  R_SG_INT_SG0TIO      - Interrupt: end of sound in ADC, ADI and ALD mode
  R_SG_INT_NUM         - Delimeter 
*/
typedef enum {
    R_SG_INT_SGTIO = 0, 
    R_SG_INT_NUM    = 1u,
} r_sg_Interrupt_t;

/***********************************************************
  Enum: r_sg_Signal_t

  Sg macro signals.

  Values:
    R_SG_SIGNAL_FAOL  - inbverted mixed output of sg
    R_SG_SIGNAL_FAO   - Mixed output of sg   
    R_SG_SIGNAL_AO    - volume output of sg        
*/
typedef enum {
    
    R_SG_SIGNAL_FAOL,
    R_SG_SIGNAL_FAO,
    R_SG_SIGNAL_AO

} r_sg_Signal_t;


/***********************************************************
  Enum: r_sg_Error_t

  SG macro driver error code.

  Values:
  R_SG_ERR_OK              - No error
  R_SG_ERR_INVALID_MODE    - this mode is not alowed
  R_SG_ERR_RANGE           - Parameter out of range
  R_SG_ERR_INSTANCE        - Unit, instance not supee_sg_err_rangeed
  R_SG_ERR_PORTINIT        - ee_sg_err_range init failed
  R_SG_ERR_LAST            - Delimiter 
*/
typedef enum {
    R_SG_ERR_OK            = 0,
    R_SG_ERR_INVALID_MODE  = 1u,
    R_SG_ERR_RANGE         = 2u,
    R_SG_ERR_INSTANCE      = 3u,
    R_SG_ERR_PORTINIT      = 4u,
    R_SG_ERR_LAST          = 5u
} r_sg_Error_t;


/***********************************************************
  Enum: r_sg_Mode_t

  Sound Generator works in 4 different modes.

  Values:
  R_SG_MODE_CS     - Continuous Sound
  R_SG_MODE_ADC    - Automatic Duration Control of sound
  R_SG_MODE_ADI    - Automatic linear Decrement/Increment
                      of amplitude
  R_SG_MODE_ALD    - Automatic Logarithmic Decrement of
                      amplitude
*/
typedef enum {    
    R_SG_MODE_CS  = 0,
    R_SG_MODE_ADC = 1u,
    R_SG_MODE_ADI = 2u,
    R_SG_MODE_ALD = 3u
} r_sg_Mode_t;

/***********************************************************
  Enum: r_sg_SoundOutSelection_t


  Sound Output Pin can be connected to Sound Output or to
  Sound Output mixed with Amplitude PWM.

  Values:
  R_SG_SOUNDOUT_SOUNDONLY   - Pure Sound on Sound Output
  R_SG_SOUNDOUT_MIXED       - Sound mixed with PWM on 
                              Sound Output Pin
*/

typedef enum {    
    R_SG_SOUNDOUT_SOUNDONLY = 0,
    R_SG_SOUNDOUT_MIXED
} r_sg_SoundOutSelection_t;

/***********************************************************
  Enum: r_sg_InvOutSelection_t


  Additional to the standard Sound Output Pin, SG has a 
  inverted output pin. It can be connected to inverted
  mixed output.

  Values:
  R_SG_INVOUT_OFF      - No Signal on Inverted Output
  R_SG_INVOUT_ON       - Inverted Mixed Frequency on 
                          Inverted Output Pin
*/
typedef enum {    
    R_SG_INVOUT_OFF = 0,
    R_SG_INVOUT_ON  = 1u
} r_sg_InvOutSelection_t;


/***********************************************************
  Enum: r_sg_RegBuffering_t

  Configuration parameter for init.

  Values:
  R_SG_REGBUF_BUS          - Register Buffering on Bus access
  R_SG_REGBUF_INTERRUPT    - Register Buffering on Interrupt
  
  When set to BUS, register values are written in the same 
  moment, when the write access to PWM register occurs.
  With Interrupt selection, register values are overwritten
  in the moment, when interrupt occurs.
  
*/
typedef enum {    
    R_SG_REGBUF_BUS       = 0,
    R_SG_REGBUF_INTERRUPT = 1u,
} r_sg_RegBuffering_t;


/***********************************************************
  Struct: r_sg_RegisterSet_t

  SG macro register set used for loading all SG registers at once

    Conf        - configuration reg; see: <R_SG_RegConf_t> 
    Df;         - Duration Factor, 
    Adi;        - Automatic Decrement/Increment (Step)
    Ith;        - Interrupt threshold
    Fl;         - Frequency Low
    Fh;         - Frequency High
    Pwm;        - Duty Cycle

*/     

typedef struct {
    uint32_t       Control;
    uint32_t       Stat;
    uint32_t       Conf;
    uint32_t       Df;
    int32_t        Adi;
    uint32_t       Ith;
    uint32_t       Fl;
    uint32_t       Fh;
    uint32_t       Pwm;
} r_sg_RegisterSet_t;


/***********************************************************
  Enum: r_sg_Parameters_t

  SG macro configuration data
  Enabled       - Enable macro 
  Mode          - Mode setting
  SoundOut      - Only PWM or mixed output ; see: <r_sg_SoundOutSelection_t>
  InvOut        - Enable / Diasable inverted output ; see: <r_sg_InvOutSelection_t>
  Buffering     - setting to load new values out of buffers on bus access ot interrupt ; see: <r_sg_RegBuffering_t>
  EnableBreak   - SvStop signal ignored or not
*/     

typedef struct {
    uint32_t                    Enabled; 
    r_sg_Mode_t                 Mode; 
    r_sg_SoundOutSelection_t    SoundOut;
    r_sg_InvOutSelection_t      InvOut;
    r_sg_RegBuffering_t         Buffering;
    uint32_t                    EnableBreak;
    uint8_t                     ClockId;
} r_sg_Parameters_t;
 


/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_SG_GetVersionStr

  Get the driver version number in human readable form (string). 

  Parameters:
  void
  
  Returns:
  ReceiveString     - Pointer to receive string destination
  
*/

const int8_t * R_SG_GetVersionStr(void); 

/*******************************************************************************
  Function: R_SG_GetMajorVersion

  Get the driver major version number. 

  Parameters:
  void
  
  Returns:
     - Major version number
  
*/

const uint16_t R_SG_GetMajorVersion(void); 

/*******************************************************************************
  Function: R_SG_GetMinorVersion

  Get the driver minor version number. 

  Parameters:
  void
  
  Returns:
     - Minor version number
  
*/

const uint16_t R_SG_GetMinorVersion(void); 


/*******************************************************************************
  Function: R_SG_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Macro Instance number

  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_DeInit(uint32_t Unit);


/*******************************************************************************
  Group: SG Error and ISR Functions
*/

/*******************************************************************************
  Function: R_SG_SetErrorCallback

  Set a user function as error handler.
  
  When the driver detects any error, it will call this function.

  Parameters:
  ErrorCallback - Pointer to a user function
  
  Returns:
  void
*/

void R_SG_SetErrorCallback(void (*ErrorCallback )(uint32_t Unit, uint32_t Error));

/*******************************************************************************
  Function: R_SG_SetIsrCallback

  Assign a user callback for the given interrupt of a unit. 
  
  The function will be called after the driver ISR has
  processed the interrupt.

  Parameters:
  Unit          - Instance number
  Interrupt       - What interrupt,  see: <r_sg_Interrupt_t>
  Isr           - Isr function pointer
  
  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_SetIsrCallback(uint32_t Unit, r_sg_Interrupt_t Interrupt, 
                                 void (*Isr)(void));
                                 

/*******************************************************************************
  Function: R_SG_EnableInt

  Enable the SG interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_sg_Interrupt_t>
  
  Returns:
  void
*/

void R_SG_EnableInt(uint32_t Unit, r_sg_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_SG_DisableInt

  Disable the SG interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_sg_Interrupt_t>
  
  Returns:
  void
*/

void R_SG_DisableInt(uint32_t Unit, r_sg_Interrupt_t Interrupt);




/***********************************************************
  Function: R_SG_SetOptimalFreqs

  PWM frequency can be set between 30 and 60 KHz
  It can be set so, that PWM_frequency = n * Output_Frequency,
  n is integer. This functions computes optimal PWM frequency,
  so that desired output frequency can be exact set

  Parameters:
  OutputFrequency   - Desired Sound frequency
 
  Returns:
  
*/

r_sg_Error_t R_SG_SetOptimalFreqs( uint32_t Unit,
    uint16_t        OutputFrequency);


/***********************************************************
  Function: R_SG_GetToneFreq

  PWM frequency can be set between 30 and 60 KHz
  It can be set so, that PWM_frequency = n * Output_Frequency,
  n is integer. This functions computes optimal PWM frequency,
  so that desired output frequency can be exact set

  Parameters:
    Unit   - 
 
  Returns:
    actual tone frequency
  
*/
uint16_t R_SG_GetToneFreq(uint32_t Unit);
    
    
/***********************************************************
  Function: R_SG_SetRegisters



  Parameters:
  Unit          - Instance number
  RegisterSet   - Set of register values, see also RegisterSet_t
 
  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_SetRegisters(
    uint32_t            Unit,
    r_sg_RegisterSet_t  RegisterSet);


/***********************************************************
  Function: R_SG_Init

  Driver init function. Provides a complete initialization of the the SG.
  Sets the configuration options as defined in the parameter structure.

  Parameters:
  Unit      - Instance number
  Config    - see: <eeSg_Parameters_t>
 
  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_Init(
    uint32_t            Unit, 
    r_sg_Parameters_t   *Config
    );
    
    
/***********************************************************
  Function: R_SG_SetClockDivider

  Clock Divider brings fCLK into the range, that can be used
  for Sound Generator. For best resolution, Divider should be
  so small as possible. (biggest possible fCLK)
  fCLK = PCLK / CLKDIV
  fCLK must be < 16.384 MHZ!!! (512 x 32KHz)
  
  Parameters:
  Unit      - Instance number
  clkDiv    - Clock Divider 1~8
  
  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_SetClockDivider(uint32_t  Unit, uint8_t ClkDiv);


/***********************************************************
  Function: R_SG_Enable

  Enables or disables sound generation
  
  Parameters:
  Unit      - Instance number
  Enable    - SG Enable/Disable

  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_Enable(
    uint32_t   Unit, 
    uint32_t   Enable);


/***********************************************************
  Function: R_SG_IsRunning

  Checks, is Sound Generator running
  
  Parameters:
  Unit      - Instance number
  

  Returns:
  0: Not running
  1: Running
*/

uint32_t R_SG_IsRunning(uint32_t Unit);


/***********************************************************
  Function: R_SG_SetMode

  Set Mode for Sound
  
  Parameters:
  Unit  - Instance number
  Mode  - New Mode, see: eeSg_Mode_t
  

  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_SetMode(
    uint32_t    Unit, 
    r_sg_Mode_t Mode);


/***********************************************************
  Function: R_SG_SelectSoundOutput

  Choose Output for Sound 
  
  Parameters:
  Unit      - Instance number
  SoundOut  - pure sound or sound mixed with Amplitude PWM
  

  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_SelectSoundOutput(
    uint32_t                    Unit, 
    r_sg_SoundOutSelection_t    SoundOut);


/***********************************************************
  Function: R_SG_SetInvOutput

  Turn inverted Output on / off
  
  Parameters:
  Unit      - Instance number
  InvOut    - inverted mixed sound on / off
  

  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_SetInvOutput(
    uint32_t                Unit, 
    r_sg_InvOutSelection_t  InvOut);


/***********************************************************
  Function: R_SG_SetBuffering

  Set register buffering to Pwm Write or on interrupt
  
  Parameters:
  Unit          - Instance number
  RegBuffering  - EE_SG_REGBUF_BUS or EE_SG_REGBUF_INTERRUPT
  
  By Bus buffering:
  All the values, that define the sound, are written on write access
  into the PWM register (amplitude) from the bus
  By Interrupt buffering:
  All the values, that define the sound, are written when interrupt
  occurs
  

  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_SetBuffering(
    uint32_t                Unit, 
    r_sg_RegBuffering_t     RegBuffering);


/***********************************************************
  Function: R_SG_SetPWMFrequency

  Set Frequency for Amplitude PWM (Initialize freq reg L)
  PWM Frequency must be in range 32 ~ 64 KHz
  SG0FL is computed from PCLK / CLKDIV and set
  
  
  Parameters:
  Unit          - Instance number
  PWMFrequency  - Frequency: 32~64KHz
  

  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_SetPWMFrequency(
    uint32_t    Unit, 
    uint16_t    PWMFrequency);


/***********************************************************
  Function: R_SG_SetToneFrequency

  Set Tone Frequency (Initialize freq reg SG0FH)
  PWM Frequency is in range 32 ~ 64 KHz
  Tone Frequency is in range 100 ~ 6000 Hz
  SG0FH is computed from fPWM / fTone and set
  SGOFL must not be changed!
  
  
  Parameters:
  Unit          - Instance number
  ToneFrequency - Tone Frequency: 100~6000Hz
  

  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_SetToneFrequency(
    uint32_t    Unit, 
    uint16_t    ToneFrequency);


/***********************************************************
  Function: R_SG_SetPWM

  Set Amplitude for Sound (PWM duty cycle ratio)
  Amplitude has 9 Bits (0-512)
  Amplitude 0: turns generator off
  Amplitude >= SG0GFL: Duty cycle = 100%
  When amplitude was 0 and Enable==1,
  any new value != 0 starts generation
  See also Register Buffering
  
  Parameters:
  Unit      - Instance number
  Amplitude - Amplitude: 0~SG0FL
  

  Returns:
  see: <r_sg_Error_t>
*/


r_sg_Error_t R_SG_SetPWM(
    uint32_t    Unit, 
    uint16_t    Pwm);


/***********************************************************
  Function: R_SG_SetVolume

  Set Amplitude for Sound (PWM duty cycle ratio)
  Amplitude is given in percent
  Amplitude 0: turns generator off
  Amplitude >= 100: Duty cycle = 100%
  When amplitude was 0 and Enable==1,
  any new value != 0 starts generation
  See also Register Buffering
  !!!Remark!!!:
  Frequency (Fl) must be set before this function is called!!!
  
  Parameters:
  Unit          - Instance number
  Percent       - PWM value percentage (of FL value)
  

  Returns:
  see: <eeSg_Error_t>
*/

r_sg_Error_t R_SG_SetVolume(
    uint32_t Unit, 
    uint8_t  Percent);


/***********************************************************
  Function: R_SG_SetDuration

  Sets the Duraction settings for Sound.
  
  Parameters:
    Unit        - Instance number
    Dfactor     - Clk Devider val which assigns no. of clks after PWM will be updated with stepsize
    Stepsize    - Stepsize in percent based on Fl (max.PWM) value

  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_SetDuration(
    uint32_t Unit, uint32_t Dfactor, int16_t Stepsize);
    
    
/***********************************************************
  Function: R_SG_GetMaximalPwm

  Get maximal value for Amplitude (PWM duty cycle ratio)
  Register FL determines maximal value and his value
  is set by eeSg_SetToneFrequency
  Amplitude >= FL: Duty cycle = 100%
  
  Parameters:
  Unit          - Instance number

  Returns:
  maximal PWM value (value for 100% duty cycle)
*/

uint16_t R_SG_GetMaximalPwm(uint32_t  Unit);


/***********************************************************
  Function: R_SG_PlaySound

  Plays sound for given number of periods in ADC mode
  Frequency must be set before with SetPWMFrequency and 
  SetToneFrequency 
  
  
  Parameters:
  Unit              - Instance number
  AmplitudePerc     - Amplitude in percent: 0~100
  Duration          - Duration counter
  DurationFactor    - divider for duration
  
  Plays sound for Duration x DurationFactor periods
  Every <DurationFactor> Clocks, duration counter is decremented
  Duration and DurationFactor have the same influence to the
  duration of the sound in ADC mode

  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_PlaySound(
    uint32_t    Unit, 
    uint16_t    AmplitudePerc,
    uint16_t    Duration,
    uint8_t     DurationFactor);


/***********************************************************
  Function: R_SG_FadeSoundLin

  Plays linear FadeIn or FadeOut sound (ADI mode)
  Frequency must be set before with SetPWMFrequency and 
  SetToneFrequency. Positive AmplitudeStep means FadeIn,
  negative AmplitudeStep means FadeOut.
  
  Parameters:
  Unit              - Instance number
  StartAmplitude    - Start amplitude in percent
  Divider     - One incrementing/decrementing step
                      every Divider Periods
  AmplitudeStep     - Add this value to Amplitude register
                      ignored in ALD mode

  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_FadeSoundLin(
    uint32_t    Unit,
    int16_t     StartAmplitudePerc,
    uint8_t     Divider,
    int16_t     AmplitudeStep);


/***********************************************************
  Function: R_SG_FadeOutSoundLog

  Plays logarythmic FadeOut sound (ALD mode)
  Frequency must be set before with SetPWMFrequency and 
  SetToneFrequency.
  
  Parameters:
  Unit              - Instance number
  StartAmplitude    - Start amplitude in percent
  Divider     - One incrementing/decrementing step
                      every Divider Periods

  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_FadeOutSoundLog(
    uint32_t    Unit,
    int16_t     StartAmplitudePerc,
    uint8_t     Divider);


/***********************************************************
  Function: R_SG_BreakEnable

  behavoiour on break (debug)
  writes to SVSTOP disable flag in EMU register
  
  
  Parameters:
  Unit              - Instance number
  Break             - Stop macro on or off
                      if true, macro will stop execution on break
                      if false, macro will continue execution on break

  Returns:
  see: <r_sg_Error_t>
*/

r_sg_Error_t R_SG_BreakEnable(
    uint32_t   Unit,
    uint32_t   BreakEnabled);


/***********************************************************
  Function: R_SG_SetIntLevel

  Sets interrupt threshold relatively to the max. PWM value (Fl)
  
  
  Parameters:
  Unit      - Instance number
  Level     - Interrupt level in relation to max. Pwm value
  

  Returns:
  see: <uint16_t>
*/

    
uint16_t R_SG_SetIntLevel(
    uint32_t    Unit, 
    uint8_t     Level);    

/***********************************************************
  Function: R_SG_SetInterruptThreshold

  When Amplitude in one of auto modes reaches this value,
  interrupt is activated!
  
  
  Parameters:
  Unit      - Instance number
  Threshold - Threshold value
  

  Returns:
  see: <eeSg_Error_t>
*/

r_sg_Error_t R_SG_SetInterruptThreshold(
    uint32_t    Unit, 
    uint16_t    Threshold);


/***********************************************************
  Function: R_SG_LinFade

  When Amplitude in one of auto modes reaches this value,
  interrupt is activated!
  
  
  Parameters:
  Unit                  - Instance number
  StartAmplitudePerc    - Threshold value
  Divider               - Divider
  Stepsize              - Stepsize  

  Returns:
  see: <eeSg_Error_t>
*/

r_sg_Error_t R_SG_LinFade(
    uint32_t    Unit,
    int16_t     StartAmplitudePerc,
    uint8_t     Divider,
    int16_t     Stepsize);
    
    
/***********************************************************
  Section: Sound Generator ISR Functions
*/



/***********************************************************
  Function: R_SG_SetIsrCallback

  Assign a user callback after the end of ADI, ADC, ALD. 
  
  The function will be called after the driver ISR has
  processed the interrupt.

  Parameters:
  Unit      - Instance number
  Isr       - Isr function pointer
  
  Returns:
  see: <eeSg_Error_t>
*/

r_sg_Error_t R_SG_SetIsrCallback(uint32_t Unit, 
                    r_sg_Interrupt_t Interrupt,     
                    void (*Isr)(void));
                                 

/***********************************************************
  Function: R_SG_Isr

  Interrupt Service Routine for SG interrupt.
  (There is only one: INTSGTI).
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_SG_Isr(uint32_t Unit);


/***********************************************************
  Function: R_SG_GetConfRegAddress

  Gives back the address of the conf-register
  
  Parameters:
  Unit       - SG instance number
  
  Returns:
  Address of SGnCONF
*/

uint32_t R_SG_GetConfRegAddress(uint32_t Unit);



#ifdef __cplusplus
}
#endif

#endif /* SG_API_H_  */

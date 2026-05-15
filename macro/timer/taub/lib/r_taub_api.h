/*
****************************************************************************
PROJECT : VLIB macro driver
FILE    : $Id: r_taub_api.h 3998 2014-09-29 17:10:47Z tobyas.hennig $
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

#ifndef TAUB_API_H_
#define TAUB_API_H_

/***********************************************************
  Title: TAUB API

  An application using the TAUB should include this header file.

  General information:
  The TAUB macro provides comprehensive timer functionalty for devices.
  There are 16 channels which can be set to a variety of modes including:
    - Interval Timer mode
    - Judge mode
    - Capture mode
    - Event Count mode
    - One Count mode
    - RFU (Trigger Start mode)
    - Capture & One Count mode
    - Judge & One Count mode
    - RFU (Interval One Count mode)
    - Up Down Count mode
    - Pulse One Count mode
    - Count Capture mode
    - Gate Count mode
    - Capture & Gate Count mode

    The are also 4 independent clocks available which can be set to a variety of
    frequencies and each timer channel can be assigned to one of these.

    The channels can also be linked in a series of master and slave
    configurations i.e. allowing the output of one channel to trigger
    another.

*/

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: TAUB Driver API 
  
  An application using TAUB should only need to include this r_taub_api.h
*/

/*******************************************************************************
  Section: Global Constants 
*/


/*******************************************************************************
  Constant:  R_TAUB_VERSION_HI and R_TAUB_VERSION_LO

  Driver version information  
*/

#define R_TAUB_VERSION_HI 0
#define R_TAUB_VERSION_LO 1

/*******************************************************************************
  Section: Global Types
*/

/* option to use SVSTOP */
#define USE_SVSTOP        (0x0000)
#define DO_NOT_USE_SVSTOP (0x0080)

#define SVSTOP            (USE_SVSTOP)

/*******************************************************************************
  Enum: r_taub_Error_t

  TAUB macro driver error code.
  
  If an error occures these enums give information about the
  reason.

  Values:
  R_TAUB_ERR_OK           - No error
  R_TAUB_ERR_NG           - Unspecified error
  R_TAUB_ERR_INSTANCE     - Unit, instance not supported

*/

typedef enum
{
    R_TAUB_ERR_OK         = 0x00,
    R_TAUB_ERR_NG         ,
    R_TAUB_ERR_RANGE      ,
    R_TAUB_ERR_INSTANCE   ,
    R_TAUB_ERR_LOCKED     ,
    R_TAUB_ERR_NOTLOCKED  ,
    R_TAUB_ERR_REG_UPDATE_FAIL
} r_taub_Error_t;

/*******************************************************************************
  Enum: r_taub_Interrupt_t

  Taub macro interrupt type. 
  
  List of all available interrupts for this macro.

  Values:
  R_TAUB_INT_CH0        - Interrupt channel #0
  R_TAUB_INT_CH1        - Interrupt channel #1
  R_TAUB_INT_CH2        - Interrupt channel #2
  R_TAUB_INT_CH3        - Interrupt channel #3
  R_TAUB_INT_CH4        - Interrupt channel #4
  R_TAUB_INT_CH5        - Interrupt channel #5
  R_TAUB_INT_CH6        - Interrupt channel #6
  R_TAUB_INT_CH7        - Interrupt channel #7
  R_TAUB_INT_CH8        - Interrupt channel #8
  R_TAUB_INT_CH9        - Interrupt channel #9
  R_TAUB_INT_CH10       - Interrupt channel #10
  R_TAUB_INT_CH11       - Interrupt channel #11
  R_TAUB_INT_CH12       - Interrupt channel #12
  R_TAUB_INT_CH13       - Interrupt channel #13
  R_TAUB_INT_CH14       - Interrupt channel #14
  R_TAUB_INT_CH15       - Interrupt channel #15
  R_TAUB_INT_NUM        - Max number of interrupt channels, delimiter
*/

typedef enum
{
    R_TAUB_INT_CH0 = 0,
    R_TAUB_INT_CH1,
    R_TAUB_INT_CH2,
    R_TAUB_INT_CH3,
    R_TAUB_INT_CH4,
    R_TAUB_INT_CH5,
    R_TAUB_INT_CH6,
    R_TAUB_INT_CH7,
    R_TAUB_INT_CH8,
    R_TAUB_INT_CH9,
    R_TAUB_INT_CH10,
    R_TAUB_INT_CH11,
    R_TAUB_INT_CH12,
    R_TAUB_INT_CH13,
    R_TAUB_INT_CH14,
    R_TAUB_INT_CH15,
    R_TAUB_INT_NUM   /* delimiter */
} r_taub_Interrupt_t;


/***********************************************************
  Enum: r_taub_Mode_t

  TAUB macro mode type, this set the mode for each channel.

  Values:
    R_TAUB_INTERVAL_TIMER_MODE             - interval mode
    R_TAUB_JUDGE_MODE                      - judge mode
    R_TAUB_CAPTURE_MODE                    - capture mode
    R_TAUB_EVENT_COUNT_MODE                - event triggered count mode
    R_TAUB_ONE_COUNT_MODE                  - one count mode
    R_TAUB_RFU_TRIGGER_START_MODE          - RFU trigger start mode
    R_TAUB_CAPTURE_ONE_COUNT_MODE          - Capture one count mode
    R_TAUB_JUDGE_ONE_COUNT_MODE            - Judge one count mode
    R_TAUB_RFU_INTERVAL_ONE_COUNT_MODE     - RFU interval one count mode
    R_TAUB_UP_DOWN_COUNT_MODE              - Up-Down count mode
    R_TAUB_PULSE_ONE_COUNT_MODE            - Pulse one count mode
    R_TAUB_COUNT_CAPTURE_MODE              - count capture mode
    R_TAUB_GATE_COUNT_MODE                 - gate count mode
    R_TAUB_CAPTURE_GATE_COUNT_MODE         - capture gate count mode
*/

typedef enum {
    R_TAUB_INTERVAL_TIMER_MODE = 0,
    R_TAUB_JUDGE_MODE,
    R_TAUB_CAPTURE_MODE,
    R_TAUB_EVENT_COUNT_MODE,
    R_TAUB_ONE_COUNT_MODE,
    /* R_TAUB_RFU_TRIGGER_START_MODE, */
    R_TAUB_CAPTURE_ONE_COUNT_MODE = 6,
    R_TAUB_JUDGE_ONE_COUNT_MODE,
    /* R_TAUB_RFU_INTERVAL_ONE_COUNT_MODE, */
    R_TAUB_UP_DOWN_COUNT_MODE = 9,
    R_TAUB_PULSE_ONE_COUNT_MODE,
    R_TAUB_COUNT_CAPTURE_MODE,
    R_TAUB_GATE_COUNT_MODE,
    R_TAUB_CAPTURE_GATE_COUNT_MODE
} r_taub_Mode_t;

/***********************************************************
  Enum: r_taub_Trigger_t

  TAUB macro trigger type, this set the trigger source for each channel.

  Values:
    R_TAUB_NONE                   - Valid only in software trigger start (Deselect other trigger sources)
    R_TAUB_VALID_EDGE_TIN         - Selects the valid-edge-detected TINn input signal. (Select the TINn
                                     valid edge, using CMURn:TIS1 and TIS0
    R_TAUB_VALID_EITHER_TIN       - Selects the valid-edge-detected TINn input signal as a start signal, and the
                                     reverse-edge-detected TINn input signal as a stop signal (capture signal)
                                     (Sets both rising and falling edges as the valid edge using CMURn:TIS1 and TIS0
    R_TAUB_START_TRIG_FROM_MASTER - RFU (Selects the start trigger signal of the master channel

    R_TAUB_INT_FROM_MASTER        - Selects the INTn output signal of the master channel
    R_TAUB_INT_FROM_UPPER         - Selects the INTn output signal of the upper channel, regardless of the
                                     setting of the master
    R_TAUB_TOUT_DEAD_TIME         - Selects the dead-time control trigger signal of the TOUTn generation unit
    R_TAUB_UP_DOWN_FROM_MASTER    - Selects the up/down control trigger signal of the master channel
*/

typedef enum {
    R_TAUB_NONE = 0,
    R_TAUB_VALID_EDGE_TIN = 1,
    R_TAUB_VALID_EITHER_TIN = 2,
    /* R_TAUB_START_TRIG_FROM_MASTER = 3, */
    R_TAUB_INT_FROM_MASTER = 4,
    R_TAUB_INT_FROM_UPPER = 5,
    R_TAUB_TOUT_DEAD_TIME = 6,
    R_TAUB_UP_DOWN_FROM_MASTER = 7
} r_taub_Trigger_t;

/***********************************************************
  Enum: r_taub_TimeInterval_t

  TAUB macro time interval selection for the CK0-3.  The period
  depends on the clock freguency selected externally and will
  further divide the frequency by the factor below.

  i.e. assume clock is 100MHz externally
         100MHz / 2^9  about 195.3KHz about 5.12us
         100MHz / 2^16 about 1.525KHz about 0.6ms

  Configuration parameter for  interval.

  Values:

  R_TAUB_TIME_INTERVAL_0   - time interval 2^0
  R_TAUB_TIME_INTERVAL_1   - time interval 2^1
  R_TAUB_TIME_INTERVAL_2   - time interval 2^2
  R_TAUB_TIME_INTERVAL_3   - time interval 2^3
  R_TAUB_TIME_INTERVAL_4   - time interval 2^4
  R_TAUB_TIME_INTERVAL_5   - time interval 2^5
  R_TAUB_TIME_INTERVAL_6   - time interval 2^6
  R_TAUB_TIME_INTERVAL_7   - time interval 2^7
  R_TAUB_TIME_INTERVAL_8   - time interval 2^8
  R_TAUB_TIME_INTERVAL_9   - time interval 2^9
  R_TAUB_TIME_INTERVAL_10  - time interval 2^10
  R_TAUB_TIME_INTERVAL_11  - time interval 2^11
  R_TAUB_TIME_INTERVAL_12  - time interval 2^12
  R_TAUB_TIME_INTERVAL_13  - time interval 2^13
  R_TAUB_TIME_INTERVAL_14  - time interval 2^14
  R_TAUB_TIME_INTERVAL_15  - time interval 2^15
*/

typedef enum {
    R_TAUB_TIME_INTERVAL_0 = 0,
    R_TAUB_TIME_INTERVAL_1,
    R_TAUB_TIME_INTERVAL_2,
    R_TAUB_TIME_INTERVAL_3,
    R_TAUB_TIME_INTERVAL_4,
    R_TAUB_TIME_INTERVAL_5,
    R_TAUB_TIME_INTERVAL_6,
    R_TAUB_TIME_INTERVAL_7,
    R_TAUB_TIME_INTERVAL_8,
    R_TAUB_TIME_INTERVAL_9,
    R_TAUB_TIME_INTERVAL_10,
    R_TAUB_TIME_INTERVAL_11,
    R_TAUB_TIME_INTERVAL_12,
    R_TAUB_TIME_INTERVAL_13,
    R_TAUB_TIME_INTERVAL_14,
    R_TAUB_TIME_INTERVAL_15
} r_taub_TimeInterval_t;

/***********************************************************
  Enum: r_taub_Channel_t

  TAUB channel selcetion 0 - 15

  Values:
  R_TAUB_CHANNEL_0        - Channel 0
  R_TAUB_CHANNEL_1        - Channel 1
  R_TAUB_CHANNEL_2        - Channel 2
  R_TAUB_CHANNEL_3        - Channel 3
  R_TAUB_CHANNEL_4        - Channel 4
  R_TAUB_CHANNEL_5        - Channel 5
  R_TAUB_CHANNEL_6        - Channel 6
  R_TAUB_CHANNEL_7        - Channel 7
  R_TAUB_CHANNEL_8        - Channel 8
  R_TAUB_CHANNEL_9        - Channel 9
  R_TAUB_CHANNEL_10       - Channel 10
  R_TAUB_CHANNEL_11       - Channel 11
  R_TAUB_CHANNEL_12       - Channel 12
  R_TAUB_CHANNEL_13       - Channel 13
  R_TAUB_CHANNEL_14       - Channel 14
  R_TAUB_CHANNEL_15       - Channel 15

  R_TAUB_CHANNEL_ALL      - option to combine all the channels
*/

typedef enum {
  R_TAUB_CHANNEL_0 = 0,
  R_TAUB_CHANNEL_1,
  R_TAUB_CHANNEL_2,
  R_TAUB_CHANNEL_3,
  R_TAUB_CHANNEL_4,
  R_TAUB_CHANNEL_5,
  R_TAUB_CHANNEL_6,
  R_TAUB_CHANNEL_7,
  R_TAUB_CHANNEL_8,
  R_TAUB_CHANNEL_9,
  R_TAUB_CHANNEL_10,
  R_TAUB_CHANNEL_11,
  R_TAUB_CHANNEL_12,
  R_TAUB_CHANNEL_13,
  R_TAUB_CHANNEL_14,
  R_TAUB_CHANNEL_15,
  R_TAUB_CHANNEL_ALL
} r_taub_Channel_t;

/* 16 timer channels */
#define R_TAUB_NUM_CHANNELS (R_TAUB_CHANNEL_ALL)

/***********************************************************
  Struct: r_taub_DMA_Setup_t

  Stores the setup of the trigger factors in relation to the TAUB unit
  and channel
*/
typedef struct {
    uint8_t          TriggerNr;
    uint32_t         TauBUnit;
    uint32_t         TauBChannel;
}r_taub_DMA_Setup_t;


/***********************************************************
  Enum: r_taub_Clock_t

  TAUB clock selection 0 - 3

  Values:
  R_TAUB_CK_0        - CK0
  R_TAUB_CK_1        - CK1
  R_TAUB_CK_2        - CK2
  R_TAUB_CK_3        - CK3
*/

typedef enum {
  R_TAUB_CK_0 = 0,
  R_TAUB_CK_1,
  R_TAUB_CK_2,
  R_TAUB_CK_3
} r_taub_Clock_t;

/***********************************************************
  Enum: r_taub_CountClock_t

  TAUB count clock selection

  Values:
  R_TAUB_COUNT_CLK_PRESCALER   - prescaler
  R_TAUB_COUNT_CLK_TIN         - Tin
  R_TAUB_COUNT_CLK_RFU         - RFU
  R_TAUB_COUNT_CLK_MASTER_INT  - Master Int
*/

typedef enum {
  R_TAUB_COUNT_CLK_PRESCALER = 0,
  R_TAUB_COUNT_CLK_TIN
  /* R_TAUB_COUNT_CLK_RFU, */
  /* R_TAUB_COUNT_CLK_MASTER_INT */
} r_taub_CountClock_t;


/***********************************************************
  Enum: r_taub_Dma_Register_t

  TAUB DMA window register selection 0 - 7

  Values:
  R_TAUB_DMA_REG_0   - DMA reg 0
  R_TAUB_DMA_REG_1   - DMA reg 1
  R_TAUB_DMA_REG_2   - DMA reg 2
  R_TAUB_DMA_REG_3   - DMA reg 3
  R_TAUB_DMA_REG_4   - DMA reg 4
  R_TAUB_DMA_REG_5   - DMA reg 5
  R_TAUB_DMA_REG_6   - DMA reg 6
  ..                  - ...
  R_TAUB_DMA_REG_15  - DMA reg 15
*/

typedef enum {
  R_TAUB_DMA_REG_0 = 0,
  R_TAUB_DMA_REG_1,
  R_TAUB_DMA_REG_2,
  R_TAUB_DMA_REG_3,
  R_TAUB_DMA_REG_4,
  R_TAUB_DMA_REG_5,
  R_TAUB_DMA_REG_6,
  R_TAUB_DMA_REG_7,
  R_TAUB_DMA_REG_8,
  R_TAUB_DMA_REG_9,
  R_TAUB_DMA_REG_10,
  R_TAUB_DMA_REG_11,
  R_TAUB_DMA_REG_12,
  R_TAUB_DMA_REG_13,
  R_TAUB_DMA_REG_14,
  R_TAUB_DMA_REG_15
}r_taub_Dma_Register_t;

/***********************************************************
  Enum: r_taub_TinEdge_t

  TAUB TIN edge selection

  Values:

*/

typedef enum {
  R_TAUB_FALLING_EDGE = 0,
  R_TAUB_RISING_EDGE,
  R_TAUB_BOTH_LOW_WIDTH,
  R_TAUB_BOTH_HIGH_WIDTH
} r_taub_TinEdge_t;


/***********************************************************
  Enum: r_taub_Overflow_t

  TAUB TIN edge selection

  Values:

  R_TAUB_OVERFLOW_OPTION_0 - Captures the OVF and counter values upon detection of a valid edge of capture input.

  R_TAUB_OVERFLOW_OPTION_1 - Sets OVF when the counter overflows (FFFFH ? 0000H).
                     Captures the counter value upon detection of a valid edge of capture input.

  R_TAUB_OVERFLOW_OPTION_2 - Captures the OVF value upon detection of a valid edge of capture input.
                     Captures FFFFH and stores it to CDRn when the counter overflows (FFFFH ? 0000H).
                     The valid edge of the capture input detected the next time is ignored.

  R_TAUB_OVERFLOW_OPTION_3 - Sets OVF when the counter overflows (FFFFH ? 0000H) and captures FFFFH and stores it
                     to CDRn.
                     The valid edge of the capture input detected the next time is ignored.
*/

typedef enum {
  R_TAUB_OVERFLOW_OPTION_0 = 0,
  R_TAUB_OVERFLOW_OPTION_1,
  R_TAUB_OVERFLOW_OPTION_2,
  R_TAUB_OVERFLOW_OPTION_3
} r_taub_Overflow_t;

/***********************************************************
  Enum: r_taub_SimultaneousTrigger_t

  TAUB TIN edge selection

  Values:
  R_TAUB_FROM_MASTER - Controls simultaneous rewrite of the data register by
                        using the signal of master channel
  R_TAUB_FROM_UPPER - Controls simultaneous rewrite of the data register by
                       using the signal of another upper channel
*/

typedef enum {
  R_TAUB_FROM_MASTER = 0,
  R_TAUB_FROM_UPPER
} r_taub_SimultaneousTrigger_t;


/***********************************************************
  Enum: r_taub_OutputConfig_t

  TAUB TIN edge selection

  Values:

  R_TAUB_OPERATION_MODE_1 - Operation mode 1
  R_TAUB_OPERATION_MODE_2 - Operation mode 2

Note:
  - R_TAUB_OPERATION_MODE_1 with TOM = 0
    R_TAUB_TOGGLE_MODE - Performs toggle operation when INTn occurs.

  - R_TAUB_OPERATION_MODE_2 with TOM = 0
    R_TAUB_SET_RESET   - Set/reset mode Set when INTn occurs upon count
                         start and reset when INTn occurs due to detection
                         of a match between CNTn and CDRn.

  - R_TAUB_OPERATION_MODE_1 with TOM = 1
    R_TAUB_SYNC_MD1    - Synchronous channel operation mode 1 Set when INTn
                         occurs on the master channel and reset when INTn
                         occurs on the slave channel.

  - R_TAUB_OPERATION_MODE_2 with TOM = 1
    R_TAUB_SYNC_MD2    - Synchronous channel operation mode 2 Set when INTn
                         occurs in the count-down status and reset when INTn
                         occurs in the count-up status.
*/

typedef enum {
  R_TAUB_OPERATION_MODE_1 = 0,
  R_TAUB_OPERATION_MODE_2
} r_taub_OutputConfig_t;


typedef enum {
    R_TAUB_TAUBnIm  = 0,
    R_TAUB_URTE0RX  = 1,
    R_TAUB_URTE1RX  = 2,
    R_TAUB_URTE2RX  = 3,
    R_TAUB_URTE3RX  = 4,
    R_TAUB_URTE4RX  = 5,
    R_TAUB_URTE5RX  = 6,
    R_TAUB_URTE6RX  = 7,
    R_TAUB_URTE7RX  = 8,
} r_taub_InputCaptureSignal;


/***********************************************************
  Function: r_taub_SetClockPeriod

  Set the timer period for one of the four channel clocks CK0-3.

  Parameters:
  Unit         - Instance number
  int          - TAUB clock <r_taub_Clock_t>
  TimeInterval - TAUB time interval <r_taub_TimeInterval_t>

  Returns:
  see: <r_taub_Error_t>
*/

r_taub_Error_t r_taub_SetClockPeriod(uint32_t Unit, r_taub_Clock_t Clock, r_taub_TimeInterval_t TimeInterval);


/***********************************************************
  Function: r_taub_SetClkSource

  Set the clock source for a TAUB channel can be either CK0-3.

  Parameters:
  Unit       - Instance number
  Channel    - TAUB channel number <r_taub_Channel_t>
  Clock      - TAUB clock <r_taub_Clock_t>

  Returns:
  see: <r_taub_Error_t>
*/

r_taub_Error_t r_taub_SetClkSource(uint32_t Unit, r_taub_Channel_t Channel, r_taub_Clock_t Clock);


/***********************************************************
  Function: r_taub_SetMode

  Set the mode for a TAUB channel 0-15.

  Parameters:
  Unit       - Instance number
  Channel    - TAUB channel number <r_taub_Channel_t>
  Mode       - TAUB Mode <r_taub_Mode_t>

   Returns:
  see: <r_taub_Error_t>
*/

r_taub_Error_t r_taub_SetMode (uint32_t Unit, r_taub_Channel_t Channel, r_taub_Mode_t Mode);


/***********************************************************
  Function: r_taub_EnableIntAtStartUp

  Enables the INT at startup, bit MD0 in the CMOR.

  Parameters:
  Unit         - Instance number
  Channel      - TAUB channel number <r_taub_Channel_t>
  IntAtStartUp - TRUE enable int at start up, FALSE do not

   Returns:
  see: <r_taub_Error_t>
*/

r_taub_Error_t r_taub_EnableIntAtStartUp(uint32_t Unit, r_taub_Channel_t Channel, uint8_t IntAtStartUp);


/***********************************************************
  Function: r_taub_SetMaster

  Set the mode for a TAUB channel 0-15.

  Parameters:
  Unit       - Instance number
  Channel    - TAUB channel number <r_taub_Channel_t>
  Master     - Sets either a Master channel (TRUE) or Slave (FALSE)

   Returns:
  see: <r_taub_Error_t>
*/

r_taub_Error_t r_taub_SetMaster(uint32_t Unit, r_taub_Channel_t Channel, uint8_t Master);


/***********************************************************
  Function: r_taub_SetTrigger

  Set the trigger source for a TAUB channel 0-15.

  Parameters:
  Unit       - Instance number
  Channel    - TAUB channel number <r_taub_Channel_t>
  Trigger    - see <r_taub_Trigger_t>

   Returns:
  see: <r_taub_Error_t>
*/

r_taub_Error_t r_taub_SetTrigger(uint32_t Unit, r_taub_Channel_t Channel, r_taub_Trigger_t Trigger);


/***********************************************************
  Function: r_taub_ClearOverflow

  Clear the overflow flag for a channel.

  Parameters:
  Unit       - Instance number
  Channel    - TAUB channel number <r_taub_Channel_t>

   Returns:
  see: <r_taub_Error_t>
*/

r_taub_Error_t r_taub_ClearOverflow(uint32_t Unit, r_taub_Channel_t Channel);


/***********************************************************
  Function: r_taub_EnableSimultaneousRewrite

  Set the trigger source for a TAUB channel 0-15.

  Parameters:
  Unit       - Instance number
  Channel    - TAUB channel number <r_taub_Channel_t>
  SimultaneousTriggerSource - see <r_taub_SimultaneousTrigger_t>

   Returns:
  see: <r_taub_Error_t>
*/

r_taub_Error_t r_taub_EnableSimultaneousRewrite(uint32_t Unit, r_taub_Channel_t Channel, r_taub_SimultaneousTrigger_t SimultaneousTriggerSource);


/***********************************************************
  Function: r_taub_EnableSynchronousChannelOperation

  Enables Synchronous Channel Operation for a TAUB channel 0-15.

  Parameters:
  Unit       - Instance number
  Channel    - TAUB channel number <r_taub_Channel_t>


   Returns:
  see: <eeTauB_Error_t>
*/

r_taub_Error_t r_taub_EnableSynchronousChannelOperation(uint32_t Unit, r_taub_Channel_t Channel);


/***********************************************************
  Function: r_taub_SetCountClock

  Set the count clock source for a TAUB channel 0-15.

  Parameters:
  Unit       - Instance number
  Channel    - TAUB channel number <r_taub_Channel_t>
  CountClock - see <r_taub_CountClock_t>

   Returns:
  see: <r_taub_Error_t>
*/

r_taub_Error_t r_taub_SetCountClock(uint32_t Unit, r_taub_Channel_t Channel, r_taub_CountClock_t CountClock);


/***********************************************************
  Function: r_taub_SetTinEdge

  Set the edge for the TIN detection TAUB channel 0-15.

  Parameters:
  Unit       - Instance number
  Channel    - TAUB channel number <r_taub_Channel_t>
  CountClock - see <r_taub_TinEdge_t>

   Returns:
  see: <r_taub_Error_t>
*/

r_taub_Error_t r_taub_SetTinEdge(uint32_t Unit, r_taub_Channel_t Channel, r_taub_TinEdge_t Edge);


/***********************************************************
  Function: r_taub_SetPeriod

  Set the period for the channel counter (used for the CDR register). 
  This is effectively a count value for the CDR register.

  Parameters:
  Unit       - Instance number
  Channel    - TAUB channel number <r_taub_Channel_t>
  Period     - TAUB count period

   Returns:
  see: <r_taub_Error_t>
*/

r_taub_Error_t r_taub_SetPeriod(uint32_t Unit, r_taub_Channel_t Channel, uint16_t Period);


/***********************************************************
  Function: r_taub_Start

  Starts the selected timer channel.

  Parameters:
  Unit       - Instance number
  Channel    - TAUB channel number <r_taub_Channel_t>

   Returns:
  see: <r_taub_Error_t>
*/

r_taub_Error_t r_taub_Start(uint32_t Unit, r_taub_Channel_t Channel);


/***********************************************************
  Function: r_taub_Stop

  Stops the selected timer channel.

  Parameters:
  Unit       - Instance number
  Channel    - TAUB channel number <r_taub_Channel_t>

   Returns:
  see: <r_taub_Error_t>
*/

r_taub_Error_t r_taub_Stop(uint32_t Unit, r_taub_Channel_t Channel);


/***********************************************************
  Function: r_taub_OutputEnable

  Enables the selected timer channel output enable pin for use
  with the count operation.

  Parameters:
  Unit       - Instance number
  Channel    - TAUB channel number <r_taub_Channel_t>

   Returns:
  see: <r_taub_Error_t>
*/

r_taub_Error_t r_taub_OutputEnable(uint32_t Unit, r_taub_Channel_t Channel);


/***********************************************************
  Function: r_taub_OutputDisable

  Disables the selected timer channel output enable pin for use
  with the count operation.

  Parameters:
  Unit       - Instance number
  Channel    - TAUB channel number <r_taub_Channel_t>

   Returns:
  see: <r_taub_Error_t>
*/

r_taub_Error_t r_taub_OutputDisable(uint32_t Unit, r_taub_Channel_t Channel);


/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_TAUB_GetVersionStr

  Get the driver version number in human readable form (string). 

  Parameters:
  void
  
  Returns:
  ReceiveString     - Pointer to receive string destination
  
*/

const int8_t * R_TAUB_GetVersionStr(void); 

/*******************************************************************************
  Function: R_TAUB_GetMajorVersion

  Get the driver major version number. 

  Parameters:
  void
  
  Returns:
     - Major version number
  
*/

const uint16_t R_TAUB_GetMajorVersion(void); 

/*******************************************************************************
  Function: R_TAUB_GetMinorVersion

  Get the driver minor version number. 

  Parameters:
  void
  
  Returns:
     - Minor version number
  
*/

const uint16_t R_TAUB_GetMinorVersion(void); 


/*******************************************************************************
  Function: R_TAUB_Init

  Driver init function.

  Parameters:
  Unit          - Macro instance number
 
  Returns:
  see: <r_taub_Error_t>
*/

 r_taub_Error_t R_TAUB_Init(uint32_t Unit);


/*******************************************************************************
  Function: R_TAUB_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Macro Instance number

  Returns:
  see: <r_taub_Error_t>
*/

 r_taub_Error_t R_TAUB_DeInit(uint32_t Unit);


/*******************************************************************************
  Group: TAUB Error and ISR Functions
*/

/*******************************************************************************
  Function: R_TAUB_SetErrorCallback

  Set a user function as error handler.
  
  When the driver detects any error, it will call this function.

  Parameters:
  ErrorCallback - Pointer to a user function
  
  Returns:
  void
*/

void R_TAUB_SetErrorCallback(void (*ErrorCallback )(uint32_t Unit, uint32_t Error));

/*******************************************************************************
  Function: R_TAUB_SetIsrCallback

  Assign a user callback for the given interrupt of a unit. 
  
  The function will be called after the driver ISR has
  processed the interrupt.

  Parameters:
  Unit          - Instance number
  Interrupt       - What interrupt,  see: <r_taub_Interrupt_t>
  Isr           - Isr function pointer
  
  Returns:
  see: <r_taub_Error_t>
*/

 r_taub_Error_t R_TAUB_SetIsrCallback(uint32_t Unit, r_taub_Interrupt_t Interrupt, 
                                 void (*Isr)(void));
                                 

/***********************************************************
  Function:  R_TAUB_Isr

  Driver interrupt service routine.

  This function shall be called by sevice interrupt handler

  Parameters:
  Unit       - Instance number
  Interrupt  - channel interrupt

  Returns:
  void
*/

void R_TAUB_Isr(uint32_t Unit, r_taub_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_TAUB_EnableInt

  Enable the TAUB interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_taub_Interrupt_t>
  
  Returns:
  void
*/

void R_TAUB_EnableInt(uint32_t Unit, r_taub_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_TAUB_DisableInt

  Disable the TAUB interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_taub_Interrupt_t>
  
  Returns:
  void
*/

void R_TAUB_DisableInt(uint32_t Unit, r_taub_Interrupt_t Interrupt);


uint16_t r_taub_GetCaptureData(uint32_t Unit, r_taub_Channel_t Channel) ;
#ifdef __cplusplus
}
#endif

#endif /* TAUB_API_H_  */

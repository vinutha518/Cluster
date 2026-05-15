/*
****************************************************************************
PROJECT : Device Driver 
FILE    : $Id: r_dev_sys.h 10 2018-12-03 11:36:39Z stoneridge $
============================================================================ 
DESCRIPTION
generic device API
============================================================================
                            C O P Y R I G H T                            
============================================================================
                           Copyright (c) 2017
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

#ifndef R_DEV_SYS_H_
#define R_DEV_SYS_H_

/*******************************************************************************
  Macro: R_DEV_WRITE_PORT_PROT_REG

  Write value to a protected port register.
  
  Protected port register sequence is special, because Port register protection
  does not read back from dedicated status register but from protected register itself.
  (see step 4 in D1LM UM v2.10 chapter 2.2.3.5 below figure 2.8 / or D1LM OPC item 23)
*/
#if defined (__ghs__)
#pragma ghs startnomisra
#endif /* __ghs */

#ifdef R_DBG_PRINT_DEV_REGS
#define R_DEV_WRITE_PORT_PROT_REG(PROT_ADDR, ADDR, VALUE) {\
    uint32_t new_val   = 0; \
  do { \
  *((volatile uint32_t*)(PROT_ADDR)) = (0x000000a5);\
  *((volatile uint32_t*)(ADDR))      = (VALUE);\
  *((volatile uint32_t*)(ADDR))      = (uint32_t)(~(uint32_t)(VALUE));\
  *((volatile uint32_t*)(ADDR))      = (VALUE);\
  new_val   = *((volatile uint32_t*)(ADDR)); \
} while (new_val != (VALUE));\
R_DBG_PRINT(R_DBG_MSG_INFO, "Writing protected port register with protection address " #ADDR); \
R_DBG_PrintRegCmd(32, (ADDR), (VALUE));}
#else
#define R_DEV_WRITE_PORT_PROT_REG(PROT_ADDR, ADDR, VALUE) {\
    uint32_t new_val   = 0; \
    do { \
  *((volatile uint32_t*)(PROT_ADDR)) = (0x000000a5);\
  *((volatile uint32_t*)(ADDR))      = (VALUE);\
  *((volatile uint32_t*)(ADDR))      = (uint32_t)(~(uint32_t)(VALUE));\
  *((volatile uint32_t*)(ADDR))      = (VALUE);\
  new_val   = *((volatile uint32_t*)(ADDR)); \
} while (new_val != (VALUE));}
#endif
#if defined (__ghs__)
#pragma ghs endnomisra
#endif


/*******************************************************************************
  Macro: R_DEV_WRITE_REG

  Write register.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_DEV_REGS
#define R_DEV_WRITE_REG(SIZE, ADDR, VALUE)  {R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
#define R_DEV_WRITE_REG(SIZE, ADDR, VALUE)  {(*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE)); }
#endif

/*******************************************************************************
  Macro: R_DEV_READ_REG

  Read register.
    SIZE allowed values are 8, 16, 32

*/

#ifdef R_DBG_PRINT_DEV_REGS
#define R_DEV_READ_REG(SIZE, ADDR)   (R_DBG_PrintRegRead##SIZE##((ADDR)))
#else
#define R_DEV_READ_REG(SIZE, ADDR)   ((*((volatile uint##SIZE##_t*)((ADDR)))))
#endif


/*******************************************************************************
  Enum: r_dev_Device_t

  Device list 

  Values:
    R_DEV_R7F7014OFFS         - Start Offset of D1LM family
    R_DEV_R7F701416           - R7F701416
    R_DEV_R7F701417           - R7F701417
    R_DEV_R7F701400           - R7F701400
    R_DEV_R7F701401           - D1L1
    R_DEV_R7F701402           - D1L2
    R_DEV_R7F701403           - D1L2H
    R_DEV_R7F701404           - D1M1_3.75M
    R_DEV_R7F701405           - D1M1_5M
    R_DEV_R7F701442           - D1M1-V2
    R_DEV_R7F701406           - D1M1H_3.75M
    R_DEV_R7F701407           - D1M1H_5M
    R_DEV_R7F701418           - R7F701418
    R_DEV_R7F701441           - D1M1A_4M
    R_DEV_R7F701408           - D1M2_3.75M
    R_DEV_R7F701410           - D1M2_5M
    R_DEV_R7F701411           - D1M2H_3.75M
    R_DEV_R7F701412           - D1M2H_5M
    R_DEV_R7F701460           - R7F701460
    R_DEV_R7F701444           - R7F701444
    R_DEV_UNKNOWN_DEVICE      - Unknown device
*/

typedef enum 
{
    R_DEV_UNKNOWN_DEVICE  ,
    R_DEV_R7F7014OFFS = 1400,
    R_DEV_R7F701400 = R_DEV_R7F7014OFFS,
    R_DEV_R7F701401       ,
    R_DEV_R7F701402       ,
    R_DEV_R7F701403       ,
    R_DEV_R7F701404       ,
    R_DEV_R7F701405       ,
    R_DEV_R7F701406       ,
    R_DEV_R7F701407       ,
    R_DEV_R7F701408       ,
    R_DEV_R7F701410 = 1410,
    R_DEV_R7F701411       ,
    R_DEV_R7F701412       ,
    R_DEV_R7F701416 = 1416,
    R_DEV_R7F701417 = 1417,
    R_DEV_R7F701418 = 1418,
    R_DEV_R7F701441 = 1441,
    R_DEV_R7F701442 = 1442,
    R_DEV_R7F701444 = 1444,
    R_DEV_R7F701460 = 1460,
    R_DEV_LAST_DX_DEV
} r_dev_Device_t;


/*******************************************************************************
  Title: CPU API

  The interface for all CPU related accesses
*/


/*******************************************************************************
  Section: Global Constants 
*/

/*******************************************************************************
  Constant: R_DEV_PIN_LAST

  Use as delimiter for pin config arrays 
  Set pin numer to R_DEV_PIN_LAST
  
*/
#define R_DEV_PIN_LAST 0xffu
/*******************************************************************************
  Section: Global Types
*/
/*******************************************************************************
  Enum: r_dev_PinDirection_t

  Direction modes of a IO pin

  Values:
    R_DEV_PIN_IN           - Set pin as input
    R_DEV_PIN_OUT          - Set pin as output
    R_DEV_PIN_DIRECT       - give direction control 
                             to the macro that is using the pin

*/
typedef enum
{
    R_DEV_PIN_OUT = 0,
    R_DEV_PIN_IN ,
    R_DEV_PIN_DIRECT
} r_dev_PinDirection_t;

/*******************************************************************************
  Enum: r_dev_PinPull_t

  Possible built in pull resistor settings

  Values:
    R_DEV_PIN_PULLNO        - no resistor connected
    R_DEV_PIN_PULLUP        - Connect pullup resistor
    R_DEV_PIN_PULLDOWN      - Connect pulldown resistor
*/

typedef enum
{
    R_DEV_PIN_PULLNO  = 0,
    R_DEV_PIN_PULLUP,
    R_DEV_PIN_PULLDOWN
} r_dev_PinPull_t;

/*******************************************************************************
  Enum: r_dev_PinInput_t
 
   Input characterstics
   
   Do not change the order of this enum!
   Values: 
    R_DEV_PIN_NONE          - no input characteristic selectable (default)
    R_DEV_PIN_CMOS1         - see datasheet
    R_DEV_PIN_SCHMITT2      - see datasheet 
    R_DEV_PIN_SCHMITT1      - see datasheet
    R_DEV_PIN_SCHMITT4      - see datasheet
    R_DEV_PIN_TTL           - see datasheet
    R_DEV_PIN_TTL_MLB       - see datasheet
*/  
typedef enum
{
    R_DEV_PIN_NONE   = 0,
    R_DEV_PIN_CMOS1,
    R_DEV_PIN_SCHMITT2,
    R_DEV_PIN_SCHMITT1,
    R_DEV_PIN_SCHMITT4,
    R_DEV_PIN_TTL,
    R_DEV_PIN_TTL_MLB
}  r_dev_PinInput_t;

/*******************************************************************************
  Type: r_dev_PortInputCapability_t
  
  The capability of input characteristics for a device port
  
  Values:
    Port                    -  port number
    InputTypeXX             -  input characteristics, see <r_dev_PinInput_t>
                               where XX stands for 00, 01, 10 and 11
                               and mean the PISA/PIS register settings
                                o 00 -> PISAn_m = 0 / PISn_m = 0
                                o 01 -> PISAn_m = 0 / PISn_m = 1
                                o 10 -> PISAn_m = 1 / PISn_m = 0
                                o 11 -> PISAn_m = 1 / PISn_m = 1
                               Refer also to D1LM UM table
                               "ports input buffer characteristics" (UM V0.5 table 2.30)
*/

typedef struct 
{
    uint8_t                 Port; 
    r_dev_PinInput_t        InputType11;
    r_dev_PinInput_t        InputType10;
    r_dev_PinInput_t        InputType01;
    r_dev_PinInput_t        InputType00;
}r_dev_PortInputCapability_t;


/*******************************************************************************
  Type: r_dev_PinConfig_t
  
  The comlplete configuration structure for a device pin 
  
  Values:
    Port                    -  port number
    Pin                     -  pin number
    Func                    -  pin function 0: GPIO, 1-4 alternative function
    Dir                     -  pin direction, see <r_dev_PinDirection_t>
    Feedback                -  1: physical (output) pin level provided to input register, 0: disabled
    Pull                    -  internal pull up configuration, see <r_dev_PinPull_t>
    OpenDrain               -  1: open drain emulation, 0: push-pull
    HiCurr                  -  1: high level current limitation, 0: low level current limitation 
    HiSpeed                 -  1: Speed limitation disabled (drive strength control off), 0: Speed limit enabled  
    InputType               -  possible types of input characteristics, see <r_dev_PinInput_t>
  
*/  

typedef struct 
{
    uint8_t                 Port; 
    uint8_t                 Pin;
    uint8_t                 Func;
    r_dev_PinDirection_t    Dir;
    uint8_t                 Feedback; 
    r_dev_PinPull_t         Pull;
    uint8_t                 OpenDrain; 
    uint8_t                 CurrLimit; 
    uint8_t                 HiSpeed; 
    uint8_t                 InputType; 
}r_dev_PinConfig_t; 


/*******************************************************************************
  Enum:  r_dev_PinRegs_t

  This enum is used as index for R_GPIO_Register_Table array

  Values:
    R_DEV_PIN_REG_PCR          - PCR   Port control register (pin based control)     R/W     32
    R_DEV_PIN_REG_P            - P     Port register                                 R/W     16
    R_DEV_PIN_REG_PSR          - PSR   Port Set Reset register                       R/W     32
    R_DEV_PIN_REG_PNOT         - PNOT  Port NOT register                               W     16
    R_DEV_PIN_REG_PPR          - PPR   Port Pin Read register                        R       16
    R_DEV_PIN_REG_PM           - PM    Port Mode register                            R/W     16
    R_DEV_PIN_REG_PMC          - PMC   Port Function Control register                R/W     16
    R_DEV_PIN_REG_PMSR         - PMSR  Port Mode register Set Reset                  R/W     32
    R_DEV_PIN_REG_PMCSR        - PMCSR Port Function Control register                R/W     32
    R_DEV_PIN_REG_PFC          - PFC   Port Function Control Expansion register      R/W     16
    R_DEV_PIN_REG_PFCE         - PFCE  Port Function Control Expansion register      R/W     16
    R_DEV_PIN_REG_PFCAE        - PFCAE Port Function Control Expansion register      R/W     16
    R_DEV_PIN_REG_PIBC         - PIBC  Port Input Buffer Control register            R/W     16
    R_DEV_PIN_REG_PBDC         - PBDC  Port Bi-Direction Control register            R/W     16
    R_DEV_PIN_REG_PIC          - PIC   Port IP control register                      R/W     16
    R_DEV_PIN_REG_PU           - PU    Pull-up option register                       R/W     16
    R_DEV_PIN_REG_PD           - PD    Pull-down option register                     R/W     16
    R_DEV_PIN_REG_PODC         - PODC  Port Open Drain Control register              R/W     16
    R_DEV_PIN_REG_PDSC         - PDSC  Port Drive Strength Control register          R/W     16
    R_DEV_PIN_REG_PICC         - PICC  Port Input Characteristic Control register    R/W     16
    R_DEV_PIN_REG_PILC         - PILC  Port Input Level Control register             R/W     16
    R_DEV_PIN_REG_INTR         - INTR  Rising Edge Detection register                R/W     16
    R_DEV_PIN_REG_INTF         - INTF  Falling Edge Detection register               R/W     16
    R_DEV_PIN_REG_PPCMD        - PPCMD Port Protection Command register              R/W     8
*/

typedef enum
{
    R_DEV_PIN_REG_PCR    = 0,
    R_DEV_PIN_REG_P      = 1,
    R_DEV_PIN_REG_PSR    = 2,
    R_DEV_PIN_REG_PPR    ,
    R_DEV_PIN_REG_PM     ,
    R_DEV_PIN_REG_PMC    ,
    R_DEV_PIN_REG_PFC    ,
    R_DEV_PIN_REG_PFCE   ,
    R_DEV_PIN_REG_PNOT   ,
    R_DEV_PIN_REG_PMSR   ,
    R_DEV_PIN_REG_PMCSR  ,
    R_DEV_PIN_REG_PFCAE  ,
    /*  OS  */
    R_DEV_PIN_REG_PIBC   ,
    R_DEV_PIN_REG_PBDC   ,
    R_DEV_PIN_REG_PIPC   ,
    R_DEV_PIN_REG_PU     ,
    R_DEV_PIN_REG_PD     ,
    R_DEV_PIN_REG_PODC   ,
    R_DEV_PIN_REG_PDSC   ,
    R_DEV_PIN_REG_PIS    ,
    R_DEV_PIN_REG_PISA   ,
    R_DEV_PIN_REG_PPCMD  ,
    R_DEV_PIN_REG_PPROTS ,
    /* delimiter - do not touch*/
    R_DEV_PIN_REG_LAST
}r_dev_PinRegs_t;




/*******************************************************************************
  Function: R_DEV_GetDev
  
  Function returns the device name. The regiters PRDNAME3 and PRDNAME2 are used 
  to "sortet out" the correct device  
  
  Parameters:
  void
  
  Returns:
  device name, see <r_dev_Device_t>   

*/
void R_DEV_PinOutStateSet(uint8_t State);
uint8_t R_DEV_PinOutStateGet(void);


r_dev_Device_t R_DEV_GetDev(void);

float32_t R_DEV_SQRTF(float32_t val);

/***********************************************************
  Function: _R_DEV_SyncP

  CPU pipeline sync (assembler routine)

  Parameters: none
  
*/
void R_DEV_SyncP(void);


void R_DEV_PinInit(const uint8_t PinGroup);



#ifdef __cplusplus
}
#endif

#endif /* R_DEV_SYS_H_  */

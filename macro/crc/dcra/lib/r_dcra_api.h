/*
****************************************************************************
PROJECT : VLIB macro driver
FILE    : $Id: r_dcra_api.h 7015 2015-12-10 14:24:52Z dennis.ostermann $
============================================================================ 
DESCRIPTION
Generic part of the DCRA macro driver
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

#ifndef DCRA_API_H_
#define DCRA_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: DCRA Driver API 
  
  An application using DCRA should only need to include this r_dcra_api.h
*/

#define DCRA_API_LOG                       (0)

#if DCRA_API_LOG
#define DCRA_API_PRINTF(...)               (void)printf(__VA_ARGS__)
#else
#define DCRA_API_PRINTF(...)               do{}while(0)
#endif

/*******************************************************************************
  Section: Global Constants 
*/


/*******************************************************************************
  Constant:  R_DCRA_VERSION_HI and R_DCRA_VERSION_LO

  Driver version information  
*/

#define R_DCRA_VERSION_HI 0
#define R_DCRA_VERSION_LO 1

/*******************************************************************************
  Section: Global Types
*/

/* Handle to open DCRA block */
typedef uint16_t r_dcra_handle_t;

/*******************************************************************************
  Enum: r_dcra_Error_t

  DCRA macro driver error code.
  
  If an error occures these enums give information about the
  reason.

  Values:
  R_DCRA_ERR_OK           - No error
  R_DCRA_ERR_NG           - Unspecified error
*/

typedef enum
{
    R_DCRA_ERR_OK               = 0x0,
    R_DCRA_ERR_NG               ,
    R_DCRA_ERR_RANGE            ,
    R_DCRA_ERR_LOCKED           ,
    R_DCRA_ERR_NOTLOCKED        ,
    R_DCRA_ERR_SYSTEM           ,
    R_DCRA_ERR_INVALID_HANDLE
} r_dcra_Error_t;


/*******************************************************************************
  Enum: r_dcra_Polynom_t

  DCRA polynom

  These settings are used as mask - don't change the defined values 

  Values:
    R_DCRA_POL_32ETH_POLY       - 32 bit ETHERNET polynom
    R_DCRA_POL_16CCIT_POLY      - 16 bit CCIT polynom
*/

typedef enum
{    
    R_DCRA_POL_32ETH_POLY       = 0x0u,
    R_DCRA_POL_16CCIT_POLY      = 0x1u
} r_dcra_Polynom_t;

/*******************************************************************************
  Enum: r_dcra_InputWidth_t

  DCRA data input bit width

  These settings are used as mask - don't change the defined values 

  Values:
    R_DCRA_ISZ_32BIT    - 32 bit input width
    R_DCRA_ISZ_16BIT    - 16 bit input width
    R_DCRA_ISZ_8BIT     -  8 bit input width
*/

typedef enum
{    
    R_DCRA_ISZ_32BIT    = 0x0u,
    R_DCRA_ISZ_16BIT    = 0x1u,
    R_DCRA_ISZ_8BIT     = 0x2u
} r_dcra_InputWidth_t;

/*******************************************************************************
  Type: r_dcra_Parameter_t

  DCRA macro configuration data

  Members:
  Polynom       - Polynom used see: <r_dcra_Polynom_t>
  CrcStartValue - Start value of the CRC.
  InputWidth    - Input bit width used see: <r_dcra_InputWidth_t> 
  
*/     

typedef struct { 
    r_dcra_Polynom_t        Polynom;
    uint32_t                CrcStartValue;
    r_dcra_InputWidth_t     InputWidth;
} r_dcra_Parameter_t;

/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_DCRA_GetVersionStr

  Get the driver version number in human readable form (string). 

  Parameters:
  void
  
  Returns:
  ReceiveString     - Pointer to receive string destination
  
*/

const int8_t * R_DCRA_GetVersionStr(void); 

/*******************************************************************************
  Function: R_DCRA_GetMajorVersion

  Get the driver major version number. 

  Parameters:
  void
  
  Returns:
     - Major version number
  
*/

const uint16_t R_DCRA_GetMajorVersion(void); 

/*******************************************************************************
  Function: R_DCRA_GetMinorVersion

  Get the driver minor version number. 

  Parameters:
  void
  
  Returns:
     - Minor version number
  
*/

const uint16_t R_DCRA_GetMinorVersion(void); 


/*******************************************************************************
  Function: R_DCRA_Init

  Driver init function.

  Parameters:
  Unit          - Macro instance number
  Config        - Macro configuration
 
  Returns:
  see: <r_dcra_Error_t>
*/

r_dcra_Error_t R_DCRA_Init(uint32_t Unit, r_dcra_Parameter_t *Config);


/*******************************************************************************
  Function: R_DCRA_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Macro Instance number

  Returns:
  see: <r_dcra_Error_t>
*/

r_dcra_Error_t R_DCRA_DeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_DCRA_SetStartValue

  Set start value of CRC.

  Parameters:
  Unit       - Macro Instance number
  Value      - Start value of CRC

  Returns:
  see: <r_dcra_Error_t>
*/

r_dcra_Error_t R_DCRA_SetStartValue(uint32_t Unit, uint32_t Value);


/*******************************************************************************
  Function: R_DCRA_Calculate

  Calculate CRC with next value.

  Parameters:
  Unit       - Macro Instance number
  Value      - value to calculate CRC for.

  Returns:
  see: <r_dcra_Error_t>
*/

r_dcra_Error_t R_DCRA_Calculate(uint32_t Unit, uint32_t Value);


/*******************************************************************************
  Function: R_DCRA_GetResult

  Get calculated result of CRC.

  Parameters:
  Unit       - Macro Instance number
  Value      - value to calculate CRC for.

  Returns:
  uint32_t value - calculation result of CRC.
*/

uint32_t R_DCRA_GetResult(uint32_t Unit);


/*******************************************************************************
  Group: DCRA Error (and ISR) Functions
*/

/*******************************************************************************
  Function: R_DCRA_SetErrorCallback

  Set a user function as error handler.
  
  When the driver detects any error, it will call this function.

  Parameters:
  ErrorCallback - Pointer to a user function
  
  Returns:
  void
*/

void R_DCRA_SetErrorCallback(void (*ErrorCallback )(uint32_t Unit, uint32_t Error));


#ifdef __cplusplus
}
#endif

#endif /* DCRA_API_H_  */

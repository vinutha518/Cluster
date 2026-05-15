/*
****************************************************************************
PROJECT : VLIB Macro Driver
FILE    : $Id: r_rtca_main.h 2385 2014-04-11 15:12:34Z michael.golczewski $
============================================================================
DESCRIPTION
Generic RTCA driver
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


#ifndef RTCA_MAIN_H_
#define RTCA_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Main Module Internal Interface

  Only the driver modules shall include this header.
*/

/*******************************************************************************
  Section: Global Defines
*/

#undef  DEF_EXTERN_
#ifndef RTCA_MAIN_GLOBALS_
#define DEF_EXTERN_ extern
#else
#define DEF_EXTERN_
#endif


/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Type: r_rtca_Dev_t

  Device instance specific data.

  All instance specific data is kept within that structure.

  Members:
  BasAddr       - Macro register base address
  Locked        - if set, the device is already initialised
  Settings      - Some settings  -  Just a placeholder/example -
  Config        - Some config    - don't need to use both
  IsrCallback   - Pointer table to Isr callback functions
*/

typedef struct
{
    uint32_t   BaseAddr;
    uint32_t   Settings;
    uint32_t   Config;
    void       (*IsrCallback[R_RTCA_INT_NUM])(void);
} r_rtca_Dev_t;


/*******************************************************************************
  Variable: r_rtca_Dev

  Array for device instance specific data.
*/

DEF_EXTERN_ r_rtca_Dev_t r_rtca_Dev[R_RTCA_MACRO_NUM];


/*******************************************************************************
  Function: R_RTCA_CheckMacroParams

  Performs a range check for standard macro parameters.

  For unused parameters fill in a 0 there

 Parameters:
  Unit          - Rtca Macro instance number

  Returns:
  see: <r_rtca_Error_t>

*/
r_rtca_Error_t R_RTCA_CheckMacroParams(uint32_t Unit);


/*******************************************************************************
  Function: R_RTCA_ErrorHandler

  Central error function.

  When the driver detects any error, it will call this function.
  In case there is a registered error callback user function,
  it  will be called. If a user handler is missing an endless loop is entered.

  Parameters:
  Unit       - Instance number
  Error      - Error code, see: <r_rtca_Error_t>

  Returns:
  void
*/

void R_RTCA_ErrorHandler(uint32_t Unit, r_rtca_Error_t Error);


#ifdef __cplusplus
}
#endif

#endif /* RTCA_MAIN_H_  */

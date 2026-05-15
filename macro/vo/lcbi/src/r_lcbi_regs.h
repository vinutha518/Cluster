/*
****************************************************************************
PROJECT : LCBI driver
FILE    : $Id: r_lcbi_regs.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Register declaration for LCBI macro driver 
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

#ifndef LCBI_REGS_H_
#define LCBI_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: LCBI Register Offsets

  Only the driver modules shall include this header.
*/

/*******************************************************************************
  Section: Global Defines
*/

/***********************************************************
  Constants: 

  R_LCBI_CDATA_BASE    - Base Adress of the CLUT
*/

#define R_LCBI_CDATA_BASE    0x000


/***********************************************************
  Constants: Access Registers (offsets)

  R_LCBI_DATA_A0S_W8    - Write 8-bit with A0 set
  R_LCBI_DATA_A0S_W16   - Write 16-bit with A0 set
  R_LCBI_DATA_A0S_W32   - Write 32-bit with A0 set
  R_LCBI_DATA_A0C_W8    - Write 8-bit with A0 clear
  R_LCBI_DATA_A0C_W16   - Write 16-bit with A0 clear
  R_LCBI_DATA_A0C_W32   - Write 32-bit with A0 clear
  R_LCBI_DATA_A0S_RT8   - Read 8-bit with A0 set and cycle start trigger
  R_LCBI_DATA_A0S_RT16  - Read 16-bit with A0 set and cycle start trigger
  R_LCBI_DATA_A0S_RT32  - Read 32-bit with A0 set and cycle start trigger
  R_LCBI_DATA_A0C_RT8   - Read 8-bit with A0 clear and cycle start trigger
  R_LCBI_DATA_A0C_RT16  - Read 16-bit with A0 clear and cycle start trigger
  R_LCBI_DATA_A0C_RT32  - Read 32-bit with A0 clear and cycle start trigger
  R_LCBI_DATA_R         - Read LCD Data Register
*/

#define R_LCBI_DATA_A0S_W8    0x400
#define R_LCBI_DATA_A0S_W16   0x404
#define R_LCBI_DATA_A0S_W32   0x408
#define R_LCBI_DATA_A0C_W8    0x40C
#define R_LCBI_DATA_A0C_W16   0x410
#define R_LCBI_DATA_A0C_W32   0x414
#define R_LCBI_DATA_A0S_RT8   0x418
#define R_LCBI_DATA_A0S_RT16  0x41C
#define R_LCBI_DATA_A0S_RT32  0x420
#define R_LCBI_DATA_A0C_RT8   0x424
#define R_LCBI_DATA_A0C_RT16  0x428
#define R_LCBI_DATA_A0C_RT32  0x42C
#define R_LCBI_DATA_R         0x430


/***********************************************************
  Constants: Control Registers (offsets)

  EE_LCDBUSIF_CKSEL      - Clock Selection
  EE_LCDBUSIF_DBGMOD     - Debug Mode
  EE_LCDBUSIF_SRESET     - Software Reset
  EE_LCDBUSIF_OPMODE     - Operation Mode
  EE_LCDBUSIF_CLUTOFFS   - CLUT Table 4-bit Offset
  EE_LCDBUSIF_TCONTROL   - Transfer Control Setting
  EE_LCDBUSIF_BCYCT      - Bus Cycle Type non-TFT
  EE_LCDBUSIF_BCYC       - non-TFT Bus Cycle Spec.
  EE_LCDBUSIF_TFTPRS     - TFT Operation Prescaler
  EE_LCDBUSIF_TFTCYC0    - TFT Cycle Specification (common phases)
  EE_LCDBUSIF_TFTCYC1    - TFT Cycle Specification (vertical phases)
  EE_LCDBUSIF_TFTCYC2    - TFT Cycle Specification (horizontal phases)
  EE_LCDBUSIF_OUTLEV     - Signal Output Level Spec.
  EE_LCDBUSIF_STATUS     - TC Status Information
  EE_LCDBUSIF_STATE      - TC and TG State Information
  EE_LCDBUSIF_IRQPEN     - Pending interrupt Register
  EE_LCDBUSIF_IRQCLR     - Pending Interrupt Clear Register 
*/

#define R_LCBI_CKSEL    0x440
#define R_LCBI_DBGMOD   0x444
#define R_LCBI_SRESET   0x448
#define R_LCBI_OPMODE   0x44C
#define R_LCBI_CLUTOFFS 0x450
#define R_LCBI_TCONTROL 0x454
#define R_LCBI_BCYCT    0x458
#define R_LCBI_BCYC     0x45C
#define R_LCBI_TFTPRS   0x460
#define R_LCBI_TFTCYC0  0x464
#define R_LCBI_TFTCYC1  0x468
#define R_LCBI_TFTCYC2  0x46C
#define R_LCBI_OUTLEV   0x470
#define R_LCBI_STATUS   0x474
#define R_LCBI_STATE    0x478
#define R_LCBI_IRQPEN   0x47C
#define R_LCBI_IRQCLR   0x480 


/*******************************************************************************
  Section: Global Types
  
  LCBI Register Definitions
*/
  
/***********************************************************
  Type: r_lcbi_RegOpmodeBits_t

  Content of the operation mode control register EE_LCDBIF_OPMODE

  Members:
  Opm_ode   - operation mode
  Opm_fast  - cycle speed
  Resn      - reserved (n=1,2)

*/

typedef struct
{
    uint32_t  Opm_ode  : 2;
    uint32_t  Res1     : 2;
    uint32_t  Opm_fast : 1;
    uint32_t Res2     : 27;
} r_lcbi_RegOpmodeBits_t;


/***********************************************************
  Union: r_lcbi_RegOpmode_t
  
  Simplifies Bit and double word access to register EE_LCDBIF_OPMODE  
  
  Members:
  Bits   - Allow bit access.
  DWord  - Allow access of the whole double word.
*/

typedef union
{
    r_lcbi_RegOpmodeBits_t Bits;
    uint32_t                 DWord;
} r_lcbi_RegOpmode_t;


/***********************************************************
  Type: r_lcbi_RegTconBits_t

  Content of the data transfer control register EE_LCDBIF_TCONTROL

  Members:
  Dtc_c  - Direct/Indirect transfer 
  Dtc_p  - 4-/8-bit palette for CLUT operation
  Dtc_m  - transfer mode (8-, 9-, 16-, 18-bit transfer mode)
  Dtc_s  - Bit shifting
  Resn    - Reserved (n=1,2)

*/

typedef struct
{
    uint32_t  Dtc_c : 1;
    uint32_t  Dtc_p : 1;
    uint32_t  Res1  : 2;
    uint32_t  Dtc_m : 2;
    uint32_t  Dtc_s : 2;
    uint32_t Res2  : 24;
} r_lcbi_RegTconBits_t;


/***********************************************************
  Union: r_lcbi_RegTcon_t
  
  Simplifies Bit and double word access to register EE_LCDBIF_TCONTROL  
  
  Members:
  Bits   - Allow bit access.
  DWord  - Allow access of the whole double word.
*/

typedef union
{
    r_lcbi_RegTconBits_t Bits;
    uint32_t               DWord;
} r_lcbi_RegTcon_t;


/***********************************************************
  Type: r_lcbi_RegBcycBits_t

  Content of the data transfer control register EE_LCDBIF_BCYC

  Members:
  Tiad      - timing spec. of non-TFT mode
  Tidz      - timing spec. of non-TFT mode
  Tidw      - timing spec. of non-TFT mode
  Tiwr_med  - timing spec. of non-TFT mode
  Res       - reserved
*/

typedef struct
{
    uint32_t Tiad     : 4;
    uint32_t Tidz     : 4;
    uint32_t Tidw     : 4;
    uint32_t Res      : 4;
    uint32_t Tiwr_med : 8;
    uint32_t Tird     : 4;
} r_lcbi_RegBcycBits_t;


/***********************************************************
  Union: r_lcbi_RegBcyc_t
  
  Simplifies Bit and double word access to register EE_LCDBIF_BCYC  
  
  Members:
  Bits  - Allow bit access.
  DWord  - Allow access of the whole double word.
*/

typedef union
{
    r_lcbi_RegBcycBits_t Bits;
    uint32_t               Dword;
} r_lcbi_RegBcyc_t;


/***********************************************************
  Type: r_lcbi_RegTftCyc0Bits_t

  Content of the data transfer control register EE_LCDBIF_TFTCYC0

  Members:
  Tfdhv  - Delay of active HSYNC to VSYNC
  Tfdch  - Delad of active pixel clock to HSYNC
  Tfdcd  - Delay of data to active pixel clock
  Tfdcw  - Width of active pixel clock
  Resn   - Reserved (n=1,2)

*/

typedef struct
{
    uint32_t Tfdhv    : 8;
    uint32_t Tfdch    : 8;
    uint32_t Tfdcd    : 7;
    uint32_t Res1     : 1;
    uint32_t Tfdcw    : 7;
    uint32_t Res2     : 1;
} r_lcbi_RegTftCyc0Bits_t;


/***********************************************************
  Union: r_lcbi_RegTftCyc0_t
  
  Simplifies Bit and double word access to register EE_LCDBIF_TFTCYC0  
  
  Members:
  Bits  - Allow bit access.
  DWord  - Allow access of the whole double word.
*/

typedef union
{
    r_lcbi_RegTftCyc0Bits_t Bits;
    uint32_t                  Dword;
} r_lcbi_RegTftCyc0_t;


/***********************************************************
  Type: r_lcbi_RegTftCyc1Bits_t

  Content of the data transfer control register EE_LCDBIF_TFTCYC1

  Members:
  Tfnhvs  - Width of VSYNC as a number of HSYNC pulses
  Tfnvb   - Vertical back porch time as a number of HSYNC pulses
  Tfnvc   - Amount of horizontal lines as a number of HSYNC pulses
  Tfnve   - Size of vertical end of frame as number of HSYNC pulses
*/

typedef struct
{
    uint32_t  Tfnhvs    : 8;
    uint32_t  Tfnvb     : 8;
    uint32_t Tfnvc     : 10;
    uint32_t  Tfnve     : 6;
} r_lcbi_RegTftCyc1Bits_t;


/***********************************************************
  Union: r_lcbi_RegTftCyc1_t
  
  Simplifies Bit and double word access to register EE_LCDBIF_TFTCYC1  
  
  Members:
  Bits  - Allow bit access.
  DWord  - Allow access of the whole double word.
*/

typedef union
{
    r_lcbi_RegTftCyc1Bits_t Bits;
    uint32_t                  Dword;
} r_lcbi_RegTftCyc1_t;


/***********************************************************
  Type: r_lcbi_RegTftCyc2Bits_t

  Content of the data transfer control register EE_LCDBIF_TFTCYC2

  Members:
  Tfnchs  - Width of HSYNC as a number of clocks
  Tfnhb   - Hertical back porch time as a number of clocks
  Tfnhc   - Amount of horizontal pixels as a number of clocks
  Tfnhe   - Size of horizontal end of line as number of clocks
*/

typedef struct
{
    uint32_t  Tfnchs    : 8;
    uint32_t  Tfnhb     : 8;
    uint32_t Tfnhc     : 10;
    uint32_t  Tfnhe     : 6;
} r_lcbi_RegTftCyc2Bits_t;


/***********************************************************
  Union: r_lcbi_RegTftCyc2_t
  
  Simplifies Bit and double word access to register EE_LCDBIF_TFTCYC2  
  
  Members:
  Bits  - Allow bit access.
  DWord  - Allow access of the whole double word.
*/

typedef union
{
    r_lcbi_RegTftCyc2Bits_t Bits;
    uint32_t                  Dword;
} r_lcbi_RegTftCyc2_t;


/***********************************************************
  Type: r_lcbi_RegStatusBits_t

  Content of the status register EE_LCDBIF_STATUS

  Members:
  Tc_Idle  - Idle
  Tc_Lock  - control register are locked/unlocked
  Tc_Rpg   - read access pending
  Res      - Reserved
*/

typedef struct
{
    uint32_t  Tc_Idle   : 1;
    uint32_t  Tc_Lock   : 1;
    uint32_t  Tc_Rpg    : 1;
    uint32_t Res       : 29;
} r_lcbi_RegStatusBits_t;


/***********************************************************
  Union: r_lcbi_RegStatus_t
  
  Simplifies Bit and double word access to register EE_LCDBIF_STATUS  
  
  Members:
  Bits  - Allow bit access.
  DWord  - Allow access of the whole double word.
*/

typedef union
{
    r_lcbi_RegStatusBits_t Bits;
    uint32_t                 DWord;
} r_lcbi_RegStatus_t;


/***********************************************************
  Type: r_lcbi_RegStateBits_t

  Content of the status register EE_LCDBIF_STATE

  Members:
  State   - internal state of LCDBIF FSM
  TftSeq  - TFT areas
  Fwp     - FIFO write pointer value
  Frp     - FIFO read pointer value
  Ffl     - FIFO fill level value
  Res1-5  - Reserved
*/

typedef struct
{
    uint32_t  State     : 3;
    uint32_t  Res1      : 1;
    uint32_t  TftSeq    : 3;
    uint32_t  Res2      : 1;
    uint32_t  Fwp       : 4;
    uint32_t  Res3      : 4;
    uint32_t  Frp       : 4;
    uint32_t  Res4      : 4;
    uint32_t  Ffl       : 4;
    uint32_t  Res5      : 4;
} r_lcbi_RegStateBits_t;


/***********************************************************
  Union: r_lcbi_RegState_t
  
  Simplifies Bit and double word access to register EE_LCDBIF_STATE  
  
  Members:
  Bits   - Allow bit access.
  DWord  - Allow access of the whole double word.
*/

typedef union
{
    r_lcbi_RegStateBits_t  Bits;
    uint32_t                 DWord;
} r_lcbi_RegState_t;



#ifdef __cplusplus
}
#endif

#endif /* LCBI_REGS_H_  */

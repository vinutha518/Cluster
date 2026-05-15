/*
****************************************************************************
PROJECT : CSIH driver
FILE    : $Id: r_csih_main.h 7165 2016-01-06 12:33:52Z matthias.nippert $
============================================================================ 
DESCRIPTION
Driver for CSIH macro
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

Warranty Disclaimer

Because the Product(s) is licensed free of charge, there is no warranty 
of any kind whatsoever and expressly disclaimed and excluded by Renesas, 
either expressed or implied, including but not limited to those for 
non-infringement of intellectual property, merchantability and/or 
fitness for the particular purpose. 
Renesas shall not have any obligation to maintain, service or provide bug 
fixes for the supplied Product(s) and/or the Application.

Each User is solely responsible for determining the appropriateness of 
using the Product(s) and assumes all risks associated with its exercise 
of rights under this Agreement, including, but not limited to the risks 
and costs of program errors, compliance with applicable laws, damage to 
or loss of data, programs or equipment, and unavailability or 
interruption of operations.

Limitation of Liability

In no event shall Renesas be liable to the User for any incidental, 
consequential, indirect, or punitive damage (including but not limited 
to lost profits) regardless of whether such liability is based on breach 
of contract, tort, strict liability, breach of warranties, failure of 
essential purpose or otherwise and even if advised of the possibility of 
such damages. Renesas shall not be liable for any services or products 
provided by third party vendors, developers or consultants identified or
referred to the User by Renesas in connection with the Product(s) and/or the 
Application.

****************************************************************************


*/


#ifndef  CSIH_MAIN_H_
#define  CSIH_MAIN_H_

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

#undef R_EXTERN
#ifndef  CSIH_MAIN_GLOBALS_
#define R_EXTERN extern 
#else
#define R_EXTERN
#endif


/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_csih_MemCfg_t

  Device instance specific data.
  All instance specific data is kept within that structure.

  Members:
  R_CSIH_MEM_CFG_NA        - Unit not configured
  R_CSIH_MEM_CFG_FIFO      - Unit configured in FIFO mode
  R_CSIH_MEM_CFG_TX_ONLY   - Unit configured in Tx-only buffer mode
  R_CSIH_MEM_CFG_DUAL      - Unit configured in dual buffer mode
  R_CSIH_MEM_CFG_DIR_AC    - Unit configured in direct access mode   
  
*/

typedef enum {
    R_CSIH_MEM_CFG_NA  = 0,
    R_CSIH_MEM_CFG_FIFO,
    R_CSIH_MEM_CFG_TX_ONLY,
    R_CSIH_MEM_CFG_DUAL,
    R_CSIH_MEM_CFG_DIR_AC
} r_csih_MemCfg_t;

/*******************************************************************************
  Enum: r_csih_CsT_t

  Device instance specific data.
  All instance specific data is kept within that structure.

  Members:
  R_CSIH_SETUP_T       - Setup timing selection
  R_CSIH_HOLD_T        - Hold timing selection
  R_CSIH_IDLE_T        - Idle timing selection
  R_CSIH_INTER_DATA_T  - Interdata timing selection
  
*/

typedef enum {
    R_CSIH_SETUP_T,
    R_CSIH_HOLD_T,
    R_CSIH_IDLE_T,
    R_CSIH_INTER_DATA_T
} r_csih_CsT_t;

/*******************************************************************************
  Type: r_csih_Dev_t

  Device instance specific data.
  All instance specific data is kept within that structure.

  Members:
  BaseAddr      -   Base address
  IsrCallback   -   Pointer to Isr callback functions
  RBuf          -   Ring Buffer structure
  Buf           -   Buffer
*/

typedef struct {
    uint32_t          BaseAddrCsih;
    uint32_t          BaseAddrChba;
    void            (*IsrCallback[R_CSIH_INT_NUM])(void);
    uint8_t           DataByteSize;
    uint8_t           DataBitSize;
    uint8_t           RxBuf[R_CSIH_RXBUF_BYTE_SIZE];
    uint8_t           TxBuf[R_CSIH_TXBUF_BYTE_SIZE];
    r_cdi_RBuf_t      RxRBuf;
    r_cdi_RBuf_t      TxRBuf;
    uint8_t           TxRemainBytes;
    uint8_t           OngoingBytes;
    uint8_t           TxOngoing;
    uint8_t           RxRemain;
    uint32_t          RxOnlyData;
    r_csih_CsSel_t    OngoingCs;
    r_csih_Mode_t     Mode;
    r_csih_TxRxMode_t TxRxMode;  
    r_csih_MemMode_t  MemCfg;
    uint8_t           DirAccessE;
    uint32_t          Unit;
#ifdef R_CSIH_VARIANT
    uint8_t           HwVariant;
#endif
} r_csih_Dev_t;


/*******************************************************************************
  Section: Global Variables
*/

/*******************************************************************************
  Variable: r_csih_Dev

  Array for device instance specific data.
*/

R_EXTERN r_csih_Dev_t r_csih_Dev[R_CSIH_MACRO_NUM];



/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_CSIH_ErrorHandler

  Central error function. 
  
  When the driver detects any error, it will call this function.
  In case there is a registered error callback user function, 
  it  will be called. If a user handler is missing an endless loop is entered. 
     
  Parameters:
  Unit       - Instance number
  Error      - Error code, see: <r_csih_Error_t>
  
  Returns:
  void
*/

void R_CSIH_ErrorHandler(uint32_t Unit, r_csih_Error_t Error);




#ifdef __cplusplus
}
#endif

#endif /* _CSIH_MAIN_H_  */

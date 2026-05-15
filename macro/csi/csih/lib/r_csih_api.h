/*
****************************************************************************
PROJECT : Csih driver
FILE    : $Id: r_csih_api.h 7165 2016-01-06 12:33:52Z matthias.nippert $
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

#ifndef CSIH_API_H_
#define CSIH_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: CSIH API

  An application using CSIH should include this header file.

*/

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_csih_Error_t

  CSIH macro driver error code.

  Values:
  R_CSIH_ERR_OK          - No error (0)
  R_CSIH_ERR_NG          - Unspecified error
  R_CSIH_ERR_PORTINIT    - CSIH port init failed
  R_CSIH_ERR_PARAM       - Wrong parameters were past
  R_CSIH_ERR_LAST        - Delimeter 
*/

typedef enum {
    R_CSIH_ERR_OK         = 0x00,
    R_CSIH_ERR_NG,
    R_CSIH_ERR_PARAM,
    R_CSIH_ERR_RBINIT,
    R_CSIH_ERR_BUFOVERRUN,
    R_CSIH_ERR_EDL_SIZE,
    R_CSIH_ERR_EDL,
    R_CSIH_ERR_NUM,
    R_CSIH_ERR_BUFO,
    R_CSIH_ERR_BUSY,
    R_CSIH_ERR_LAST
} r_csih_Error_t;



/*******************************************************************************
  Enum: r_csih_StatError_t

  CSIH macro status error codes.

  Values:
  OvrErr      - Error flow detected
  DcErr       - Data Consistency error
  PaErr       - Parity error
*/

typedef struct {
    uint8_t PaErr  : 1;
    uint8_t DcErr  : 1;
    uint8_t OvrErr : 1;
    uint8_t unused : 5; 
} r_csih_StatErrorBit_t;

typedef union {
    r_csih_StatErrorBit_t Bit;
    uint8_t Byte; 
} r_csih_StatError_t;



/*******************************************************************************
  Enum: r_csih_StatError_t

  CSIH macro status error codes.

  Values:
    R_CSIH_STATUS_OVE  - Overrun erro status
    R_CSIH_STATUS_OPE  - Parity error status
    R_CSIH_STATUS_DCE  - Data consistency error status
    R_CSIH_STATUS_EMF  - Empty FIFO status
    R_CSIH_STATUS_FLF  - Full FIFO status
    R_CSIH_STATUS_TSF  - Transmission status
    R_CSIH_STATUS_OFE  - Overflow error status
    R_CSIH_STATUS_TMOE - Time out error status
    R_CSIH_STATUS_ALL  - all statuses
*/
typedef enum {
    R_CSIH_STATUS_OVE  = 0,
    R_CSIH_STATUS_OPE  = 1,
    R_CSIH_STATUS_DCE  = 3,
    R_CSIH_STATUS_EMF  = 4,
    R_CSIH_STATUS_FLF  = 5,
    R_CSIH_STATUS_TSF  = 7,
    R_CSIH_STATUS_OFE  = 14,
    R_CSIH_STATUS_TMOE = 15,
    R_CSIH_STATUS_ALL
} r_csih_Status_t;


/*******************************************************************************
  Enum: r_csih_Interrupt_t

  CSIH macro interrupt type. List of all available interrupts for this macro

  Values:
    R_CSIH_INT_TIC         - Transmission interrupt
    R_CSIH_INT_TIR         - Reception interrupt
    R_CSIH_INT_TIJC        - Transmission Job interrupt
    R_CSIH_INT_TIRE        - Error interrupt
    R_CSIH_INT_NUM         - Delimeter 
*/

typedef enum {
    R_CSIH_INT_TIRE,
    R_CSIH_INT_TIR ,
    R_CSIH_INT_TIC ,
    R_CSIH_INT_TIJC,
    R_CSIH_INT_NUM
} r_csih_Interrupt_t;


/*******************************************************************************
  Enum: r_csih_Mode_t

  CSIH memory mode configuration.
  Configuration parameter for CSIH init. See: <r_csih_InitCfg_t>

  Values:
  R_CSIH_MASTER        - Master mode
  R_CSIH_SLAVE         - Slave mode

*/

typedef enum {    
    R_CSIH_MASTER,
    R_CSIH_SLAVE
} r_csih_Mode_t;

/*******************************************************************************
  Enum: r_csih_Parity_t

  Parity config for CS.
  Configuration parameter of CS for CSIH init. See: <r_csih_ChipSelCfg_t>

  Values:
    R_CSIH_PARITY_NONE     - No parity
    R_CSIH_PARITY_FIXED_0  - Extra bit fixed to 0 is added
    R_CSIH_PARITY_ODD      - Extra parity bit, Odd cfg
    R_CSIH_PARITY_EVEN     - Extra parity bit, Odd cfg
    R_CSIH_PARITY_NUM      - Delimiter
*/

typedef enum {    
    R_CSIH_PARITY_NONE     = 0,
    R_CSIH_PARITY_FIXED_0  = 1,
    R_CSIH_PARITY_ODD      = 2,
    R_CSIH_PARITY_EVEN     = 3,
    R_CSIH_PARITY_NUM
} r_csih_Parity_t;


/*******************************************************************************
  Enum: r_csih_MemMode_t

  CSIH memory mode configuration.
  Configuration parameter for CSIH init. See: <r_csih_InitCfg_t>

  Values:
  R_CSIH_MODE_FIFO         - Memory configured in FIFO mode
  R_CSIH_MODE_DUAL_BUF     - Memory configured in Dual-Buffer mode
  R_CSIH_MODE_TX_BUF       - Memory configured in Tx-only Buffer mode
  R_CSIH_MODE_NUM          - Max possible value

*/

typedef enum {    
    R_CSIH_MODE_FIFO = 0,
    R_CSIH_MODE_DUAL_BUF,
    R_CSIH_MODE_TX_BUF,
    R_CSIH_MODE_NUM
} r_csih_MemMode_t;


/*******************************************************************************
  Enum: r_csih_Dir_t

  Direction config for CS.
  Configuration parameter of CS for CSIH init. See: <r_csih_ChipSelCfg_t>

  MSB or LSB mode

  Values:
    R_CSIH_MSB    - Data is sent MSB first
    R_CSIH_LSB    - Data is sent LSB first
*/

typedef enum {    
    R_CSIH_MSB = 0,
    R_CSIH_LSB
} r_csih_Dir_t;


/*******************************************************************************
  Enum: r_csih_ClkPhase_t

  Clock Phase config for CS.
  Configuration parameter of CS for CSIH init. See: <r_csih_ChipSelCfg_t>

  Clock is active low or high

  Values:
    R_CSIH_CLK_ACTIVE_LOW  - Clock is active low (idle level is high)
    R_CSIH_CLK_ACTIVE_HIGH - Clock is active high (idle level is low)
*/

typedef enum {    
    R_CSIH_CLK_ACTIVE_LOW = 0,
    R_CSIH_CLK_ACTIVE_HIGH
} r_csih_ClkPhase_t;


/*******************************************************************************
  Enum: r_csih_DataPhase_t

  Clock Phase config for CS.
  Configuration parameter of CS for CSIH init. See: <r_csih_ChipSelCfg_t>

  Clock is active low or high

  Values:
    R_CSIH_DATA_PHASE_RISE - Data valid on rising clock edge
    R_CSIH_DATA_PHASE_FALL - Data valid on falling clock edge
*/

typedef enum {    
    R_CSIH_DATA_PHASE_RISE = 0,
    R_CSIH_DATA_PHASE_FALL
} r_csih_DataPhase_t;


/*******************************************************************************
  Enum: r_csih_CsLevel_t

  Active level of the CS.
  Configuration parameter of CS for CSIH init. See: <r_csih_ChipSelCfg_t>

  Clock is active low or high

  Values:
    R_CSIH_CS_ACTIVE_LOW
    R_CSIH_CS_ACTIVE_HIGH
*/

typedef enum {    
    R_CSIH_CS_ACTIVE_LOW = 0,
    R_CSIH_CS_ACTIVE_HIGH
} r_csih_CsLevel_t;


/*******************************************************************************
  Enum: r_csih_TxRxMode_t

  Tx-only, Rx-only or Tx and Rx configuration

  Values:
    R_CSIH_TX_ONLY   - Tx-only mode
    R_CSIH_RX_ONLY   - Rx-only mode
    R_CSIH_TX_RX     - Tx and Rx enabled mode
    R_CSIH_TX_RX_OFF - Tx and Rx disabled (idle/init state)
*/

typedef enum {
    R_CSIH_TX_ONLY,
    R_CSIH_RX_ONLY,
    R_CSIH_TX_RX,
    R_CSIH_TX_RX_OFF
} r_csih_TxRxMode_t;


/*******************************************************************************
  Type: r_csih_CsSelBit_t

  Selection of the CS for transmission and reception.
  See: <r_csih_CsSel_t>

  Values:
    Cs0 - Chip Select 0 is/was enabled for the communication
    Cs1 - Chip Select 1 is/was enabled for the communication
    Cs2 - Chip Select 2 is/was enabled for the communication
    Cs3 - Chip Select 3 is/was enabled for the communication
    Cs4 - Chip Select 4 is/was enabled for the communication
    Cs5 - Chip Select 5 is/was enabled for the communication
    Cs6 - Chip Select 6 is/was enabled for the communication
    Cs7 - Chip Select 7 is/was enabled for the communication
*/

typedef struct {    
    uint8_t Cs0 : 1;
    uint8_t Cs1 : 1;
    uint8_t Cs2 : 1;
    uint8_t Cs3 : 1;
    uint8_t Cs4 : 1;
    uint8_t Cs5 : 1;
    uint8_t Cs6 : 1;
    uint8_t Cs7 : 1;
} r_csih_CsSelBit_t;


/*******************************************************************************
  Type: r_csih_CsSel_t

  Selection of the CS for transmission and reception.

  Values:
    Bit     - Bitwise access to enable individual CS see: <r_csih_CsSelBit_t>
    Byte    - Byte access to fast configuration
*/

typedef union {    
    uint8_t             Byte;
    r_csih_CsSelBit_t   Bit;
} r_csih_CsSel_t;


/*******************************************************************************
  Type: r_csih_ChipSelCfg_t

  Csih macro Chip Select configuration data. See: <r_csih_InitCfg_t>

  Values:
    Chipselect    - Chip Select number (CSm)
    Parity        - Parity configuration see: <r_csih_Parity_t>
    DataLength    - Data Length config see: <r_csih_DataLength_t>
    Prescaler     - Additional prescaler see: <r_csih_CsPresc_t>
    RecessiveCfg  - Configuration is recessive if broadcast
    Direction     - LSB or MSB first see: <r_csih_Dir_t>
    ClockPhase    - Clock Phase Selection see: <r_csih_ClkPhase_t>
    DataPhase     - Data Phase selection see: <r_csih_DataPhase_t>
    ForceIdle     - Force Idle between each data or when CS changes see: <uint8_t>
    IdleTime      - Configure Idle time (CS inactive) in ns
    HoldTime      - Configure Hold time (CS inactive) in ns
    SetupTime     - Configure Setup time (CS inactive) in ns
    InterDataTime - Configure Setup time (CS inactive) in ns
    CsActiveLevel - Active level of the CS; see <r_csih_CsLevel_t>
  
*/     

typedef struct {
    uint8_t             Chipselect;
    r_csih_Parity_t     Parity;
    uint8_t             DataLength;
    uint32_t            Baudrate;
    uint8_t             RecessiveCfg;
    r_csih_Dir_t        Direction;
    r_csih_ClkPhase_t   ClockPhase;
    r_csih_DataPhase_t  DataPhase; 
    uint8_t             ForceIdle;
    uint16_t            IdleTime;
    uint16_t            HoldTime;
    uint16_t            SetupTime;
    uint16_t            InterDataTime;
    r_csih_CsLevel_t    CsActiveLevel;
} r_csih_ChipSelCfg_t;


/*******************************************************************************
  Type: r_csih_InitSlaveCfg_t

  Csih macro configuration data for slave mode

  Values:
    MemCfg        - CSIH Memory mode (FIFO etc..) see: <r_csih_MemMode_t>
    DataLength    - Data Length config see: <r_csih_DataLength_t>
    Direction     - LSB or MSB first see: <r_csih_Dir_t>
    ClockPhase    - Clock Phase Selection see: <r_csih_ClkPhase_t>
    DataPhase     - Data Phase selection see: <r_csih_DataPhase_t>
*/     

typedef struct {
    r_csih_MemMode_t   MemCfg;
    uint8_t            DataLength;
    r_csih_Dir_t       Direction;
    r_csih_ClkPhase_t  ClockPhase;
    r_csih_DataPhase_t DataPhase;
} r_csih_InitSlaveCfg_t;


/*******************************************************************************
  Type: r_csih_InitSlaveOptFeat_t

  Csih macro configuration data (slave mode only)

  Values:
    TimeOut         - Time out configuration for Rx data read (in SCK*8bit)
    Parity          - Parity enable see: <r_csih_Parity_t>
    TicNormalE      - TIC int. happens after transfer in direct access mode (cont trans)
    IntDelayedE     - Delay 1/2 SCLK the interrupt generation
    HandShakeE      - Hand shake feature enable
    SlaveSelE       - Slave Selection feature enable  
*/     

typedef struct {
    uint8_t         TimeOut;
    r_csih_Parity_t Parity;
    uint8_t       TicNormalE;
    uint8_t       IntDelayedE;
    uint8_t       HandShakeE;
    uint8_t       SlaveSelE;
} r_csih_InitSlaveOptFeat_t;


/*******************************************************************************
  Type: r_csih_InitCfg_t

  Csih macro configuration data (master mode only)

  Values:
    MemCfg        - CSIH Memory mode (FIFO etc..) see: <r_csih_MemMode_t>
    NbCs          - Number of used Chip Selects
    ChipSelCfg    - Table of Chip Select configuration see: <r_csih_ChipSelCfg_t>
*/     

typedef struct {
    r_csih_Mode_t        Mode;
    r_csih_MemMode_t     MemCfg;
    uint8_t              NbCs;
    r_csih_ChipSelCfg_t* ChipSelCfg;
} r_csih_InitCfg_t;


/*******************************************************************************
  Type: r_csih_ExtFunc_t

  CSIH macro driver error code (master mode only)

  Values:
    JobMode,       - Job mode enable (autosar & CIRE & TIJC interrupt)
    ExtDatLengE    - Extended data length feature enable
    DataConsCheckE - Data consistency check feature enable
    CsRetInact     - Chips Select return to inactive if no more data 
    TicNormal      - TIC int. happens after transfer in direct access mode (cont trans)
    IntDelayedE    - Delay 1/2 SCLK the interrupt generation
    HandShakeE     - Hand shake feature enable
    SlaveSelE      - Slave Selection enable (slave mode)
    TimeOut        - Time out Configuration (0 is off) (Slave mode)
  
*/

typedef struct {
    uint8_t JobMode; 
    uint8_t ExtDatLengE;
    uint8_t DataConsCheckE;
    uint8_t CsRetInact;
    uint8_t TicNormal;
    uint8_t IntDelayedE;
    uint8_t HandShakeE;
    uint8_t SlaveSelE;
    uint8_t   TimeOut;
} r_csih_ExtFunc_t;


/*******************************************************************************
  Type: r_csih_FrameS_t

  CSIH macro driver error code (master mode only)

  Values:
    Data    - pointer to data
    CsSel   - Chip Select config
    NbData  - Max number of data
    IntReq  - An interrupt is requested at the end of the data set
    Eoj     - End of Job at the last data
    Databitsize   - Bit size of the data  
*/

typedef struct {
    uint16_t       *Data;
    r_csih_CsSel_t CsSel;
    uint32_t           NbData;
    uint8_t      IntReq;
    uint8_t      Eoj;
    uint8_t        Databitsize;
} r_csih_FrameS_t;


/*******************************************************************************
  Type: r_csih_FrameR_t

  CSIH macro driver error code (master mode only)

  Values:
    Data    - pointer to data
    CsSel   - Chip Select config
    NbData  - Max number of data
    ErrCheckEnable     - End of Job at the last data
  
*/

typedef struct {
    uint16_t       *Data;
    r_csih_CsSel_t CsSel;
    uint32_t           NbData;
    uint8_t      ErrCheckEnable;
    r_csih_StatError_t Err;
}r_csih_FrameR_t;




/*******************************************************************************
  Section: Global Macros
    
*/


/*******************************************************************************
  Section: Global API Functions
  
  Group: Initialisation API
  
*/

/*******************************************************************************
  Function: r_csih_Init

  Driver init function.

  Parameters:
  Unit       - Instance number
  Config     - see: <r_csih_InitCfg_t>
 
  Returns:
  see: <r_csih_Error_t>
*/

uint32_t R_CSIH_Init( uint32_t Unit, r_csih_InitCfg_t *Config, r_csih_ExtFunc_t* OptExtFeat);

/*******************************************************************************
  Function: R_CSIH_DeInit

  Sets the macro with reset values

  Parameters:
  Unit       - Instance number
 
  Returns:
  see: <r_csih_Error_t>
*/

uint32_t R_CSIH_DeInit(uint32_t Unit);


/*******************************************************************************
  Function: R_CSIH_Enable

  Powers on the macro.

  Parameters:
  Unit       - Instance number
  ExtFunc    - Extended feature configuration (optional: NULL)

  Returns:
  see: <r_csih_Error_t>
*/

uint32_t R_CSIH_Enable(uint32_t Unit, r_csih_TxRxMode_t Mode);


/*******************************************************************************
  Function: R_CSIH_Disable

  Unpowers on the macro.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <r_csih_Error_t>
*/

r_csih_Error_t R_CSIH_Disable(uint32_t Unit);


/*******************************************************************************
  Group: FIFO Mode
*/


/*******************************************************************************
  Function: R_CSIH_FifoInit

  Initialises the FIFO mode and sets the interrupt threshold levels

  Parameters:
  Unit              - Instance number
  RxLevel           - Reception interrupt happens when number of received data is equal to RxLevel
  TxLevel           - Transmission interrupt happens when number of non sent transmission data is 
                      equal to TxLevel

  Returns:
  See: <r_csih_Error_t>
*/

uint32_t R_CSIH_FifoInit (uint32_t Unit, uint8_t RxLevel, uint8_t TxLevel);


/*******************************************************************************
  Function: R_CSIH_FifoReset

  Resets the FIFO. Any ongoing communication is aborted immediately

  Parameters:
  Unit              - Instance number

  Returns:
  See: <r_csih_Error_t>
*/

uint32_t R_CSIH_FifoReset (uint32_t Unit);


/*******************************************************************************
  Function: R_CSIH_FifoPush

  Adds data to the FIFO (no JOB mode)
  
  Parameters:
  Unit      - Instance number
  Frame     - includes data nb of data etc... See <r_csih_FrameS_t>
  
  Returns:
  Number of data pushed into the FIFO
*/

uint32_t R_CSIH_FifoPush(uint32_t Unit, r_csih_FrameS_t *Frame);


/*******************************************************************************
  Function: R_CSIH_FifoPull

  Reads data from the FIFO (no JOB mode)

  Parameters:
  Unit              - Instance number
  Frame             - includes data nb of data etc... See <r_csih_FrameR_t>

  Returns:
  Number of data pushed into the FIFO
*/

uint32_t R_CSIH_FifoPull ( uint32_t Unit, r_csih_FrameR_t *Frame);


/*******************************************************************************
  Function: R_CSIH_FifoIntCfg

  Reads data from the FIFO (no JOB mode)

  Parameters:
  Unit             - Instance number
  TxLevel          - TIC happens when nb of unsent TX = TxLevel
  RxLevel          - TIC happens when nb of receivable data = RxLevel

  Returns see <r_csih_Error_t>
   
*/

r_csih_Error_t R_CSIH_FifoIntCfg (uint32_t Unit, uint8_t RxLevel, uint8_t TxLevel);


/*******************************************************************************
  Function: R_CSIH_FifoGetNbUnsent

  Returns number of unsent data in the FIFO

  Parameters:
  Unit              - Instance number

  Returns:
  Number of unsent data in the FIFO
*/

uint8_t R_CSIH_FifoGetNbUnsent(uint32_t Unit);


/*******************************************************************************
  Function: R_CSIH_FifoGetNbRec

  Returns number of received data in the FIFO

  Parameters:
  Unit              - Instance number

  Returns:
  Number of received data in the FIFO
*/

uint8_t R_CSIH_FifoGetNbRec(uint32_t Unit);


/*******************************************************************************
  Group: General, JOB Enabled mode
*/


/*******************************************************************************
  Function: R_CSIH_AbortJob

  Aborts ongoing job

  Parameters:
  Unit       - Instance number
 
  Returns:
  see: <r_csih_Error_t>
*/

r_csih_Error_t R_CSIH_AbortJob(uint32_t Unit);


/*******************************************************************************
  Group: Error and interrupts
*/

/*******************************************************************************
  Function: R_CSIH_SetErrorCallback

  Set a user function as an the error handler.

  Parameters:
  *ErrorHandler - a pointer to a user function (callback) 
                  with two paramerters (unit and error number) 
  
  Returns:
  void
 
*/

void R_CSIH_SetErrorCallback(void ( *ErrorCallback )(uint32_t Unit, uint32_t Error));

/*******************************************************************************
  Section: Csih ISR Functions
*/

/*******************************************************************************
  Function: R_CSIH_SetIsrCallback

  Assign a user callback for the given interrupt of a unit. 
  
  The function will be called after the driver ISR has
  processed the interrupt.

  Parameters:
  Unit          - Instance number
  IntType       - What kind of interrupt; see: <r_csih_Interrupt_t>
  Isr           - Isr function pointer
  
  Returns:
  see: <r_csih_Error_t>
*/

r_csih_Error_t R_CSIH_SetIsrCallback(uint32_t Unit, r_csih_Interrupt_t IntType, 
                                 void (*Isr)(void));
                                 

/*******************************************************************************
  Function: R_CSIH_IsrTic

  Driver TIC interrupt service routine. 
  This function shall be called by device interrupt handler 
  (dfxxxx_isr.c) and can be found in ..\csih\src\csih_isr.c.


  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_CSIH_IsrTic(uint32_t Unit);

/*******************************************************************************
  Function: R_CSIH_IsrTijc

  Driver TIJC interrupt service routine. 
  This function shall be called by device interrupt handler 
  (dfxxxx_isr.c) and can be found in ..\csih\src\csih_isr.c.

  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_CSIH_IsrTijc(uint32_t Unit);

/*******************************************************************************
  Function: R_CSIH_IsrTir

  Driver TIR interrupt service routine. 
  This function shall be called by device interrupt handler 
  (dfxxxx_isr.c) and can be found in ..\csih\src\csih_isr.c.

  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_CSIH_IsrTir(uint32_t Unit);

/*******************************************************************************
  Function: R_CSIH_IsrTire

  Driver TIRE interrupt service routine. 
  This function shall be called by device interrupt handler 
  (dfxxxx_isr.c) and can be found in ..\csih\src\csih_isr.c.

  Parameters:
  Unit       - Instance number
  
  Returns:
  void
*/

void R_CSIH_IsrTire(uint32_t Unit);

/*******************************************************************************
  Function: R_CSIH_EnableInt

  Enable the Csih interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_csih_Interrupt_t>
  
  Returns:
  see: <r_csih_Error_t>
*/

r_csih_Error_t R_CSIH_EnableInt(uint32_t Unit, r_csih_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_CSIH_DisableInt

  Disable the Csih interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_csih_Interrupt_t>
  
  Returns:
  see: <r_csih_Error_t>
*/

r_csih_Error_t R_CSIH_DisableInt(uint32_t Unit, r_csih_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_CSIH_GetIntMask

  Get the masking status of the interrupt 'Interrupt'.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_csih_Interrupt_t>
  
  Returns:
  see: <uint8_t>
*/

uint8_t R_CSIH_GetIntMask(uint32_t Unit, r_csih_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_CSIH_ForceInt

  Sets the flag of the requested interrupt.
  
  Parameters:
  Unit       - Instance number
  Interrupt  - Interrupt type see: <r_csih_Interrupt_t>
  
  Returns:
  see: <r_csih_Error_t>
*/
void R_CSIH_ForceInt(uint32_t Unit, r_csih_Interrupt_t Interrupt);


/*******************************************************************************
  Function: R_CSIH_GetStatError

  Sets the flag of the requested interrupt.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_csih_StatError_t>
*/
r_csih_StatError_t R_CSIH_GetStatError(uint32_t Unit);


/*******************************************************************************
  Function: R_CSIH_GetStatError

  Sets the flag of the requested interrupt.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  see: <r_csih_StatError_t>
*/
void R_CSIH_ClearStatError(uint32_t Unit, r_csih_StatError_t Error);


/*******************************************************************************
  Function: R_CSIH_TrigReception

  Sets the flag of the requested interrupt.
  
  Parameters:
  Unit       - Instance number
  Num        - Number of data to be received
  
  Returns:
  see: <r_csih_StatError_t>
*/
uint32_t R_CSIH_TrigReception(uint32_t Unit, uint32_t Num);


/*******************************************************************************
  Function: R_CSIH_DbPush

  Indicates the transmission status.
  
  Parameters:
  Unit       - Instance number
  Address    - Address in CSIH RAM (Offset)
  Frame      - Definition of the frame to write. See: <r_csih_FrameS_t>
  
  Returns:
  Number of data puled
*/
uint32_t R_CSIH_DbPush ( uint32_t Unit, uint8_t Address, r_csih_FrameS_t *Frame);


/*******************************************************************************
  Function: R_CSIH_DbPull

  Indicates the transmission status.
  
  Parameters:
  Unit       - Instance number
  Address    - Address in CSIH RAM (Offset)
  Frame      - Definition of the frame to read. See: <r_csih_FrameR_t>
  
  Returns:
  Number of data puled
*/
uint32_t R_CSIH_DbPull ( uint32_t Unit, uint8_t Address, r_csih_FrameR_t *Frame);
  
/*******************************************************************************
  Function: R_CSIH_DbTrig

  Indicates the transmission status.
  
  Parameters:
  Unit       - Instance number
  Address    - Address in CSIH RAM (Offset)
  NbData     - Number of data to be sent
  
  Returns:
  See <r_csih_Error_t>
*/
r_csih_Error_t R_CSIH_DbTrig ( uint32_t Unit, uint8_t Address, uint8_t NbData);


/*******************************************************************************
  Function: R_CSIH_DbNbRemain

  Indicates the transmission status.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Number of data to be received (-1 if parameter error)
*/
uint32_t R_CSIH_DbNbRemain (uint32_t Unit);


/*******************************************************************************
  Function: R_CSIH_TxbTrig

  Indicates the transmission status.
  
  Parameters:
  Unit       - Instance number
  Address    - Address in CSIH RAM (Offset)
  NbData     - Number of data to be sent
  
  Returns:
  See <r_csih_Error_t>
*/
r_csih_Error_t R_CSIH_TxbTrig ( uint32_t Unit, uint8_t Address, uint8_t NbData);


/*******************************************************************************
  Function: R_CSIH_TxbNbRemain

  Indicates the transmission status.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  Number of data to be received (-1 if parameter error)
*/
uint32_t R_CSIH_TxbNbRemain (uint32_t Unit);


/*******************************************************************************
  Function: R_CSIH_TxbPull

  Indicates the transmission status.
  
  Parameters:
  Unit       - Instance number
  Frame      - Definition of the frame to read. See: <r_csih_FrameR_t>
  
  Returns:
  Number of data puled
*/
uint32_t R_CSIH_TxbPull ( uint32_t Unit, r_csih_FrameR_t *Frame);


/*******************************************************************************
  Function: R_CSIH_DbPush

  Indicates the transmission status.
  
  Parameters:
  Unit       - Instance number
  Address    - Address in CSIH RAM (Offset)
  Frame      - Definition of the frame to write. See: <r_csih_FrameS_t>
  
  Returns:
  Number of data puled
*/
uint32_t R_CSIH_DbPush(uint32_t Unit, uint8_t Address, r_csih_FrameS_t *Frame);


/*******************************************************************************
  Function: R_CSIH_TxbPush

  Indicates the transmission status.
  
  Parameters:
  Unit       - Instance number
  Address    - Address in CSIH RAM (Offset)
  Frame      - Definition of the frame to write. See: <r_csih_FrameS_t>
  
  Returns:
  Number of data puled
*/
uint32_t R_CSIH_TxbPush(uint32_t Unit, uint8_t Address, r_csih_FrameS_t *Frame);


/*******************************************************************************
  Function: R_CSIH_BrkPtCfg

  Configures the macro to stop or not at a debug break point
  
  Parameters:
  Unit       - Instance number
  Stop       - TRUE: stop ; FALSE: do not stop
  
  Returns:
  see: <r_csih_Error_t>
*/
r_csih_Error_t R_CSIH_BrkPtCfg(uint32_t Unit, uint8_t Stop);


/*******************************************************************************
  Function: R_CSIH_StatusClear

  Clear the status bit
  
  Parameters:
  Unit       - Instance number
  Status     - See: <r_csih_StatusBit_t>
  
  Returns:
  void
*/
void R_CSIH_StatusClear(uint32_t Unit, r_csih_Status_t Status);


/* Some handy defines */
#define R_CSIH_STATUS_OVE_CLEAR(U)   (R_CSIH_StatusClear((U),R_CSIH_STATUS_OVE ))
#define R_CSIH_STATUS_OPE_CLEAR(U)   (R_CSIH_StatusClear((U),R_CSIH_STATUS_OPE ))
#define R_CSIH_STATUS_DCE_CLEAR(U)   (R_CSIH_StatusClear((U),R_CSIH_STATUS_DCE ))
#define R_CSIH_STATUS_EMF_CLEAR(U)   (R_CSIH_StatusClear((U),R_CSIH_STATUS_EMF ))
#define R_CSIH_STATUS_FLF_CLEAR(U)   (R_CSIH_StatusClear((U),R_CSIH_STATUS_FLF ))
#define R_CSIH_STATUS_TR_CLEAR(U)    (R_CSIH_StatusClear((U),R_CSIH_STATUS_TSF ))
#define R_CSIH_STATUS_OFE_CLEAR(U)   (R_CSIH_StatusClear((U),R_CSIH_STATUS_OFE ))
#define R_CSIH_STATUS_TOE_CLEAR(U)   (R_CSIH_StatusClear((U),R_CSIH_STATUS_TMOE)) 






/*******************************************************************************
  Function: R_CSIH_StatusGet

  retrieves the status bit
  
  Parameters:
  Unit       - Instance number
  Status     - See: <r_csih_StatusBit_t>
  
  Returns:
  TRUE for set, FALSE for cleared; see: <uint8_t>
*/
uint8_t R_CSIH_StatusGet(uint32_t Unit, r_csih_Status_t Status);


/* Some handy defines */
#define R_CSIH_STATUS_OVE_GET(U)   (R_CSIH_StatusGet((U),R_CSIH_STATUS_OVE ))
#define R_CSIH_STATUS_OPE_GET(U)   (R_CSIH_StatusGet((U),R_CSIH_STATUS_OPE ))
#define R_CSIH_STATUS_DCE_GET(U)   (R_CSIH_StatusGet((U),R_CSIH_STATUS_DCE ))
#define R_CSIH_STATUS_EMF_GET(U)   (R_CSIH_StatusGet((U),R_CSIH_STATUS_EMF ))
#define R_CSIH_STATUS_FLF_GET(U)   (R_CSIH_StatusGet((U),R_CSIH_STATUS_FLF ))
#define R_CSIH_STATUS_TR_GET(U)    (R_CSIH_StatusGet((U),R_CSIH_STATUS_TSF ))
#define R_CSIH_STATUS_OFE_GET(U)   (R_CSIH_StatusGet((U),R_CSIH_STATUS_OFE ))
#define R_CSIH_STATUS_TOE_GET(U)   (R_CSIH_StatusGet((U),R_CSIH_STATUS_TMOE)) 
                                                        





/*******************************************************************************
  Function: R_CSIH_DaSwitch

  Switch to and back from direct memory access mode
  
  Parameters:
  Unit       - Instance number
  On         - TRUE to switch to direct memory mode; See: <r_csih_StatusBit_t>
  
  Returns:
  Error, see <r_csih_Error_t>
*/
r_csih_Error_t R_CSIH_DaSwitch(uint32_t Unit, uint8_t On);


/*******************************************************************************
  Function: R_CSIH_DaWrite

  Write (max 1) data to the CSIH
  
  Parameters:
  Unit       - Instance number
  Frame      - Data frame specification; See: <r_csih_FrameS_t>
  
  Returns:
  Number of sent data
*/
uint32_t R_CSIH_DaWrite(uint32_t Unit, r_csih_FrameS_t *Frame);


/*******************************************************************************
  Function: R_CSIH_DaRead

  Reads (max 1) data from the CSIH
  
  Parameters:
  Unit       - Instance number
  Frame      - Data frame specification; See: <r_csih_FrameR_t>
  
  Returns:
  Number of received data
*/
uint32_t R_CSIH_DaRead(uint32_t Unit, r_csih_FrameR_t *Frame);


/*******************************************************************************
  Function: R_CSIH_LbmSwitch

  switch to Loop-Back mode or to normal mode
  
  Parameters:
  Unit       - Instance number
  On         - TRUE for Loop-Back mode and FALSE for normal mode; See: <uint8_t>
  
  Returns:
  Error: see <r_csih_Error_t>
*/
r_csih_Error_t R_CSIH_LbmSwitch(uint32_t Unit, uint8_t On);

/*******************************************************************************
  Function: R_CSIH_DbPushData

  To push data to transmission data to the dual buffer with different data length.
  
  Parameters:
  Unit       - Instance number
  Address    - Address in CSIH RAM (Offset)
  Frame      - Definition of the frame to write. See: <r_csih_FrameS_t>
  
  Returns:
  Number of data puled
*/
uint8_t R_CSIH_DbPushData(uint8_t Unit, uint8_t Address, r_csih_FrameS_t *Frame);

/*******************************************************************************
  Function: R_CSIH_Init_Svstop

  To configure SVSTOP bit of emulation register.
  
  Parameters:
  Unit       - Instance number
  Svstop     - uint8_t

  Returns:
  void
*/
void R_CSIH_Init_Svstop ( uint8_t Unit , uint8_t Svstop);


/*******************************************************************************
  Function: R_CSIH_GetClock

  Gets the clock for the selected CsiH unit

  Parameters:
  Unit          CsiH unit

  Returns:
  uint32_t      clock frequency

*/
uint32_t R_CSIH_GetClock(uint32_t Unit);


#ifdef __cplusplus
}
#endif

#endif /* CSIH_API_H_  */

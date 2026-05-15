/*
****************************************************************************
PROJECT : CSIH driver
FILE    : $Id: r_csih_regs.h 3935 2014-09-23 08:52:32Z golczewskim $
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

#ifndef CSIH_REGS_H_
#define CSIH_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: CSIH Register Offsets

  Only the driver modules shall include this header.
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constants: Register Offsets

  R_CSIH_CTL_0     - Control register 0 
  R_CSIH_CTL_1     - Control register 1
  R_CSIH_CTL_2     - Control register 2
  R_CSIH_STR_0     - Status register 0
  R_CSIH_STC_0     - Status clear register 0
  R_CSIH_EMU       - Emulation register
  
*/


/* User Register CSIH */
#define R_CSIH_CTL_0   (R_CSIH_USER_OFFSET + 0x00u)
#define R_CSIH_CTL_1   (R_CSIH_USER_OFFSET + 0x10u) 
#define R_CSIH_CTL_2   (R_CSIH_USER_OFFSET + 0x14u)
#define R_CSIH_STR_0   (R_CSIH_USER_OFFSET + 0x04u) 
#define R_CSIH_STC_0   (R_CSIH_USER_OFFSET + 0x08u) 
#define R_CSIH_EMU     (R_CSIH_USER_OFFSET + 0x18u)

#define R_CSIH_MCTL_0  (R_CSIH_OS_OFFSET +  0x40u)      
#define R_CSIH_CFG(n)  (R_CSIH_OS_OFFSET + (0x44u+4u*(n)))
#define R_CSIH_BRS(n)  (R_CSIH_OS_OFFSET + (0x68u+4u*(n)))

#define R_CSIH_MCTL_1  (R_CSIH_OS_OFFSET + 0x00u)
#define R_CSIH_MCTL_2  (R_CSIH_OS_OFFSET + 0x04u)
#define R_CSIH_TX0W    (R_CSIH_OS_OFFSET + 0x08u)
#define R_CSIH_TX0H    (R_CSIH_OS_OFFSET + 0x0cu)
#define R_CSIH_RX0W    (R_CSIH_OS_OFFSET + 0x10u)
#define R_CSIH_RX0H    (R_CSIH_OS_OFFSET + 0x14u)
#define R_CSIH_MRWP    (R_CSIH_OS_OFFSET + 0x18u)




/*******************************************************************************
  Section: Global Types
  
  CSIH Register Definitions
*/
  
/*******************************************************************************
  Type: r_csih_RegCtl0Bit_t
  
  Content of control register CTL0
  
  Members:
  Mbs       - Data consistency check enable 
  JobE      - Job enable
  Rxe       - Reception enable 
  Txe       - Transmission enable
  Pwr       - Operation control
*/

typedef struct {
    uint8_t Mbs     : 1;  
    uint8_t JobE    : 1;  
    uint8_t Unused  : 3;
    uint8_t Rxe     : 1;  
    uint8_t Txe     : 1;  
    uint8_t Pwr     : 1;  
} r_csih_RegCtl0Bit_t;


/*******************************************************************************
  Union: r_csih_RegCtl0_t
  
  Simplifies Bit and Byte access to register CTL0    
  
  Members:
  Bit       - Allow bit access; see: <r_csih_RegCtl0Bit_t>
  Byte      - Allow access to the whole byte
*/

typedef union {
    r_csih_RegCtl0Bit_t Bit;
    uint8_t             Byte;
} r_csih_RegCtl0_t;

  
/*******************************************************************************
  Type: r_csih_RegCtl1Bit_t
  
  Content of control register CTL1
  
  Members:
  Sse       - Slave Select function Enable
  Hse       - Hand-Shake function Mode Selection
  Sit       - Interrupt Delay Mode Selection
  Lbm       - Loop-Back Mode
  Csri      - Chip Select Return to Inactive
  Dcs       - Data Consistency Check Enable/Disable
  Je        - Job Mode Enable
  Edle      - Extended Data Length Mode Enable
  Csl0      - Chip Select 0 Output Level
  Csl1      - Chip Select 1 Output Level
  Csl2      - Chip Select 2 Output Level
  Csl3      - Chip Select 3 Output Level
  Csl4      - Chip Select 4 Output Level
  Csl5      - Chip Select 5 Output Level
  Csl6      - Chip Select 6 Output Level
  Csl7      - Chip Select 7 Output Level
  Slit      - Select CSIHTIC Interrupt timing
  Ckr       - Initial output level of the data output signal 
  Phe       - CPU-controlled priority-based communication function.
*/

typedef struct {
    uint16_t Sse    : 1;  
    uint16_t Hse    : 1;  
    uint16_t Sit    : 1;
    uint16_t Lbm    : 1;  
    uint16_t Csri   : 1;  
    uint16_t Dcs    : 1;  
    uint16_t Je     : 1;  
    uint16_t Edle   : 1;  
    uint16_t Csl0   : 1;  
    uint16_t Csl1   : 1;  
    uint16_t Csl2   : 1;  
    uint16_t Csl3   : 1;  
    uint16_t Csl4   : 1;  
    uint16_t Csl5   : 1;  
    uint16_t Csl6   : 1;  
    uint16_t Csl7   : 1;  
    uint16_t Slit   : 1;
    uint16_t Ckr    : 1;
    uint16_t Phe    : 1;
    uint16_t Unused : 13;  
} r_csih_RegCtl1Bit_t;


/*******************************************************************************
  Union: r_csih_RegCtl1_t
  
  Simplifies Bit and Byte access to register CTL1    
  
  Members:
  Bit       - Allow bit access; see: <r_csih_RegCtl1Bit_t>
  Word      - Allow access to the whole word
*/

typedef union {
    r_csih_RegCtl1Bit_t Bit;
    uint32_t            Word;
} r_csih_RegCtl1_t;

  
/*******************************************************************************
  Type: r_csih_RegCtl2Bit_t
  
  Content of control register CTL2
  
  Members:
  Brs       - Baud Rate Selection Bits
  Prs       - Prescaler Section Bits
  
*/

typedef struct {
    uint16_t Brs    : 12;
    uint16_t Unused : 1;
    uint16_t Prs    : 3;
} r_csih_RegCtl2Bit_t;


/*******************************************************************************
  Union: r_csih_RegCtl2_t
  
  Simplifies Bit and Byte access to register CTL2    
  
  Members:
  Bit       - Allow bit access; see: <r_csih_RegCtl2Bit_t>
  Hwrod     - Allow access to the half word (16 bits)
*/

typedef union {
    r_csih_RegCtl2Bit_t Bit;
    uint16_t            Hword;
} r_csih_RegCtl2_t;

  
/*******************************************************************************
  Type: r_csih_RegMCtl0Bit_t
  
  Content of control register MCTL0
  
  Members:
  To        - Time-out Setting Selection
  Mms       - Memory Mode Selection
  
*/

typedef struct {
    uint16_t To      : 5;
    uint16_t Unused0 : 3;
    uint16_t Mms     : 2;
    uint16_t Unused1 : 6;
} r_csih_RegMCtl0Bit_t;


/*******************************************************************************
  Union: r_csih_RegMCtl0_t
  
  Simplifies Bit and Byte access to register MCTL0    
  
  Members:
  Bit       - Allow bit access; see: <r_csih_RegMCtl0Bit_t>
  Hwrod     - Allow access to the half word (16 bits)
*/

typedef union {
    r_csih_RegMCtl0Bit_t    Bit;
    uint16_t                Hword;
} r_csih_RegMCtl0_t;

  
/*******************************************************************************
  Type: r_csih_RegMCtl1Bit_t
  
  Content of control register MCTL1
  
  Members:
  Ffs       - FIFO Full Interrupt Configuration
  Fes       - FIFO Empty Interrupt Configuration
  
*/

typedef struct {
    uint32_t Ffs     : 7;
    uint32_t Unused0 : 9;
    uint32_t Fes     : 7;
    uint32_t Unused1 : 9;
} r_csih_RegMCtl1Bit_t;


/*******************************************************************************
  Union: r_csih_RegMCtl1_t
  
  Simplifies Bit and Byte access to register MCTL1    
  
  Members:
  Bit       - Allow bit access; see: <r_csih_RegMCtl1Bit_t>
  Word      - Allow access to the word (32 bits)
*/

typedef union {
    r_csih_RegMCtl1Bit_t    Bit;
    uint32_t                Word;
} r_csih_RegMCtl1_t;

  
/*******************************************************************************
  Type: r_csih_RegMCtl2Bit_t
  
  Content of control register MCTL2
  
  Members:
  Sop       - Send Of Pointer
  Nd        - Number of Data
  Btst      - Buffer Transfer Start Trigger
  
*/

typedef struct {
    uint32_t Sop     : 7;
    uint32_t Unused0 : 9;
    uint32_t Nd      : 8;
    uint32_t Unused1 : 7;
    uint32_t Btst    : 1;
} r_csih_RegMCtl2Bit_t;


/*******************************************************************************
  Union: r_csih_RegMCtl2_t
  
  Simplifies Bit and Byte access to register BCTL1    
  
  Members:
  Bit       - Allow bit access; see: <r_csih_RegMCtl2Bit_t>
  Word      - Allow access to the word (32 bits)
*/

typedef union {
    r_csih_RegMCtl2Bit_t    Bit;
    uint32_t                Word;
} r_csih_RegMCtl2_t;

  
/*******************************************************************************
  Type: r_csih_RegMRwp0Bit_t
  
  Content of control register MRWP0
  
  Members:
  Trwa       - Tx-Buffer Read/Write Pointer
  Rra        - Rx-Buffer Read Pointer
  
*/

typedef struct {
    uint32_t Trwa    : 7;
    uint32_t Unused0 : 9;
    uint32_t Rra     : 7;
    uint32_t Unused1 : 9;
} r_csih_RegMRwp0Bit_t;


/*******************************************************************************
  Union: r_csih_RegMRwp0_t
  
  Simplifies Bit and Byte access to register MRWP0    
  
  Members:
  Bit       - Allow bit access; see: <r_csih_RegMRwp0Bit_t>
  Word      - Allow access to the word (32 bits)
*/

typedef union {
    r_csih_RegMRwp0Bit_t    Bit;
    uint32_t                Word;
} r_csih_RegMRwp0_t;

  
/*******************************************************************************
  Type: r_csih_RegCfgBit_t
  
  Content of control register MCFGm
  
  Members:
  Sp        - Chip Select n Setup Timing
  In        - Chip Select n Inter-Data Delay Timing
  Hd        - Chip Select n Hold Timing
  Idl       - Chip Select n Idle Timing Selection
  Dap       - Chip Select n Idle Enforcement Configuration
  Ckp       - Chip Select n Clock And Data Phase Selection
  Dir       - Chip Select n Serial Data Direction Selection
  Rcb       - Chip Select n Recessive Configuration for Broadcasting Selection
  Dls       - Chip Select n Data Length Selection
  Ps        - Chip Select n Parity Selection
  Pscl      - Chip Select n Prescaler Selection
  
*/

typedef struct {
    uint8_t Sp      : 4;
    uint8_t In      : 4;
    uint8_t Hd      : 4;
    uint8_t Id      : 3;
    uint8_t Idl     : 1;
    uint8_t Dap     : 1;
    uint8_t Ckp     : 1;
    uint8_t Dir     : 1;
    uint8_t Rcb     : 1;
    uint8_t Unused  : 4;
    uint8_t Dls     : 4;
    uint8_t Ps      : 2;
    uint8_t Pscl    : 2; /*in F1x this bit is different */
} r_csih_RegCfgBit_t;


/*******************************************************************************
  Union: r_csih_RegCfg_t
  
  Simplifies Bit and Byte access to register MCFGm    
  
  Members:
  Bit       - Allow bit access; see: <r_csih_RegCfgBit_t>
  Word      - Allow access to the word (32 bits)
*/

typedef union {
    r_csih_RegCfgBit_t      Bit;
    uint32_t                Word;
} r_csih_RegCfg_t;

  
/*******************************************************************************
  Type: r_csih_RegTxwBit_t
  
  Content of control register TXW
  
  Members:
  Tx        - Transmission data
  Cs0       - Chip Select 0 Activation
  Cs1       - Chip Select 1 Activation
  Cs2       - Chip Select 2 Activation
  Cs3       - Chip Select 3 Activation
  Cs4       - Chip Select 4 Activation
  Cs5       - Chip Select 5 Activation
  Cs6       - Chip Select 6 Activation
  Cs7       - Chip Select 7 Activation
  Edl       - Extended Data Length Configuration
  Eoj       - End-Of-Job Configuration
  Cire      - Communication Interrupt request
  
*/

typedef struct {
    uint16_t Tx     : 16;
    uint8_t Cs     : 8;
#if 0
    uint8_t Cs1     : 1;
    uint8_t Cs2     : 1;
    uint8_t Cs3     : 1;
    uint8_t Cs4     : 1;
    uint8_t Cs5     : 1;
    uint8_t Cs6     : 1;
    uint8_t Cs7     : 1;
#endif
    uint8_t Unused  : 5;
    uint8_t Edl     : 1;
    uint8_t Eoj     : 1;
    uint8_t Cire    : 1;
} r_csih_RegTxwBit_t;


/*******************************************************************************
  Union: r_csih_RegTxw_t
  
  Simplifies Bit and Byte access to register TXW    
  
  Members:
  Bit       - Allow bit access; see: <r_csih_RegTxwBit_t>
  Word      - Allow access to the word (32 bits)
*/

typedef union {
    r_csih_RegTxwBit_t      Bit;
    uint32_t                Word;
} r_csih_RegTxw_t;

  
/*******************************************************************************
  Type: r_csih_RegTxh_t
  
*/

typedef uint16_t r_csih_RegTxh_t;

  
/*******************************************************************************
  Type: r_csih_RegRxwBit_t
  
  Content of control register TXW
  
  Members:
  Rx        - Reception Data
  Cs0       - Chip Select 0 Activated
  Cs1       - Chip Select 1 Activated
  Cs2       - Chip Select 2 Activated
  Cs3       - Chip Select 3 Activated
  Cs4       - Chip Select 4 Activated
  Cs5       - Chip Select 5 Activated
  Cs6       - Chip Select 6 Activated
  Cs7       - Chip Select 7 Activated
  Dce       - Transmission Data Consistency Error Flag
  Rpe       - Reception Data Parity Error Flag
  
*/

typedef struct {
    uint16_t Rx;
    uint8_t Cs0     : 1;
    uint8_t Cs1     : 1;
    uint8_t Cs2     : 1;
    uint8_t Cs3     : 1;
    uint8_t Cs4     : 1;
    uint8_t Cs5     : 1;
    uint8_t Cs6     : 1;
    uint8_t Cs7     : 1;
    uint8_t Dce     : 1;
    uint8_t Rpe     : 1;
    uint8_t Unused  : 6;
} r_csih_RegRxwBit_t;


/*******************************************************************************
  Union: r_csih_RegRxw_t
  
  Simplifies Bit and Byte access to register TXW    
  
  Members:
  Bit       - Allow bit access; see: <r_csih_RegRxwBit_t>
  Word      - Allow access to the word (32 bits)
*/

typedef union {
    r_csih_RegRxwBit_t      Bit;
    uint32_t                Word;
} r_csih_RegRxw_t;

  
/*******************************************************************************
  Type: r_csih_RegRxh_t
  
*/

typedef uint16_t r_csih_RegRxh_t;

  
/*******************************************************************************
  Type: r_csih_RegERxwBit_t
  
  Content of control register TXW
  
  Members:
  Erx       - Reception Data
  Ecs0      - Chip Select 0 Activated
  Ecs1      - Chip Select 1 Activated
  Ecs2      - Chip Select 2 Activated
  Ecs3      - Chip Select 3 Activated
  Ecs4      - Chip Select 4 Activated
  Ecs5      - Chip Select 5 Activated
  Ecs6      - Chip Select 6 Activated
  Ecs7      - Chip Select 7 Activated
  Edce       - Transmission Data Consistency Error Flag
  Erpe       - Reception Data Parity Error Flag
  
*/

typedef struct {
    uint16_t Erx    : 16;
    uint8_t Ecs0    : 1;
    uint8_t Ecs1    : 1;
    uint8_t Ecs2    : 1;
    uint8_t Ecs3    : 1;
    uint8_t Ecs4    : 1;
    uint8_t Ecs5    : 1;
    uint8_t Ecs6    : 1;
    uint8_t Ecs7    : 1;
    uint8_t Edce    : 1;
    uint8_t Erpe    : 1;
    uint8_t Unused  : 6;
} eeEcsiH_RegERxwBit_t;


/*******************************************************************************
  Union: r_csih_RegERxw_t
  
  Simplifies Bit and Byte access to register TXW    
  
  Members:
  Bit       - Allow bit access; see: <eeEcsiH_RegERxwBit_t>
  Word      - Allow access to the word (32 bits)
*/

typedef union {
    eeEcsiH_RegERxwBit_t    Bit;
    uint32_t                Word;
} r_csih_RegERxw_t;

  
/*******************************************************************************
  Type: r_csih_RegStr0Bit_t
  
  Content of status register STR 0
  
  Members:
  Ove       - Overrun Error Flag
  Pe        - Parity Error Flag
  Dce       - Data Consistency Check Error Flag
  Emf       - Buffer Empty Status Flag
  Flf       -  Buffer Full Status Flag
  Tsf       - Transfer Status Flag
  Ofe       - Overflow Error Flag
  Tmoe      - Time Out Error Flag
  Spf       - Number of unsent data in FIFO buffer mode
  Srp       - Number of receivable words in FIFO buffer mode
  
*/

typedef struct {
    uint8_t Ove     : 1;
    uint8_t Pe      : 1;
    uint8_t Unused0 : 1;
    uint8_t Dce     : 1;
    uint8_t Emf     : 1;
    uint8_t Flf     : 1;
    uint8_t Unused1 : 1;
    uint8_t Tsf     : 1;
    uint8_t Unused2 : 6;
    uint8_t Ofe     : 1;
    uint8_t Tmoe    : 1;
    uint8_t Spf     : 8;
    uint8_t Srp     : 8;
} r_csih_RegStr0Bit_t;


/*******************************************************************************
  Union: r_csih_RegStr0_t
  
  Simplifies Bit and Byte access to register STR0    
  
  Members:
  Bit       - Allow bit access; see: <r_csih_RegStr0Bit_t>
  Word      - Allow access to the word (32 bits)
*/

typedef union {
    r_csih_RegStr0Bit_t Bit;
    uint32_t            Word;
} r_csih_RegStr0_t;

  
/*******************************************************************************
  Type: r_csih_RegStc0Bit_t
  
  Content of status register STR 0
  
  Members:
  Ovec      - Overrun Error Flag Clear Command
  Pec       - Parity Error Flag Clear Command
  Dcec      - Data Consistency Check Error Flag Clear Command
  Pct       - Pointer Clear Command
  Ofec      - Overflow Error Flag Clear Command
  Moec      - Time Out Error Flag Clear Command
  
*/

typedef struct {
    uint8_t Ovec    : 1;
    uint8_t Pec     : 1;
    uint8_t Unused0 : 1;
    uint8_t Dcec    : 1;
    uint8_t Unused1 : 4;
    uint8_t Pct     : 1;
    uint8_t Unused2 : 5;
    uint8_t Ofec    : 1;
    uint8_t Moec    : 1;
} r_csih_RegStc0Bit_t;


/*******************************************************************************
  Union: r_csih_RegStc0_t
  
  Simplifies Bit and Byte access to register STCR 0    
  
  Members:
  Bit       - Allow bit access; see: <r_csih_RegStc0Bit_t>
  Word      - Allow access to the half word (16 bits)
*/

typedef union {
    r_csih_RegStc0Bit_t Bit;
    uint16_t            Hword;
} r_csih_RegStc0_t;

  
/*******************************************************************************
  Type: r_csih_RegEmuBit_t
  
  Content of status register EMU
  
  Members:
  svsdis        - SVSTOP disable
  
*/

typedef struct {
    uint8_t Unused0 : 7;
    uint8_t svsdis  : 1;
} r_csih_RegEmuBit_t;


/*******************************************************************************
  Union: r_csih_RegEmu_t
  
  Simplifies Bit and Byte access to register EMU    
  
  Members:
  Bit       - Allow bit access; see: <r_csih_RegEmuBit_t>
  Word      - Allow access to the Byte (8 bits)
*/

typedef union {
    r_csih_RegEmuBit_t  Bit;
    uint8_t             Byte;
} r_csih_RegEmu_t;

/*******************************************************************************
  Type: r_csih_RegBrsBit_t
  
  Register sets the baud rate for each chip select signal.
  
  Members:
  Brs      - Overrun Error Flag Clear Command
*/

typedef struct {
    uint16_t Brs    : 12;
    uint16_t Unused : 4;
} r_csih_RegBrsBit_t;


/*******************************************************************************
  Union: r_csih_RegBrs_t
  
  Simplifies Bit and Byte access to register STCR 0    
  
  Members:
  Bit       - Allow bit access; see: <r_csih_RegBrsBit_t>
  HWord     - Allow access to the half word (16 bits)
*/

typedef union {
    r_csih_RegBrsBit_t  Bit;
    uint16_t            Hword;
} r_csih_RegBrs_t;


#ifdef __cplusplus
}
#endif

#endif /* CSIH_REGS_H_  */

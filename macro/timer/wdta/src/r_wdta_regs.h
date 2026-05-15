/*
****************************************************************************
PROJECT : WDTA regs
FILE    : $Id: r_wdta_regs.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Driver for WDTA macro
============================================================================
                            C O P Y R I G H T                                    
============================================================================
                           Copyright (c) 2013
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


 Version Author Description
 $Log: $

*/

#ifndef R_WDTA_REGS_H_
#define R_WDTA_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: Wdta Register Offsets

  Only the driver modules shall include this header.
*/

/***********************************************************
  Section: Global Defines
*/

/***********************************************************
  Constants: Register Offsets

  R_WDTA_WDTAnWDTE    Watchdog timer enable register
  R_WDTA_WDTAnEVAC    Watchdog timer enable register for VAC
  R_WDTA_WDTAnREF     Watchdog timer reference value register
  R_WDTA_WDTAnMD      Watchdog timer mode register

*/

#define R_WDTA_WDTE    (0x00)
#define R_WDTA_EVAC    (0x04)
#define R_WDTA_REF     (0x08)
#define R_WDTA_MD      (0x0C)


/***********************************************************
  Type: r_wdta_Reg_MD_t
  
  Content of the timer mode register WDTA0MD
  
  Members:
  
  Res  - Reserved
  Ovf  - Interval time
  Wie  - Output enable setting
  Erm  - Error mode setting
  Ws   - Window size
*/  
  
typedef struct {
    uint8_t Ws   : 2; 
    uint8_t Erm  : 1;
    uint8_t Wie  : 1;
    uint8_t Ovf  : 3;  
    uint8_t Res  : 1; 
     
} r_wdta_RegMdBit_t;

/***********************************************************
  Union: r_wdta_RegMd_t
  
  Simplifies Bit and Byte access to register CTL0    
  
  Members:
  Bit      - Allow bit access; see: <r_wdta_RegMd_t>
  Byte     - Allow access ofthe whole byte
*/

typedef union {
    r_wdta_RegMdBit_t   Bit;
    uint8_t             Byte;
} r_wdta_RegMd_t;
                         

#ifdef __cplusplus
}
#endif

#endif /* R_WDTA_REGS_H_  */

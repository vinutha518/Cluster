/*
****************************************************************************
PROJECT : RTCA driver
FILE    : $Id: r_rtca_regs.h 2731 2014-05-20 11:12:20Z michael.golczewski $
============================================================================ 
DESCRIPTION
Driver for RTCA macro
============================================================================
                            C O P Y R I G H T                                    
============================================================================
                           Copyright (c) 2008
                                  by 
                       NEC Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing

Warranty Disclaimer

Because the Product(s) is licensed free of charge, there is no warranty 
of any kind whatsoever and expressly disclaimed and excluded by NEC, 
either expressed or implied, including but not limited to those for 
non-infringement of intellectual property, merchantability and/or 
fitness for the particular purpose. 
NEC shall not have any obligation to maintain, service or provide bug 
fixes for the supplied Product(s) and/or the Application.

Each User is solely responsible for determining the appropriateness of 
using the Product(s) and assumes all risks associated with its exercise 
of rights under this Agreement, including, but not limited to the risks 
and costs of program errors, compliance with applicable laws, damage to 
or loss of data, programs or equipment, and unavailability or 
interruption of operations.

Limitation of Liability

In no event shall NEC be liable to the User for any incidental, 
consequential, indirect, or punitive damage (including but not limited 
to lost profits) regardless of whether such liability is based on breach 
of contract, tort, strict liability, breach of warranties, failure of 
essential purpose or otherwise and even if advised of the possibility of 
such damages. NEC shall not be liable for any services or products 
provided by third party vendors, developers or consultants identified or
referred to the User by NEC in connection with the Product(s) and/or the 
Application.

****************************************************************************


 Version Author Description
 $Log: $

*/

#ifndef RTCA_REGS_H_
#define RTCA_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************
  Title: RTCA Register Offsets

  Only the driver modules shall include this header.
*/

/***********************************************************
  Section: Global Defines
*/

/***********************************************************
  Constants: Register Offsets

    R_RTCA_CTL0   - Control register 0   
    R_RTCA_CTL1   - Control register 1   
    R_RTCA_CTL2   - Control register 2   
    R_RTCA_SUBC   - Sub-Counter register        
    R_RTCA_SRBU   - Sub-Counter register read buffer       
    R_RTCA_SCMP   - Sub-Counter compare register
    R_RTCA_SUBU   - Clock error correction register
    R_RTCA_SECC   - Seconds count register
    R_RTCA_SEC    - Seconds count buffer register
    R_RTCA_MINC   - Minute count register     
    R_RTCA_MIN    - Minute count buffer register
    R_RTCA_HOURC  - Hour count register       
    R_RTCA_HOUR   - Hour count buffer register
    R_RTCA_WEEKC  - Day of the week count register       
    R_RTCA_WEEK   - Day of the week count buffer register
    R_RTCA_DAYC   - Day count register       
    R_RTCA_DAY    - Day count buffer register
    R_RTCA_MONC   - Month count register       
    R_RTCA_MON    - Month count buffer register
    R_RTCA_YEARC  - Year count register       
    R_RTCA_YEAR   - Year count buffer register
    R_RTCA_TIMEC  - Time count register       
    R_RTCA_TIME   - Time count buffer register
    R_RTCA_CALC   - Calender count register       
    R_RTCA_CAL    - Calender count buffer register
    R_RTCA_ALM    - Alarm minute setting register
    R_RTCA_ALH    - Alarm hour setting register
    R_RTCA_ALW    - Alarm day of the week setting register
    R_RTCA_EMU    - Emulation register (Bit7 = SVSDIS)

*/

#define R_RTCA_CTL0    0x00
/* Content of control register CTL0
  
  Members:
    SLSB    - Operation Mode
    AMPM    - 12 / 24 h selection
    CEST    - Subcounter enable
    CE      - Start / Stop subcounter

  defines the bit position (POS) inside the register
  and the bit width (WID)
*/
#define R_RTCA_CTL0_SLSB_POS   4
#define R_RTCA_CTL0_SLSB_WID   1
#define R_RTCA_CTL0_AMPM_POS   5
#define R_RTCA_CTL0_AMPM_WID   1
#define R_RTCA_CTL0_CEST_POS   6
#define R_RTCA_CTL0_CEST_WID   1
#define R_RTCA_CTL0_CE_POS     7
#define R_RTCA_CTL0_CE_WID     1


#define R_RTCA_CTL1    0x04
/* Content of control register CTL1
  
  Members:
    CT[2:0] - Interrupt request generation timing 
    1SE     - Enable / Disable 1 second interrupt generation 
    ALME    - Enable / Disable alarm interrupt request signal
    HZE     - Enable / Disable 1Hz output pulse

  defines the bit position (POS) inside the register
  and the bit width (WID)
*/
#define R_RTCA_CTL1_CT_POS     0
#define R_RTCA_CTL1_CT_WID     3
#define R_RTCA_CTL1_1SE_POS    3
#define R_RTCA_CTL1_1SE_WID    1
#define R_RTCA_CTL1_ALME_POS   4
#define R_RTCA_CTL1_ALME_WID   1
#define R_RTCA_CTL1_HZE_POS    5
#define R_RTCA_CTL1_HZE_WID    1


#define R_RTCA_CTL2    0x08
/* Content of control register CTL2
  Members:
 
    WAIT    - Restart / Stop all clock counters
    WST     - Status of all clock counters
    RSUB    - Enable / Disable transfer value from SUBC to SRBU
    RSST    - Indicates status of transfer from SUBC to SRBU
    WSST    - Indicates status of SCMP write operation
    WUST    - Indicates status of SUBU write operation

  defines the bit position (POS) inside the register
  and the bit width (WID)
*/
#define R_RTCA_CTL2_WAIT_POS   0
#define R_RTCA_CTL2_WAIT_WID   1
#define R_RTCA_CTL2_WST_POS    1
#define R_RTCA_CTL2_WST_WID    1
#define R_RTCA_CTL2_RSUB_POS   2
#define R_RTCA_CTL2_RSUB_WID   1
#define R_RTCA_CTL2_RSST_POS   3
#define R_RTCA_CTL2_RSST_WID   1
#define R_RTCA_CTL2_WSST_POS   4
#define R_RTCA_CTL2_WSST_WID   1
#define R_RTCA_CTL2_WUST_POS   5
#define R_RTCA_CTL2_WUST_WID   1


#define R_RTCA_SUBC    0x0c
#define R_RTCA_SRBU    0x10
#define R_RTCA_SCMP    0x3C

#define R_RTCA_SUBU    0x38
/* Content of control register SUBU
  Members:

    F[5:0]  - Error Correction Value
    F6      - Increment / Decrement flag
    DEV     - Error correction occurence

  defines the bit position (POS) inside the register
  and the bit width (WID)
*/
#define R_RTCA_SUBU_F_POS     0
#define R_RTCA_SUBU_F_WID     6
#define R_RTCA_SUBU_F6_POS    6
#define R_RTCA_SUBU_F6_WID    1
#define R_RTCA_SUBU_DEV_POS   7
#define R_RTCA_SUBU_DEV_WID   1


#define R_RTCA_SEC     0x14
#define R_RTCA_MIN     0x18
#define R_RTCA_HOUR    0x1c
#define R_RTCA_WEEK    0x20
#define R_RTCA_DAY     0x24
#define R_RTCA_MON     0x28
#define R_RTCA_YEAR    0x2c

#define R_RTCA_SECC    0x4c
#define R_RTCA_MINC    0x50
#define R_RTCA_HOURC   0x54
#define R_RTCA_WEEKC   0x58
#define R_RTCA_DAYC    0x5c
#define R_RTCA_MONC    0x60
#define R_RTCA_YEARC   0x64

#define R_RTCA_TIMEC   0x68
#define R_RTCA_TIME    0x30
#define R_RTCA_CALC    0x6c
#define R_RTCA_CAL     0x34

#define R_RTCA_ALM     0x40
#define R_RTCA_ALH     0x44
#define R_RTCA_ALW     0x48
#define R_RTCA_EMU     0x74


#ifdef __cplusplus
}
#endif

#endif /*RTCA_REGS_H_  */


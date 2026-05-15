/*
****************************************************************************
PROJECT : SG regs
FILE    : $Id: r_sg_regs.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
Driver for SG macro
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

#ifndef SG_REGS_H_
#define SG_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: Sg Register Offsets

  Only the driver modules shall include this header.
*/

/***********************************************************
  Section: Global Defines
*/

/***********************************************************
  Constants: Register Offsets

  R_SG_SG0CTL          - Control register
  R_SG_SG0STAT         - Status register (read only)
  R_SG_SG0CONF         - Configuration register
  R_SG_SG0DF           - Duration factor
  R_SG_SG0ADI          - Automatic decrement/increment
  R_SG_SG0ITH          - Interrupt threshold
  R_SG_SG0FL           - Frequency register low
  R_SG_SG0FH           - Frequency register high
  R_SG_SG0PWM          - Amplitude register
  R_SG_SG0EMU          - Emulation register

*/

#define R_SG_SGCTL        0x00
#define R_SG_SGSTAT       0x04
#define R_SG_SGCONF       0x08
#define R_SG_SGDF         0x0C
#define R_SG_SGADI        0x10
#define R_SG_SGITH        0x14
#define R_SG_SGFL         0x18
#define R_SG_SGFH         0x1C
#define R_SG_SGPWM        0x20
#define R_SG_SGEMU        0x24


/***********************************************************
  Type: R_SG_RegCtlBit_t
  
  Content of the Sound Generator Control register R_SG_SG0CTL
  
  Members:
  
    En          - Enable
    ClkDiv      - Clock Divider
    Res         - Reserved
*/  
  
typedef struct {
    uint32_t En     : 1; 
    uint32_t ClkDiv : 3;  
    uint32_t Res    : 28; 
} R_SG_RegCtlBit_t;


/***********************************************************
  Union: R_SG_RegCtl_t
  
  Simplifies Bit and Byte access to register CTL    
  
  Members:
  Bit       - Allow bit access; see: <R_SG_RegCtlBit_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_SG_RegCtlBit_t    Bit;
    uint32_t            Word;
} R_SG_RegCtl_t;
                         

/***********************************************************
  Type: R_SG_RegStatBit_t
  
  Content of the Sound Generator Status register EE_SG_SG0STAT
  
  Members:
  
    Res         - Reserved
    Run         - SG is running
*/  
  
typedef struct {
    uint32_t Run    : 1; 
    uint32_t Res    : 31; 
} R_SG_RegStatBit_t;


/***********************************************************
  Union: R_SG_RegStat_t
  
  Simplifies Bit and Byte access to register STAT
  
  Members:
  Bits      - Allow bit access; see: <R_SG_RegStatBit_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_SG_RegStatBit_t   Bit;
    uint32_t            Word;
} R_SG_RegStat_t;
                         

/***********************************************************
  Type: R_SG_RegConfBit_t
  
  Content of the Sound Generator Control register EE_SG_SG0CONF
  
  Members:
  
    Mode        - Mode
    OS          - Sound Output Selection
    BE          - Register Buffering Enabled
    SI          - Selection Inverted sound output 
    Res         - Reserved
*/  
  
typedef struct {
    uint32_t Mode   : 2; 
    uint32_t Os     : 1;  
    uint32_t Be     : 1;  
    uint32_t Si     : 1;  
    uint32_t Res    : 27; 
} R_SG_RegConfBit_t;



/***********************************************************
  Union: R_SG_RegConf_t
  
  Simplifies Bit and Byte access to register CONF    
  
  Members:
  Bit       - Allow bit access; see: <R_SG_RegCtl_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_SG_RegConfBit_t   Bit;
    uint32_t            Word;
} R_SG_RegConf_t;
                         
                         
/***********************************************************
  Type: R_SG_Reg_DFBit_t
  
  Duration Factor
  Content of the Sound Generator register EE_SG_SG0DF
  
  Members:
  
    Df          - Duration Factor
    Res         - Reserved
*/  
  
typedef struct {
    uint32_t Df         : 8;  
    uint32_t Res        : 24; 
} R_SG_RegDFBit_t;



/***********************************************************
  Union: R_SG_RegDf_t
  
  Simplifies Bit and Byte access to register CONF    
  
  Members:
  Bit       - Allow bit access; see: <R_SG_RegDFBit_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_SG_RegDFBit_t     Bit;
    uint32_t            Word;
} R_SG_RegDf_t;
                         

/***********************************************************
  Type: R_SG_Reg_ADIBit_t
  
  Automatic Decrement/Increment
  Content of the Sound Generator Control register EE_SG_SG0ADI
  
  Members:
  
    Adi         - Automatic Decrement/Increment
    Res         - Reserved
*/  
  
typedef struct {
    uint32_t Adi        : 9;  
    uint32_t Res        : 23; 
} R_SG_RegADIBit_t;



/***********************************************************
  Union: R_SG_RegAdi_t
  
  Simplifies Bit and Byte access to register ADI
  
  Members:
  Bit       - Allow bit access; see: <R_SG_RegADIBit_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_SG_RegADIBit_t    Bit;
    uint32_t            Word;
} R_SG_RegAdi_t;
                         


/***********************************************************
  Type: R_SG_Reg_ITHBit_t
  
  Interrupt Threshold Register
  Content of the Sound Generator register EE_SG_SG0ITH
  
  Members:
  
    Ith         - Interrupt Threshold
    Res         - Reserved
*/  
  
typedef struct {
    uint32_t Ith        : 9;  
    uint32_t Res        : 23; 
} R_SG_RegIthBit_t;



/***********************************************************
  Union: R_SG_RegIth_t
  
  Simplifies Bit and Byte access to register ITH
  
  Members:
  Bit       - Allow bit access; see: <R_SG_RegITHBit_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_SG_RegIthBit_t    Bit;
    uint32_t            Word;
} R_SG_RegIth_t;
                         

/***********************************************************
  Type: R_SG_Reg_FLBit_t
  
  Frequency register Low
  Content of the Sound Generator Control register EE_SG_SG0FL
  
  Members:
  
    Fl          - Freqency register Low
    Res         - Reserved
*/  
  
typedef struct {
    uint32_t Fl         : 9;  
    uint32_t Res        : 23; 
} R_SG_RegFLBit_t;



/***********************************************************
  Union: R_SG_RegFl_t
  
  Simplifies Bit and Byte access to register FL    
  
  Members:
  Bit       - Allow bit access; see: <R_SG_RegFLBit_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_SG_RegFLBit_t     Bit;
    uint32_t            Word;
} R_SG_RegFl_t;
                         


/***********************************************************
  Type: R_SG_Reg_FHBit_t
  
  Frequency register High
  Content of the Sound Generator Control register EE_SG_SG0FH
  
  Members:
  
    Fh          - Freqency register Low
    Res         - Reserved
*/  
  
typedef struct {
    uint32_t Fh         : 9;  
    uint32_t Res        : 23; 
} R_SG_RegFHBit_t;



/***********************************************************
  Union: R_SG_RegFh_t
  
  Simplifies Bit and Byte access to register FH    
  
  Members:
  Bit       - Allow bit access; see: <R_SG_RegFHBit_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_SG_RegFHBit_t     Bit;
    uint32_t            Word;
} R_SG_RegFh_t;
                         

/***********************************************************
  Type: R_SG_Reg_PWMBit_t
  
  Amplitude Register
  Content of the Sound Generator register EE_SG_SG0PWM
  
  Members:
  
    Pwm         - Amplitude (duty cycle for PWM)
    Res         - Reserved
*/  
  
typedef struct {
    uint32_t Pwm        : 9;  
    uint32_t Res        : 23; 
} R_SG_RegPwmBit_t;



/***********************************************************
  Union: R_SG_RegPwm_t
  
  Simplifies Bit and Byte access to register PWM
  
  Members:
  Bit       - Allow bit access; see: <R_SG_RegPwmBit_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_SG_RegPwmBit_t    Bit;
    uint32_t            Word;
} R_SG_RegPwm_t;
                         

/***********************************************************
  Type: R_SG_Reg_SVSDisBit_t
  
  Emulation Register
  Content of the Sound Generator register EE_SG_SG0SVSDIS
  
  Members:
  
    Res         - Reserved
    SvsDis      - SVSTOP disable
*/  
  
typedef struct {
    uint32_t Res        : 7; 
    uint32_t SvsDis     : 1;  
} R_SG_RegSVSDisBit_t;



/***********************************************************
  Union: R_SG_RegSvsDis_t
  
  Simplifies Bit and Byte access to register SVSDIS
  
  Members:
  Bit       - Allow bit access; see: <R_SG_RegSVSDISBit_t>
  Word      - Allow access to the whole long word
*/

typedef union {
    R_SG_RegSVSDisBit_t     Bit;
    uint8_t                Byte;
} R_SG_RegSvsDis_t;
                         






#ifdef __cplusplus
}
#endif

#endif /* SG_REGS_H_  */

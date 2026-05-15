/*
****************************************************************************
PROJECT : TAUB
FILE    : $Id: r_taub_regs.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================
DESCRIPTION
Driver for TAUB macro
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
Purpose: only for testing, not for mass production

DISCLAIMER 

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

SAMPLE CODE is not part of the licensed software, as such it must not be used in
mass-production applications. It can only be used for evaluation and
demonstration purposes at customer’s premises listed in the signed license
agreement.
****************************************************************************
*/


#ifndef R_TAUB_REGS_H_
#define R_TAUB_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: TAUB Register Offsets

  Only the driver modules shall include this header.
*/

/***********************************************************
  Section: Global Defines
*/

/***********************************************************
  Constants: Register Offsets
*/

/* Channel Data Register 0 R/W */
#define R_TAUB_CDR0  0x0000

/* Channel Data Register 1 R/W */
#define R_TAUB_CDR1  0x0004

/* Channel Data Register 2 R/W */
#define R_TAUB_CDR2  0x0008

/* Channel Data Register 3 R/W */
#define R_TAUB_CDR3  0x000C

/* Channel Data Register 4 R/W */
#define R_TAUB_CDR4  0x0010

/* Channel Data Register 5 R/W */
#define R_TAUB_CDR5  0x0014

/* Channel Data Register 6 R/W */
#define R_TAUB_CDR6  0x0018

/* Channel Data Register 7 R/W */
#define R_TAUB_CDR7  0x001C

/* Channel Data Register 8 R/W */
#define R_TAUB_CDR8  0x0020

/* Channel Data Register 9 R/W */
#define R_TAUB_CDR9  0x0024

/* Channel Data Register 10 R/W */
#define R_TAUB_CDR10  0x0028

/* Channel Data Register 11 R/W */
#define R_TAUB_CDR11  0x002C

/* Channel Data Register 12 R/W */
#define R_TAUB_CDR12  0x0030

/* Channel Data Register 13 R/W */
#define R_TAUB_CDR13  0x0034

/* Channel Data Register 14 R/W */
#define R_TAUB_CDR14  0x0038

/* Channel Data Register 15 R/W */
#define R_TAUB_CDR15  0x003C


/* Channel Counter Register 0 R */
#define R_TAUB_CNT0  0x0080

/* Channel Counter Register 1 R */
#define R_TAUB_CNT1  0x0084

/* Channel Counter Register 2 R */
#define R_TAUB_CNT2  0x0088

/* Channel Counter Register 3 R */
#define R_TAUB_CNT3  0x008C

/* Channel Counter Register 4 R */
#define R_TAUB_CNT4  0x0090

/* Channel Counter Register 5 R */
#define R_TAUB_CNT5  0x0094

/* Channel Counter Register 6 R */
#define R_TAUB_CNT6  0x0098

/* Channel Counter Register 7 R */
#define R_TAUB_CNT7  0x009C

/* Channel Counter Register 8 R */
#define R_TAUB_CNT8  0x00A0

/* Channel Counter Register 9 R */
#define R_TAUB_CNT9  0x00A4

/* Channel Counter Register 10 R */
#define R_TAUB_CNT10 0x00A8

/* Channel Counter Register 11 R */
#define R_TAUB_CNT11 0x00AC

/* Channel Counter Register 12 R */
#define R_TAUB_CNT12 0x00B0

/* Channel Counter Register 13 R */
#define R_TAUB_CNT13 0x00B4

/* Channel Counter Register 14 R */
#define R_TAUB_CNT14 0x00B8

/* Channel Counter Register 15 R */
#define R_TAUB_CNT15 0x00BC


/* Channel Mode OS Register 0 R/W */
#define R_TAUB_CMOR0 0x0200

/* Channel Mode OS Register 1 R/W */
#define R_TAUB_CMOR1 0x0204

/* Channel Mode OS Register 2 R/W */
#define R_TAUB_CMOR2 0x0208

/* Channel Mode OS Register 3 R/W */
#define R_TAUB_CMOR3 0x020C

/* Channel Mode OS Register 4 R/W */
#define R_TAUB_CMOR4 0x0210

/* Channel Mode OS Register 5 R/W */
#define R_TAUB_CMOR5 0x0214

/* Channel Mode OS Register 6 R/W */
#define R_TAUB_CMOR6 0x0218

/* Channel Mode OS Register 7 R/W */
#define R_TAUB_CMOR7 0x021C

/* Channel Mode OS Register 8 R/W */
#define R_TAUB_CMOR8 0x0220

/* Channel Mode OS Register 9 R/W */
#define R_TAUB_CMOR9 0x0224

/* Channel Mode OS Register 10 R/W */
#define R_TAUB_CMOR10 0x0228

/* Channel Mode OS Register 11 R/W */
#define R_TAUB_CMOR11 0x022C

/* Channel Mode OS Register 12 R/W */
#define R_TAUB_CMOR12 0x0230

/* Channel Mode OS Register 13 R/W */
#define R_TAUB_CMOR13 0x0234

/* Channel Mode OS Register 14 R/W */
#define R_TAUB_CMOR14 0x0238

/* Channel Mode OS Register 15 R/W */
#define R_TAUB_CMOR15 0x023C

/*  Content of the R_TAUB_CMORn: TAU Channel Mode OS Register

  Members:
    CKS  - CKS1 and CKS0: Clock Select Bits
    CCS0 - CCS0: Count Clock Select Bits
    MAS  - MAS: Master Mode Bit
    STS  - STS2 to STS0: Start Trigger Select Bits
    COS  - COS1 and COS0: Capture Overflow Select Bits
    MD   - MD4 to MD1: Mode Control Bits
    MD0  - MD0: Mode Control Bits
  defines the bit position (POS) inside the register
  and the bit width (WID)
*/
#define R_TAUB_CMOR_CKS_POS  14
#define R_TAUB_CMOR_CCS0_POS 12
#define R_TAUB_CMOR_MAS_POS  11
#define R_TAUB_CMOR_STS_POS   8
#define R_TAUB_CMOR_COS_POS   6
#define R_TAUB_CMOR_MD_POS    1
#define R_TAUB_CMOR_MD0_POS   0
#define R_TAUB_CMOR_CKS_WID   2
#define R_TAUB_CMOR_CCS0_WID  1
#define R_TAUB_CMOR_MAS_WID   1
#define R_TAUB_CMOR_STS_WID   3
#define R_TAUB_CMOR_COS_WID   2
#define R_TAUB_CMOR_MD_WID    4
#define R_TAUB_CMOR_MD0_WID   1

/* Channel Mode User Register 0 R/W */
#define R_TAUB_CMUR0 0x00C0

/* Channel Mode User Register 1 R/W */
#define R_TAUB_CMUR1 0x00C4

/* Channel Mode User Register 2 R/W */
#define R_TAUB_CMUR2 0x00C8

/* Channel Mode User Register 3 R/W */
#define R_TAUB_CMUR3 0x00CC

/* Channel Mode User Register 4 R/W */
#define R_TAUB_CMUR4 0x00D0

/* Channel Mode User Register 5 R/W */
#define R_TAUB_CMUR5 0x00D4

/* Channel Mode User Register 6 R/W */
#define R_TAUB_CMUR6 0x00D8

/* Channel Mode User Register 7 R/W */
#define R_TAUB_CMUR7 0x00DC

/* Channel Mode User Register 8 R/W */
#define R_TAUB_CMUR8 0x00E0

/* Channel Mode User Register 9 R/W */
#define R_TAUB_CMUR9 0x00E4

/* Channel Mode User Register 10 R/W */
#define R_TAUB_CMUR10 0x00E8

/* Channel Mode User Register 11 R/W */
#define R_TAUB_CMUR11 0x00EC

/* Channel Mode User Register 12 R/W */
#define R_TAUB_CMUR12 0x00F0

/* Channel Mode User Register 13 R/W */
#define R_TAUB_CMUR13 0x00F4

/* Channel Mode User Register 14 R/W */
#define R_TAUB_CMUR14 0x00F8

/* Channel Mode User Register 15 R/W */
#define R_TAUB_CMUR15 0x00FC

/*  Content of the TAUBCMURn: TAU Channel Mode User Register

  Members:
    TIS  - TIS1 and TIS0: TINn Edge Select Bits

  defines the bit position (POS) inside the register
  and the bit width (WID)
*/
#define R_TAUB_CMUR_TIS_POS  0
#define R_TAUB_CMUR_TIS_WID  2


/* Channel Status Register 0 R */
#define R_TAUB_CSR0 0x0140

/* Channel Status Register 1 R */
#define R_TAUB_CSR1 0x0144

/* Channel Status Register 2 R */
#define R_TAUB_CSR2 0x0148

/* Channel Status Register 3 R */
#define R_TAUB_CSR3 0x014C

/* Channel Status Register 4 R */
#define R_TAUB_CSR4 0x0150

/* Channel Status Register 5 R */
#define R_TAUB_CSR5 0x0154

/* Channel Status Register 6 R */
#define R_TAUB_CSR6 0x0158

/* Channel Status Register 7 R */
#define R_TAUB_CSR7 0x015C

/* Channel Status Register 8 R */
#define R_TAUB_CSR8 0x0160

/* Channel Status Register 9 R */
#define R_TAUB_CSR9 0x0164

/* Channel Status Register 10 R */
#define R_TAUB_CSR10 0x0168

/* Channel Status Register 11 R */
#define R_TAUB_CSR11 0x016C

/* Channel Status Register 12 R */
#define R_TAUB_CSR12 0x0170

/* Channel Status Register 13 R */
#define R_TAUB_CSR13 0x0174

/* Channel Status Register 14 R */
#define R_TAUB_CSR14 0x0178

/* Channel Status Register 15 R */
#define R_TAUB_CSR15 0x017C


/* Channel Status Clear Trigger Register 0 W */
#define R_TAUB_CSC0 0x0180

/* Channel Status Clear Trigger Register 1 W */
#define R_TAUB_CSC1 0x0184

/* Channel Status Clear Trigger Register 2 W */
#define R_TAUB_CSC2 0x0188

/* Channel Status Clear Trigger Register 3 W */
#define R_TAUB_CSC3 0x018C

/* Channel Status Clear Trigger Register 4 W */
#define R_TAUB_CSC4 0x0190

/* Channel Status Clear Trigger Register 5 W */
#define R_TAUB_CSC5 0x0194

/* Channel Status Clear Trigger Register 6 W */
#define R_TAUB_CSC6 0x0198

/* Channel Status Clear Trigger Register 7 W */
#define R_TAUB_CSC7 0x019C

/* Channel Status Clear Trigger Register 8 W */
#define R_TAUB_CSC8 0x01A0

/* Channel Status Clear Trigger Register 9 W */
#define R_TAUB_CSC9 0x01A4

/* Channel Status Clear Trigger Register 10 W */
#define R_TAUB_CSC10 0x01A8

/* Channel Status Clear Trigger Register 11 W */
#define R_TAUB_CSC11 0x01AC

/* Channel Status Clear Trigger Register 12 W */
#define R_TAUB_CSC12 0x01B0

/* Channel Status Clear Trigger Register 13 W */
#define R_TAUB_CSC13 0x01B4

/* Channel Status Clear Trigger Register 14 W */
#define R_TAUB_CSC14 0x01B8

/* Channel Status Clear Trigger Register 15 W */
#define R_TAUB_CSC15 0x01BC

/* Content of the R_TAUB_CSCn: TAU Channel Status Clear Register

   Members:
    CLOV - Clear Overflow Flag Bit

   defines the bit position (POS) inside the register
   and the bit width (WID)
*/
#define R_TAUB_CSC_CLOV_POS  0
#define R_TAUB_CSC_CLOV_WID  1


/* Channel Enable Status Register R */
#define R_TAUB_TE 0x01C0


/* Channel Start Trigger Register W */
#define R_TAUB_TS 0x01C4


/* Channel Stop Trigger Register W */
#define R_TAUB_TT 0x01C8


/* Presaler Clock Select Register R/W */
#define R_TAUB_TPS 0x0240

/* Content of the TAUBTPS: TAU Prescaler Clock Select Register

   Members:
    PRS0 - Prescaler Clock Select Bits for CK0
    PRS1 - Prescaler Clock Select Bits for CK1
    PRS2 - Prescaler Clock Select Bits for CK2
    PRS3 - Prescaler Clock Select Bits for CK3
   defines the bit position (POS) inside the register
   and the bit width (WID)
*/
#define R_TAUB_TPS_PRS3_POS  12
#define R_TAUB_TPS_PRS3_WID   4
#define R_TAUB_TPS_PRS2_POS   8
#define R_TAUB_TPS_PRS2_WID   4
#define R_TAUB_TPS_PRS1_POS   4
#define R_TAUB_TPS_PRS1_WID   4
#define R_TAUB_TPS_PRS0_POS   0
#define R_TAUB_TPS_PRS0_WID   4


/* Channel Output Register R/W */
#define R_TAUB_TO 0x0058


/* Channel Output Enable Register R/W */
#define R_TAUB_TOE 0x005C


/* Channel Output Mode Register R/W */
#define R_TAUB_TOM 0x0248


/* Channel Output Configuration Register R/W */
#define R_TAUB_TOC 0x024C


/* Channel Output Active Level Register R/W */
#define R_TAUB_TOL 0x0040


/* Channel Dead Time Output Enable Register R/W */
#define R_TAUB_TDE  0x0250


/* Channel Dead Time Output Level Register R/W */
#define R_TAUB_TDL 0x0054


/* Channel Reload Data Enable Register R/W */
#define R_TAUB_RDE 0x0260


/* Channel Reload Data Mode Register R/W */
#define R_TAUB_RDM 0x0264


/* Channel Reload Data Control CH Select Register R/W */
#define R_TAUB_RDS 0x0268


/* Channel Reload Data Control Register R/W */
#define R_TAUB_RDC 0x026C


/* Channel Reload Data Trigger Register W */
#define R_TAUB_RDT 0x0044


/* Channel Reload Status Register R */
#define R_TAUB_RSF 0x0048


/* TAU Emulation Register R/W */
#define R_TAUB_EMU 0x0290



#endif /* TAUB_REGS_H_  */

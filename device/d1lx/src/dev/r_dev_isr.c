/*
****************************************************************************
PROJECT : device driver for V-Lib
FILE    : $Id: r_dev_isr.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================
DESCRIPTION
r7f701412 interrupt service routines
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2013 - 2014
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
demonstration purposes at customer's premises listed in the signed license
agreement.

****************************************************************************
*/


/*******************************************************************************
  Title: r7f701412 interrupt serivicing 

  
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"


/* The debug output for register accesses shall be only active 
on special request, otherwise the output might get "overloaded"  
If you want to see all register accesses, activate R_DBG_SHOW_INT_REGS
in your application's Makefile */
#ifndef R_DBG_SHOW_INT_REGS
  #undef  R_DBG_PRINT_DEV_REGS
#endif   
#include "r_dev_api.h"

/*******************************************************************************
  Section: Local Constants 
*/
/* FENMI factor status and clear registers */
#define LOC_FENMIF                  0xFF450000u      /* FENMI factor register */
#define LOC_FENMIFC                 0xFF450008u      /* FENMI factor clear register */

/* FENMI factor status and clear register bits */
#define LOC_NMI_FACTOR_NMI0F        0x1u
#define LOC_NMI_FACTOR_WDTA0NMIF    0x2u
#define LOC_NMI_FACTOR_WDTA1NMIF    0x4u
#define LOC_NMI_FACTOR_BAEXCNMIF    0x8u

/* gfxss0 Slave IP error capture and clear registers */
#define LOC_GSSLVERR                0xFF834830u
#define LOC_GSSLVERRC               0xFF834834u
#define LOC_GSSLPERR                0xFF834838u
#define LOC_GSMPUERR1               0xFF834828u
#define LOC_GSMPUERR0               0xFF834820u

/* gfxss0 Slave IP error capture and clear register bits */
#define LOC_GSSLVERR_SFMA0TTOUT   0x01u
#define LOC_GSSLVERR_SFMA1TTOUT   0x02u
#define LOC_GSSLVERR_VI0AHEINT      0x04u
#define LOC_GSSLVERR_SVO0MBE        0x80u


/*******************************************************************************
  Section: Local types 
*/
/*******************************************************************************
  Constant: loc_IntCtrl
  
  Stores the IC register address of all interrupts
  
*/
static const uint32_t loc_IntCtrl[R_DEV_INT_LAST] =
{
     0xFFFEEA00u,  
     0xFFFEEA02u,  
     0xFFFEEA04u,  
     0xFFFEEA06u,  
     0xFFFEEA08u,  
     0xFFFEEA0Au,    
     0xFFFEEA0Cu,  
     0xFFFEEA0Eu,  
     0xFFFEEA10u,  
     0xFFFEEA12u,  
     0xFFFEEA14u,  
     0xFFFEEA16u,  
     0xFFFEEA18u,  
     0xFFFEEA1Au,  
     0xFFFEEA1Cu,  
     0xFFFEEA1Eu,  
     0xFFFEEA20u,  
     0xFFFEEA22u,  
     0xFFFEEA24u,  
     0xFFFEEA26u,  
     0xFFFEEA28u,  
     0xFFFEEA2Au,  
     0xFFFEEA2Cu,  
     0xFFFEEA2Eu,  
     0xFFFEEA30u,  
     0xFFFEEA32u,  
     0xFFFEEA34u,  
     0xFFFEEA36u,  
     0xFFFEEA38u,  
     0xFFFEEA3Au,  
     0xFFFEEA3Cu,  
     0xFFFEEA3Eu,  
     0xFFFFB040u,  
     0xFFFFB042u,  
     0xFFFFB044u,  
     0xFFFFB046u,  
     0xFFFFB048u,  
     0xFFFFB04Au,  
     0xFFFFB04Cu,  
     0xFFFFB04Eu,  
     0xFFFFB050u,  
     0xFFFFB052u,  
     0xFFFFB054u,  
     0xFFFFB056u,  
     0xFFFFB058u,  
     0xFFFFB05Au,  
     0xFFFFB05Cu,  
     0xFFFFB05Eu,  
     0xFFFFB060u,  
     0xFFFFB062u,  
     0xFFFFB064u,  
     0xFFFFB066u,  
     0xFFFFB068u,  
     0xFFFFB06Au,  
     0xFFFFB06Cu,  
     0xFFFFB06Eu,  
     0xFFFFB070u,  
     0xFFFFB072u,  
     0xFFFFB074u,  
     0xFFFFB076u,  
     0xFFFFB078u,  
     0xFFFFB07Au,  
     0xFFFFB07Cu,  
     0xFFFFB07Eu,  
     0xFFFFB080u,  
     0xFFFFB082u,  
     0xFFFFB084u,  
     0xFFFFB086u,  
     0xFFFFB088u,  
     0xFFFFB08Au,  
     0xFFFFB08Cu,  
     0xFFFFB08Eu,  
     0xFFFFB090u,  
     0xFFFFB092u,  
     0xFFFFB094u,  
     0xFFFFB096u,  
     0xFFFFB098u,  
     0xFFFFB09Au,  
     0xFFFFB09Cu,  
     0xFFFFB09Eu,  
     0xFFFFB0A0u,  
     0xFFFFB0A2u,  
     0xFFFFB0A4u,  
     0xFFFFB0A6u,  
     0xFFFFB0A8u,  
     0xFFFFB0AAu,  
     0xFFFFB0ACu,  
     0xFFFFB0AEu,  
     0xFFFFB0B0u,  
     0xFFFFB0B2u,  
     0xFFFFB0B4u,  
     0xFFFFB0B6u,  
     0xFFFFB0B8u,  
     0xFFFFB0BAu,  
     0xFFFFB0BCu,  
     0xFFFFB0BEu,  
     0xFFFFB0C0u,  
     0xFFFFB0C2u,  
     0xFFFFB0C4u,  
     0xFFFFB0C6u,  
     0xFFFFB0C8u,  
     0xFFFFB0CAu,  
     0xFFFFB0CCu,  
     0xFFFFB0CEu,  
     0xFFFFB0D0u,  
     0xFFFFB0D2u,  
     0xFFFFB0D4u,  
     0xFFFFB0D6u,  
     0xFFFFB0D8u,  
     0xFFFFB0DAu,  
     0xFFFFB0DCu,  
     0xFFFFB0DEu,  
     0xFFFFB0E0u,  
     0xFFFFB0E2u,  
     0xFFFFB0E4u,  
     0xFFFFB0E6u,  
     0xFFFFB0E8u,  
     0xFFFFB0EAu,  
     0xFFFFB0ECu,  
     0xFFFFB0EEu,  
     0xFFFFB0F0u,  
     0xFFFFB0F2u,  
     0xFFFFB0F4u,  
     0xFFFFB0F6u,  
     0xFFFFB0F8u,  
     0xFFFFB0FAu,  
     0xFFFFB0FCu,  
     0xFFFFB0FEu,  
     0xFFFFB100u,  
     0xFFFFB102u,  
     0xFFFFB104u,                          
     0xFFFFB106u,                          
     0xFFFFB108u,  
     0xFFFFB10Au,  
     0xFFFFB10Cu,  
     0xFFFFB10Eu,  
     0xFFFFB110u,  
     0xFFFFB112u,  
     0xFFFFB114u,  
     0xFFFFB116u,  
     0xFFFFB118u,  
     0xFFFFB11Au,  
     0xFFFFB11Cu,  
     0xFFFFB11Eu,  
     0xFFFFB120u,  
     0xFFFFB122u,  
     0xFFFFB124u,  
     0xFFFFB126u,  
     0xFFFFB128u,  
     0xFFFFB12Au,  
     0xFFFFB12Cu,  
     0xFFFFB12Eu,  
     0xFFFFB130u,  
     0xFFFFB132u,  
     0xFFFFB134u,  
     0xFFFFB136u,  
     0xFFFFB138u,  
     0xFFFFB13Au,  
     0xFFFFB13Cu,  
     0xFFFFB13Eu,  
     0xFFFFB140u,  
     0xFFFFB142u,  
     0xFFFFB144u,  
     0xFFFFB146u,  
     0xFFFFB148u,  
     0xFFFFB14Au,  
     0xFFFFB14Cu,  
     0xFFFFB14Eu,  
     0xFFFFB150u,  
     0xFFFFB152u,  
     0xFFFFB154u,  
     0xFFFFB156u,  
     0xFFFFB158u,  
     0xFFFFB15Au,  
     0xFFFFB15Cu,  
     0xFFFFB15Eu,  
     0xFFFFB160u,  
     0xFFFFB162u,  
     0xFFFFB164u,  
     0xFFFFB166u,  
     0xFFFFB168u,  
     0xFFFFB16Au,  
     0xFFFFB16Cu,  
     0xFFFFB16Eu,  
     0xFFFFB170u,  
     0xFFFFB172u,  
     0xFFFFB174u,  
     0xFFFFB176u,  
     0xFFFFB178u,  
     0xFFFFB17Au,  
     0xFFFFB17Cu,  
     0xFFFFB17Eu,  
     0xFFFFB180u,  
     0xFFFFB182u,  
     0xFFFFB184u,  
     0xFFFFB186u,  
     0xFFFFB188u,  
     0xFFFFB18Au,  
     0xFFFFB18Cu,  
     0xFFFFB18Eu,  
     0xFFFFB190u,  
     0xFFFFB192u,  
     0xFFFFB194u,  
     0xFFFFB196u,  
     0xFFFFB198u,  
     0xFFFFB19Au,  
     0xFFFFB19Cu,  
     0xFFFFB19Eu,  
     0xFFFFB1A0u,  
     0xFFFFB1A2u,  
     0xFFFFB1A4u,  
     0xFFFFB1A6u,  
     0xFFFFB1A8u,  
     0xFFFFB1AAu,  
     0xFFFFB1ACu,  
     0xFFFFB1AEu,  
     0xFFFFB1B0u,  
     0xFFFFB1B2u,  
     0xFFFFB1B4u,  
     0xFFFFB1B6u,  
     0xFFFFB1B8u,  
     0xFFFFB1BAu,  
     0xFFFFB1BCu,  
     0xFFFFB1BEu,  
     0xFFFFB1C0u,  
     0xFFFFB1C2u,  
     0xFFFFB1C4u,  
     0xFFFFB1C6u,  
     0xFFFFB1C8u,  
     0xFFFFB1CAu,  
     0xFFFFB1CCu,  
     0xFFFFB1CEu,  
     0xFFFFB1D0u,  
     0xFFFFB1D2u,  
     0xFFFFB1D4u,  
     0xFFFFB1D6u,  
     0xFFFFB1D8u,  
     0xFFFFB1DAu,  
     0xFFFFB1DCu,  
     0xFFFFB1DEu,  
     0xFFFFB1E0u,  
     0xFFFFB1E2u,  
     0xFFFFB1E4u,  
     0xFFFFB1E6u,  
     0xFFFFB1E8u,  
     0xFFFFB1EAu,  
     0xFFFFB1ECu,  
     0xFFFFB1EEu,  
     0xFFFFB1F0u,  
     0xFFFFB1F2u,  
     0xFFFFB1F4u,  
     0xFFFFB1F6u,  
     0xFFFFB1F8u,  
     0xFFFFB1FAu,  
     0xFFFFB1FCu,  
     0xFFFFB1FEu,

};


/*******************************************************************************
  Section: Local functions
*/
/* Notify function for other modules */
static void (*loc_NmiCallback)(void);
static uint32_t loc_FeNmiIsr(void);


/*******************************************************************************
  Function: R_DEV_IntSys_FeNmiIsr

  local ISR for handling the FE-NMI interrupt(s) 
  Parameters:
  void
  
  Returns:
  NMI code (reason factor) 
  
*/
static uint32_t loc_FeNmiIsr(void) 
{
    uint32_t nmifactor, subfactor;
    
    nmifactor = R_DEV_READ_REG(32, LOC_FENMIF);
    if ((nmifactor & LOC_NMI_FACTOR_NMI0F) != 0u) 
    {
        /* User can implement handling of NMI0F factor here. */
        R_DEV_WRITE_REG(32, LOC_FENMIFC, LOC_NMI_FACTOR_NMI0F);
        for (;;)
        {
        }
    }
    if ((nmifactor & LOC_NMI_FACTOR_WDTA0NMIF) != 0u) 
    {
        /* User can implement handling of WDTA0NMIF factor here. */
        R_DEV_WRITE_REG(32, LOC_FENMIFC, LOC_NMI_FACTOR_WDTA0NMIF);
        for (;;)
        {
        }
    }
    if ((nmifactor & LOC_NMI_FACTOR_WDTA1NMIF) != 0u) 
    {
        /* User can implement handling of WDTA1NMIF factor here. */
        R_DEV_WRITE_REG(32, LOC_FENMIFC, LOC_NMI_FACTOR_WDTA1NMIF);
        for (;;)
        {
        }
    }
    if ((nmifactor & LOC_NMI_FACTOR_BAEXCNMIF) != 0u) 
    {
        /* handle (gfxss) bus access exceptions (all BAECXNMI sources) */
        
        /* list of all BAECXNMI sources:
           GSSLPERR    (EMC0 - SDECC1ERR to SDECC2ERR, ILASDM1ERR to ILASDM4ERR)
           GSSLVERR    (SVO0MBEINT, VI0AHBEINT, SFMA1TTOUT, SFMA0TTOUT)
           GSMPUERR1   (SFMA1MPUERR, SFMA0MPUERR, EMCMPUERR, CPUHBSMPUERR)
           GSMPUERR0   (GMPU3)
        */
        
        /* from EMC0
           GSSLPERR    (EMC0 - SDECC1ERR to SDECC2ERR, ILASDM1ERR to ILASDM4ERR) */
        subfactor = R_DEV_READ_REG(32, LOC_GSSLPERR);
        if (subfactor != 0u) 
        {
            /* User can implement handling of GSSLPERR factor here. */
            for (;;)
            {
                /* EMC0 error */
            }
        }
               
        /* GSSLVERR from SVO, VI and SFMA
            (SVO0MBEINT, VI0AHBEINT, SFMA1TTOUT, SFMA0TTOUT) */
        subfactor = R_DEV_READ_REG(32, LOC_GSSLVERR);
        if ((subfactor & LOC_GSSLVERR_SFMA0TTOUT) != 0u) 
        {
            /* User can implement handling of SFMA0TTOUT factor here. */
            R_DEV_WRITE_REG(32, LOC_GSSLVERRC, LOC_GSSLVERR_SFMA0TTOUT);
            for (;;)
            {
            }
        }
        if ((subfactor & LOC_GSSLVERR_SFMA1TTOUT) != 0u) 
        {
            /* User can implement handling of SFMA1TTOUT factor here. */
            R_DEV_WRITE_REG(32, LOC_GSSLVERRC, LOC_GSSLVERR_SFMA1TTOUT);
            for (;;)
            {
            }
        }
        if ((subfactor & LOC_GSSLVERR_VI0AHEINT) != 0u) 
        {
            R_DEV_WRITE_REG(32, LOC_GSSLVERRC, LOC_GSSLVERR_VI0AHEINT);
        }
        if ((subfactor & LOC_GSSLVERR_SVO0MBE) != 0u) 
        {
            R_DEV_WRITE_REG(32, LOC_GSSLVERRC, LOC_GSSLVERR_SVO0MBE);
        }
        R_DEV_WRITE_REG(32, LOC_FENMIFC, LOC_NMI_FACTOR_BAEXCNMIF);
        
        subfactor = R_DEV_READ_REG(32, LOC_GSMPUERR1);
        if (subfactor != 0u) 
        {
            for (;;)
            {
                /* User can implement handling of GMPU4, GMPU3,
                                           GMPU1, and GMPU0 factors here. */
                /* error: (protection violation errors)
                     SFMA1MPUERR (GMPU4),
                     SFMA0MPUERR (GMPU3),
                     EMCMPUERR   (GMPU1) or
                     CPUHBSMPUERR  (GMPU0) */
            }
        }
        
        /*  GSMPUERR0   (GMPU3)*/
        subfactor = R_DEV_READ_REG(32, LOC_GSMPUERR0);
        if (subfactor != 0u) 
        {
            for (;;)
            {
                /* User can implement handling of GMPU3 factor here. */
                /* GMPU3 error (unmapped access errors) */
                /* Hint: You might have used a DS2.0 device with the
                         XBus limitation, but did not activated the
                         software GFXBUS_WORKAROUND */

            
            }
        }
    }
    return (nmifactor);
}

/*******************************************************************************
  Section: Global functions
*/

/*******************************************************************************
  Function: R_DEV_FeNmiIsr

  See: <r_dev_api.h> for details 
*/
void R_DEV_FeNmiIsr(void)
{    
    uint32_t ret;
    /* handle FENMI via device specific sys callback */
    ret = loc_FeNmiIsr();
    if (0u == ret)
    {        
        /* otherwise use standard callback */
        if (0u != loc_NmiCallback)
        {
            loc_NmiCallback();
        } else 
        {
            for (;;)
            {
                /* FeNmi Error occurred */
            }
        }
    }
}


/*******************************************************************************
  Function: void R_DEV_IntSetPrio

  See: <r_dev_api.h> for details 
*/

void R_DEV_IntSetPrio(r_dev_IntSel_t Int, uint8_t Prio) 
{
    uint8_t regval = 0 ;
    R_DBG_PRINT(R_DBG_MSG_DET, "Setting Interrupt priority"); 
    /* first read the actual value */
    regval = R_DEV_READ_REG(8, loc_IntCtrl[Int]); 
    /* modifiy value */
    regval &= 0xF0u;
    regval |= Prio; 
    /* write the new value */
    R_DEV_WRITE_REG(8, loc_IntCtrl[Int], regval); 
}

/*******************************************************************************
  Function: void R_DEV_IntClearFlag

  See: <r_dev_api.h> for details 
*/

void R_DEV_IntClearFlag(r_dev_IntSel_t Int) 
{
    uint8_t regval = 0 ;
    if (Int < R_DEV_INT_LAST) 
    {
        /* first read the actual value */
        regval = R_DEV_READ_REG(8, loc_IntCtrl[Int]+1); 
        /* modifiy value */
        regval &= (uint8_t)(~0x10u);
        /* write the new value */
        R_DEV_WRITE_REG(8, loc_IntCtrl[Int]+1u, regval); 
    }
    else
    {
        R_DEV_ERROR((uint32_t)Int, R_DEV_ERR_INTR, "We do not have this interrupt!");  
    }

}

/*******************************************************************************
  Function: void R_DEV_IntSetMode

  See: <r_dev_api.h> for details 
*/

void R_DEV_IntSetMode(r_dev_IntSel_t Int, uint8_t Mode) 
{
    uint8_t regval = 0 ;
    uint8_t check  = 0 ;
    if (Int < R_DEV_INT_LAST) 
    {
        /* first read the actual value */
        regval = R_DEV_READ_REG(8, loc_IntCtrl[Int]); 
        if (R_DEV_INT_DIRECT_BRANCH == Mode)
        {
            /* modifiy value */
            regval &= (uint8_t)(~((uint8_t)0x40u));
        }
        else 
        {
            /* modifiy value */
            regval |= (uint8_t)0x40u;
            
        }
        /* write the new value */
        R_DEV_WRITE_REG(8, loc_IntCtrl[Int], regval); 
        check = R_DEV_READ_REG(8, loc_IntCtrl[Int]);
        if (regval != check)
        {
            R_DEV_ERROR((uint32_t)Int, R_DEV_ERR_INTR, "Setting mismatch!");  
        }     

    }
    else
    {
        R_DEV_ERROR((uint32_t)Int, R_DEV_ERR_INTR, "We do not have this interrupt!");  
    }


}

/*******************************************************************************
  Function: void R_DEV_IntSetFlag

  See: <r_dev_api.h> for details 
*/

void R_DEV_IntSetFlag(r_dev_IntSel_t Int) 
{
    uint8_t regval = 0 ;
    if (Int < R_DEV_INT_LAST) 
    {
        /* first read the actual value */
        regval = R_DEV_READ_REG(8, loc_IntCtrl[Int]+1); 
        /* modifiy value */
        regval |= (uint8_t)0x10u;
        /* write the new value */
        R_DEV_WRITE_REG(8, loc_IntCtrl[Int]+1u, regval); 
    }
    else
    {
        R_DEV_ERROR((uint32_t)Int, R_DEV_ERR_INTR, "We do not have this interrupt!");  
    }

}

/*******************************************************************************
  Function: void R_DEV_IntGetFlag

  See: <r_dev_api.h> for details 
*/

uint8_t R_DEV_IntGetFlag(r_dev_IntSel_t Int) 
{
    uint8_t regval = 0 ;
    if (Int < R_DEV_INT_LAST) 
    {
        /* first read the actual value */
        regval = R_DEV_READ_REG(8, loc_IntCtrl[Int]+1); 
        regval = (regval >> 4u) & 0x01u;
    }
    else
    {
        R_DEV_ERROR((uint32_t)Int, R_DEV_ERR_INTR, "We do not have this interrupt!");  
    }
    return regval; 

}

/*******************************************************************************
  Function: void R_DEV_IntGetMask

  See: <r_dev_api.h> for details 
*/

uint8_t R_DEV_IntGetMask(r_dev_IntSel_t Int) 
{
    uint8_t regval = 0 ;
    if (Int < R_DEV_INT_LAST) 
    {
        /* first read the actual value */
        regval = R_DEV_READ_REG(8, loc_IntCtrl[Int]+1); 
        regval = (regval >> 7u) & 0x01u;
    }
    else
    {
        R_DEV_ERROR((uint32_t)Int, R_DEV_ERR_INTR, "We do not have this interrupt!");  
    }
    return regval; 
}

/*******************************************************************************
  Function: R_DEV_IntEnable

  See: <r_dev_api.h> for details 
*/
void R_DEV_IntEnable(r_dev_IntSel_t Int, uint8_t Enable) 
{
    uint8_t regval = 0 ;
    
    /* first read the actual value */
    regval = R_DEV_READ_REG(8, loc_IntCtrl[Int]); 

    /* clear bit 7 to enable the interrupt */
    if (0u != Enable) 
    {
        regval &= (uint8_t)(~(0x80u)); 
    } 
    /* Set bit 7 to disable interrupt */
    else {
        regval |= 0x80u; 
    }
    /* write the new value */
    R_DEV_WRITE_REG(8, loc_IntCtrl[Int], regval); 

}


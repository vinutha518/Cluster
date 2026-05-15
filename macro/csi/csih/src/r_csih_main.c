/*
****************************************************************************
PROJECT : CSIH driver
FILE    : $Id: r_csih_main.c 7324 2016-01-22 10:05:32Z matthias.nippert $
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


/*******************************************************************************
  Title: Main Module

  Implementation of the API and other global functions.
*/

/*
  CSIH terminology

  Transmitter
  Receiver
  Master
  Slave
  Multi-master
  Arbitration
  Synchronization

  START and STOP condition


  Transfers

*/

#if defined (__ghs__)
#pragma ghs startnomisra
#endif
/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_cdi_api.h"
#include "r_config_csih.h"
#include "r_csih_regs.h"
#include "r_csih_api.h"
#include "r_csih_sys.h"
#define CSIH_MAIN_GLOBALS_
#include "r_csih_main.h"


/*******************************************************************************
  Section: Local Defines
*/

/*******************************************************************************
  Constants: Baudrate setting limits

  LOC_CSIH_MAX_DIVIDER      -  Max value of divider k in
                                CTL2 register.

  LOC_CSIH_MAX_PRESCALER    -  Max value of divider m in
                                CTL2 register.
*/

#define LOC_CSIH_MAX_DIVIDER            4095u /* CsihBrs0-11*/
#define LOC_CSIH_MIN_DIVIDER            1u    /* CsihBrs0-11*/
#define LOC_CSIH_MAX_PRESCALER          7u    /* CsihPrs0-2*/

#define LOC_CSIH_SETUP_T_SET_NUM        16u
#define LOC_CSIH_INTER_DATA_T_SET_NUM   16u
#define LOC_CSIH_HOLD_T_SET_NUM         16u
#define LOC_CSIH_IDLE_SET_NUM           8u

#define LOC_CSIH_NB_BYTES(n) ( ( ((n)-1u)>>3u) + 1u)

#define LOC_CSIH_NULL                   ((void*)0)
#define LOC_CSIH_TRUE                   1u
#define LOC_CSIH_FALSE                  0

/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: loc_ErrorHandler

  A function pointer to a user callback (Applications error handler)

  The value is assigned by the application using the driver


*/
static void ( *loc_ErrorHandler )(uint32_t Unit, uint32_t Error) = LOC_CSIH_NULL;


static const uint16_t loc_InterDataSettings[] = {0u, 5u, 10u, 10u, 20u, 30u, 40u, 60u, 80u, 90u, 100u, 110u, 120u, 140u, 160u, 180u, 200u};
static const uint16_t loc_SpIdlHdSettings[]   = {5u, 10u, 15u, 25u, 35u, 45u, 65u, 85u, 95u, 105u, 115u, 125u, 145u, 165u, 185u, 205u};


/*******************************************************************************
  Section: Local Functions
*/


/*******************************************************************************
  Function: loc_csih_BufPush
*/

int32_t loc_csih_BufPush (r_csih_Dev_t *Dev, uint8_t Address, r_csih_FrameS_t *Frame) 
{
    uint8_t  nbdata, cnt;
    uint32_t basechba;
    uint16_t *data;
    r_csih_RegTxw_t   txw;
    r_csih_RegMRwp0_t mrwp;

    basechba = Dev->BaseAddrChba;

    data     = Frame->Data;
    nbdata   = Frame->NbData;
    cnt      = 0;

    /* MNI: This code block writes to a variable provided by the application.
            As long as we don't expect data to be delivered by the driver, this is undesired.
            The actions have been pulled into the code below. */
#if 0
    if (Dev->Mode == R_CSIH_SLAVE) 
    {
        Frame->CsSel.Byte = 0;
        Frame->IntReq     = LOC_CSIH_FALSE;
        Frame->Eoj        = LOC_CSIH_FALSE;
    }
#endif

    /* In case of CIRE or EOJ, decrease counter
     * as the last data must be done
     * separately
     */
    if ( (Dev->Mode != R_CSIH_SLAVE) && (Frame->IntReq || Frame->Eoj) )
    {
        nbdata--;
    }

    /* Set offset address */
    mrwp.Word     = R_CSIH_READ_REG(32, basechba + R_CSIH_MRWP);
    mrwp.Bit.Trwa = Address;
    R_CSIH_WRITE_REG(32, basechba + R_CSIH_MRWP, mrwp.Word);

    if (Dev->Mode == R_CSIH_SLAVE) 
    {
        /* According to UM, set bits to 0xFE for slave mode. */
        txw.Word    = ~(1);
    }
    else
    {
        txw.Word    = ~(Frame->CsSel.Byte);
    }
    txw.Word    = (txw.Word << 16) & 0x00ff0000;
    if (cnt < nbdata) 
    {
        /* Prepare first word with CS info and write */
        txw.Bit.Tx = data[cnt++];
        R_CSIH_WRITE_REG(32, basechba + R_CSIH_TX0W, txw.Word);
    }

    while (cnt < nbdata) 
    {
        /* write next data and check finished status */
        R_CSIH_WRITE_REG(16, basechba + R_CSIH_TX0H, data[cnt++]);
    }

    /* if job features are used, write last data */
    if ( (Dev->Mode != R_CSIH_SLAVE) && (Frame->IntReq || Frame->Eoj) )
    {
        if (Frame->IntReq) 
        {
            txw.Bit.Cire = 1;
        }
        if (Frame->Eoj) 
        {
            txw.Bit.Eoj = 1;
        }
        txw.Bit.Tx = data[cnt++];
        /* Write last data and check finished status */
        R_CSIH_WRITE_REG(32, basechba + R_CSIH_TX0W, txw.Word);
        nbdata ++;
    }
    return nbdata;
}

/*******************************************************************************
  Function: loc_csih_BufPull
*/

int loc_csih_BufPull (r_csih_Dev_t *Dev, uint8_t Address, r_csih_FrameR_t *Frame) 
{
    uint16_t *data;
    uint8_t  nbdata, cs;
    uint32_t basechba;
    r_csih_RegRxw_t   rxw;
    r_csih_RegMRwp0_t mrwp;

    basechba = Dev->BaseAddrChba;

    /* Set offset address */
    mrwp.Word    = R_CSIH_READ_REG(32, basechba + R_CSIH_MRWP);
    mrwp.Bit.Rra = Address;
    R_CSIH_WRITE_REG(32, basechba + R_CSIH_MRWP, mrwp.Word);

    nbdata = 0;
    data   = Frame->Data;
    if (Frame->ErrCheckEnable == LOC_CSIH_FALSE) 
    {
        while (nbdata < Frame->NbData) 
        {
            data[nbdata++]  = R_CSIH_READ_REG(16, basechba + R_CSIH_RX0H);
        }
    } 
    else 
    {
        while (nbdata < Frame->NbData) 
        {
            rxw.Word        = R_CSIH_READ_REG(32, basechba + R_CSIH_RX0W);
            cs              = (uint8_t) ((~(rxw.Word))>>16 & 0xff);
            if (cs != Frame->CsSel.Byte) 
            {
                break;
            }
            data[nbdata++]  = rxw.Bit.Rx;
        }
    }
    return nbdata;
}


/*******************************************************************************
  Function: loc_csih_BufTrig
*/

r_csih_Error_t loc_csih_BufTrig (r_csih_Dev_t *Dev, uint8_t Address, uint8_t NbData) 
{
    r_csih_Error_t ret;
    r_csih_RegMCtl2_t mctl2;
    r_csih_RegStr0_t str;
    int base;

    /* check transmission status */
    base = Dev->BaseAddrCsih;
    str.Word = R_CSIH_READ_REG(32, base + R_CSIH_STR_0);

    if (str.Bit.Tsf == 1) 
    {
        ret = R_CSIH_ERR_BUSY;
    }
    else 
    {
        mctl2.Word     = 0;
        mctl2.Bit.Btst = 1;
        mctl2.Bit.Nd   = NbData;
        mctl2.Bit.Sop  = Address;

        base = Dev->BaseAddrChba;
        R_CSIH_WRITE_REG(32, base + R_CSIH_MCTL_2, mctl2.Word);
        ret = R_CSIH_ERR_OK;
    }
    return ret;
}


/*******************************************************************************
  Function: loc_csih_BufNbRemain
*/

int loc_csih_BufNbRemain (r_csih_Dev_t *Dev, r_csih_MemMode_t Mode) 
{
    uint32_t basechba;
    r_csih_RegMCtl2_t mctl2;
    int ret;

    if (Dev->MemCfg != Mode) 
    {
        ret = -1;
    } 
    else 
    {
        basechba   = Dev->BaseAddrChba;
        mctl2.Word = R_CSIH_READ_REG(32, basechba + R_CSIH_MCTL_2);
        ret        = mctl2.Bit.Nd;
    }
    return ret;
}


/*******************************************************************************
  Function: loc_csih_SetBaudrate
*/

uint32_t loc_csih_SetBaudrate (uint32_t Unit, uint32_t Baudrate) 
{
    uint16_t            deviderk;
    uint32_t                freq, pclk;
    uint8_t             prescaler;
    r_csih_RegCtl2_t    ctl2temp;
    r_csih_RegBrs_t     brgtemp[4];
    uint32_t            base, base1;

    if (Baudrate == 0) 
    {
        /* set to slave mode */
        ctl2temp.Hword = 0xe000;
    }
    else 
    {
        /* Determine internal BRG settings */
        prescaler = 0;
        pclk      = R_SYS_CSIH_GetClock(Unit);
        if (Baudrate > pclk/4) 
        {
            return 0;
        } 
        else 
        {
            freq      = pclk >> 1;
            deviderk  = freq / Baudrate;
            while ((prescaler < LOC_CSIH_MAX_PRESCALER) && \
                    ((deviderk > LOC_CSIH_MAX_DIVIDER) || (deviderk == LOC_CSIH_MIN_DIVIDER-1) )) 
            {
                prescaler++;
                freq        = freq>>1;
                deviderk    = freq / Baudrate;
            }
            if ((deviderk  < LOC_CSIH_MIN_DIVIDER)   || \
                (deviderk  > LOC_CSIH_MAX_DIVIDER)   || \
                (prescaler > LOC_CSIH_MAX_PRESCALER)) 
            {
                /* return error */
                return 0;
            }

            /* Configure the macro's internal BRG */
            ctl2temp.Hword   = 0;
            ctl2temp.Bit.Prs = prescaler;
            /*ctl2temp.Bit.Brs = deviderk;*/
            brgtemp[0].Bit.Brs = deviderk;
            Baudrate = (pclk / deviderk) >> (prescaler + 1);
        }
    }

    base = r_csih_Dev[Unit].BaseAddrCsih;
    base1 = r_csih_Dev[Unit].BaseAddrChba;
#ifdef R_CSIH_VARIANT
    if ( R_CSIH_VARIANT_ALTERNATIVE == r_csih_Dev[Unit].HwVariant )
    {
        /* ALT version doesn't have four different BRS registers,
           but a single one integrated into CTL2. */
        ctl2temp.Bit.Brs = brgtemp[0].Bit.Brs;
        R_CSIH_WRITE_REG(16, base + R_CSIH_CTL_2, ctl2temp.Hword);
    }
    else
#endif
    {
        R_CSIH_WRITE_REG(16, base + R_CSIH_CTL_2, ctl2temp.Hword);
        R_CSIH_WRITE_REG(16, base1 + R_CSIH_BRS(0), brgtemp[0].Hword);
        R_CSIH_WRITE_REG(16, base1 + R_CSIH_BRS(1), brgtemp[0].Hword*1);
        R_CSIH_WRITE_REG(16, base1 + R_CSIH_BRS(2), brgtemp[0].Hword*1);
        R_CSIH_WRITE_REG(16, base1 + R_CSIH_BRS(3), brgtemp[0].Hword*1);
    }
    /* Return internal BRG output */
    return Baudrate;
}


/*******************************************************************************
  Function: loc_csih_GetCsTimeSetting
*/

uint8_t loc_csih_GetCsTimeSetting (uint32_t Bdr, uint16_t Time, r_csih_CsT_t Setting, uint8_t SitEnabled) 
{
    uint8_t  i, nb_settings, test;
    uint16_t  *settings;
    uint32_t  delay[16];
    uint32_t  div;

    settings = (uint16_t*) loc_SpIdlHdSettings;
    switch (Setting) 
    {
        case R_CSIH_SETUP_T:
            nb_settings = LOC_CSIH_SETUP_T_SET_NUM;
        break;
        case R_CSIH_HOLD_T:
            nb_settings = LOC_CSIH_HOLD_T_SET_NUM;
        break;
        case R_CSIH_IDLE_T:
            nb_settings = LOC_CSIH_IDLE_SET_NUM;
        break;
        case R_CSIH_INTER_DATA_T:
            nb_settings = LOC_CSIH_INTER_DATA_T_SET_NUM;
            settings    = (uint16_t*) loc_InterDataSettings;
        break;
        default:
            nb_settings = 0;
        break;
    }

    /* calculate divider for calculations */
    if (Bdr >= 1000) 
    {
        div = 1000;
    } 
    else 
    {
        div = 1;
    }

    /* determine setup timings for all settings */
    test = SitEnabled && Setting != LOC_CSIH_SETUP_T_SET_NUM && Setting != LOC_CSIH_IDLE_SET_NUM;
    for (i=0; i<nb_settings; i++) 
    {
        delay[i]  = (test) ? settings[i]+5 : settings[i];                /* add 0,5SCK when necessary */
        delay[i] *= 1000 * div;
        delay[i] /= Bdr/(100);        /* 100 is to compensate the 1000 and 10*x,5 clocks in the settings */
    }

    i = 0;
    /* Select the correct Setting */
    while (i<nb_settings && delay[i++] < Time);

    i--;
    return i;
}


/*******************************************************************************
  Function: loc_csih_ConfigureCs
*/
uint8_t loc_csih_ConfigureCs (uint32_t Unit, r_csih_InitCfg_t* Config, uint32_t DefaultBdr) 
{
    uint8_t     nbcs, j, brss;
    uint32_t    csihbase, chbabase, bdr, targetbdr;
    r_csih_RegCtl1_t    ctl1;
    r_csih_RegCfg_t     cfg, firstCfg;
    r_csih_ChipSelCfg_t *cscfg;

    csihbase = r_csih_Dev[Unit].BaseAddrCsih;
    chbabase = r_csih_Dev[Unit].BaseAddrChba;

    ctl1.Word = R_CSIH_READ_REG(16, csihbase + R_CSIH_CTL_1);

    for (nbcs=0; nbcs < Config->NbCs; nbcs++) 
    {
        cscfg = &Config->ChipSelCfg[nbcs];
        cfg.Word = 0;
        /* Calculate prescaler value */
        j = 0;
        targetbdr = cscfg->Baudrate;
        do 
        {
            bdr = DefaultBdr >> (j++);
        } while (targetbdr != bdr && j < 4);
        /*cfg.Bit.Pscl = j-1; */                          /* cs prescaler */

        cfg.Bit.Ps  = cscfg->Parity;         /* parity */
        cfg.Bit.Dls = cscfg->DataLength;     /* data length */
        cfg.Bit.Rcb = cscfg->RecessiveCfg;   /* recessive cfg */
        cfg.Bit.Dir = cscfg->Direction;      /* LSB, MSB */
        cfg.Bit.Ckp = cscfg->ClockPhase;
        if (cscfg->ClockPhase == R_CSIH_CLK_ACTIVE_HIGH) 
        {
            if (cscfg->DataPhase == R_CSIH_DATA_PHASE_RISE) 
            {
                cfg.Bit.Dap = 1;
            }
        } 
        else 
        {
            cfg.Bit.Dap = cscfg->DataPhase;
        }
        if (nbcs > 3)
        {
            brss = nbcs -4;
        }
        else
        {
            brss = nbcs;
        }
        cfg.Bit.Pscl = brss;
        cfg.Bit.Idl = cscfg->ForceIdle;

        cfg.Bit.Sp  = loc_csih_GetCsTimeSetting( cscfg->Baudrate,
                            cscfg->SetupTime, R_CSIH_SETUP_T, ctl1.Bit.Sit);
        cfg.Bit.Id  = loc_csih_GetCsTimeSetting( cscfg->Baudrate,
                            cscfg->IdleTime, R_CSIH_IDLE_T, ctl1.Bit.Sit);
        cfg.Bit.Hd  = loc_csih_GetCsTimeSetting( cscfg->Baudrate,
                            cscfg->HoldTime, R_CSIH_HOLD_T, ctl1.Bit.Sit);
        cfg.Bit.In  = loc_csih_GetCsTimeSetting( cscfg->Baudrate,
                            cscfg->InterDataTime, R_CSIH_INTER_DATA_T, ctl1.Bit.Sit);
        
        /* CSIH always has 8 chip select config registers but might not have 8 CS "PINS". */
        /* So we set all CFG registers to use the first valid CFG given to this function. */
        /* Their configuration will be set to recessive in order to prevent interferences. */
        /* As these default setting are written once and first only, */
        /*  they will be overwritten by the desired settings */
        if (0 == nbcs)
        {
            firstCfg.Word = cfg.Word;
            /* set config to recessive */
            firstCfg.Bit.Rcb = 1;

            /* write default value to registers */
            for (nbcs=0; nbcs < 8; nbcs++)
            {
                R_CSIH_WRITE_REG(32, chbabase + R_CSIH_CFG( nbcs ), firstCfg.Word);
            }
            
            /* Set back to zero to continue outer for-loop */
            nbcs = 0;
        }
        
        /* write value to register */
        R_CSIH_WRITE_REG(32, chbabase + R_CSIH_CFG( cscfg->Chipselect ), cfg.Word);
    }
    
    return nbcs;
}


/*******************************************************************************
  Function: loc_csih_ConfigureSlave
*/
void loc_csih_ConfigureSlave (uint32_t Unit, r_csih_InitCfg_t* Config) 
{
    uint32_t chbabase;
    r_csih_RegCfg_t     cfg;
    r_csih_ChipSelCfg_t *cscfg;

    chbabase = r_csih_Dev[Unit].BaseAddrChba;

    cscfg       = &Config->ChipSelCfg[0];
    cfg.Word    = 0;
    cfg.Bit.Dls = cscfg->DataLength;     /* data length */
    cfg.Bit.Dir = cscfg->Direction;      /* LSB, MSB */
    cfg.Bit.Ckp = cscfg->ClockPhase;
    if (cscfg->ClockPhase == R_CSIH_CLK_ACTIVE_HIGH) 
    {
        if (cscfg->DataPhase == R_CSIH_DATA_PHASE_RISE) 
        {
            cfg.Bit.Dap = 1;
        }
    } 
    else 
    {
        cfg.Bit.Dap = cscfg->DataPhase;
    }

    /* write value to register */
    R_CSIH_WRITE_REG(32, chbabase + R_CSIH_CFG(0), cfg.Word);

    return;
}


/*******************************************************************************
  Function: loc_csih_GetMaxBaudrate
*/

int loc_csih_GetMaxBaudrate (r_csih_InitCfg_t *Config) 
{
    uint8_t  i;
    uint32_t nextbdr;
    uint32_t maxbdr;

    /* default value is that of CS0 */
    maxbdr = Config->ChipSelCfg[0].Baudrate;

    /* find bigger baud rate settings */
    for (i=1; i<Config->NbCs; i++) 
    {
        nextbdr = Config->ChipSelCfg[i].Baudrate;
        maxbdr  = ( nextbdr > maxbdr) ? nextbdr : maxbdr;
    }
    return maxbdr;
}


/*******************************************************************************
  Function: loc_csih_SetMainBaudrate
*/

uint32_t loc_csih_SetMainBaudrate (uint32_t Unit, r_csih_InitCfg_t *Config) 
{
    uint32_t bdr1, bdr2;

    /* Get the max baud rate */
    if (Config->Mode == R_CSIH_MASTER) 
    {
        bdr1 = loc_csih_GetMaxBaudrate(Config);
        /* Set max baud rate as default */
        bdr2 = loc_csih_SetBaudrate(Unit, bdr1);
        if (bdr1 != bdr2) 
        {
            bdr1 = (uint32_t)(-1);
        }
    } 
    else 
    {
        loc_csih_SetBaudrate(Unit, 0);
        bdr1 = 0;
    }
    return bdr1;
}


/*******************************************************************************
  Section: Global Functions
*/


/*******************************************************************************
  Function: R_CSIH_ErrorHandler
*/

void R_CSIH_ErrorHandler (uint32_t Unit, r_csih_Error_t Error) 
{
    if (loc_ErrorHandler != LOC_CSIH_NULL) 
    {      /* we've got a registered error handler here ...*/
        loc_ErrorHandler(Unit, (uint32_t)Error);   /* ... and we gonna use it!*/
    }
    else
    {                                /* we don't have a handler */
        for (;;)                          /* so, let's wait here */
        {
        } 
    }
}

/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_CSIH_Init
*/

uint32_t R_CSIH_Init (uint32_t Unit, r_csih_InitCfg_t *Config, r_csih_ExtFunc_t *OptFeat) 
{
    uint32_t                err;
    uint8_t             i, nbcs, datalength;
    uint32_t            baudrate;
    uint32_t            basecsih, basechba;
    r_csih_RegCtl1_t    ctl1;
    r_csih_RegMCtl0_t   mctl0temp;
    r_csih_Dev_t        *devptr;
    r_csih_CsLevel_t    level;
    r_csih_ChipSelCfg_t *cs_cfg;

    /* Verify given parameters */
    err = 0;

    if ((Unit   >= R_CSIH_MACRO_NUM)            ||
        (Config == LOC_CSIH_NULL)               ||
        (Config->ChipSelCfg == LOC_CSIH_NULL)   ||
        (Config->MemCfg > R_CSIH_MODE_NUM)  ) 
    {
        err = 1;
    } 
    else 
    {
        i = 0;
        nbcs = R_SYS_CSIH_GetNbChipSelect(Unit);
        if (Config->NbCs > nbcs) 
        {
            err |= 1;
        }
        while ((i<Config->NbCs) && (!err)) 
        {
            if(Config->ChipSelCfg[i++].Chipselect >= nbcs) 
            {
                err |= 1;
            }
        }
    }
    if (0 != err) 
    {
        R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_PARAM);
        return err;
    }

    /* initialise r_csih_Dev[Unit] */
    devptr = &r_csih_Dev[Unit];

    devptr->TxRxMode      = R_CSIH_TX_RX_OFF;
    devptr->DataBitSize   = 0;
    devptr->DataByteSize  = 0;
    devptr->TxRemainBytes = 0;
    devptr->OngoingBytes  = 0;
    devptr->TxOngoing     = 0;
    devptr->RxRemain      = 0;
    devptr->RxOnlyData    = 0;

    for(i=0; i<Config->NbCs ; i++) 
    {
        datalength = Config->ChipSelCfg[i].DataLength;
#ifdef R_CSIH_VARIANT
        /* This variant's datalength support is from 7 to 16 */
        if (datalength < 7)
        {
            R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_PARAM);
            return R_CSIH_ERR_PARAM;
        }
#endif
        datalength = LOC_CSIH_NB_BYTES(datalength);
        /* verify that the ring buffer size is big enough for one data! */
        if ((datalength > R_CSIH_RXBUF_BYTE_SIZE) ||
             (datalength > R_CSIH_TXBUF_BYTE_SIZE)) 
        {
            R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_RBINIT);
            return R_CSIH_ERR_RBINIT;
        }
    }

    /* Set base addresses and unit data */
    devptr->BaseAddrCsih = R_SYS_CSIH_GetCsihBaseAddr(Unit);
    devptr->BaseAddrChba = R_SYS_CSIH_GetChbaBaseAddr(Unit);
#ifdef R_CSIH_VARIANT
    devptr->HwVariant    = R_SYS_CSIH_GetCsihHwVariant(Unit);
#endif
    basecsih    = devptr->BaseAddrCsih;
    basechba    = devptr->BaseAddrChba;

    /* Clear PWR, TXE, and RXE, set normal memory mode */
    R_CSIH_WRITE_REG(8, basecsih + R_CSIH_CTL_0, 0x00);

    ctl1.Word = 0x00000000;
    if (OptFeat != LOC_CSIH_NULL) 
    {
        ctl1.Bit.Hse  = OptFeat->HandShakeE;
        ctl1.Bit.Csri = OptFeat->CsRetInact;
        ctl1.Bit.Dcs  = OptFeat->DataConsCheckE;
        ctl1.Bit.Je   = OptFeat->JobMode;
        ctl1.Bit.Edle = OptFeat->ExtDatLengE;
        ctl1.Bit.Slit = !(OptFeat->TicNormal);
        ctl1.Bit.Sit  = OptFeat->IntDelayedE;
        ctl1.Bit.Sse  = OptFeat->SlaveSelE;        
    }
    /* set CS active levels */
    if (Config->Mode == R_CSIH_MASTER) 
    {
        for (i=0; i<Config->NbCs ; i++) 
        {
            cs_cfg = &Config->ChipSelCfg[i];
            level  = cs_cfg->CsActiveLevel;
            if (level == R_CSIH_CS_ACTIVE_HIGH) 
            {
                ctl1.Word |= 1 << (8 + cs_cfg->Chipselect);
            }
        }
    }
    
#ifdef R_CSIH_VARIANT
    if ( R_CSIH_VARIANT_ALTERNATIVE == devptr->HwVariant )
    {
        /* ALT version doesn't have CTL1.SLRS[24] and CTL1.PHE[18], mask them out */
        /* Note: The code above doesn't even set them (yet) */
        ctl1.Word &= ~0x01040000;
    }
#endif

    R_CSIH_WRITE_REG(32, basecsih + R_CSIH_CTL_1, ctl1.Word);

    /* Set Main baud rate see <loc_SetMainBaudrate> */
    baudrate = loc_csih_SetMainBaudrate(Unit, Config);
    if (baudrate == (uint32_t)(-1)) 
    {
        R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_NG);
        return R_CSIH_ERR_NG;
    }

    devptr->Mode = Config->Mode;

    /* Configure the CS see: <loc_ConfigureCs> */
    if (Config->Mode == R_CSIH_MASTER) 
    {
        loc_csih_ConfigureCs(Unit, Config, baudrate);
    } 
    else 
    {
        loc_csih_ConfigureSlave(Unit, Config);
    }

    mctl0temp.Hword    = 0;
    /* Configure time out */
    if ((Config->Mode == R_CSIH_SLAVE) && (OptFeat!= LOC_CSIH_NULL)) 
    {
        mctl0temp.Hword = (OptFeat->TimeOut) & 0x1f;
    }
    /* Configure memory mode */
    mctl0temp.Bit.Mms  = Config->MemCfg;
    R_CSIH_WRITE_REG(16, basechba + R_CSIH_MCTL_0, mctl0temp.Hword);
    devptr->MemCfg     = Config->MemCfg;
    devptr->DirAccessE = LOC_CSIH_FALSE;

    /* Set the default callbacks for all interrupts  */
    devptr->IsrCallback[R_CSIH_INT_TIC]  = LOC_CSIH_NULL;
    devptr->IsrCallback[R_CSIH_INT_TIR]  = LOC_CSIH_NULL;
    devptr->IsrCallback[R_CSIH_INT_TIJC] = LOC_CSIH_NULL;
    devptr->IsrCallback[R_CSIH_INT_TIRE] = LOC_CSIH_NULL;
    /* Setup device interrupt controller see: <R_SYS_CSIH_IntCInit> */
    R_SYS_CSIH_IntCInit(Unit);

    return R_CSIH_ERR_OK;
}

/*******************************************************************************
  Function: R_CSIH_SlaveInit
*/

uint32_t R_CSIH_SlaveInit (uint32_t Unit, r_csih_InitSlaveCfg_t *Config, r_csih_InitSlaveOptFeat_t *OptConfig) 
{

    uint32_t            basecsih, basechba;
    r_csih_RegCtl1_t    ctl1temp;
    r_csih_RegMCtl0_t   mctl0temp;
    r_csih_RegCfg_t     cfg;
    r_csih_Dev_t        *devptr;

    /* Verify given parameters */

    if ((Unit >= R_CSIH_MACRO_NUM) || \
          (Config == LOC_CSIH_NULL)  || \
          (Config->MemCfg > R_CSIH_MODE_NUM)) 
    {
        R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_PARAM);
        return R_CSIH_ERR_PARAM;
    }

    devptr = &r_csih_Dev[Unit];
    /* Set base addresses and unit data */
    devptr->BaseAddrCsih = R_SYS_CSIH_GetCsihBaseAddr(Unit);
    devptr->BaseAddrChba = R_SYS_CSIH_GetChbaBaseAddr(Unit);
    basecsih    = devptr->BaseAddrCsih;
    basechba    = devptr->BaseAddrChba;

    /* Clear PWR, TXE, and RXE, set normal memory mode */
    R_CSIH_WRITE_REG(8, basecsih + R_CSIH_CTL_0, 0x00);

    ctl1temp.Word = 0x00000000;
    if (OptConfig != LOC_CSIH_NULL) 
    {
        ctl1temp.Bit.Sse  = OptConfig->SlaveSelE;
        ctl1temp.Bit.Hse  = OptConfig->HandShakeE;
        ctl1temp.Bit.Slit = OptConfig->TicNormalE;
        ctl1temp.Bit.Sit  = !(OptConfig->IntDelayedE);
    }
#ifdef R_CSIH_VARIANT
    if ( R_CSIH_VARIANT_ALTERNATIVE == devptr->HwVariant )
    {
        /* ALT version doesn't have CTL1.SLRS[24] and CTL1.PHE[18], mask them out */
        /* Note: The code above doesn't even set them (yet) */
        ctl1temp.Word &= ~0x01040000;
    }
#endif

    R_CSIH_WRITE_REG(32, basecsih + R_CSIH_CTL_1, ctl1temp.Word);

    /* Set macro to slave mdoe */
    R_CSIH_WRITE_REG(16, basecsih + R_CSIH_CTL_2, 0xc000);

    /* Configure memory mode */
    mctl0temp.Hword   = 0;
    mctl0temp.Bit.Mms = Config->MemCfg;
    if (OptConfig != LOC_CSIH_NULL) 
    {
        mctl0temp.Bit.To = OptConfig->TimeOut;
    }
    R_CSIH_WRITE_REG(16, basechba + R_CSIH_MCTL_0, mctl0temp.Hword);

    cfg.Word    = 0;
    cfg.Bit.Dls = Config->DataLength;
    cfg.Bit.Dir = Config->Direction;
    cfg.Bit.Dap = Config->ClockPhase;
    cfg.Bit.Ckp = Config->DataPhase;
    if (OptConfig != LOC_CSIH_NULL) 
    {
        cfg.Bit.Ps  = OptConfig->Parity;
    }
    R_CSIH_WRITE_REG(16, basechba + R_CSIH_CFG(0), cfg.Word);

    /* Enable Tx and Rx */
    R_CSIH_WRITE_REG(8, basecsih + R_CSIH_CTL_0, 0x60);

    /* Set the default callbacks for all interrupts  */
    devptr->IsrCallback[R_CSIH_INT_TIC]  = LOC_CSIH_NULL;
    devptr->IsrCallback[R_CSIH_INT_TIR]  = LOC_CSIH_NULL;
    devptr->IsrCallback[R_CSIH_INT_TIJC] = LOC_CSIH_NULL;
    devptr->IsrCallback[R_CSIH_INT_TIRE] = LOC_CSIH_NULL;
    /* Setup device interrupt controller see: <R_SYS_CSIH_IntCInit> */
    R_SYS_CSIH_IntCInit(Unit);

    return R_CSIH_ERR_OK;
}


/*******************************************************************************
  Function: R_CSIH_DeInit
*/

uint32_t R_CSIH_DeInit (uint32_t Unit) 
{
    uint32_t     r;
    uint32_t     i;
    uint32_t b;

    if (Unit < R_CSIH_MACRO_NUM) 
    {
        R_CSIH_Disable(Unit);
        b = r_csih_Dev[Unit].BaseAddrCsih;
        R_CSIH_WRITE_REG(8,  b + R_CSIH_CTL_0, 0x00);
        R_CSIH_WRITE_REG(32, b + R_CSIH_CTL_1, 0x00000000);
        R_CSIH_WRITE_REG(16, b + R_CSIH_CTL_2, 0xe000);
        R_CSIH_WRITE_REG(16, b + R_CSIH_STC_0, 0xc10b);

        b = r_csih_Dev[Unit].BaseAddrChba;
        R_CSIH_WRITE_REG(16, b + R_CSIH_MCTL_0, 0x001f);
        R_CSIH_WRITE_REG(32, b + R_CSIH_MCTL_1, 0x00000000);
        R_CSIH_WRITE_REG(32, b + R_CSIH_MCTL_2, 0x00000000);
        R_CSIH_WRITE_REG(32, b + R_CSIH_MRWP  , 0x00000000);

        for (i=0 ; i<8 ; i++) 
        {
            R_CSIH_WRITE_REG(32, b + R_CSIH_CFG(i)  , 0x00000000);
        }

        r = R_CSIH_ERR_OK;
    } 
    else 
    {
        r = R_CSIH_ERR_PARAM;
    }
    return r;
}


/*******************************************************************************
  Function: R_CSIH_Enable(uint32_t Unit)
*/

uint32_t R_CSIH_Enable (uint32_t Unit, r_csih_TxRxMode_t Mode) 
{
    volatile uint32_t   base;
    r_csih_RegCtl0_t    ctl0temp;

    if (Unit >= R_CSIH_MACRO_NUM) 
    {
        return R_CSIH_ERR_PARAM;
    } 
    else 
    {
        switch(Mode) 
        {
            case R_CSIH_TX_ONLY:
                ctl0temp.Byte  = 0xc0;
                break;
            case R_CSIH_RX_ONLY:
                ctl0temp.Byte  = 0xa0;
                break;
            case R_CSIH_TX_RX:
                ctl0temp.Byte  = 0xe0;
                break;
            default:
                ctl0temp.Byte  = 0x80;
                break;
        }
        r_csih_Dev[Unit].TxRxMode = Mode;
        base = r_csih_Dev[Unit].BaseAddrCsih;
        R_CSIH_WRITE_REG(8, base + R_CSIH_CTL_0, ctl0temp.Byte);
    }
    return R_CSIH_ERR_OK;
}


/*******************************************************************************
  Function: R_CSIH_Disable(uint32_t Unit)
*/

r_csih_Error_t R_CSIH_Disable (uint32_t Unit) 
{
    uint32_t       base;
    uint8_t        regval8;
    r_csih_Error_t ret;

    if (Unit < R_CSIH_MACRO_NUM) 
    {
        base     = r_csih_Dev[Unit].BaseAddrCsih;
        regval8  = R_CSIH_READ_REG(8, base + R_CSIH_CTL_0);
        regval8 &= 0xEF;
        R_CSIH_WRITE_REG(8, base + R_CSIH_CTL_0, regval8);
        ret = R_CSIH_ERR_OK;
    } 
    else 
    {
        ret = R_CSIH_ERR_PARAM;
    }
    return ret;
}


/*******************************************************************************
  Function: R_CSIH_AbortJob(uint32_t Unit)
*/

r_csih_Error_t R_CSIH_AbortJob (uint32_t Unit) 
{
    uint32_t         base;
    r_csih_RegCtl0_t ctl1;
    r_csih_Error_t   ret;

    if (Unit < R_CSIH_MACRO_NUM) 
    {
        base          = r_csih_Dev[Unit].BaseAddrCsih;
        ctl1.Byte     = R_CSIH_READ_REG(8, base + R_CSIH_CTL_0);
        ctl1.Bit.JobE = 1;
        R_CSIH_WRITE_REG(8, base + R_CSIH_CTL_0, ctl1.Byte);
        ret = R_CSIH_ERR_OK;
    } 
    else 
    {
        ret = R_CSIH_ERR_PARAM;
    }
    return ret;
}


/*******************************************************************************
  Function: R_CSIH_SetErrorCallback
*/

void R_CSIH_SetErrorCallback (void ( *ErrorCallback )(uint32_t Unit, uint32_t Error)) 
{
    loc_ErrorHandler    = ErrorCallback;
}


/*******************************************************************************
  Function: R_CSIH_ForceInt
*/

void R_CSIH_ForceInt(uint32_t Unit, r_csih_Interrupt_t Interrupt) 
{
    R_SYS_CSIH_ForceClearInt(Unit, Interrupt, LOC_CSIH_TRUE);
}


/*******************************************************************************
  Function: R_CSIH_FifoInit
*/
uint32_t R_CSIH_FifoInit (uint32_t Unit, uint8_t RxLevel, uint8_t TxLevel) 
{
    uint32_t          basecsih;
    uint32_t          basechba;
    r_csih_RegCtl0_t  ctl0;
    r_csih_RegMCtl0_t mctl0;
    r_csih_RegMCtl1_t mctl1;
    r_csih_Dev_t      *dev;

    if (Unit >= R_CSIH_MACRO_NUM) 
    {
        R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_PARAM);
        return R_CSIH_ERR_PARAM;
    } 
    else 
    {
        dev      = &r_csih_Dev[Unit];
        basecsih = dev->BaseAddrCsih;
        basechba = dev->BaseAddrChba;
    }

    if (dev->MemCfg != R_CSIH_MODE_FIFO) 
    {
        R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_NG);
        return R_CSIH_ERR_NG;
    }

    /* Clear PWR bit and set FIFO memory mode */
    ctl0.Byte     = R_CSIH_READ_REG(8, basecsih + R_CSIH_CTL_0);
    ctl0.Bit.Mbs  = 0;
    ctl0.Bit.Pwr  = 0;
    R_CSIH_WRITE_REG(8, basecsih + R_CSIH_CTL_0, ctl0.Byte);
    mctl0.Hword   = R_CSIH_READ_REG(16, basechba + R_CSIH_MCTL_0);
    mctl0.Bit.Mms = 0;
    R_CSIH_WRITE_REG(16, basechba + R_CSIH_MCTL_0, mctl0.Hword);
    /* Set PWR bit again */
    ctl0.Bit.Pwr  = 1;
    R_CSIH_WRITE_REG(8, basecsih + R_CSIH_CTL_0, ctl0.Byte);

    /* Set interupt thresholds */
    mctl1.Bit.Ffs = RxLevel;
    mctl1.Bit.Fes = TxLevel;
    R_CSIH_WRITE_REG(32, basechba + R_CSIH_MCTL_1, mctl1.Word);

    return R_CSIH_ERR_OK;
}


/*******************************************************************************
  Function: R_CSIH_FifoIntCfg
*/
r_csih_Error_t R_CSIH_FifoIntCfg (uint32_t Unit, uint8_t RxLevel, uint8_t TxLevel) 
{
    uint32_t          basechba;
    r_csih_RegMCtl1_t mctl1;
    r_csih_Dev_t      *dev;

    if (Unit >= R_CSIH_MACRO_NUM) 
    {
        R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_PARAM);
        return R_CSIH_ERR_PARAM;
    } 
    else 
    {
        dev      = &r_csih_Dev[Unit];
        basechba = dev->BaseAddrChba;
    }

    if (dev->MemCfg != R_CSIH_MODE_FIFO) 
    {
        R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_NG);
        return R_CSIH_ERR_NG;
    }

    /* Set interupt thresholds */
    mctl1.Word    = 0x00000000;
    mctl1.Bit.Ffs = RxLevel;
    mctl1.Bit.Fes = TxLevel;
    R_CSIH_WRITE_REG(32, basechba + R_CSIH_MCTL_1, mctl1.Word);

    return R_CSIH_ERR_OK;
}


/*******************************************************************************
  Function: R_CSIH_FifoReset
*/
uint32_t R_CSIH_FifoReset (uint32_t Unit) 
{
    uint32_t     basecsih;
    uint16_t     stcr0;
    uint32_t         ret;

    if (Unit < R_CSIH_MACRO_NUM) 
    {
        basecsih = r_csih_Dev[Unit].BaseAddrCsih;
        stcr0    = 0x4100;
        R_CSIH_WRITE_REG(16, basecsih + R_CSIH_STC_0, stcr0);
        ret      = R_CSIH_ERR_OK;
    } 
    else 
    {
        R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_PARAM);
        ret = R_CSIH_ERR_PARAM;
    }
    return ret;
}


/*******************************************************************************
  Function: R_CSIH_FifoPush
*/

uint32_t R_CSIH_FifoPush (uint32_t Unit, r_csih_FrameS_t *Frame) 
{
    uint8_t  nbdata, cnt, unsent, txlim;
    char     finished;
    uint32_t basechba, basecsih;
    r_csih_RegStr0_t  str;
    r_csih_RegTxw_t   txw;
    r_csih_RegMCtl1_t ctl1;
    uint16_t *data;

    if ((Unit >= R_CSIH_MACRO_NUM) || (Frame->Data == LOC_CSIH_NULL) || (Frame->NbData==0)) 
    {
        return 0;
    }
    
    /* TODO: Do I also need to check here if NbData fits into FIFO? Other driver functions do it. */
    
    data     = Frame->Data;
    nbdata   = Frame->NbData;
    cnt      = 0;
    basecsih = r_csih_Dev[Unit].BaseAddrCsih;
    basechba = r_csih_Dev[Unit].BaseAddrChba;

    R_CSIH_DisableInt(Unit, R_CSIH_INT_TIC);

    /* In case of CIRE or EOJ, decrease counter
     * as the last data must be done
     * separately
     */
    if ((Frame->IntReq) || (Frame->Eoj)) 
    {
        nbdata--;
    }

    str.Word = R_CSIH_READ_REG(32, basecsih + R_CSIH_STR_0);
    finished = (str.Bit.Flf) || (cnt == nbdata);

    txw.Word = ~(Frame->CsSel.Byte);
    txw.Word = (txw.Word << 16) & 0x00ff0000;
    if (!finished) 
    {
        /* Prepare first word with CS info */
        txw.Bit.Tx |= data[cnt++];

        /* Write first data and check finished status */
        R_CSIH_WRITE_REG(32, basechba + R_CSIH_TX0W, txw.Word);
        str.Word = R_CSIH_READ_REG(32, basecsih + R_CSIH_STR_0);
        finished = (str.Bit.Flf) || cnt == nbdata;
    }

    while (!finished) 
    {
        /* write next data and check finished status */
        R_CSIH_WRITE_REG(16, basechba + R_CSIH_TX0H, data[cnt++]);
        str.Word = R_CSIH_READ_REG(32, basecsih + R_CSIH_STR_0);
        finished = (str.Bit.Flf) || cnt == nbdata;
    }

    /* if job features are used, write last data */
    if (Frame->IntReq || Frame->Eoj) 
    {
        finished = str.Bit.Flf;
        /* if not finished, prepare last data */
        if (!finished) 
        {
            if (Frame->IntReq) 
            {
                txw.Bit.Cire = 1;
            }
            if (Frame->Eoj) 
            {
                txw.Bit.Eoj = 1;
            }
            txw.Bit.Tx = data[cnt++];
            /* Write last data and check finished status */
            R_CSIH_WRITE_REG(32, basechba + R_CSIH_TX0W, txw.Word);
        }
    }

    /* check if int request should be cleared */
    ctl1.Word = R_CSIH_READ_REG(32, basechba + R_CSIH_MCTL_1);
    txlim     = (uint8_t)(ctl1.Bit.Fes);
    str.Word  = R_CSIH_READ_REG(32, basecsih + R_CSIH_STR_0);
    unsent      = (uint8_t)(str.Bit.Spf);
    if (unsent > txlim ) 
    {
        R_SYS_CSIH_ForceClearInt(Unit, R_CSIH_INT_TIC, LOC_CSIH_FALSE);
        /* if int should have happaned in the mean time, set flag again */
        ctl1.Word = R_CSIH_READ_REG(32, basechba + R_CSIH_MCTL_1);
        txlim     = (uint8_t)(ctl1.Bit.Fes);
        str.Word  = R_CSIH_READ_REG(32, basecsih + R_CSIH_STR_0);
        unsent      = (uint8_t)(str.Bit.Spf);
        if (unsent <= txlim) 
        {
            R_SYS_CSIH_ForceClearInt(Unit, R_CSIH_INT_TIC, LOC_CSIH_TRUE);
        }
    }

    /* enable int TIC again */
    R_CSIH_EnableInt(Unit, R_CSIH_INT_TIC);

    return cnt;
}


/*******************************************************************************
  Function: R_CSIH_FifoPull
*/

uint32_t R_CSIH_FifoPull (uint32_t Unit, r_csih_FrameR_t *Frame) 
{
    uint8_t nbdata, cs;
    uint8_t received, rxlim;
    uint16_t *data;
    char finished;
    uint32_t basechba, basecsih;
    r_csih_RegMCtl1_t ctl1;
    r_csih_RegStr0_t str;
    r_csih_RegRxw_t  rxw;
    uint8_t        mask;

    if ((Unit >= R_CSIH_MACRO_NUM) || (Frame->Data == LOC_CSIH_NULL)) 
    {
        return 0;
    }
    
    /* TODO: Do I also need to check here if NbData fits into FIFO? Other driver functions do it. */
    
    data        = Frame->Data;
    nbdata      = 0;
    basecsih    = r_csih_Dev[Unit].BaseAddrCsih;
    basechba    = r_csih_Dev[Unit].BaseAddrChba;

    str.Word    = R_CSIH_READ_REG(32, basecsih + R_CSIH_STR_0);
    finished    = ( (str.Bit.Srp == 0) || (Frame->NbData == 0));

    Frame->Err.Byte = 0;

    mask = R_SYS_CSIH_GetIntMask(Unit, R_CSIH_INT_TIR);
    if (mask) 
    {
        R_CSIH_DisableInt(Unit, R_CSIH_INT_TIR);
    }

    if (Frame->ErrCheckEnable == LOC_CSIH_FALSE) 
    {
        while (!finished) 
        {
            data[nbdata++]  = R_CSIH_READ_REG(16, basechba + R_CSIH_RX0H);
            str.Word        = R_CSIH_READ_REG(32, basecsih + R_CSIH_STR_0);
            finished        = ( (str.Bit.Srp == 0) || (nbdata == Frame->NbData));
        }
    } 
    else 
    {
        while (!finished) 
        {
            rxw.Word        = R_CSIH_READ_REG(32, basechba + R_CSIH_RX0W);
            cs              = (uint8_t) ((~(rxw.Word))>>16 & 0xff);
            if (cs != Frame->CsSel.Byte ) 
            {
                break;
            }
            data[nbdata++]  = rxw.Bit.Rx;

            if ((rxw.Word)&0x03000000) 
            {
                if (rxw.Bit.Dce == 1)
                {
                    Frame->Err.Bit.DcErr = 1;
                }
                if (rxw.Bit.Rpe == 1)
                {
                    Frame->Err.Bit.PaErr = 1;
                }
                finished = 1;
            } 
            else 
            {
                str.Word = R_CSIH_READ_REG(32, basecsih + R_CSIH_STR_0);
                finished = (str.Bit.Srp == 0) || (nbdata == Frame->NbData);
            }
        }
    }

    if (mask) 
    {
        /* check if flag should be cleared */
        ctl1.Word = R_CSIH_READ_REG(32, basechba + R_CSIH_MCTL_1);
        rxlim     = (uint8_t)(ctl1.Bit.Ffs);
        rxlim     = 128 - rxlim;
        str.Word  = R_CSIH_READ_REG(32, basecsih + R_CSIH_STR_0);
        received      = (uint8_t)(str.Bit.Srp);
        if (received < rxlim ) 
        {
            R_SYS_CSIH_ForceClearInt(Unit, R_CSIH_INT_TIR, LOC_CSIH_FALSE);
            /* if int should have happaned in the mean time, set flag again */
            ctl1.Word = R_CSIH_READ_REG(32, basechba + R_CSIH_MCTL_1);
            rxlim     = (uint8_t)(ctl1.Bit.Ffs);
            rxlim     = 128 - rxlim;
            str.Word  = R_CSIH_READ_REG(32, basecsih + R_CSIH_STR_0);
            received  = (uint8_t)(str.Bit.Srp);
            if (!(received < rxlim)) 
            {
                R_SYS_CSIH_ForceClearInt(Unit, R_CSIH_INT_TIR, LOC_CSIH_TRUE);
            }
        }
        R_CSIH_EnableInt(Unit, R_CSIH_INT_TIR);
    }
    return nbdata;
}


/*******************************************************************************
  Function: R_CSIH_FifoGetNbUnsent
*/

uint8_t R_CSIH_FifoGetNbUnsent (uint32_t Unit) 
{
    uint8_t     ret;
    uint32_t    basecsih;
    r_csih_RegStr0_t str;

    if (Unit < R_CSIH_MACRO_NUM) 
    {
        basecsih = r_csih_Dev[Unit].BaseAddrCsih;
        str.Word = R_CSIH_READ_REG(32, basecsih + R_CSIH_STR_0);
        ret      = (int)(str.Bit.Spf);
    } 
    else 
    {
        ret = 0;
    }

    return ret;
}


/*******************************************************************************
  Function: R_CSIH_FifoGetNbRec
*/

uint8_t R_CSIH_FifoGetNbRec (uint32_t Unit) 
{
    uint8_t     ret;
    uint32_t    basecsih;
    r_csih_RegStr0_t str;

    if (Unit < R_CSIH_MACRO_NUM) 
    {
        basecsih = r_csih_Dev[Unit].BaseAddrCsih;
        str.Word = R_CSIH_READ_REG(32, basecsih + R_CSIH_STR_0);
        ret      = (int)(str.Bit.Srp);
    } 
    else 
    {
        ret = 0;
    }
    return ret;
}


/*******************************************************************************
  Function: R_CSIH_StatusGet
*/

uint8_t R_CSIH_StatusGet (uint32_t Unit, r_csih_Status_t Status) 
{
    volatile uint8_t   ret;
    volatile uint32_t    u32;
    volatile r_csih_RegStr0_t str;

    if ((Unit < R_CSIH_MACRO_NUM) && (Status!= R_CSIH_STATUS_ALL)) 
    {
        u32      = r_csih_Dev[Unit].BaseAddrCsih;
        str.Word = R_CSIH_READ_REG(32, u32 + R_CSIH_STR_0);
        u32      = str.Word & (1<<Status);
        ret      = (u32==0) ? LOC_CSIH_FALSE : LOC_CSIH_TRUE;
    } 
    else 
    {
        ret = LOC_CSIH_FALSE;
    }
    return ret;
}


/*******************************************************************************
  Function: R_CSIH_StatusClear
*/

void R_CSIH_StatusClear (uint32_t Unit, r_csih_Status_t Status) 
{
    uint32_t b;
    volatile r_csih_RegStc0_t stc;

    if (Unit < R_CSIH_MACRO_NUM) 
    {
        b = r_csih_Dev[Unit].BaseAddrCsih;
        if (Status == R_CSIH_STATUS_ALL) 
        {
            stc.Hword = 0xc10b;
        } 
        else 
        {
            stc.Hword = 1 << Status;
        }
        R_CSIH_WRITE_REG(16, b + R_CSIH_STC_0, stc.Hword);
    }
}

/*******************************************************************************
  Function: R_CSIH_DbPush
*/

uint32_t R_CSIH_DbPush (uint32_t Unit, uint8_t Address, r_csih_FrameS_t *Frame) 
{
    uint8_t  nbdata;
    r_csih_Dev_t      *dev;

    if( (Unit >= R_CSIH_MACRO_NUM)              || 
        (Frame->Data   == LOC_CSIH_NULL)        || 
        (Frame->NbData == 0)                    || 
        (Address > ((R_CSIH_FIFO_SIZE/2u)-1))   || 
        (Address+Frame->NbData > (R_CSIH_FIFO_SIZE/2u)) )
    {
         return 0;
    }
    dev      = &r_csih_Dev[Unit];
    dev->Unit = Unit;
    if (dev->MemCfg != R_CSIH_MODE_DUAL_BUF) 
    {
        return 0;
    } 
    else 
    {
        nbdata = loc_csih_BufPush(dev, Address, Frame);
    }
    return nbdata;
}


/*******************************************************************************
  Function: R_CSIH_DbPull
*/

uint32_t R_CSIH_DbPull (uint32_t Unit, uint8_t Address, r_csih_FrameR_t *Frame) 
{
    uint8_t  nbdata;
    r_csih_Dev_t      *dev;

    if( (Unit >= R_CSIH_MACRO_NUM)              || 
        (Frame->Data   == LOC_CSIH_NULL)        || 
        (Frame->NbData == 0)                    || 
        (Address > ((R_CSIH_FIFO_SIZE/2u)-1))   || 
        (Address+Frame->NbData > (R_CSIH_FIFO_SIZE/2u)) )
    {
        return 0;
    }
    dev = &r_csih_Dev[Unit];
    dev->Unit = Unit;

    if (dev->MemCfg != R_CSIH_MODE_DUAL_BUF) 
    {
        nbdata = 0;
    } 
    else 
    {
        nbdata = loc_csih_BufPull (dev, Address, Frame);
    }

    return nbdata;
}


/*******************************************************************************
  Function: R_CSIH_DbTrig
*/

r_csih_Error_t R_CSIH_DbTrig (uint32_t Unit, uint8_t Address, uint8_t NbData) 
{
    r_csih_Dev_t      *dev;
    r_csih_Error_t    ret;

    if( (Unit >= R_CSIH_MACRO_NUM)              ||
        (Address > ((R_CSIH_FIFO_SIZE/2u)-1))   || 
        (Address+NbData > (R_CSIH_FIFO_SIZE/2u)) )
    {
         return R_CSIH_ERR_PARAM;
    }
    dev      = &r_csih_Dev[Unit];
    dev->Unit = Unit;

    if (dev->MemCfg != R_CSIH_MODE_DUAL_BUF) 
    {
        ret = R_CSIH_ERR_NG;
    } 
    else 
    {
        ret = loc_csih_BufTrig(dev, Address, NbData);
    }
    return ret;
}


/*******************************************************************************
  Function: R_CSIH_DbNbRemain
*/

uint32_t R_CSIH_DbNbRemain (uint32_t Unit) 
{
    int ret;

    if(Unit < R_CSIH_MACRO_NUM) 
    {
        ret = loc_csih_BufNbRemain(&r_csih_Dev[Unit], R_CSIH_MODE_DUAL_BUF);
    } 
    else 
    {
        ret = -1;
    }
    return ret;
}


/*******************************************************************************
  Function: R_CSIH_TxbTrig
*/

r_csih_Error_t R_CSIH_TxbTrig (uint32_t Unit, uint8_t Address, uint8_t NbData) 
{
    r_csih_Dev_t      *dev;
    r_csih_Error_t    ret;

    if( (Unit >= R_CSIH_MACRO_NUM)          ||
        (Address > (R_CSIH_FIFO_SIZE-1) )   ||
        (Address + NbData > R_CSIH_FIFO_SIZE) )
    {
        return R_CSIH_ERR_PARAM;
    }
    dev      = &r_csih_Dev[Unit];
    dev->Unit = Unit;

    if (dev->MemCfg != R_CSIH_MODE_TX_BUF) 
    {
        ret = R_CSIH_ERR_NG;
    } 
    else 
    {
        ret = loc_csih_BufTrig(dev, Address, NbData);
    }
    return ret;
}


/*******************************************************************************
  Function: R_CSIH_TxbNbRemain
*/

uint32_t R_CSIH_TxbNbRemain (uint32_t Unit) 
{
    int ret;

    if(Unit < R_CSIH_MACRO_NUM) 
    {
        ret = loc_csih_BufNbRemain(&r_csih_Dev[Unit], R_CSIH_MODE_TX_BUF);
    } 
    else 
    {
        ret = -1;
    }
    return ret;
}


/*******************************************************************************
  Function: R_CSIH_TxbPull
*/

/*
 * ToDo: add internal buffer as on CSIH
 */

uint32_t R_CSIH_TxbPull (uint32_t Unit, r_csih_FrameR_t *Frame) 
{
    uint8_t      nbdata;
    r_csih_Dev_t *dev;

    nbdata =  0;
    if ( ! ((Unit >= R_CSIH_MACRO_NUM) || \
            (Frame->Data   == LOC_CSIH_NULL) || \
            (Frame->NbData == 0)       )   )
    {
        dev = &r_csih_Dev[Unit];
        dev->Unit = Unit;
        if (dev->MemCfg == R_CSIH_MODE_TX_BUF) 
        {
            *Frame->Data = R_CSIH_READ_REG(32, dev->BaseAddrChba + R_CSIH_RX0H);
            nbdata       = 1;
        }
    }
    return nbdata;
}


/*******************************************************************************
  Function: R_CSIH_TxbPush
*/

uint32_t R_CSIH_TxbPush (uint32_t Unit, uint8_t Address, r_csih_FrameS_t *Frame) 
{
    uint8_t  nbdata;
    r_csih_Dev_t      *dev;

    if( (Unit >= R_CSIH_MACRO_NUM)          ||
        (Frame->Data   == LOC_CSIH_NULL)    ||
        (Frame->NbData == 0)                ||
        (Address > (R_CSIH_FIFO_SIZE-1) )   ||
        (Address + Frame->NbData > R_CSIH_FIFO_SIZE) )
    {
         return 0;
    }
    dev      = &r_csih_Dev[Unit];
    dev->Unit = Unit;
    if (dev->MemCfg != R_CSIH_MODE_TX_BUF) 
    {
        return 0;
    } 
    else 
    {
        nbdata = loc_csih_BufPush(dev, Address, Frame);
    }
    return nbdata;
}

/*******************************************************************************
  Function: R_CSIH_BrkPtCfg
*/

r_csih_Error_t R_CSIH_BrkPtCfg (uint32_t Unit, uint8_t Stop) 
{
    r_csih_Error_t ret;
    uint32_t       base;
    uint8_t        val;

    if (Unit < R_CSIH_MACRO_NUM) 
    {
        val  = (Stop) ? 0x00 : 0x80;
        base = r_csih_Dev[Unit].BaseAddrCsih;
        R_CSIH_WRITE_REG(8, base + R_CSIH_EMU, val);
        ret = R_CSIH_ERR_OK;
    } 
    else 
    {
        R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_PARAM);
        ret = R_CSIH_ERR_PARAM;
    }
    return ret;
}


/*******************************************************************************
  Function: R_CSIH_DaSwitch
*/

r_csih_Error_t R_CSIH_DaSwitch (uint32_t Unit, uint8_t On) 
{
    r_csih_Error_t   ret;
    r_csih_RegCtl0_t ctl0;
    r_csih_RegStr0_t str;
    uint32_t basecsih;
    r_csih_Dev_t *dev;

    if (Unit < R_CSIH_MACRO_NUM) 
    {
        dev = &r_csih_Dev[Unit];
        dev->Unit = Unit;

        /* switch to and from FIFO mode is forbidden */
        if (dev->MemCfg == R_CSIH_MODE_FIFO) 
        {
            R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_PARAM);
            ret = R_CSIH_ERR_NG;
        } 
        else 
        {
            basecsih = dev->BaseAddrCsih;
            /* check power bit */
            ctl0.Byte   = R_CSIH_READ_REG(8, basecsih + R_CSIH_CTL_0);
            if (ctl0.Bit.Pwr == 0) 
            {
                ret = R_CSIH_ERR_NG;
            } 
            else 
            {
                /* check status bit */
                str.Word    = R_CSIH_READ_REG(32, basecsih + R_CSIH_STR_0);
                if (str.Bit.Tsf ==1) 
                {
                    ret = R_CSIH_ERR_BUSY;
                } 
                else 
                {
                    /* switch memory mode */
                    if (On) 
                    {
                        ctl0.Bit.Mbs    = 1;
                        dev->DirAccessE = LOC_CSIH_TRUE;
                    } 
                    else 
                    {
                        ctl0.Bit.Mbs    = 0;
                        dev->DirAccessE = LOC_CSIH_FALSE;
                    }
                    R_CSIH_WRITE_REG(8, basecsih + R_CSIH_CTL_0, ctl0.Byte);
                    ret = R_CSIH_ERR_OK;
                }
            }
        }
    } 
    else 
    {
        R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_PARAM);
        ret = R_CSIH_ERR_PARAM;
    }
    return ret;
}


/*******************************************************************************
  Function: R_CSIH_DaWrite
*/

uint32_t R_CSIH_DaWrite (uint32_t Unit, r_csih_FrameS_t *Frame) 
{
    uint8_t  nbdata;
    uint32_t basechba;
    r_csih_Dev_t    *dev;
    r_csih_RegTxw_t txw;

    nbdata = 0;

    if (Unit < R_CSIH_MACRO_NUM) 
    {
        dev = &r_csih_Dev[Unit];
        dev->Unit = Unit;
        if (dev->DirAccessE) 
        {
            txw.Word = ~(Frame->CsSel.Byte);
            txw.Word = (txw.Word << 16) & 0x00ff0000;
            txw.Bit.Tx = *Frame->Data;
            txw.Bit.Eoj = Frame->Eoj;
            /* Write first data and check finished status */
            basechba = dev->BaseAddrChba;
            R_CSIH_WRITE_REG(32, basechba + R_CSIH_TX0W, txw.Word);
            nbdata = 1;
        }
    }
    return nbdata;
}

/*******************************************************************************
  Function: R_CSIH_DaRead
*/

uint32_t R_CSIH_DaRead (uint32_t Unit, r_csih_FrameR_t *Frame) 
{
    uint32_t basechba;
    uint32_t     nbdata;

    r_csih_Dev_t   *dev;
    r_csih_RegRxw_t rxw;

    nbdata = 0;

    if (Unit < R_CSIH_MACRO_NUM) 
    {
        dev = &r_csih_Dev[Unit];
        dev->Unit = Unit;

        if (dev->DirAccessE) 
        {
            basechba = dev->BaseAddrChba;
            rxw.Word = R_CSIH_READ_REG(32, basechba + R_CSIH_RX0W);
            Frame->CsSel.Byte = (uint8_t) ((~(rxw.Word))>>16 & 0xff);
            *Frame->Data      = rxw.Bit.Rx;
            if ((rxw.Word)&0x03000000) 
            {
                if (rxw.Bit.Dce == 1) 
                {
                    Frame->Err.Bit.DcErr = 1;
                }
                if (rxw.Bit.Rpe == 1) 
                {
                    Frame->Err.Bit.PaErr = 1;
                }
            }
            nbdata = 1;
        }
    } 
    else 
    {
        R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_PARAM);
    }
    return nbdata;
}



/*******************************************************************************
  Function: R_CSIH_LbmSwitch
*/

r_csih_Error_t R_CSIH_LbmSwitch (uint32_t Unit, uint8_t On) 
{
    r_csih_Error_t   ret;
    r_csih_RegCtl0_t ctl0;
    r_csih_RegCtl1_t ctl1;
    r_csih_RegStr0_t str;
    uint32_t basecsih;
    r_csih_Dev_t *dev;
    uint8_t pwr;

    if (Unit < R_CSIH_MACRO_NUM) 
    {
        dev = &r_csih_Dev[Unit];
        dev->Unit = Unit;
        basecsih = dev->BaseAddrCsih;

        /* check status bit */
        str.Word    = R_CSIH_READ_REG(32, basecsih + R_CSIH_STR_0);
        if (str.Bit.Tsf ==1) 
        {
            ret = R_CSIH_ERR_BUSY;
        } 
        else 
        {
            /* check and clear power bit*/
            ctl0.Byte    = R_CSIH_READ_REG(8, basecsih + R_CSIH_CTL_0);
            if (ctl0.Bit.Pwr == 1) 
            {
                pwr = LOC_CSIH_TRUE;
                ctl0.Bit.Pwr = 0;
                R_CSIH_WRITE_REG(8, basecsih + R_CSIH_CTL_0, ctl0.Byte);
            } 
            else 
            {
                pwr = LOC_CSIH_FALSE;
            }

            /* switch to LBM mode*/
            ctl1.Word    = R_CSIH_READ_REG(32, basecsih + R_CSIH_CTL_1);
            if (On) 
            {
                ctl1.Bit.Lbm    = 1;
            } 
            else 
            {
                ctl1.Bit.Lbm    = 0;
            }
            R_CSIH_WRITE_REG(32, basecsih + R_CSIH_CTL_1, ctl1.Word);

            /* if necessary, set Pwr bit */
            if (pwr) 
            {
                ctl0.Bit.Pwr = 1;
                R_CSIH_WRITE_REG(8, basecsih + R_CSIH_CTL_0, ctl0.Byte);
            }

            ret = R_CSIH_ERR_OK;
        }
    } 
    else 
    {
        R_CSIH_ErrorHandler(Unit, R_CSIH_ERR_PARAM);
        ret = R_CSIH_ERR_PARAM;
    }
    return ret;
}


/*******************************************************************************
  Function: R_CSIH_DbPushData
*/

uint8_t R_CSIH_DbPushData (uint8_t Unit, uint8_t Address, r_csih_FrameS_t *Frame)
{
    uint8_t  nbdata, cnt;
    uint32_t basechba;
    uint16_t *data;
    r_csih_RegTxw_t   txw;
    r_csih_RegMRwp0_t mrwp;
    uint8_t DataBitSize;

    if( (Unit >= R_CSIH_MACRO_NUM)              ||
        (Frame->Data   == LOC_CSIH_NULL)        ||
        (Frame->NbData == 0)                    ||
        (Address > ((R_CSIH_FIFO_SIZE/2u)-1))   ||
        (Address + Frame->NbData > (R_CSIH_FIFO_SIZE/2u)) )
    {
        return 0;
    }
    basechba    = r_csih_Dev[Unit].BaseAddrChba;

    if (r_csih_Dev[Unit].MemCfg == R_CSIH_MODE_DUAL_BUF)
    {
        data     = Frame->Data;
        nbdata   = Frame->NbData;
        cnt      = 0;

        /* MNI: This code block writes to a variable provided by the application.
                As long as we don't expect data to be delivered by the driver, this is undesired.
                The actions have been pulled into the code below. */
    #if 0
        if (r_csih_Dev[Unit].Mode == R_CSIH_SLAVE)
        {
            Frame->CsSel.Byte = 0;
            Frame->IntReq     = LOC_CSIH_FALSE;
            Frame->Eoj        = LOC_CSIH_FALSE;
        }
    #endif

        /* In case of CIRE or EOJ, decrease counter
         * as the last data must be done
         * separately
         */
        if ( (r_csih_Dev[Unit].Mode != R_CSIH_SLAVE) && (Frame->IntReq || Frame->Eoj) )
        {
            nbdata--;
        }

        /* Set offset address */
        mrwp.Word     = R_CSIH_READ_REG(32, basechba + R_CSIH_MRWP);
        mrwp.Bit.Trwa = Address;
        R_CSIH_WRITE_REG(32, basechba + R_CSIH_MRWP, mrwp.Word);

        if (r_csih_Dev[Unit].Mode == R_CSIH_SLAVE) 
        {
            /* According to UM, set bits to 0xFE for slave mode. */
            txw.Bit.Cs = ~(1);
        }
        else
        {
            txw.Bit.Cs = Frame->CsSel.Byte;
        }
        /*txw.Word    = (txw.Word << 16) & 0x00ff0000;*/
        
        if (Frame->Databitsize <= 16)
        {           
            while (cnt < nbdata)
            {
                /* Prepare first word with CS info and write */
                txw.Bit.Tx = data[cnt++];
                R_CSIH_WRITE_REG(32, basechba + R_CSIH_TX0W, txw.Word);        
            }
            /*while(cnt < nbdata)
            {          
                R_CSIH_WRITE_REG(16, basechba + R_CSIH_TX0H, data[cnt++]);
            } */           
        }
        else
        {
            while (cnt < nbdata)
            {
                DataBitSize = Frame->Databitsize;
                /* If the transmission data length is more than 16 bits */
                if (DataBitSize > 16)
                {
                    txw.Bit.Edl = 1;
                    txw.Bit.Tx = data[cnt++];
                    R_CSIH_WRITE_REG(32, basechba + R_CSIH_TX0W, txw.Word);
                    DataBitSize -= 16;
    
                    while (DataBitSize)
                    {
                        if (DataBitSize >= 16)
                        {
                            R_CSIH_WRITE_REG(16, basechba + R_CSIH_TX0H, data[cnt++]);
                            DataBitSize -= 16;
                        }
                        else
                        {
                            DataBitSize = 0;
                            txw.Bit.Edl = 0;
                            txw.Bit.Tx = data[cnt++];
                            R_CSIH_WRITE_REG(32, basechba + R_CSIH_TX0W, txw.Word);
                        }
                    }
                }
            }
        }

        /* if job features are used, write last data */
        if ( (r_csih_Dev[Unit].Mode != R_CSIH_SLAVE) && (Frame->IntReq || Frame->Eoj) )
        {
            if(Frame->IntReq)
            {
                txw.Bit.Cire = 1;
            }
            if(Frame->Eoj)
            {
                txw.Bit.Eoj = 1;
            }
            txw.Bit.Tx = data[cnt++];
            /* Write last data and check finished status */
            R_CSIH_WRITE_REG(32, basechba + R_CSIH_TX0W, txw.Word);
            nbdata ++;
        }
    }
    else
    {
        return 0;
    }
    return nbdata;
}

/*******************************************************************************
  Function: R_CSIH_Init_Svstop
*/

void R_CSIH_Init_Svstop (uint8_t Unit , uint8_t Svstop)
{
    r_csih_RegEmu_t EmuReg;
    uint32_t baseAddrCsih;
    baseAddrCsih  = r_csih_Dev[Unit].BaseAddrCsih;
    EmuReg.Bit.svsdis = Svstop;
    R_CSIH_WRITE_REG(8, baseAddrCsih + R_CSIH_EMU, EmuReg.Byte);
}



/*******************************************************************************
  Function: R_CSIH_GetClock
*/

uint32_t R_CSIH_GetClock (uint32_t Unit) 
{
    return R_SYS_CSIH_GetClock(Unit);
}

#if defined (__ghs__)
#pragma ghs endnomisra
#endif



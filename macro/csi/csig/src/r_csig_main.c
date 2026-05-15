/*
****************************************************************************
PROJECT : CSIG driver
FILE    : $Id: r_csig_main.c 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================
DESCRIPTION
Driver for CSIG macro
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

/*******************************************************************************
  Title: Main Module

  Implementation of the API and other global functions.
*/

/*
  csig terminology

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


/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_cdi_api.h"
#include "r_config_csig.h"
#include "r_csig_regs.h"
#include "r_csig_api.h"
#include "r_csig_sys.h"
#define CSIG_MAIN_GLOBALS_
#include "r_csig_main.h"
#undef CSIG_MAIN_GLOBALS_
#include "r_dev_api.h"


/*******************************************************************************
  Section: Local Defines
*/

/*******************************************************************************
  Constants: Baudrate setting limits

  loc_MAX_DIVIDER      -  Max value of divider k in
                                CTL2 register.

  loc_MAX_PRESCALER    -  Max value of divider m in
                                CTL2 register.
*/

#define loc_MAX_DIVIDER            4095 /* CsigBrs0-11*/
#define loc_MIN_DIVIDER            1
#define loc_MAX_PRESCALER          7


/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: loc_ErrorHandler

  A function pointer to a user callback (Applications error handler)

  The value is assigned by the application using the driver


*/
static void (*loc_ErrorHandler )(uint32_t Unit, int32_t Error) = 0u;



/*******************************************************************************
  Section: Local Functions
*/

static uint32_t loc_SecureRbWrite(
    int32_t Unit, r_cdi_RBuf_t *RBuf, void *Buffer, uint32_t Num, r_csig_Interrupt_t IntType); 
static uint32_t loc_RbWrite(
    int32_t Unit, r_cdi_RBuf_t *RBuf, void *Buffer, uint32_t Num, r_csig_Interrupt_t IntType);
static uint32_t loc_RbRead(r_cdi_RBuf_t *RBuf, void *Buffer, uint32_t Num); 
static uint32_t loc_SecureRbRead(
    int32_t Unit, r_cdi_RBuf_t *RBuf, void *Buffer, uint32_t Num, r_csig_Interrupt_t IntType);
static uint32_t loc_SetBaudrate(uint32_t Unit, uint32_t Baudrate);
static int32_t R_CSIG_Write_N_Edl(uint32_t Unit, void *Buffer, uint32_t Num, uint8_t CopyAll);
static int32_t R_CSIG_Write_Edl(uint32_t Unit, void *Buffer, uint32_t Num, uint8_t CopyAll); 

/*******************************************************************************
  Function: loc_SecureRbWrite

  Writes data to the ring buffer with concurrent access of
  an ISR to the same buffer.

  The function writes to the ring buffer and makes sure, that
  no 'IntType' interrupt can occur during that operation. This is
  required to keep data in the ring buffer consistent.

  Parameters:
  Unit       - Instance number
  RBuf       - Ring buffer
  Buffer     - Data buffer
  Num        - Number of bytes in data buffer
  IntType    - Specified interrupt

  Returns:
  Number of characters, which have been copied.
*/
static uint32_t loc_SecureRbWrite(
    int32_t                  Unit,
    r_cdi_RBuf_t        *RBuf,
    void                *Buffer,
    uint32_t             Num,
    r_csig_Interrupt_t   IntType)
{
    uint32_t  len;
    uint8_t masked;

    masked = R_CSIG_GetIntMask(Unit, IntType);
    if(0 == masked) 
    {
        R_CSIG_DisableInt(Unit, IntType);
    }
    len = R_CDI_RbWrite(RBuf, Buffer, Num);
    if(0 == masked) 
    {
        R_CSIG_EnableInt(Unit, IntType);
    }
    return len;
}

/*******************************************************************************
  Function: loc_RbWrite

  Writes data to the ring buffer with concurrent access of
  an ISR to the same buffer.

  The function writes to the ring buffer and makes sure, that
  no 'IntType' interrupt can occur during that operation. This is
  required to keep data in the ring buffer consistent.

  Parameters:
  Unit       - Instance number
  RBuf       - Ring buffer
  Buffer     - Data buffer
  Num        - Number of data in data buffer
  IntType    - Specified interrupt

  Returns:
  Number of characters, which have been copied.
*/

static uint32_t loc_RbWrite(
    int32_t                  Unit,
    r_cdi_RBuf_t        *RBuf,
    void                *Buffer,
    uint32_t             Num,
    r_csig_Interrupt_t   IntType)
{

    Num = loc_SecureRbWrite(Unit, RBuf, Buffer, Num, IntType);
    return Num;
}


/*******************************************************************************
  Function: loc_RbRead

  Read data from the ring buffer with concurrent access of
  an ISR to the same buffer.

  The function reads data from a ring buffer and makes sure,
  that no 'IntType' interrupt can occur during that operation.
  This is required to keep data in the ring buffer consistent.

  Parameters:
  Unit       - Instance number
  RBuf       - Ring buffer
  Buffer     - Data buffer
  Num        - Max number to be read from the buffer
  IntType    - Specified interrupt

  Returns:
  Number of characters, which have been copied.
*/

static uint32_t loc_RbRead(
    r_cdi_RBuf_t        *RBuf,
    void                *Buffer,
    uint32_t             Num)
{
    uint32_t len;
    len = R_CDI_RbRead(RBuf, Buffer, Num);
    return len;
}


/*******************************************************************************
  Function: loc_SecureRbRead

  Read data from the ring buffer with concurrent access of
  an ISR to the same buffer.

  The function reads data from a ring buffer and makes sure,
  that no 'IntType' interrupt can occur during that operation.
  This is required to keep data in the ring buffer consistent.

  Parameters:
  Unit       - Instance number
  RBuf       - Ring buffer
  Buffer     - Data buffer
  Num        - Max number to be read from the buffer
  IntType    - Specified interrupt

  Returns:
  Number of characters, which have been copied.
*/
static uint32_t loc_SecureRbRead(
    int32_t                  Unit,
    r_cdi_RBuf_t        *RBuf,
    void                *Buffer,
    uint32_t             Num,
    r_csig_Interrupt_t  IntType)
{
    uint8_t masked;
    masked = R_CSIG_GetIntMask(Unit, IntType);
    if(0 == masked) {
        R_CSIG_DisableInt(Unit, IntType);
    }
    Num  = loc_RbRead(RBuf, Buffer, Num);
    if(0 == masked) {
        R_CSIG_EnableInt(Unit, IntType);
    }
    return Num;
}


/*******************************************************************************
  Function: loc_SetBaudrate
*/

static uint32_t loc_SetBaudrate(uint32_t Unit, uint32_t Baudrate) 
{

    uint16_t            deviderk;
    uint32_t             freq, pclk;
    uint8_t             prescaler;
    r_csig_RegCtl2_t    ctl2temp;
    uint32_t            base;

    if(Baudrate == 0u) {
        /* set to slave mode */
        ctl2temp.Hword = 0xe000;
    } else {
        /* Determine internal BRG settings */
        prescaler = 0u;
        pclk      = R_SYS_CSIG_GetClock(Unit);
        if(Baudrate > (pclk/4)) 
        {
            return 0;
        } 
        else 
        {
            freq      = pclk / 2 ;
            deviderk  = freq / Baudrate;
            while ((prescaler < loc_MAX_PRESCALER) && \
                    ((deviderk > loc_MAX_DIVIDER) || (deviderk == (loc_MIN_DIVIDER-1)) )) 
            {
                prescaler++;
                freq        = freq / 2 ;
                deviderk    = freq / Baudrate;
            }
            if((deviderk  < loc_MIN_DIVIDER)   || \
               (deviderk  > loc_MAX_DIVIDER)   || \
               (prescaler > loc_MAX_PRESCALER) ) 
            {
                /* return error */
                return 0;
            }

            /* Configure the macro's internal BRG */
            ctl2temp.Hword   = 0u;
            ctl2temp.Bit.Prs = prescaler;
            ctl2temp.Bit.Brs = deviderk;

            Baudrate = (uint32_t)(pclk / deviderk) >> (prescaler + 1u);
        }
    }
    /* dbg info: CSIG set baudrate */
    base = r_csig_Dev[Unit].BaseAddrCsig;
    R_CSIG_WRITE_REG(16, base + R_CSIG_CTL_2, ctl2temp.Hword);

    /* Return internal BRG output */
    return Baudrate;
}

/*******************************************************************************
  Function: R_CSIG_Write_N_Edl
*/

static int32_t R_CSIG_Write_N_Edl(uint32_t Unit, void *Buffer, uint32_t Num, uint8_t CopyAll) 
{
    uint32_t          writeno, nb;   /* number of written characters in rb */
    uint32_t          remaining;    /* characters left to transmit to rb */
    uint8_t           readno;
    volatile uint8_t  regval;
    uint16_t          txdata;
    uint8_t           masked;
    uint8_t           num;
    uint8_t           *dsize;
    r_csig_Dev_t      *csig_dev;

    csig_dev = &r_csig_Dev[Unit];
    dsize    = &csig_dev->DataByteSize;

    writeno   = loc_RbWrite(Unit, &csig_dev->TxRBuf, Buffer, Num, R_CSIG_INT_TIC);
    remaining = Num - writeno;

    Buffer    = (void*)((uint32_t)Buffer + (*dsize * writeno));

    readno    = 0u;
    do 
    {
        if (0 != remaining) 
        {
            writeno    = loc_RbWrite(Unit, &csig_dev->TxRBuf, Buffer, remaining, R_CSIG_INT_TIC);
            remaining -= writeno;
            Buffer    = (void*)((uint32_t)Buffer + (*dsize * writeno));
        }
        masked = R_CSIG_GetIntMask(Unit, R_CSIG_INT_TIC);
        if(0 == masked) 
        {
            R_CSIG_DisableInt(Unit, R_CSIG_INT_TIC);
        }
        regval = csig_dev->OngoingBytes;
        nb = R_CDI_RbNum(&csig_dev->TxRBuf);
        if ((0 == regval) && (nb > 0)) 
        {
            /* if edl, then read 2 data (2 bytes), if not, read one data */
            txdata = 0u;
            num    = 1u;
            readno = loc_RbRead(&csig_dev->TxRBuf, &txdata, num);
            if (readno == num) {
                /* send first Data manually to init Interrupt*/
                csig_dev->TxRemainBytes = *dsize;
                csig_dev->OngoingBytes  = *dsize ;
                R_CSIG_WriteData(Unit, txdata, 0);
            } else {
                R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_BUFO);
                return 0;
            }
        }
        if(0 == masked) 
        {
            R_CSIG_EnableInt(Unit, R_CSIG_INT_TIC);
        }
        /* In master mode, all data should be copied to the driver's internal buffer */
    } while ((0 != remaining) && ((CopyAll == 1) || (writeno > 0)) ) ;
    Num -= remaining;
    return Num;
}

/*******************************************************************************
  Function: R_CSIG_Write_Edl

*/

static int32_t R_CSIG_Write_Edl(uint32_t Unit, void *Buffer, uint32_t Num, uint8_t CopyAll) 
{
    uint32_t          writeno, nb;   /* number of written characters in rb */
    uint32_t          remaining; /* characters left to transmit to rb */
    uint8_t           readno;
    volatile uint8_t  regval;
    uint16_t          txdata;
    uint8_t         masked;
    uint8_t           num, size;
    uint8_t           *dsize;
    r_csig_Dev_t      *csig_dev;

    csig_dev = &r_csig_Dev[Unit];
    dsize    = &csig_dev->DataByteSize;

    num  = csig_dev->TxRBuf.Num - 1;
    num -= R_CDI_RbNum(&csig_dev->TxRBuf);
    num /= *dsize;

    Num  = (num < Num) ? num : Num;
    Num *= *dsize;

    writeno   = loc_RbWrite(Unit, &csig_dev->TxRBuf, Buffer, Num, R_CSIG_INT_TIC);
    remaining = Num - writeno;
    Buffer    = (void*)((uint32_t)Buffer + writeno);
    readno    = 0u;
    do {
        if (0 != remaining) {
            writeno    = loc_RbWrite(Unit, &csig_dev->TxRBuf, Buffer, remaining, R_CSIG_INT_TIC);
            remaining -= writeno;
            Buffer    = (void*)((uint32_t)Buffer + writeno);
        }
        masked = R_CSIG_GetIntMask(Unit, R_CSIG_INT_TIC);
        if(0 == masked) {
            R_CSIG_DisableInt(Unit, R_CSIG_INT_TIC);
        }
        regval = csig_dev->OngoingBytes;
        nb = R_CDI_RbNum(&csig_dev->TxRBuf);
        if ((regval == 0) && (nb > 0)) 
        {
            /* if edl, then read 2 data (2 bytes), if not, read one data */
            txdata = 0u;
            num    = (*dsize > 2) ? 2 : 1;
            size   = (*dsize > 1) ? 2 : 1;
            readno = loc_RbRead(&csig_dev->TxRBuf, &txdata, num);
            if (readno == num) {
                /* send first Data manually to init Interrupt*/
                csig_dev->TxRemainBytes = *dsize;
                csig_dev->OngoingBytes  = size ;
                R_CSIG_WriteData(Unit, txdata, 1);
            } else {
                R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_BUFO);
                return 0;
            }
        }
        if(0 == masked) 
        {
            R_CSIG_EnableInt(Unit, R_CSIG_INT_TIC);
        }
        /* In master mode, all data should be copied to the driver's internal buffer */
    } while ((0 != remaining) && ((CopyAll == 1) || (writeno > 0)) ) ;

    Num -= remaining;
    Num /= *dsize;
    return Num;
}



/*******************************************************************************
  Section: Global Functions
*/


/*******************************************************************************
  Function: R_CSIG_ErrorHandler
*/

void R_CSIG_ErrorHandler(uint32_t Unit, r_csig_Error_t Error) 
{

    if (loc_ErrorHandler != 0u) /* we've got a registered error handler here ...*/
    {      
        loc_ErrorHandler(Unit, Error);   /* ... and we gonna use it!*/
    }
    else 
    {                                /* we don't have a handler */
        for (;;)                          /* so, let's wait here */
        {
            R_DEV_Nop();  
        } 
    }
}


/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_CSIG_Init
*/

int32_t R_CSIG_Init(uint32_t Unit,
                        r_csig_InitCfg_t *Config,
                        r_csig_OptFeat_t *OptFeat) {

    volatile uint8_t    size;
    volatile uint32_t   num;
    int32_t                 err;
    uint32_t            baudrate;
    uint32_t            basecsig, basechbb;
    r_csig_RegCtl1_t    ctl1;
    r_csig_RegCfg_t     cfg;
    r_csig_Dev_t        *devptr;
    uint8_t             tmp;

    err       = 0u;
    cfg.Word  = 0u;

    /* Verify given parameters */
    if ((Unit   >= R_CSIG_MACRO_NUM) || \
        ( Config == 0           ) || \
        ( Config->DataLength < 7 ))
    {
        R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_PARAM);
        return R_CSIG_ERR_PARAM;
    }

    if ((Config->DataLength > 16) && ((OptFeat == 0) || (OptFeat->ExtDatLengE == 0)) ) 
    {
        R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_PARAM);
        return R_CSIG_ERR_PARAM;
    }

    devptr                = &r_csig_Dev[Unit];
    devptr->DataBitSize   = Config->DataLength;
    devptr->DataByteSize  = ((Config->DataLength - 1 ) / 8) +1;
    devptr->TxRemainBytes = 0u;
    devptr->OngoingBytes  = 0u;
    devptr->TxOngoing     = 0u;
    devptr->RxRemain      = 0u;
    devptr->RxOnlyData    = 0u;
    devptr->TxRxMode      = R_CSIG_TX_RX_OFF;
    devptr->LastRxRead    = 1u;

    /* verify that the ring buffer size is big enough for one data! */
    if ((devptr->DataByteSize > R_CSIG_RXBUF_BYTE_SIZE) ||
         (devptr->DataByteSize > R_CSIG_TXBUF_BYTE_SIZE)   ) 
    {
        R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_RBINIT);
        return R_CSIG_ERR_RBINIT;
    }

    /* Set base addresses and unit data */
    devptr->BaseAddrCsig = R_SYS_CSIG_GetCsigBaseAddr(Unit);
    devptr->BaseAddrChbb = R_SYS_CSIG_GetChbbBaseAddr(Unit);
    basecsig    = devptr->BaseAddrCsig;
    basechbb    = devptr->BaseAddrChbb;

    /* dbg info: CSIG: Clear PWR, TXE, and RXE, set normal memory mode */
    R_CSIG_WRITE_REG(8, basecsig + R_CSIG_CTL_0, 0x00);

    /* configure CTL1 */
    ctl1.Word = 0u;
    ctl1.Bit.Ckr = Config->ClockPhase;
    if (OptFeat != 0u) 
    {
        if(Config->Mode == R_CSIG_SLAVE)
        {
            ctl1.Bit.Sse  = OptFeat->SlaveSelE;
        }
        ctl1.Bit.Hse  = OptFeat->HandShakeE;
        ctl1.Bit.Dcs  = OptFeat->DataConsCheckE;
        ctl1.Bit.Edle = OptFeat->ExtDatLengE;
        if (0 == OptFeat->TicNormal) 
        {
            ctl1.Bit.Slit = 1u;
        }
        else 
        {
            ctl1.Bit.Slit = 0;
        }
        ctl1.Bit.Sit  = OptFeat->IntDelayedE;
        cfg.Bit.Ps    = OptFeat->Parity;
    }
    
    /* dbg info: CSIG: transfer protocol configuration */
    R_CSIG_WRITE_REG(32, basecsig + R_CSIG_CTL_1, ctl1.Word);

    /* Set Main baud rate see <loc_SetBaudrate> */
    if(Config->Mode == R_CSIG_SLAVE) {
        loc_SetBaudrate(Unit, 0);
    } else {
        baudrate = loc_SetBaudrate(Unit, Config->Baudrate);
        if ((baudrate != Config->Baudrate) || (baudrate==0)) 
        {
            R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_NG);
            return R_CSIG_ERR_NG;
        }
    }
    devptr->Mode = Config->Mode;

    /* Set cfg */
    if(Config->DataLength > 16) 
    {
        cfg.Bit.Dls = Config->DataLength - (uint8_t)(((Config->DataLength) >> 4u) << 4u);
    } 
    else 
    {
        if(Config->DataLength == 16) {
            cfg.Bit.Dls = 0u;
        } else {
            cfg.Bit.Dls = Config->DataLength;
        }
    }
    cfg.Bit.Dir = Config->Direction;
    /* no more available after macro correction */
    if(Config->ClockPhase == R_CSIG_CLK_ACTIVE_LOW) {
        cfg.Bit.Dap = Config->DataPhase;
    } else {
        if(Config->DataPhase == R_CSIG_DATA_PHASE_RISE) {
            cfg.Bit.Dap = 1u;
        }
    }
    /* dbg info: CSIG: dataphase, length, direction */
    R_CSIG_WRITE_REG(32, basechbb + R_CSIG_CFG0, cfg.Word);

    /* clear SCE bit (will be set by Rx enable or recep trigger API */
    /* dbg info: CSIG:  clear SCE bit */
    R_CSIG_WRITE_REG(8, basechbb + R_CSIG_BCTL_0, 0x00);

    /* Setup ring buffers */
    size = devptr->DataByteSize;
    num  = R_CSIG_RXBUF_BYTE_SIZE / size;
    if (size > 2) {
        tmp = num;
        num  = (tmp * size) + 1;   /* entire numbers of data only can be stored */
        if(num > R_CSIG_RXBUF_BYTE_SIZE) {
            tmp = size;
            num -= tmp;
        }
        size = 1u;
    }

    tmp = size;
    err = R_CDI_RbSetup(&devptr->RxRBuf, devptr->RxBuf, num, tmp);

    if (0 != err) 
    {
        R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_RBINIT);
        return R_CSIG_ERR_RBINIT;
    }
    err = R_CDI_RbSetup(&devptr->TxRBuf, devptr->TxBuf, num, tmp);
    if (0 != err) 
    {
        R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_RBINIT);
        return R_CSIG_ERR_RBINIT;
    }

    /* Set the default callbacks for all interrupts  */
    devptr->IsrCallback[R_CSIG_INT_TIC]  = 0u;
    devptr->IsrCallback[R_CSIG_INT_TIR]  = 0u;
    devptr->IsrCallback[R_CSIG_INT_TIRE] = 0u;
    /* Setup device interrupt controller see: <R_SYS_CSIG_IntCInit> */
    R_SYS_CSIG_IntCInit(Unit);


   return R_CSIG_ERR_OK;
}

/*******************************************************************************
  Function: R_CSIG_WriteData
*/
r_csig_Error_t R_CSIG_WriteData(uint32_t Unit, uint16_t TxData, uint8_t Edl) {
    uint32_t          base;
    volatile uint8_t  regval;
    r_csig_RegTxw_t   txw;

    if (Unit >= R_CSIG_MACRO_NUM) 
    {
        R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_PARAM);
    } 
    else 
    {
        base = r_csig_Dev[Unit].BaseAddrChbb;
        txw.Word     = 0u;
        txw.Bit.Edl  = (0 != Edl) ? 1 : 0;
        txw.Bit.Tx   = TxData;
        R_CSIG_WRITE_REG(32, base + R_CSIG_TX0W, txw.Word);
    }
    return R_CSIG_ERR_OK;
}


/*******************************************************************************
  Function: R_CSIG_Write
*/

int32_t R_CSIG_Write(uint32_t Unit, void *Buffer, uint32_t Num, uint8_t CopyAll) {
    int32_t ret;
    uint8_t dsize;

    if (Unit >= R_CSIG_MACRO_NUM) {
        ret = 0u;
        R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_PARAM);
    } else {
        if (Num == 0u) 
        {
            return 0;
        }
        dsize = r_csig_Dev[Unit].DataByteSize;
        if (dsize > 2) {
            ret = R_CSIG_Write_Edl(Unit, Buffer, Num, CopyAll);
        } else {
            ret = R_CSIG_Write_N_Edl(Unit, Buffer, Num, CopyAll);
        }
    }
    return ret;
}


/*******************************************************************************
  Function: R_CSIG_Read
*/

uint32_t R_CSIG_Read(uint32_t Unit, void *Buffer, uint32_t MaxNum) {
    volatile uint32_t  len, n;
    volatile uint8_t edl;
    volatile uint8_t   size;
    r_csig_Dev_t       *dev;
    uint16_t           value;
    uint8_t          mask;

    if (Unit >= R_CSIG_MACRO_NUM) {
        R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_PARAM);
        len = 0u;
    } else {
        dev  = &r_csig_Dev[Unit];
        size = dev->DataByteSize;
        if (size > 2) {
            /* EDL mode */
            uint8_t s = size;
            edl     = 1u;
            len     = R_CDI_RbNum(&dev->RxRBuf);
            len    /= s;
            MaxNum  = (len < MaxNum) ? len : MaxNum;
            MaxNum *= size;
            /* Make sure only entire data sets are read out */
        } else {
            edl = 0u;
        }

        if(dev->LastRxRead == 0u) {
            mask = R_CSIG_GetIntMask(Unit, R_CSIG_INT_TIR);
            /* if the interrupt is not masked, mask it */
            if (0  == mask) {
                R_CSIG_DisableInt(Unit, R_CSIG_INT_TIR);
            }
        }

        len = loc_SecureRbRead(Unit, &dev->RxRBuf, Buffer, MaxNum,  R_CSIG_INT_TIR);

        if(dev->LastRxRead == 0u) {
            value = R_CSIG_ReadData(Unit);
            /* currently, this case is not prepared for EDL */
            n = loc_SecureRbWrite(Unit, &dev->RxRBuf, &value, 1u, R_CSIG_INT_TIR);
            if(n != 1u) {
                R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_BUFOVERRUN);
            }
            dev->LastRxRead = 1u;

            if (0  == mask) {
                /* if the interrupt was not masked, unmask it */
                R_CSIG_EnableInt(Unit, R_CSIG_INT_TIR);
            }
        }

        if (0 != edl) 
        {
            uint8_t s = size;
            len /= s;
        }
    }
    return len;
}


/*******************************************************************************
  Function: R_CSIG_ReadData
*/
uint16_t R_CSIG_ReadData(uint32_t Unit) 
{
    uint32_t    base;
    uint16_t    rxdata;

    if (Unit >= R_CSIG_MACRO_NUM) 
    {
        R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_PARAM);
    } 
    else 
    {
        base   = r_csig_Dev[Unit].BaseAddrChbb;
        rxdata = R_CSIG_READ_REG(16, base + R_CSIG_RX0);
    }

    return rxdata;
}

/*******************************************************************************
  Function: R_CSIG_ReadDCE
*/
/*Reads and checks set of DCE bit*/

uint32_t R_CSIG_ReadDCE(uint32_t Unit) 
{

    uint32_t CSIGStr0Val = 0u;

    CSIGStr0Val = R_CSIG_READ_REG(32, r_csig_Dev[Unit].BaseAddrCsig + R_CSIG_STR_0);

    return (CSIGStr0Val & 0x8u);
}

/*******************************************************************************
  Function: R_CSIG_ReadOvr
*/
/*Reads and checks set of overrun bit*/

uint32_t R_CSIG_ReadOvr(uint32_t Unit) 
{

    uint32_t CSIGStr0Val = 0u;

    CSIGStr0Val = R_CSIG_READ_REG(32, r_csig_Dev[Unit].BaseAddrCsig + R_CSIG_STR_0);

    return (CSIGStr0Val & 0x1u);
}

/*******************************************************************************
  Function: R_CSIG_GetTransferStat
*/
/*Reads and checks set Transfer Status bit*/

uint32_t R_CSIG_GetTransferStat(uint32_t Unit) 
{

    uint32_t CSIGStr0Val = 0u;

    CSIGStr0Val = R_CSIG_READ_REG(32, r_csig_Dev[Unit].BaseAddrCsig + R_CSIG_STR_0);

    return (CSIGStr0Val & 0x80u);
}

/*******************************************************************************
  Function: R_CSIG_ReadErx
*/
/* Reads the ERX emulation register */

uint16_t R_CSIG_ReadErx(uint32_t Unit) 
{

    uint16_t CSIGErxVal = 0u;

    CSIGErxVal = R_CSIG_READ_REG(16, r_csig_Dev[Unit].BaseAddrChbb + R_CSIG_ERX0);

    return (CSIGErxVal);
}


/*******************************************************************************
  Function: R_CSIG_SlaveRxEnable
*/

int32_t R_CSIG_SlaveRxEnable(int32_t  Unit ) 
{

    r_csig_RegCtl0_t    ctl0temp;
    uint32_t            base;

    if (Unit > R_CSIG_MACRO_NUM ) {
        R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_PARAM);
        return R_CSIG_ERR_PARAM;
    }

    /* Set base addresses and unit data */
    base          = r_csig_Dev[Unit].BaseAddrCsig;
    ctl0temp.Byte = R_CSIG_READ_REG(8, base + R_CSIG_CTL_0);

    /* Check that it is Rx-Only mode */
    if((ctl0temp.Bit.Txe == 1) || (ctl0temp.Bit.Rxe == 0u)) 
    {
        return R_CSIG_ERR_NG;
    }
    /* dbg info: CSIG: Enable slave reception */
    base = r_csig_Dev[Unit].BaseAddrChbb;
    R_CSIG_WRITE_REG(8, base + R_CSIG_BCTL_0, 0x01);

    return R_CSIG_ERR_OK;
}


/*******************************************************************************
  Function: R_CSIG_SlaveRxDisable
*/

int32_t R_CSIG_SlaveRxDisable(int32_t  Unit ) 
{

    r_csig_RegCtl0_t    ctl0temp;
    uint32_t            base;

    if (Unit > R_CSIG_MACRO_NUM ) {
        R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_PARAM);
        return R_CSIG_ERR_PARAM;
    }

    /* Set base addresses and unit data */
    base          = r_csig_Dev[Unit].BaseAddrCsig;
    ctl0temp.Byte = R_CSIG_READ_REG(8, base + R_CSIG_CTL_0);

    /* Check that it is Rx-Only mode */
    if((ctl0temp.Bit.Txe == 1u) || (ctl0temp.Bit.Rxe == 0)) 
    {
        return R_CSIG_ERR_NG;
    }

    base = r_csig_Dev[Unit].BaseAddrChbb;
    /* dbg info: CSIG: Disable slave reception */
    R_CSIG_WRITE_REG(8, base + R_CSIG_BCTL_0, 0x00);

    return R_CSIG_ERR_OK;
}


/*******************************************************************************
  Function: R_CSIG_DeInit
*/

int32_t R_CSIG_DeInit(uint32_t Unit) 
{
    int32_t      err;
    err = R_CSIG_Disable(Unit);

    return err;
}


/*******************************************************************************
  Function: R_CSIG_Enable(uint32_t Unit)
*/

int32_t R_CSIG_Enable(uint32_t Unit, r_csig_TxRxMode_t Mode) 
{
    volatile uint32_t   base;
    r_csig_RegCtl0_t    ctl0temp;

    if (Unit >= R_CSIG_MACRO_NUM) {
        return R_CSIG_ERR_PARAM;
    } else {
        switch(Mode) {
            case R_CSIG_TX_ONLY:
                ctl0temp.Byte  = 0xc1;
                break;
            case R_CSIG_RX_ONLY:
                ctl0temp.Byte  = 0xa1;
                break;
            case R_CSIG_TX_RX:
                ctl0temp.Byte  = 0xe1;
                break;
            default:
                ctl0temp.Byte  = 0x81;
                break;
        }

        r_csig_Dev[Unit].TxRxMode = Mode;
        base = r_csig_Dev[Unit].BaseAddrCsig;
        /* dbg info: CSIG: Enable Tx/Rx */
        R_CSIG_WRITE_REG(8, base + R_CSIG_CTL_0, ctl0temp.Byte);
    }
    return R_CSIG_ERR_OK;
}


/*******************************************************************************
  Function: R_CSIG_Disable(uint32_t Unit)
*/

int32_t R_CSIG_Disable(uint32_t Unit) 
{
    volatile uint32_t   base;
    r_csig_RegCtl0_t    ctl0temp;

    if (Unit >= R_CSIG_MACRO_NUM) 
    {
        return R_CSIG_ERR_PARAM;
    } else {
        r_csig_Dev[Unit].TxRxMode = R_CSIG_TX_RX_OFF;

        base     = r_csig_Dev[Unit].BaseAddrCsig;
        ctl0temp.Byte    = R_CSIG_READ_REG(8, base + R_CSIG_CTL_0);
        ctl0temp.Bit.Pwr = 0u;
        /* dbg info: CSIG: Disable */
        R_CSIG_WRITE_REG(8, base + R_CSIG_CTL_0, ctl0temp.Byte);
    }
    return R_CSIG_ERR_OK;
}

/*******************************************************************************
  Function: R_CSIG_SetErrorCallback
*/

void R_CSIG_SetErrorCallback(void(*ErrorCallback )(uint32_t Unit, int32_t Error)) 
{

    loc_ErrorHandler    = ErrorCallback;

}


/*******************************************************************************
  Function: R_CSIG_ForceInt
*/

void R_CSIG_ForceInt(uint32_t Unit, r_csig_Interrupt_t Interrupt) 
{

    R_SYS_CSIG_ForceClearInt(Unit, Interrupt, 1);
}


/*******************************************************************************
  Function: R_CSIG_GetStatError
*/

r_csig_StatError_t R_CSIG_GetStatError(uint32_t Unit) 
{
    volatile uint32_t   base;
    r_csig_StatError_t  ret;
    r_csig_RegStr0_t    stat;

    ret.Byte= 0x00;
    if (Unit < R_CSIG_MACRO_NUM) {
        base      = r_csig_Dev[Unit].BaseAddrCsig;
        stat.Word = R_CSIG_READ_REG(32, base + R_CSIG_STR_0);
        if (stat.Bit.Dce == 1) 
        {
            ret.Bit.DcErr  = 1u;
        }
        if (stat.Bit.Pe == 1) 
        {
            ret.Bit.PaErr  = 1u;
        }
        if(stat.Bit.Ove == 1)
        {
            ret.Bit.OvrErr = 1u;
        }
    }
    return ret;
}


/*******************************************************************************
  Function: R_CSIG_ClearStatError
*/

void R_CSIG_ClearStatError(uint32_t Unit, r_csig_StatError_t Error) {
    volatile uint32_t   base;
    r_csig_RegStc0_t    cstat;

    if (Unit < R_CSIG_MACRO_NUM) {
        base        = r_csig_Dev[Unit].BaseAddrCsig;
        cstat.Hword = 0x0000;
        if(0 != Error.Bit.OvrErr)
        {
            cstat.Bit.Ovec = 1u;
        }
        if(0 != Error.Bit.PaErr  )
        {
            cstat.Bit.Pec  = 1u;
        }
        if(0 != Error.Bit.DcErr  )
        {
            cstat.Bit.Dcec = 1u;
        }
        /* dbg info: CSIG: Clear errors status */
        R_CSIG_WRITE_REG(16, base + R_CSIG_STC_0, cstat.Hword);
    }
    return;
}


/*******************************************************************************
  Function: R_CSIG_TrigReception
*/
int32_t R_CSIG_TrigReception(uint32_t Unit, uint32_t Num) {
    r_csig_Dev_t *devptr;
    uint32_t   base;
    uint16_t   dummy;

    if (Unit >= R_CSIG_MACRO_NUM) {
        return R_CSIG_ERR_PARAM;
    } else {
        devptr = &r_csig_Dev[Unit];
    }
    if(devptr->TxRxMode != R_CSIG_RX_ONLY) {
        return R_CSIG_ERR_NG;
    }
    if(devptr->RxOnlyData != 0u)
    {
        return R_CSIG_ERR_BUSY;
    } else {
        devptr->RxOnlyData = Num;
    }
    base   = devptr->BaseAddrChbb;
    /* dbg info: CSIG: Trigger reception */
    R_CSIG_WRITE_REG(8, base + R_CSIG_BCTL_0, 0x01);
    dummy  = 0u;
    dummy += R_CSIG_READ_REG(32, base + R_CSIG_RX0);
    return R_CSIG_ERR_OK;
}

/*******************************************************************************
  Function: R_CSIG_BrkPtCfg
*/

r_csig_Error_t R_CSIG_BrkPtCfg(uint32_t Unit, uint8_t Stop) {

    r_csig_Error_t ret;
    uint32_t       base;
    uint8_t        val;

    ret = R_CSIG_ERR_OK;

    if (Unit >= R_CSIG_MACRO_NUM) 
    {
        ret = R_CSIG_ERR_PARAM;
        R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_PARAM);
    } 
    else 
    {
        val  = (0 != Stop) ? 0x00 : 0x80;
        base = r_csig_Dev[Unit].BaseAddrCsig;
        /* dbg info: CSIG: set debug behaviour */
        R_CSIG_WRITE_REG(8, base + R_CSIG_EMU, val);
    }
    return ret;
}

/*******************************************************************************
  Function: R_CSIG_LoopBackE
*/

r_csig_Error_t R_CSIG_LoopBackE(uint32_t Unit, uint8_t Enable) {
    r_csig_Error_t   ret;
    uint32_t         base;
    r_csig_RegCtl1_t ctl1;
    r_csig_RegCtl0_t ctl0;

    ret = R_CSIG_ERR_OK;

    if (Unit >= R_CSIG_MACRO_NUM) {
        ret = R_CSIG_ERR_PARAM;
        R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_PARAM);
    } else {
        base = r_csig_Dev[Unit].BaseAddrCsig;
        /* dbg info: CSIG: Enable loopback - start */

        /* Clear PWR bit */
        ctl0.Byte    = R_CSIG_READ_REG(8, base + R_CSIG_CTL_0);
        ctl0.Bit.Pwr = 0u;
        R_CSIG_WRITE_REG(8, base + R_CSIG_CTL_0, ctl0.Byte);

        /* Configure LBM */
        ctl1.Word = R_CSIG_READ_REG(32, base + R_CSIG_CTL_1);
        if(0 != Enable) 
        {
            ctl1.Bit.Lbm = 1u;
        }
        else
        {
            ctl1.Bit.Lbm = 0u;
        }
        R_CSIG_WRITE_REG(32, base + R_CSIG_CTL_1, ctl1.Word);

        /* Set PWR bit */
        ctl0.Byte    = R_CSIG_READ_REG(8, base + R_CSIG_CTL_0);
        ctl0.Bit.Pwr = 1u;
        R_CSIG_WRITE_REG(8, base + R_CSIG_CTL_0, ctl0.Byte);
        /* dbg info: CSIG: Enable loopback - end */
    }
    return ret;
}


/*******************************************************************************
  Function: R_CSIG_GetNbRec
*/

uint8_t R_CSIG_GetNbRec(uint32_t Unit) {

    uint8_t ret;

    if (Unit >= R_CSIG_MACRO_NUM) {
        ret = 0u;
    } else {
        ret = R_CDI_RbNum(&r_csig_Dev[Unit].RxRBuf);
    }
    return ret;
}

/*******************************************************************************
  Function: R_CSIG_SetFilterBypass
*/
uint32_t R_CSIG_SetFilterBypass(uint32_t Unit)
{
    return R_SYS_CSIG_SetFilterBypass(Unit);
}


/*******************************************************************************
  Function: R_CSIG_GetClock
*/

uint32_t R_CSIG_GetClock(uint32_t Unit) 
{
    return R_SYS_CSIG_GetClock(Unit);
}


/*******************************************************************************
  Function: R_CSIG_EnableDTSInt
*/

void R_CSIG_EnableDTSInt(uint32_t Unit); 
void R_CSIG_EnableDTSInt(uint32_t Unit)
{
    R_SYS_CSIG_EnableDTSInt(Unit);
}


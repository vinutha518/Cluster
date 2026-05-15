/*
****************************************************************************
PROJECT : CSIG driver
FILE    : $Id: r_csig_isr.c 3935 2014-09-23 08:52:32Z golczewskim $
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
  Title: CSIG Interrupt Functions

  Handle the CSIG interrupts.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_cdi_api.h"
#include "r_dev_api.h"
#include "r_csig_api.h"
#include "r_csig_sys.h"
#include "r_config_csig.h"
#include "r_csig_regs.h"
#include "r_csig_main.h"
/*******************************************************************************
  Section: Local  Functions
*/
static void R_CSIG_IsrTirRx(uint32_t Unit); 
static void R_CSIG_IsrTirStd(uint32_t Unit);


/*******************************************************************************
  Function: R_CSIG_IsrTirStd
*/

static void R_CSIG_IsrTirStd(uint32_t Unit)
{
    uint16_t  value;
    uint32_t  len;
    uint8_t   size, n, *remain;
    r_csig_Dev_t *dev;

    uint8_t last, edl;  /* last data in EDL */
    
    dev = &r_csig_Dev[Unit];

    remain = &dev->RxRemain;
    /* if no more Rx was expected, this is new data (part) */
    if(*remain == 0u)
    {
        *remain = dev->DataByteSize;
    }

    edl = (dev->DataByteSize > 2) ? 1 : 0;
    if(0 != edl)
    {
        size = (*remain > 1) ? 2 : 1;
        n    = size;
    } 
    else
    {
        n    = 1u;
        size = *remain;
    }

    /* read data */
    value = R_CSIG_ReadData(Unit);
    len   = R_CDI_RbWrite(&dev->RxRBuf, (void*)&value, n);
    if (len != n)
    {
        R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_BUFOVERRUN);
    }

    /* if necessary and available, run callback function */
    *remain -= size;
    last = (*remain == 0u) ? 1 : 0;
    if ((dev->IsrCallback[R_CSIG_INT_TIR] != 0) && (last == 1)) 
    {
        dev->IsrCallback[R_CSIG_INT_TIR]();
    }
}

/*******************************************************************************
  Function: R_CSIG_IsrTirRx
*/
static void R_CSIG_IsrTirRx(uint32_t Unit) 
{

    uint16_t  value;
    uint32_t  len, tmp;
    uint8_t   size, n, *remain;
    r_csig_Dev_t *devptr;

    uint8_t last, edl;  /* last data in EDL */

    devptr = &r_csig_Dev[Unit];
    remain = &devptr->RxRemain;
    /* if no more Rx was expected, this is new data (part) */
    if(*remain == 0u) 
    {
        *remain = devptr->DataByteSize;
    }

    edl = (devptr->DataByteSize > 2) ? 1 : 0;
    if (0 != edl) 
    {
        size = (*remain > 1) ? 2 : 1;
        n    = size;
    }
    else
    {
        n    = 1u;
        size = *remain;
    }

    /* read data */
    tmp = devptr->RxOnlyData - 1; 
    if((devptr->Mode == R_CSIG_MASTER) && (tmp == 1))
    {
        R_CSIG_WRITE_REG(8, devptr->BaseAddrChbb + R_CSIG_BCTL_0, 0x00);
    }

    if (0 != edl) 
    {
        value = R_CSIG_ReadData(Unit);
        len   = R_CDI_RbWrite(&devptr->RxRBuf, (void*)&value, n);
        if (len != n)
        {
            R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_BUFOVERRUN);
        }
    } 
    else
    {
        /* check that there is enough space */
        tmp  = R_CSIG_TXBUF_BYTE_SIZE;
        if(size == 2 )
        {
            tmp = tmp >> 1;
        }
        tmp -= R_CDI_RbNum(&devptr->RxRBuf);
        /* if not enough space, data is not read
           (not an issue in master mode or if HS is used) */
        if(tmp < 2 )
        {
            devptr->LastRxRead  = 0u;
        }
        else
        {
            value = R_CSIG_ReadData(Unit);
            len   = R_CDI_RbWrite(&devptr->RxRBuf, (void*)&value, n);
            if (len != n)
            {
                R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_BUFOVERRUN);
            }
            devptr->LastRxRead    = 1u;
        }
    }

    /* if necessary and available, run callback function */
    *remain -= size;
    last = (*remain == 0u) ? 1 : 0;
    if ((devptr->IsrCallback[R_CSIG_INT_TIR] != 0) && (last == 1))
    {
        devptr->IsrCallback[R_CSIG_INT_TIR]();
    }
}


/*******************************************************************************
  Section: Global API Functions

  For comments see file r_csig_api.h
*/


/*******************************************************************************
  Function: R_CSIG_SetIsrCallback

*/
r_csig_Error_t R_CSIG_SetIsrCallback(
    uint32_t Unit,
    r_csig_Interrupt_t IntType,
    void (*Isr)(void))
{

    if (Unit >= R_CSIG_MACRO_NUM) 
    {
        return R_CSIG_ERR_PARAM;
    }
    else
    {
        r_csig_Dev[Unit].IsrCallback[IntType] = Isr;
    }
    return R_CSIG_ERR_OK;
}

/*******************************************************************************
  Function: R_CSIG_GetIntMask

*/
uint8_t R_CSIG_GetIntMask(uint32_t Unit, r_csig_Interrupt_t Interrupt) 
{
    /* Check range */
    if ((Unit >= R_CSIG_MACRO_NUM) || (Interrupt >= R_CSIG_INT_NUM))
    {    
        return R_CSIG_ERR_PARAM;
    }
    R_SYS_CSIG_GetIntMask(Unit, Interrupt);

    return R_CSIG_ERR_OK;

}



/*******************************************************************************
  Function: R_CSIG_EnableInt

*/
r_csig_Error_t R_CSIG_EnableInt(uint32_t Unit, r_csig_Interrupt_t Interrupt) 
{
    /* Check range */
    if ((Unit >= R_CSIG_MACRO_NUM) || (Interrupt >= R_CSIG_INT_NUM))
    {    
        return R_CSIG_ERR_PARAM;
    }
    R_SYS_CSIG_EnableInt(Unit, Interrupt);

    return R_CSIG_ERR_OK;

}


/*******************************************************************************
  Function: R_CSIG_DisableInt

*/
r_csig_Error_t R_CSIG_DisableInt(uint32_t Unit, r_csig_Interrupt_t Interrupt) 
{
    /* Check range */
    if ((Unit >= R_CSIG_MACRO_NUM) ||(Interrupt >= R_CSIG_INT_NUM))
    {
        return R_CSIG_ERR_PARAM;
    }
    R_SYS_CSIG_DisableInt(Unit, Interrupt);

    return R_CSIG_ERR_OK;
}



/*******************************************************************************
  Function: R_CSIG_IsrTic
*/
void R_CSIG_IsrTic(uint32_t Unit)
{
    uint32_t len;
    /*static uint32_t toggle = 0u;*/
    uint16_t txdata;
    uint8_t  size, n;
    uint8_t *rbytes, *dsize, *obytes;
    uint8_t last, edl;  /* last data in EDL */
    void (*callback)(void);
    r_csig_Dev_t *devptr;
    /* uint8_t  selectedID; */

/*    enable the port toggle lines below for testing interrupt timings, since communication not occurring
      as expected when including these lines!*/
/****************************************/
#if 0
      if (toggle%2)
        eePortC_ClearPin(0u,8u);
      else
        eePortC_SetPin(0u,8u);

        toggle++;
#endif

    if (Unit >= R_CSIG_MACRO_NUM)
    {
       R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_PARAM);
    }
    else
    {
        devptr = &r_csig_Dev[Unit];

        if(devptr->TxRxMode != R_CSIG_RX_ONLY) {

            rbytes = &devptr->TxRemainBytes;
            dsize  = &devptr->DataByteSize;
            obytes = &devptr->OngoingBytes;

            *rbytes -= *obytes;

            if(*rbytes == 0u) {
                last = 1u;
                size = (*dsize > 1) ? 2 : 1;
            } else {
                last = 0u;
                size = (*rbytes > 1) ? 2 : 1;
            }

            edl = (*dsize > 2) ? 1 : 0;

            /* check for data to be sent and send if necessary */
            if ((0 != edl) && (size > 1)) 
            {
                n = (R_CDI_RbNum(&devptr->TxRBuf) >= 2) ? 2 : 0;
            } 
            else 
            {
                n = 1u;
            }
            txdata = 0u;
            len    = R_CDI_RbRead(&devptr->TxRBuf, &txdata, n);

            if (0 != len) 
            {
                /* save how much bytes are currently sent */
                *obytes = size;
                /* if previous data was last, reset total nb bytes cnt */
                if (0 != last) 
                {
                    *rbytes = *dsize;
                }
                edl = (*rbytes > 2) ? 1 : 0;
                R_CSIG_WriteData(Unit, txdata, edl);
            } 
            else 
            {
                if(0 == last) 
                {
                    /* if data is still expected (EDL), error */
                    R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_EDL);
                } 
                else 
                {
                    *obytes = 0u;
                }
            }
        } 
        else 
        {
            /* for Rx-only mode, always allow call-back function */
            last = 1u;
        }

        callback = devptr->IsrCallback[R_CSIG_INT_TIC];
        if ((0 != last) && (0 != callback)) 
        {
            callback();
        }
    }
}

/*******************************************************************************
  Function: R_CSIG_IsrTir
*/

void R_CSIG_IsrTir(uint32_t Unit)
{
    if (Unit >= R_CSIG_MACRO_NUM)
    {
        R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_PARAM);
    } 
    else 
    {
        switch(r_csig_Dev[Unit].TxRxMode) 
        {
        case R_CSIG_RX_ONLY:
            R_CSIG_IsrTirRx(Unit);
            break;
        default:
            R_CSIG_IsrTirStd(Unit);
            break;
        }
    }
}

/*******************************************************************************
  Function: R_CSIG_IsrTire
*/

void R_CSIG_IsrTire(uint32_t Unit) 
{
    if (Unit >= R_CSIG_MACRO_NUM)
    {
        R_CSIG_ErrorHandler(Unit, R_CSIG_ERR_PARAM);
    }
    else 
    {
        if (r_csig_Dev[Unit].IsrCallback[R_CSIG_INT_TIRE] != 0u)
        {
            r_csig_Dev[Unit].IsrCallback[R_CSIG_INT_TIRE]();
        }
    }
}


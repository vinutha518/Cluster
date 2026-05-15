/*
****************************************************************************
PROJECT : VLIB Macro Driver  
FILE    : $Id: r_rtca_main.c 7171 2016-01-06 17:52:36Z tobyas.hennig $
============================================================================ 
DESCRIPTION
Driver for the RTCA macro
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
  Title: RTCA Main Module

  Implementation of the API and other global functions.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_rtca.h"
#include "r_rtca_regs.h"
#include "r_rtca_api.h"
#define RTCA_MAIN_GLOBALS_
#include "r_rtca_main.h"
#include "r_rtca_sys.h"


/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Constant: loc_RtcaVersion

  A string containig the macro driver version information  
*/

static const int8_t  loc_RtcaDrvVer[] = R_VERSION(RTCA, R_RTCA_VERSION_HI, R_RTCA_VERSION_LO);


/***********************************************************
  Section: Global  Variables
*/

/*RTCA clock*/
/* #define R_CKSC16    0xFF420100 PRÜFEN!!! */


/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: loc_ErrorHandler

  A function pointer to a user callback (Applications error handler)  
  
  The value is assigned by the application using the driver 
*/

static void (*loc_ErrorHandler )(uint32_t Unit, uint32_t Error) = 0u;


/*******************************************************************************
  Section: Local Functions
*/
uint8_t loc_GetDecimal(uint8_t BcdCode, r_rtca_HourFormat_t Hour) ; 
uint32_t loc_BuildGroup(r_rtca_Data_t *Data); 
uint8_t loc_GetBcd(uint8_t DecValue, r_rtca_HourFormat_t Hour); 
uint8_t loc_GetDecimal(uint8_t BcdCode, r_rtca_HourFormat_t Hour); 
void loc_ReadGroup(uint32_t GroupData, r_rtca_Data_t *Data);  
static uint8_t loc_SetBits8(uint8_t RegValue, uint16_t BitValue, uint8_t BitPos, uint8_t BitWidth);  

/***********************************************************
  Function: loc_GetDecimal

  This function is used to convert BCD coded register values to Decimal values.
  
  Application data always uses normal decimal / hexadecimal values. This function
  is a driver service funtion to convert the register values for further usage.
  The BCD code of hour values in 12h mode differs.

  Parameters:
    
  BcdCode       - Register value
  Hour          - Information about register value (hour value(12/24) 
                  or non hour value)

  Returns:
  
  Hexadecimal value
*/

uint8_t loc_GetDecimal(uint8_t BcdCode, r_rtca_HourFormat_t Hour) 
{
    uint8_t tmp_bcd;
    uint8_t number;
    
    if ((Hour == R_RTCA_12H) && ( 0!= (BcdCode & 0x20u))) 
    {
        if (BcdCode == 0x32u) 
        {
            BcdCode = 0x0;
        } 
        else 
        {
            BcdCode &= 0xDFu;
        }
    }
     
    if (Hour == R_RTCA_12H) 
    {
        if (BcdCode == 12u) 
        {
            BcdCode = 0;
        }
    }
   
    number  = BcdCode & 0x0Fu;
    tmp_bcd = BcdCode & 0xF0u;
    tmp_bcd = ((tmp_bcd >> 4u) & 0x0Fu);
    number  = ((tmp_bcd * 10u) + number);
    
    return number;    
}


/***********************************************************
  Function: loc_GetBcd

  This function is used to convert BCD coded register values to Decimal values.
  
  Application data always uses normal decimal / hexadecimal values. This function
  is a driver service funtion to convert the register values for further usage.
  The BCD code of hour values in 12h mode differs.

  Parameters:
    
  BcdCode       - Register value
  Hour          - Information about register value (hour value(12/24) 
                  or non hour value)

  Returns:
  BCD coded value
*/

uint8_t loc_GetBcd(uint8_t DecValue, r_rtca_HourFormat_t Hour) 
{    
    uint8_t digit;
    uint8_t digit_10;
    uint8_t tmp_num;
    uint8_t bcd_value;
    
    bcd_value = 0;
    digit     = 0;
    digit_10  = 0;
    tmp_num   = DecValue;
    
    /* BCD code differs in 12h mode for hours. 
       For all other values Hour is R_RTCA_NON */
    if (Hour == R_RTCA_12H) 
    {    
        if (DecValue == 0) 
        {
            tmp_num = 12u;
        } 
        else if (DecValue > 12u) 
        {
            tmp_num -= 12;
        }
        else 
        {
            
        }    
    }

    if (tmp_num >= 10u) 
    {
        while (tmp_num >= 10u) 
        {
            tmp_num = tmp_num - 10u;
            digit_10++;
        }
    } 

    digit     = tmp_num;
    bcd_value = (uint8_t) (digit_10 << 4u) | digit; 
    
   /* if ((Hour == R_RTCA_12H) && (DecValue > 12u))
    {    
        bcd_value |= 0x20u;
    }*/

    return bcd_value;
}


/***********************************************************
  Function: loc_BuildGroup

  This driver service function is used to build a date or time group

  Parameters:
    
  Data      - see: <eeRtca_Data_t>

  Returns:
  BCD coded value
  
*/

uint32_t loc_BuildGroup(r_rtca_Data_t *Data) 
{    
    uint32_t groupData;
    
    groupData = 0;
    
    if (Data->Group == R_RTCA_DATE) 
    {
        groupData  = (uint32_t)Data->Year << 8u;
        groupData |= (uint32_t)Data->Month;
        groupData  = (uint32_t)groupData << 8u;
        groupData |= (uint32_t)Data->Day;
        groupData  = (uint32_t)groupData << 8u;
        groupData |= Data->Week; 
        
    } 
    else 
    {
        groupData  = (uint32_t)Data->H << 8u;
        groupData |= Data->Min;
        groupData  = (uint32_t)groupData << 8u;
        groupData |= Data->Sec;
    }

   return groupData;
}


/*******************************************************************************
  Function: loc_ReadGroup

  This driver service function is used to read a data group.
  
  The 32 Bit value will be seperated into several single Values which contain to
  the data structure.
  
  Parameters:
    
    Data      - Central Structure which contains time Information
                see: <eeRtca_Data_t>
                
    Groupdata - 32 Bit register data
    
  Returns:
  BCD coded value
  
*/

void loc_ReadGroup(uint32_t GroupData, r_rtca_Data_t *Data) 
{     
    if (Data->Group == R_RTCA_DATE) 
    {
        Data->Week  = GroupData | 0x0Fu;
        GroupData   = GroupData >> 4;
        Data->Day   = GroupData | 0x0Fu;
        GroupData   = GroupData >> 4;
        Data->Month = GroupData | 0x0Fu;
        GroupData   = GroupData >> 4;
        Data->Year  = GroupData | 0x0Fu;
    } 
    else if (Data->Group == R_RTCA_TIMEVAL) 
    {      
        Data->Sec   = GroupData | 0x0Fu;
        GroupData   = GroupData >> 4;
        Data->Min   = GroupData | 0x0Fu;
        GroupData   = GroupData >> 4;
        Data->H     = GroupData | 0x0Fu;            
   }
   else 
   {
       
   }
}


/*******************************************************************************
  Function: loc_SetBits8
*/

static uint8_t loc_SetBits8(uint8_t RegValue, uint16_t BitValue, uint8_t BitPos, uint8_t BitWidth) 
{
    uint8_t RegMask   = 0x00;
    uint8_t RegValMod = 0x00;
    uint8_t i;

    /* generate mask*/
    for (i = 0u; i < BitWidth; i++) 
    {
        RegMask = (uint16_t)((uint16_t)RegMask << 1u) | 1u;
    }
    RegMask = (uint16_t)((uint16_t)RegMask << BitPos);

    /* clear bits*/
    RegValMod = RegValue & (uint16_t)(~((uint16_t)RegMask));

    /* set bits*/
    RegValMod |= (uint16_t)((uint16_t)BitValue << BitPos);

    return RegValMod;
}


/*******************************************************************************
  Section: Global Functions
  See: <rtca_main.h>
*/

/*******************************************************************************
  Function: R_RTCA_CheckMacroParams
  
  See: <rtca_main.h>
*/

r_rtca_Error_t R_RTCA_CheckMacroParams(uint32_t Unit)
{
    /* Feel free to add any more usefull parameter checks */
    if (R_RTCA_MACRO_NUM <= Unit)
    {
        return  R_RTCA_ERR_RANGE; 
    }
    return R_RTCA_ERR_OK;  
}


/*******************************************************************************
  Function: R_RTCA_ErrorHandler
  
  See: <rtca_main.h>
*/

void R_RTCA_ErrorHandler(uint32_t Unit, r_rtca_Error_t Error) 
{
    if (0 != loc_ErrorHandler) 
    {      /* we've got a registered error handler here ...*/
        loc_ErrorHandler(Unit, (uint32_t) Error);      /* ... and we gonna use it!*/
    } 
    else 
    {
        for(;;) 
        {        /* we don't have a handler */
        }        /* so, let's wait here */
    }
}


/*******************************************************************************
  Section: Global API Functions
  
  See: <rtca_api.h>
*/


/*******************************************************************************
  Function: R_RTCA_SetErrorCallback
*/

void R_RTCA_SetErrorCallback(void(*ErrorCallback )(uint32_t Unit, uint32_t Error)) 
{
    loc_ErrorHandler = ErrorCallback;
}


/*******************************************************************************
  Function: R_RTCA_GetVersionStr

  See: <rtca_api.h> 
*/

const int8_t * R_RTCA_GetVersionStr(void) 
{
    return (const int8_t *) loc_RtcaDrvVer;
}


/*******************************************************************************
  Function: R_RTCA_GetMajorVersion

  See: <rtca_api.h> 
*/

const uint16_t R_RTCA_GetMajorVersion(void) 
{
    return (uint16_t)R_RTCA_VERSION_HI; 
}


/*******************************************************************************
  Function: R_RTCA_GetMinorVersion

  See: <rtca_api.h> 
*/

const uint16_t R_RTCA_GetMinorVersion(void) 
{
    return (uint16_t)R_RTCA_VERSION_LO; 
}


/*******************************************************************************
  Function: R_RTCA_SetCorrection
*/

r_rtca_Error_t R_RTCA_SetCorrection(uint32_t Unit, r_rtca_Correction_t Val) 
{
    uint32_t       base            = 0;
    uint8_t        regVal          = 0;
    uint8_t        status          = 0;
    uint8_t        r_rtca_reg_SUBU = 0;
    r_rtca_Error_t error_mode      = R_RTCA_ERR_OK;
    
    if (Unit >= R_RTCA_MACRO_NUM) 
    {
        error_mode = R_RTCA_ERR_INSTANCE;
    } 
    else 
    {
        base = r_rtca_Dev[Unit].BaseAddr;
    
        r_rtca_reg_SUBU = R_RTCA_READ_REG(8, base + R_RTCA_SUBU);
        r_rtca_reg_SUBU = loc_SetBits8(r_rtca_reg_SUBU, Val.Sign, 
                                       R_RTCA_SUBU_F6_POS, R_RTCA_SUBU_F6_WID);
        r_rtca_reg_SUBU = loc_SetBits8(r_rtca_reg_SUBU, Val.Val, 
                                       R_RTCA_SUBU_F_POS, R_RTCA_SUBU_F_WID);
        r_rtca_reg_SUBU = loc_SetBits8(r_rtca_reg_SUBU, Val.Interv, 
                                       R_RTCA_SUBU_DEV_POS, R_RTCA_SUBU_DEV_WID);

        status = 0xFFu;
        regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL0);
        
        if ( 0 != (regVal & 0x40u)) 
        {
            /* wait till write operation is finished*/
            while (0 != (status & 0x20u)) 
            {
                status = R_RTCA_READ_REG(8, base + R_RTCA_CTL2);  
            }
        }

        R_RTCA_WRITE_REG (8, base + R_RTCA_SUBU, r_rtca_reg_SUBU);
    }
    return error_mode;
} 


/*******************************************************************************
  Function: R_RTCA_Set
*/

r_rtca_Error_t R_RTCA_Set(uint32_t Unit, r_rtca_Data_t *Data)
{    
    uint32_t       base            = 0;
    uint8_t        regVal          = 0;
    uint32_t       groupVal        = 0;
    uint8_t        change          = 0;
    r_rtca_Error_t error_mode      = R_RTCA_ERR_OK;
  
    if (Unit >= R_RTCA_MACRO_NUM) 
    {
        error_mode = R_RTCA_ERR_INSTANCE;
    } 
    else 
    {
        change = 0;
        base = r_rtca_Dev[Unit].BaseAddr; 

        if ((Data->Group == R_RTCA_DATE) || (Data->Group == R_RTCA_BOTH)) 
        {
            if (Data->Method == R_RTCA_GROUP) 
            {    
                if (Data->Group == R_RTCA_BOTH) 
                {
                    change      = 1;
                    Data->Group = R_RTCA_DATE;
                }

                groupVal =  loc_BuildGroup(Data);
                
                R_RTCA_WRITE_REG(32, base + R_RTCA_CAL, groupVal);

                if (Data->Group == R_RTCA_BOTH) 
                {
                    Data->Group = R_RTCA_DATE;
                }

                if (0 != change)
                {
                    Data->Group = R_RTCA_BOTH;
                    change = 0;
                }   
            } 
            else 
            {
                regVal = Data->Week;
                R_RTCA_WRITE_REG(8, base + R_RTCA_WEEK, regVal);
                regVal = Data->Day;
                R_RTCA_WRITE_REG(8, base + R_RTCA_DAY, regVal);
                regVal = Data->Month;
                R_RTCA_WRITE_REG(8, base + R_RTCA_MON, regVal);
                regVal = Data->Year;
                R_RTCA_WRITE_REG(8, base + R_RTCA_YEAR, regVal);
            }
        } 

        if ((Data->Group == R_RTCA_TIMEVAL) || (Data->Group == R_RTCA_BOTH)) 
        {
            if (Data->Method == R_RTCA_GROUP) 
            {
                if (Data->Group == R_RTCA_BOTH) 
                {
                    change = 1;
                    Data->Group = R_RTCA_TIMEVAL;
                }

                groupVal =  loc_BuildGroup(Data);
                R_RTCA_WRITE_REG(32, base + R_RTCA_TIME, groupVal);

                if ( 0 != change)
                {
                    Data->Group = R_RTCA_BOTH;
                    change = 0;
                }              
            } 
            else 
            {
            	regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL0);
                if(Data->Format == R_RTCA_12H)
                	regVal &= 0xDF;
                else
                	regVal |= 0x20;
                 R_RTCA_WRITE_REG(8, base + R_RTCA_CTL0, regVal);
                regVal = Data->Sec;
                R_RTCA_WRITE_REG(8, base + R_RTCA_SEC, regVal);
                regVal = Data->Min;
                R_RTCA_WRITE_REG(8, base + R_RTCA_MIN, regVal);
                regVal = Data->H;
                R_RTCA_WRITE_REG(8, base + R_RTCA_HOUR, regVal);
            }
        }
    }

    return error_mode;
}


/*******************************************************************************
  Function: R_RTCA_Get
*/

r_rtca_Error_t R_RTCA_Get(uint32_t Unit, r_rtca_Data_t *Data)
{    
    uint32_t       base       = 0;
    uint8_t        regVal     = 0;
    uint32_t       groupVal   = 0;
    r_rtca_Error_t error_mode = R_RTCA_ERR_OK;
   
    if (Unit >= R_RTCA_MACRO_NUM) 
    {
        error_mode = R_RTCA_ERR_INSTANCE;
    } 
    else 
    {
        base = r_rtca_Dev[Unit].BaseAddr; 
        
        if (Data->Method == R_RTCA_GROUP)
        {
            groupVal = R_RTCA_READ_REG(32, base + R_RTCA_TIME);
            loc_ReadGroup(groupVal, Data);
        } 
        else 
        {        
            if ((Data->Group == R_RTCA_TIMEVAL) || (Data->Group == R_RTCA_BOTH))
            {
                regVal    = R_RTCA_READ_REG(8, base + R_RTCA_SEC);
                Data->Sec = loc_GetDecimal(regVal, R_RTCA_NON);  
                regVal    = R_RTCA_READ_REG(8, base + R_RTCA_MIN);
                Data->Min = loc_GetDecimal(regVal, R_RTCA_NON); 
                regVal    = R_RTCA_READ_REG(8, base + R_RTCA_HOUR);
                Data->H   = loc_GetDecimal( regVal, Data->Format);
            }
        }   
            
        if (Data->Method == R_RTCA_GROUP)
        {
            groupVal = R_RTCA_READ_REG(32, base + R_RTCA_DATE);
            loc_ReadGroup(groupVal, Data); 
            
        } 
        else 
        { 
            if ((Data->Group == R_RTCA_DATE) || (Data->Group == R_RTCA_BOTH))   
            {
                regVal      = R_RTCA_READ_REG(8, base + R_RTCA_WEEK);
                Data->Week  = loc_GetDecimal(regVal, R_RTCA_NON);  
                regVal      = R_RTCA_READ_REG(8, base + R_RTCA_DAY);
                Data->Day   = loc_GetDecimal(regVal, R_RTCA_NON); 
                regVal      = R_RTCA_READ_REG(8, base + R_RTCA_MON);     
                Data->Month = loc_GetDecimal(regVal, R_RTCA_NON);
                regVal      = R_RTCA_READ_REG(8, base + R_RTCA_YEAR);     
                Data->Year  = loc_GetDecimal(regVal, R_RTCA_NON);
            }
        }
    }
    return error_mode;
}


/*******************************************************************************
  Function: R_RTCA_UpdateTime

  Central function to update all time registers.

  Parameters:
  Unit       - Instance number
  Data       - Time and Date information, see: <r_rtca_Data_t>
 
  Returns:
  see: <r_rtca_Error_t>
*/

r_rtca_Error_t R_RTCA_UpdateTime(uint32_t Unit, r_rtca_Data_t *Data)
{
    r_rtca_Data_t bcdData;
    uint32_t      base            = 0;
    uint8_t       regVal          = 0;
    uint8_t       r_rtca_reg_CTL2 = 0;
    
    if (Unit >= R_RTCA_MACRO_NUM) 
    {
        return R_RTCA_ERR_INSTANCE;
    } 
    else 
    {
        base = r_rtca_Dev[Unit].BaseAddr; 

        /* Ensure that all counters are running */
        r_rtca_reg_CTL2 = 0xFFu;

        while (0 != (r_rtca_reg_CTL2 & 0x02u)) 
        {
            r_rtca_reg_CTL2 = R_RTCA_READ_REG(8, base + R_RTCA_CTL2);
        }

        r_rtca_reg_CTL2 = loc_SetBits8(r_rtca_reg_CTL2, 1u, 
                                       R_RTCA_CTL2_WAIT_POS, R_RTCA_CTL2_WAIT_WID);
        R_RTCA_WRITE_REG (8, base + R_RTCA_CTL2, r_rtca_reg_CTL2);
       

        r_rtca_reg_CTL2 &= 0xFEu;

        while (0 == (r_rtca_reg_CTL2 & 0x02u)) 
        {
            r_rtca_reg_CTL2 = R_RTCA_READ_REG(8, base + R_RTCA_CTL2);
        }

        /* Prepare CTR0 register */
//        regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL0);//these lines are commented only for w501/u350,not in original lib file
//
//        if (0 != (regVal & 0x20u))
//
//        {
//            Data->Format = R_RTCA_24H;
//        }
//        else
//        {
//            Data->Format = R_RTCA_12H;
//        }
               
        bcdData.Sec    = loc_GetBcd(Data->Sec, R_RTCA_NON);
        bcdData.Min    = loc_GetBcd(Data->Min, R_RTCA_NON);
        bcdData.H      = loc_GetBcd(Data->H,   Data->Format);
		if(Data-> AmPm == 1 && Data->Format == R_RTCA_12H)
		{
			 bcdData.H |= 0x20;
		}


        bcdData.Week   = loc_GetBcd(Data->Week,  R_RTCA_NON);
        bcdData.Day    = loc_GetBcd(Data->Day,   R_RTCA_NON);
        bcdData.Month  = loc_GetBcd(Data->Month, R_RTCA_NON);
        bcdData.Year   = loc_GetBcd(Data->Year,  R_RTCA_NON);
   
        bcdData.Method = Data->Method;
        bcdData.Format = Data->Format;
        bcdData.AmPm   = Data->AmPm;
        bcdData.Group  = Data->Group;
        
        /* Now write Values in registers */
        R_RTCA_Set(Unit, &bcdData);

        /* Restart counters*/    
        regVal  = R_RTCA_READ_REG(8, base + R_RTCA_CTL2);
        regVal &= 0xFEu;
        R_RTCA_WRITE_REG (8, base + R_RTCA_CTL2, regVal);
        
        while (0 != (regVal & 0x02u)) 
        {
           regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL2);
        }
      
        return R_RTCA_ERR_OK; 
    }
}


/*******************************************************************************
  Function: R_RTCA_ReadCounterBuf
*/

r_rtca_Error_t R_RTCA_ReadCounterBuf(uint32_t Unit, r_rtca_Data_t *Data)
{
    uint8_t  regVal;
    uint32_t base;

    if (Unit >= R_RTCA_MACRO_NUM) 
    {
        return R_RTCA_ERR_INSTANCE;
    } 
    else 
    { 
        base   = r_rtca_Dev[Unit].BaseAddr;
        regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL0);
        
//        if (0 != (regVal & 0x20u))//these lines are commented only for w501/u350,not in original lib file
//        {
//            Data->Format = R_RTCA_24H;
//        }
//        else
//        {
//            Data->Format = R_RTCA_12H;
//        }

        /*Ensure that all counters are running if macro is enabled*/
        if (0 != (regVal & 0x80u)) 
        {
            regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL2);
            while (0 != (regVal & 0x02u)) 
            {
                regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL2);
            }
        }

        regVal |= 0x01u;
        R_RTCA_WRITE_REG (8, base + R_RTCA_CTL2, regVal);
      
        regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL2);
        while (0 == (regVal & 0x02u)) 
        {
            regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL2);
        }
        
        regVal      = R_RTCA_READ_REG(8, base + R_RTCA_SEC);
        Data->Sec   = loc_GetDecimal(regVal, R_RTCA_NON);  
        regVal      = R_RTCA_READ_REG(8, base + R_RTCA_MIN);
        Data->Min   = loc_GetDecimal(regVal, R_RTCA_NON); 
        regVal      = R_RTCA_READ_REG(8, base + R_RTCA_HOUR);
        Data->H     = loc_GetDecimal( regVal, Data->Format);
        
        regVal      = R_RTCA_READ_REG(8, base + R_RTCA_WEEK);
        Data->Week  = loc_GetDecimal(regVal, R_RTCA_NON);  
        regVal      = R_RTCA_READ_REG(8, base + R_RTCA_DAY);
        Data->Day   = loc_GetDecimal(regVal, R_RTCA_NON); 
        regVal      = R_RTCA_READ_REG(8, base + R_RTCA_MON);     
        Data->Month = loc_GetDecimal(regVal, R_RTCA_NON);
        regVal      = R_RTCA_READ_REG(8, base + R_RTCA_YEAR);     
        Data->Year  = loc_GetDecimal(regVal, R_RTCA_NON);
     
    }
    regVal  = R_RTCA_READ_REG(8, base + R_RTCA_CTL2);
    regVal &= 0xFEu;
        
    R_RTCA_WRITE_REG (8, base + R_RTCA_CTL2, regVal);
       
    return R_RTCA_ERR_OK; 
}


/*******************************************************************************
  Function: R_RTCA_ReadCounter
*/

r_rtca_Error_t R_RTCA_ReadCounter(uint32_t Unit, r_rtca_Data_t *Data)
{
    uint8_t   regVal   = 0;
    uint32_t  regVal32 = 0;
    uint32_t  base     = 0;
    uint8_t   secVal   = 0;

    if (Unit >= R_RTCA_MACRO_NUM) 
    {
        return R_RTCA_ERR_RANGE;
    } 
    else 
    { 
        base   = r_rtca_Dev[Unit].BaseAddr;
        regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL0);
        
        if (0 != (regVal & 0x20u))
        {
            Data->Format = R_RTCA_24H;
        } 
        else  
        {
            Data->Format = R_RTCA_12H;
        }

        /*Ensure that all counters are running*/
        regVal = 0xffu;
        while (0 != (regVal & 0x02u)) 
        {
            regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL2);
        }

        /* First read of sec counter register to ensure reading within 1 sec*/
        do 
        {
            secVal = R_RTCA_READ_REG(8, base + R_RTCA_SECC);
            secVal = loc_GetDecimal(secVal, R_RTCA_NON);  
            
            if (Data->Method == R_RTCA_GROUP)
            {
                regVal32 = R_RTCA_READ_REG(32, base + R_RTCA_TIMEC);
                loc_ReadGroup(regVal32, Data);
            } 
            else 
            {
                if ((Data->Group == R_RTCA_TIMEVAL) || (Data->Group == R_RTCA_BOTH)) 
                {
                    regVal    = R_RTCA_READ_REG(8, base + R_RTCA_SECC);
                    Data->Sec = loc_GetDecimal(regVal, R_RTCA_NON);  
                    regVal    = R_RTCA_READ_REG(8, base + R_RTCA_MINC);
                    Data->Min = loc_GetDecimal(regVal, R_RTCA_NON); 
                    regVal    = R_RTCA_READ_REG(8, base + R_RTCA_HOURC);
                    Data->H   = loc_GetDecimal(regVal, Data->Format);
                }
            }   
            
            if (Data->Method == R_RTCA_GROUP)
            {
                regVal32 = R_RTCA_READ_REG(32, base + R_RTCA_CALC);
                loc_ReadGroup(regVal32, Data); 
            } 
            else 
            { 
                if ((Data->Group == R_RTCA_DATE) || (Data->Group == R_RTCA_BOTH))
                {
                    regVal      = R_RTCA_READ_REG(8, base + R_RTCA_WEEKC);
                    Data->Week  = loc_GetDecimal(regVal, R_RTCA_NON);  
                    regVal      = R_RTCA_READ_REG(8, base + R_RTCA_DAYC);
                    Data->Day   = loc_GetDecimal(regVal, R_RTCA_NON); 
                    regVal      = R_RTCA_READ_REG(8, base + R_RTCA_MONC);     
                    Data->Month = loc_GetDecimal(regVal, R_RTCA_NON);
                    regVal      = R_RTCA_READ_REG(8, base + R_RTCA_YEARC);     
                    Data->Year  = loc_GetDecimal(regVal, R_RTCA_NON);
                }
            }
            
        } while(secVal != Data->Sec);

        return R_RTCA_ERR_OK; 
    }
}


/*******************************************************************************
  Function: R_RTCA_SetTime
*/

r_rtca_Error_t R_RTCA_SetTime(uint32_t Unit, r_rtca_Data_t *Data)
{
    r_rtca_Data_t bcdData;
    
    if (Unit >= R_RTCA_MACRO_NUM) 
    {
        return R_RTCA_ERR_RANGE;
    } 
    else 
    {
        bcdData.Sec    = loc_GetBcd(Data->Sec, R_RTCA_NON);
        bcdData.Min    = loc_GetBcd(Data->Min, R_RTCA_NON);
        bcdData.H      = loc_GetBcd(Data->H,   Data->Format);
   
        bcdData.Week   = loc_GetBcd(Data->Week,  R_RTCA_NON);
        bcdData.Day    = loc_GetBcd(Data->Day,   R_RTCA_NON);
        bcdData.Month  = loc_GetBcd(Data->Month, R_RTCA_NON);
        bcdData.Year   = loc_GetBcd(Data->Year,  R_RTCA_NON);
   
        bcdData.Method = Data->Method;
        bcdData.Format = Data->Format;
        bcdData.AmPm   = Data->AmPm;
        bcdData.Group  = Data->Group;
        
        /* Now write Values in registers */
        R_RTCA_Set(Unit, &bcdData);
      
        return R_RTCA_ERR_OK; 
    }
}


/*******************************************************************************
  Function: R_RTCA_SetAlarm
*/

r_rtca_Error_t R_RTCA_SetAlarm(uint32_t Unit, r_rtca_Alarm_t Alarm)
{
    uint32_t  base;
    uint8_t   regVal;
   
    if (Unit >= R_RTCA_MACRO_NUM) 
    {
        return R_RTCA_ERR_INSTANCE;        
    } 
    else 
    { 
        base   = r_rtca_Dev[Unit].BaseAddr;
        
        regVal = loc_GetBcd(Alarm.Minute, R_RTCA_NON);
        R_RTCA_WRITE_REG(8, base + R_RTCA_ALM, regVal);

        regVal = loc_GetBcd(Alarm.Hour, Alarm.Format);
        R_RTCA_WRITE_REG(8, base + R_RTCA_ALH, regVal);
        R_RTCA_WRITE_REG(8, base + R_RTCA_ALW, Alarm.Day);
        
        R_RTCA_EnableInt(Unit, R_RTCA_INT_AL);

        return R_RTCA_ERR_OK; 
    }
}


/*******************************************************************************
  Function: R_RTCA_SetCompare
*/

r_rtca_Error_t R_RTCA_SetCompare(uint32_t Unit, uint32_t CompVal) 
{
    uint32_t base;
    uint8_t  regVal;
    uint8_t  status;
    
    if (Unit >= R_RTCA_MACRO_NUM) 
    {
        return R_RTCA_ERR_INSTANCE;        
    } 
    else 
    {
        base   = r_rtca_Dev[Unit].BaseAddr;
        status = 0xFF;
        
        regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL0);
        
        if ( 0 != (regVal & 0x40u)) 
        {
            /* wait till write operation is finished*/
            while (0 != (status & 0x20u)) 
            {
                status = R_RTCA_READ_REG(8, base + R_RTCA_CTL2);  
            }
        }
        
        R_RTCA_WRITE_REG(32, base + R_RTCA_SCMP, CompVal);

        return R_RTCA_ERR_OK;
    }
} 


/*******************************************************************************
  Function: R_RTCA_SetSvstop
*/

r_rtca_Error_t R_RTCA_SetSvstop(uint32_t Unit, r_rtca_State_t Endis ) 
{    
    uint32_t base;
    
    if (Unit >= R_RTCA_MACRO_NUM) 
    {
        return R_RTCA_ERR_INSTANCE;        
    } 
    else 
    {     
        /* update the BaseAddr */
        r_rtca_Dev[Unit].BaseAddr = R_RTCA_SYS_BaseAddr(Unit);
        base                      = r_rtca_Dev[Unit].BaseAddr;

        if (0 != Endis) 
        {
            R_RTCA_WRITE_REG(8, base + R_RTCA_EMU, 0);
        } 
        else 
        {
            R_RTCA_WRITE_REG(8, base + R_RTCA_EMU, 0x80);
        }

        return R_RTCA_ERR_OK;
    }
}


/*******************************************************************************
  Function: R_RTCA_OutputCtl
*/

r_rtca_Error_t R_RTCA_OutputCtl(uint32_t Unit, r_rtca_State_t State) 
{
    uint32_t base;
    uint8_t  regVal;
    
    if (Unit >= R_RTCA_MACRO_NUM) 
    {
        return R_RTCA_ERR_RANGE;    
    } 
    else 
    {
        base   = r_rtca_Dev[Unit].BaseAddr;
        regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL1);

        if (0 != State) 
        {
            regVal |= 0x20u;
        } 
        else 
        {
            regVal &= 0x1Fu;
        }

        R_RTCA_WRITE_REG(8, base + R_RTCA_CTL1, regVal);
    
        return R_RTCA_ERR_OK;
    }
}   


/*******************************************************************************
  Function: R_RTCA_ReadSubCntBuf
*/

uint32_t R_RTCA_ReadSubCntBuf(uint32_t Unit) 
{
    uint32_t    base;
    uint32_t    srbuVal;
    uint8_t     regVal;
    
    if (Unit >= R_RTCA_MACRO_NUM) 
    {
        return R_RTCA_ERR_INSTANCE;        
    } 
    else 
    {
        base   = r_rtca_Dev[Unit].BaseAddr;
        regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL2);
                
        if (0 != (regVal & 0x04u)) 
        {
            while (0 == (regVal & 0x08u)) 
            {
                regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL2);  
            }
        } 
        else 
        {
            regVal |= 0x04u;     
            R_RTCA_WRITE_REG(8, base + R_RTCA_CTL2, regVal);
        }
        
        while (0 == (regVal & 0x08u)) 
        {
            regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL2);  
        } 
        srbuVal = R_RTCA_READ_REG(32, base + R_RTCA_SRBU);
        regVal &= 0x3Bu;
        R_RTCA_WRITE_REG(8, base + R_RTCA_CTL2, regVal);
    }

    return srbuVal;
}


/*******************************************************************************
  Function: R_RTCA_GetSubCounterVal
*/

uint32_t  R_RTCA_GetSubCounterVal(uint32_t Unit)
{    
    uint32_t base;
    uint32_t regVal;
   
    if (Unit >= R_RTCA_MACRO_NUM) 
    {
        return R_RTCA_ERR_INSTANCE;        
    } 
    else 
    {
        base   = r_rtca_Dev[Unit].BaseAddr; 
        regVal = R_RTCA_READ_REG(32, base + R_RTCA_SUBC);     
    }

    return regVal;
}


/*******************************************************************************
  Function: R_RTCA_CtrCtl
*/      
        
r_rtca_Error_t R_RTCA_CtrCtl(uint32_t Unit, r_rtca_State_t EnDis)
{
    uint8_t  regVal;
    uint32_t base;
        
    if (Unit >= R_RTCA_MACRO_NUM) 
    {
        return R_RTCA_ERR_INSTANCE;        
    } 
    else 
    {
        base = r_rtca_Dev[Unit].BaseAddr;
        
        if (EnDis == R_RTCA_ENABLE) 
        {
            regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL0);
            if (0 == (regVal & 0x40u))
            {
                regVal |= 0x80u;
                R_RTCA_WRITE_REG(8, base + R_RTCA_CTL0, regVal);
            }

            /*wait till counter runs*/
            while (0 == (regVal & 0x40u)) 
            {
                regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL0);
            }
        } 
        else 
        {
            regVal  = R_RTCA_READ_REG(8, base + R_RTCA_CTL0);
            regVal &= 0x30u;
            R_RTCA_WRITE_REG(8, base + R_RTCA_CTL0, regVal); 
            regVal  = R_RTCA_READ_REG(8, base + R_RTCA_CTL0);

            /*wait till counter stops*/
            while (0 != (regVal & 0x40u)) 
            {
                regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL0);
            }
        }

        return R_RTCA_ERR_OK; 
    }
}


/*******************************************************************************
  Function: R_RTCA_SetInt
*/

r_rtca_Error_t R_RTCA_SetInt(uint32_t Unit, r_rtca_IntConf_t IntConf)
{
    uint32_t   base            = 0;
    uint8_t    regVal          = 0;
    uint8_t    r_rtca_reg_CTL1 = 0;
      
    if (Unit >= R_RTCA_MACRO_NUM) 
    {
        return R_RTCA_ERR_INSTANCE;        
    } 
    else 
    {    
        /* update the BaseAddr */
        r_rtca_Dev[Unit].BaseAddr = R_RTCA_SYS_BaseAddr(Unit);
        base                      = r_rtca_Dev[Unit].BaseAddr;
  
        r_rtca_reg_CTL1 = 0;
        /* Enable 1 Hz output is not part of this function */
        regVal = R_RTCA_READ_REG(8, base + R_RTCA_CTL1);
        if (0 != (regVal & 0x20u)) 
        {
            r_rtca_reg_CTL1 = loc_SetBits8(r_rtca_reg_CTL1, 1u, 
                                           R_RTCA_CTL1_HZE_POS, R_RTCA_CTL1_HZE_WID);
        }
        
        if ( R_RTCA_ENABLE == IntConf.AlarmInt) 
        {
            r_rtca_reg_CTL1 = loc_SetBits8(r_rtca_reg_CTL1, 1u, 
                                           R_RTCA_CTL1_ALME_POS, R_RTCA_CTL1_ALME_WID);
            R_RTCA_EnableInt(Unit, R_RTCA_INT_AL);
        }
 
        if (0 != IntConf.IntervInt) 
        {
            r_rtca_reg_CTL1 = loc_SetBits8(r_rtca_reg_CTL1, (uint8_t) IntConf.IntervInt, 
                                           R_RTCA_CTL1_CT_POS, R_RTCA_CTL1_CT_WID);
            R_RTCA_EnableInt(Unit, R_RTCA_INT_R);
        }
        
        if (R_RTCA_ENABLE == IntConf.SecInt) 
        {
            r_rtca_reg_CTL1 = loc_SetBits8(r_rtca_reg_CTL1, 1u, 
                                           R_RTCA_CTL1_1SE_POS, R_RTCA_CTL1_1SE_WID);
            R_RTCA_EnableInt(Unit, R_RTCA_INT_1S);
        }

        R_RTCA_WRITE_REG(8, base + R_RTCA_CTL1, r_rtca_reg_CTL1);

        return R_RTCA_ERR_OK;
    }
}


/*******************************************************************************
  Function: R_RTCA_Init

  See: <rtca_api.h> 
*/

 r_rtca_Error_t R_RTCA_Init(uint32_t Unit, r_rtca_Parameter_t *Config, r_rtca_Data_t *Data)
{
    r_rtca_Error_t err             = R_RTCA_ERR_OK;
    uint32_t       base            = 0;
    uint8_t        r_rtca_reg_CTL0 = 0;

    err = R_RTCA_CheckMacroParams(Unit); 

    if (R_RTCA_ERR_OK != err) 
    {
        return err; 
    }
    if (0 == r_rtca_Dev[Unit].BaseAddr)
    {
        /* update the BaseAddr */
        r_rtca_Dev[Unit].BaseAddr = R_RTCA_SYS_BaseAddr(Unit);
        base                      = r_rtca_Dev[Unit].BaseAddr;
        
        err = R_RTCA_SYS_HardwareInit(Unit);

        r_rtca_reg_CTL0 = R_RTCA_READ_REG(8, base + R_RTCA_CTL0);
        r_rtca_reg_CTL0 = loc_SetBits8(r_rtca_reg_CTL0, Config->Mode, 
                                       R_RTCA_CTL0_SLSB_POS, R_RTCA_CTL0_SLSB_WID);
        r_rtca_reg_CTL0 = loc_SetBits8(r_rtca_reg_CTL0, Config->Format, 
                                       R_RTCA_CTL0_AMPM_POS, R_RTCA_CTL0_AMPM_WID); 
        R_RTCA_WRITE_REG (8, base + R_RTCA_CTL0, r_rtca_reg_CTL0);
        
        if (Config->Mode == R_RTCA_FREQ_MODE) 
        {
            R_RTCA_WRITE_REG(32, base + R_RTCA_SCMP, Config->CompVal);    
        } 
        else 
        {
            R_RTCA_SetCorrection(Unit, Config->CorrectVal);
        }

        /* Now write Values in registers */
        R_RTCA_SetTime(Unit, Data);        
    }
    else 
    {   
        /* please, no double-init*/
        err = R_RTCA_ERR_LOCKED; 
    }
    return err;
}


/*******************************************************************************
  Function: R_RTCA_DeInit

  See: <rtca_api.h> 
*/

r_rtca_Error_t R_RTCA_DeInit(uint32_t Unit)
{
    r_rtca_Error_t err;

    err = R_RTCA_CheckMacroParams(Unit); 

    if (R_RTCA_ERR_OK != err) 
    {
        return err; 
    }
    if (0 != r_rtca_Dev[Unit].BaseAddr)
    {
        
        r_rtca_Dev[Unit].BaseAddr = 0;
        err = R_RTCA_SYS_HardwareDeInit(Unit);
    }
    else 
    {   
        /* nothing to do at this point of time  */
        err = R_RTCA_ERR_NOTLOCKED; 
    }
    return err;
}




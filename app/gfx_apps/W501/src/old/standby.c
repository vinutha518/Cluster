/*
****************************************************************************
Project : Audio generation demo
FILE    : $Id: standby.c 7262 2016-01-15 13:34:37Z michael.golczewski $
============================================================================
DESCRIPTION
Audio generation Test Application
============================================================================
                            C O P Y R I G H T
============================================================================
                           Copyright (c) 2011
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
*/


/***********************************************************
  Title: Main Module

  Implementation of the standby application.
*/


/***********************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "stbc_api.h"
//#include "rtc.h"
#include "standby.h"
#include "r_gpio_api.h"
#include "config.h"
//#include "sound_sg.h"
#include "sound_pcm.h"
//#include "sound_ssif.h"
#include "r_tauj_api.h"
#include "r_taub_api.h"
#include "r_bsp_stdio_api.h"
#include "r_dev_api.h"


/***********************************************************
  Section: Local Defines
*/
#define STBC_ENABLE_DEEPSTOP 1
#define STBC_DISABLE_DEEPSTOP 0


/***********************************************************
  Functions Global
*/
extern void eeTask_Processing(void);


/***********************************************************
  Functions Local variables
*/
static uint8_t ImmediateShutdownEnable;


/***********************************************************
  Function: eePclk_SetStandbyMask
*/
static void eePclk_SetStandbyMask()
{
    /* ensure the RTC clock is enabled in the AWO area in deepstop */
    R_DEV_WRITE_REG(32, CKSC_ARTCAD_STPM, RTCAD_STP_MSK);
}


/***********************************************************
  Function: eeStandby_CheckWakeUp
*/
ee_Wakeup_t eeStandby_CheckWakeUp(void)
{
ee_Wakeup_t _wake_up = E_WAKEUP_NONE;
uint32_t u32_temp = 0;

    /* check for any wakeups */
    u32_temp =  R_STBC_GetWakeUpEvent();

    /* RTC alarm has priority */
    if(u32_temp & R_STBC_WUF_INTRTCA0AL)
    {
        R_BSP_STDIO_Printf("ISO0 RTC alarm wakeup occured (WUF = 0x%08X)\n", u32_temp);
        _wake_up = E_WAKEUP_ALARM;
    }

    /* RTC interval wakeup */
    else if(u32_temp & R_STBC_WUF_INTRTCA0R)
    {
        R_BSP_STDIO_Printf("ISO0 RTC interval wakeup occured (WUF = 0x%08X)\n", u32_temp);
        _wake_up = E_WAKEUP_INTERVAL;
    }
    
    /* INTP0 external pin */
    else if(u32_temp & R_STBC_WUF_INTP0)
    {
        R_BSP_STDIO_Printf("ISO0 Ext Pin INTP%d occured (WUF = 0x%08X)\n", 0, u32_temp);
        _wake_up = E_WAKEUP_EXPIN;
    }
    
    else
    {
        /* maybe debugger or some other */
        if(0 != u32_temp)
        {
            _wake_up = E_WAKEUP_OTHER;
        }
        else
        {
            _wake_up = E_WAKEUP_NONE;
        }
    }

    return _wake_up;
}


/***********************************************************
  Function: eeStandby_Init
*/
void eeStandby_Init(void)
{
    ImmediateShutdownEnable = R_FALSE;
}


/***********************************************************
  Function: eeStandby_Processing
*/
void eeStandby_Processing_Immediate(uint8_t unused)
{
    ImmediateShutdownEnable = R_TRUE;
    eeStandby_Processing();
}


/***********************************************************
  Function: eeStandby_Processing
*/
void eeStandby_Processing(void)
{
uint8_t b_temp = R_TRUE;
r_Error_t error = R_ERR_OK;
uint32_t u32_temp;

    R_BSP_STDIO_Printf("Enter Deep stop mode with wakeup factors:\n");
    R_BSP_STDIO_Printf(" - INTP%d\n", 0);
    R_BSP_STDIO_Printf(" - RTC Interval 1 minute\n");
    R_BSP_STDIO_Printf(" - RTC Alarm (if enabled by user)\n");

    /* make sure you set a wakeup pin that is powered on in the AWO area, otherwise
       it will not wake up
       enable the external input pin INTP for wakeup 
    */
    R_DEV_IntEnable(R_DEV_INT_P0, 1);

    R_BSP_STDIO_Printf("Osc disabled in deep stop: \n");
    R_BSP_STDIO_Printf(" - Main Osc\n");
    R_BSP_STDIO_Printf(" - PLL's\n");
    R_BSP_STDIO_Printf(" - HS Osc\n");

    R_BSP_STDIO_Printf("Entering DeepStop mode ... \n");

    /* do not allow any more sound requests */
    //SoundSG_Disable();
    SoundPCM_Disable();
    //SoundSSIF_Disable();

    /* when all applications are ready to sleep we can shut down */
    while(b_temp) 
    {
        /* this may take some time */
       // b_temp = SoundSG_IsBusy();
        b_temp |= SoundPCM_IsBusy();
       // b_temp |= SoundSSIF_IsBusy();

        /* Allow task processing to continue to allow component shutdown */
        eeTask_Processing();
    }


    /* incase were using the UART */
#ifdef USE_RS232_TERMINAL
    R_BSP_STDIO_DeInit();
#endif

    __DI();
    
    /* init the TAUJ as this can still operate in the AWO area */
    R_TAUJ_Init(TAUJ_UNIT);

    /* init the TAUB */
    R_TAUB_Init(TAUB_UNIT);

    
    /* Freeze the output of the FOUT signal (STBC_IO_HOLD0 Port 0) using IO hold 
       not much point in doing this as its a clock but shows
       that it works oh and dont forget to release it when you wake up (i.e R_STBC_SetIoHold(0))
    
    R_STBC_SetIoHold(STBC_IO_HOLD0);
    */

    /* we will allow the following wake up events when we go to deep stop
        Debugger
        INTP0 pin 
        RTCA0 Alarm  
        RTCA0 interval   
    */
    R_STBC_EnableWakeUpFactor(  R_STBC_WUL_INTDCUTDI_BIT | R_STBC_WUL_INTP0_BIT |
                                R_STBC_WUL_INTRTCA0AL_BIT |R_STBC_WUL_INTRTCA0R_BIT);
   

    /* if the RTC clock acceleration option is selected then enable the MOSC in standby */
    u32_temp = R_DEV_ClkFrequencyHz(R_DEV_CKS_RTC);
    if (u32_temp == EE_CLK_SOSC_FRQ)
    {
        /* RTC clock is using the subosc so stop the MOSC in standby */
         error += R_DEV_SetGenStopMask(R_DEV_MOSC, STBC_DISABLE_DEEPSTOP);
    }
    else
    {
        /* RTC clock is using the master osc so enable the MOSC in standby */
        error += R_DEV_SetGenStopMask(R_DEV_MOSC, STBC_ENABLE_DEEPSTOP);
    }

    /* note:
        The PLL is not stopped before entering Standby this is because during
        BSP init the PLL's are stopped and restarted again.  If you stop the PLL 
        here then anyone that uses the PLL (i.e. CPU) needs to be switched to 
        another clock before the PLL is stopped	otherwise the STBC will not receive
        an acknowledge from it.
        If the PLL is not stopped before it enters standby it may report incorrect
        status after wakeup, PLL must be stopped and restarted after wake up then.
        this is done in the BSP currently
    */

    /* clocks operating in standby */
    /* sub osc - always operating */

    /* clocks stopped operating in standby */
    error += R_DEV_SetGenStopMask(R_DEV_HS_RING, STBC_DISABLE_DEEPSTOP);
    /* PLL0, PLL1 and 2 are configured to stop in the BSP */

    /* check all the clocks have stopped */
    if(R_ERR_OK == error)
    {
        /* do nothing */        
    }
    else
    {
        /* error something went wrong */
        while(1);  
    }

    /* ensure the clk is enabled to the RTC in standby */
    eePclk_SetStandbyMask();

    /* enter deep stop lowest current possible ISO0 */
    R_STBC_EnableStandby();
   
    /* After wakeup, we start at reset vector, BUT 
     if we end up here - deep stop entry did not succeed */
    while(1)
    {
        asm("nop");
    } 
}


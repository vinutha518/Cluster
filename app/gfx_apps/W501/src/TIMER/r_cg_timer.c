/*
 * r_cg_timer.c
 *
 *  Created on: Jan 19, 2019
 *      Author: MSWELT12
 */
#include "r_typedefs.h"        /* Renesas basic types, e.g. uint32_t */
#include "r_bsp_api.h"         /* Board support package */
#include "sound_pcm.h"
#include "UDSS.h"
#include "my_can.h"
#include "r_typedefs.h"
#include "sound_pcm.h"
#include "r_dev_api.h"
#include "r_bsp_api.h"
#include "r_bsp_stdio_api.h"
#include "UDSS_APP.h"
#include "UDS_READ_WRITE.h"
#include "r_gpio_api.h"
/*#include "r_taub_isr.h"*/
/*#include "r_taub_main.h"*/
#include "r_taub_api.h"
#include "r_tauj_api.h"
#include "r_cg_timer.h"

/*#include "r_taub_sys.h"*/
uint32_t R_TAU0_Create();
uint32_t R_TAU1_Create();

uint8_t TimerJ_Init();

void locTauj0ch0Isr();
uint32_t R_TAU2_Create();


#ifdef GRAYSCALE_TFT
uint32_t R_TAU2_PWM();
#endif

uint32_t R_TAU2_PWM_TFT();

uint32_t R_TAU2_PWM_GCLK();

PUBLIC uint32_t R_TAU0_Create()
{
	if(R_TAUB_Init(0x00) != R_TAUB_ERR_OK)
		return R_NG;

	if(R_TAUB_Init(0x01) != R_TAUB_ERR_OK)
		return R_NG;


	if(R_TAUB_Init(0x02) != R_TAUB_ERR_OK)
		return R_NG;


	r_taub_SetClockPeriod(0,R_TAUB_CK_1,R_TAUB_TIME_INTERVAL_9);  		//-->CK1 - PCLK/2^8=5MHz/2^8=19607Hz
	r_taub_SetClkSource(0,R_TAUB_CHANNEL_0,R_TAUB_CK_1);			//-->CK1
	r_taub_SetMode(0,R_TAUB_CHANNEL_0,R_TAUB_INTERVAL_TIMER_MODE);
	//r_taub_SetMaster(0,R_TAUB_CHANNEL_0,1);
	r_taub_SetTrigger(0,R_TAUB_CHANNEL_0,R_TAUB_NONE);
	r_taub_SetCountClock(0,R_TAUB_CHANNEL_0,R_TAUB_COUNT_CLK_PRESCALER);
	r_taub_SetPeriod(0,R_TAUB_CHANNEL_0,0x01);
	r_taub_Start(0,R_TAUB_CHANNEL_0);
	R_TAUB_EnableInt(0,R_TAUB_INT_CH0);
	R_TAUB_SetIsrCallback(0,R_TAUB_INT_CH0,&taub_isr);
}
PUBLIC uint32_t R_TAU1_Create()
{

	r_taub_SetClockPeriod(1,R_TAUB_CK_1,R_TAUB_TIME_INTERVAL_8);  		//-->CK1 - PCLK/2^8=5MHz/2^8=19607Hz
	r_taub_SetClkSource(1,R_TAUB_CHANNEL_1,R_TAUB_CK_1);			//-->CK1
	r_taub_SetMode(1,R_TAUB_CHANNEL_1,R_TAUB_INTERVAL_TIMER_MODE);
	//r_taub_SetMaster(0,R_TAUB_CHANNEL_0,1);
	r_taub_SetTrigger(1,R_TAUB_CHANNEL_1,R_TAUB_NONE);
	r_taub_SetCountClock(1,R_TAUB_CHANNEL_1,R_TAUB_COUNT_CLK_PRESCALER);
	r_taub_SetPeriod(1,R_TAUB_CHANNEL_1,0x01);
	r_taub_Start(1,R_TAUB_CHANNEL_1);
	R_TAUB_EnableInt(1,R_TAUB_INT_CH1);
	R_TAUB_SetIsrCallback(1,R_TAUB_INT_CH1,&taub1_isr);
}



PUBLIC uint32_t R_TAU2_Create()
{

	r_taub_SetClockPeriod(2,R_TAUB_CK_3,R_TAUB_TIME_INTERVAL_11);  		//-->CK1 - PCLK/2^8=5MHz/2^8=19607Hz
	r_taub_SetClkSource(2,R_TAUB_CHANNEL_2,R_TAUB_CK_3);			//-->CK1
	r_taub_SetMode(2,R_TAUB_CHANNEL_2,R_TAUB_INTERVAL_TIMER_MODE);
	//r_taub_SetMaster(0,R_TAUB_CHANNEL_0,1);
	r_taub_SetTrigger(2,R_TAUB_CHANNEL_2,R_TAUB_NONE);
	r_taub_SetCountClock(2,R_TAUB_CHANNEL_2,R_TAUB_COUNT_CLK_PRESCALER);
	r_taub_SetPeriod(2,R_TAUB_CHANNEL_2,0x01);
	r_taub_Start(2,R_TAUB_CHANNEL_2);
	R_TAUB_EnableInt(2,R_TAUB_INT_CH2);
	R_TAUB_SetIsrCallback(2,R_TAUB_INT_CH2,&taub_grayscale_isr);
}

#ifdef GRAYSCALE_TFT
PUBLIC uint32_t R_TAU2_PWM()
{
	//Settings for Master Channel
	r_taub_SetClockPeriod(2,R_TAUB_CK_1,R_TAUB_TIME_INTERVAL_4);  		//-->CK1 - PCLK/2^8=5MHz/2^8=19607Hz
	r_taub_SetClkSource(2,R_TAUB_CHANNEL_10,R_TAUB_CK_1);			//-->CK1
	r_taub_SetMode(2,R_TAUB_CHANNEL_10,R_TAUB_INTERVAL_TIMER_MODE);
	r_taub_SetMaster(2,R_TAUB_CHANNEL_10,1);
	r_taub_SetTrigger(2,R_TAUB_CHANNEL_10,R_TAUB_NONE);
	r_taub_SetCountClock(2,R_TAUB_CHANNEL_10,R_TAUB_COUNT_CLK_PRESCALER);
	r_taub_SetPeriod(2,R_TAUB_CHANNEL_10,300);
	r_taub_EnableIntAtStartUp(0,R_TAUB_CHANNEL_10, 1);						// enable interrupt at startup

	//Settings for Slave Channel
	r_taub_SetClkSource		(2,R_TAUB_CHANNEL_13,R_TAUB_CK_1);				//-->CK1
	r_taub_SetMode			(2,R_TAUB_CHANNEL_13,R_TAUB_ONE_COUNT_MODE);
	r_taub_SetMaster		(2,R_TAUB_CHANNEL_13,0);
	r_taub_SetTrigger		(2,R_TAUB_CHANNEL_13,R_TAUB_INT_FROM_MASTER);
	r_taub_SetCountClock	(2,R_TAUB_CHANNEL_13,R_TAUB_COUNT_CLK_PRESCALER);
	r_taub_SetPeriod		(2,R_TAUB_CHANNEL_13,300/2);						// for 50% duty
	r_taub_EnableIntAtStartUp(2,R_TAUB_CHANNEL_13, 1);						// enable interrupt at startup
//	r_taub_OutputEnable	(2,R_TAUB_CHANNEL_13);
	r_taub_EnableSynchronousChannelOperation(2, R_TAUB_CHANNEL_13);

	R_TAUB_EnableInt		(2,R_TAUB_INT_CH10);
	R_TAUB_SetIsrCallback	(2,R_TAUB_INT_CH10,&taub_isr_Master);
	R_TAUB_EnableInt		(2,R_TAUB_INT_CH13);
	R_TAUB_SetIsrCallback	(2,R_TAUB_INT_CH13,&taub_isr_Slave);

	r_taub_Start			(2,R_TAUB_CHANNEL_10);
	r_taub_Start			(2,R_TAUB_CHANNEL_13);

}
#endif
PUBLIC uint32_t R_TAU2_PWM_TFT()
{
	//Settings for Master Channel
	r_taub_SetClockPeriod(2,R_TAUB_CK_1,R_TAUB_TIME_INTERVAL_4);  		//-->CK1 - PCLK/2^8=5MHz/2^8=19607Hz
	r_taub_SetClkSource(2,R_TAUB_CHANNEL_4,R_TAUB_CK_1);			//-->CK1
	r_taub_SetMode(2,R_TAUB_CHANNEL_4,R_TAUB_INTERVAL_TIMER_MODE);
	r_taub_SetMaster(2,R_TAUB_CHANNEL_4,1);
	r_taub_SetTrigger(2,R_TAUB_CHANNEL_4,R_TAUB_NONE);
	r_taub_SetCountClock(2,R_TAUB_CHANNEL_4,R_TAUB_COUNT_CLK_PRESCALER);
	r_taub_SetPeriod(2,R_TAUB_CHANNEL_4,300);
	r_taub_EnableIntAtStartUp(0,R_TAUB_CHANNEL_4, 1);						// enable interrupt at startup

	//Settings for Slave Channel
	r_taub_SetClkSource		(2,R_TAUB_CHANNEL_5,R_TAUB_CK_1);				//-->CK1
	r_taub_SetMode			(2,R_TAUB_CHANNEL_5,R_TAUB_ONE_COUNT_MODE);
	r_taub_SetMaster		(2,R_TAUB_CHANNEL_5,0);
	r_taub_SetTrigger		(2,R_TAUB_CHANNEL_5,R_TAUB_INT_FROM_MASTER);
	r_taub_SetCountClock	(2,R_TAUB_CHANNEL_5,R_TAUB_COUNT_CLK_PRESCALER);
	r_taub_SetPeriod		(2,R_TAUB_CHANNEL_5,300/1);						// for 50% duty
	r_taub_EnableIntAtStartUp(2,R_TAUB_CHANNEL_5, 1);						// enable interrupt at startup
	//r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);
	r_taub_EnableSynchronousChannelOperation(2, R_TAUB_CHANNEL_5);

	R_TAUB_EnableInt		(2,R_TAUB_CHANNEL_4);
	R_TAUB_SetIsrCallback	(2,R_TAUB_CHANNEL_4,&taub_isr_Master_TFT);
	R_TAUB_EnableInt		(2,R_TAUB_CHANNEL_5);
	R_TAUB_SetIsrCallback	(2,R_TAUB_CHANNEL_5,&taub_isr_Slave_TFT);

	r_taub_Start			(2,R_TAUB_CHANNEL_4);
	r_taub_Start			(2,R_TAUB_CHANNEL_5);

}

PUBLIC uint32_t R_TAU2_PWM_GCLK()
{
	//Settings for Master Channel
		r_taub_SetClockPeriod(2,R_TAUB_CK_1,R_TAUB_TIME_INTERVAL_1);  		//-->CK1 - PCLK/2^8=5MHz/2^8=19607Hz
		r_taub_SetClkSource(2,R_TAUB_CHANNEL_8,R_TAUB_CK_1);			//-->CK1
		r_taub_SetMode(2,R_TAUB_CHANNEL_8,R_TAUB_INTERVAL_TIMER_MODE);
		r_taub_SetMaster(2,R_TAUB_CHANNEL_8,1);
		r_taub_SetTrigger(2,R_TAUB_CHANNEL_8,R_TAUB_NONE);
		r_taub_SetCountClock(2,R_TAUB_CHANNEL_8,R_TAUB_COUNT_CLK_PRESCALER);
		r_taub_SetPeriod(2,R_TAUB_CHANNEL_8,300);
		r_taub_EnableIntAtStartUp(0,R_TAUB_CHANNEL_8, 1);						// enable interrupt at startup

		//Settings for Slave Channel
		r_taub_SetClkSource		(2,R_TAUB_CHANNEL_11,R_TAUB_CK_1);				//-->CK1
		r_taub_SetMode			(2,R_TAUB_CHANNEL_11,R_TAUB_ONE_COUNT_MODE);
		r_taub_SetMaster		(2,R_TAUB_CHANNEL_11,0);
		r_taub_SetTrigger		(2,R_TAUB_CHANNEL_11,R_TAUB_INT_FROM_MASTER);
		r_taub_SetCountClock	(2,R_TAUB_CHANNEL_11,R_TAUB_COUNT_CLK_PRESCALER);
		r_taub_SetPeriod		(2,R_TAUB_CHANNEL_11,60/1);						// for 50% duty
		r_taub_EnableIntAtStartUp(2,R_TAUB_CHANNEL_11, 1);						// enable interrupt at startup
		//r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);
		r_taub_EnableSynchronousChannelOperation(2, R_TAUB_CHANNEL_11);

		R_TAUB_EnableInt		(2,R_TAUB_CHANNEL_8);
		R_TAUB_SetIsrCallback	(2,R_TAUB_CHANNEL_8,&taub_isr_Master_GCLK);
		R_TAUB_EnableInt		(2,R_TAUB_CHANNEL_11);
		R_TAUB_SetIsrCallback	(2,R_TAUB_CHANNEL_11,&taub_isr_Slave_GCLK);

		r_taub_Start			(2,R_TAUB_CHANNEL_8);
		r_taub_Start			(2,R_TAUB_CHANNEL_11);

}
uint8_t TimerJ_Init()
{
         r_tauj_Error_t error = R_TAUJ_ERR_OK;
    uint8_t Inst = 0;
    error = R_TAUJ_Init(0);
    /* set the clock period for CK0 (TPS) */
    error += R_TAUJ_SetClockPeriod(Inst, R_TAUJ_CK_0,  R_TAUJ_TIME_INTERVAL_11/*R_TAUJ_TIME_INTERVAL_3*/); // R_TAUJ_TIME_INTERVAL_8
    /* select the clock source for the timer channel CK_0 - 3 in the CMOR */
    error += R_TAUJ_SetClkSource(Inst, (r_tauj_Channel_t)0, R_TAUJ_CK_0);

    /* select the mode for the timer channel (CMOR) */
    error += R_TAUJ_SetMode(Inst, (r_tauj_Channel_t)0, R_TAUJ_INTERVAL_TIMER_MODE);
    R_TAUJ_SetTrigger(0,R_TAUJ_CHANNEL_0,R_TAUJ_NONE);
    R_TAUJ_SetCountClock(0,R_TAUJ_CHANNEL_0,R_TAUJ_COUNT_CLK_PRESCALER);
    /* Write the count value in the CDR */
    error += R_TAUJ_SetPeriod(Inst, (r_tauj_Channel_t)0, 15/*0x01*/ /*0x15E0*//*0x12C0 * 15*/);

    /* Enable the output (TOE) */
//    error += R_TAUJ_OutputDisable(Inst, R_TAUJ_CHANNEL_0);
    R_TAUJ_Start(0,R_TAUJ_CHANNEL_0);
    /* Enable interrupt */
    R_TAUJ_EnableInt(0, R_TAUJ_INT_CH0);

    /* Set ISR routine */
    R_TAUJ_SetIsrCallback(0, R_TAUJ_INT_CH0, &locTauj0ch0Isr);




}


/****/

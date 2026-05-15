/*
 ****************************************************************************
PROJECT : Audio generation demo
FILE   : $Id: sound_pcm.c 4717 2015-02-02 10:12:30Z golczewskim $
============================================================================
DESCRIPTION
Audio generation Test Application
============================================================================
                            C O P Y R I G H T
============================================================================
                           Copyright (c) 2011
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

/***********************************************************
  Section: Includes
 */
#include "dr7f701461.dvf.h"
#include "r_typedefs.h"
#include "r_pcmp_api.h"
#include "r_gpio_api.h"
#include "sound_pcm.h"
#include "r_dma_api.h"
#include "r_g3dma_api.h"
#include "config.h"
#include "r_bsp_stdio_api.h"
/*#include "state.h"*/
#include "sound_array_pcm.h"
#include "sound_array_pcm2.h"
#include "r_cg_timer.h"
#include "r_taub_api.h"
#include "init.h"

//#include "r_sg_main.h"
/***********************************************************
  Section: defines
 */

/***********************************************************
  Variable: PcmPinConfigEnable


  Port structure for the PCM sound generator pins.
 */

static const r_dev_PinConfig_t SgPcmPinConfigEnable[] = 
		/*  Port   Pin    Func      Dir       Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
		{3u,   /*5u*/3u,   1u,     R_DEV_PIN_OUT, 0u,   R_DEV_PIN_PULLNO, 0u,         0u,       0u,  R_DEV_PIN_CMOS1},  /* PCMP0AN1 */
		{3u,   /*4u*/2u,   1u,     R_DEV_PIN_OUT, 0u,   R_DEV_PIN_PULLNO, 0u,         0u,       0u,  R_DEV_PIN_CMOS1},  /* PCMP0AP1 */
		{3u,   9u,   1u,     R_DEV_PIN_OUT, 0u,   R_DEV_PIN_PULLNO, 0u,         0u,       0u,  R_DEV_PIN_CMOS1},  /* PCMP0BN1 */
		{3u,   8u,   1u,     R_DEV_PIN_OUT, 0u,   R_DEV_PIN_PULLNO, 0u,         0u,       0u,  R_DEV_PIN_CMOS1},  /* PCMP0BP1 */

		/* delimiter - do not remove */
		{0u, (uint8_t)R_DEV_PIN_LAST, 0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}
};  


/****************/
/* hardware DMA */
/****************/

/* waveform
   This sound array is generated in 16bits but will be written in 32bit via the DMA
   So make sure it is aligned correctly and a multiple of 4
 */

#define DMA_UNIT                    0
#define DMA_CHANNEL_ID              0

static const uint16_t Offset = 0x00;
static const uint16_t Period = 0XE00;//0xBF;
extern _Bool chime_start1;
extern _Bool Sound_playing;

static r_pcmp_Parameters_t Pcm_Param = {
		/* ClockDiv                 */ R_PCMP_CLOCK_DIV_1,
		/* OperationMode            */ /*R_PCMP_OPERATION_MODE1*/R_PCMP_OPERATION_MODE0,
		/* StereoMode               */ /*R_PCMP_MODE_STEREO,*/R_PCMP_MODE_MONO,
		/* DoubleMode               */ R_PCMP_DOUBLE_MODE_OFF,
		/* PinMaskOutputs           */ 0, /* actual level high = 1 */
		/* PinMaskInverted          */ R_PCMP_OUTPUT_AN0 | R_PCMP_OUTPUT_BN0 | R_PCMP_OUTPUT_AN1 | R_PCMP_OUTPUT_BN1, /* invert all = 1 */
		/* PinMaskStaticLevelActive */ 0  /* enable static level = 1 */
};

static r_pcmp_Parameters_t Pcm_Param_OFF = {
		/* ClockDiv                 */ R_PCMP_CLOCK_DIV_1,
		/* OperationMode            */ /*R_PCMP_OPERATION_MODE1*/R_PCMP_OPERATION_MODE0,
		/* StereoMode               */ /*R_PCMP_MODE_STEREO,*/R_PCMP_MODE_MONO,
		/* DoubleMode               */ R_PCMP_DOUBLE_MODE_OFF,
		/* PinMaskOutputs           */ 0 , /* actual level high = 1 */
		/* PinMaskInverted          */ R_PCMP_OUTPUT_AN0 | R_PCMP_OUTPUT_BN0 | R_PCMP_OUTPUT_AN1 | R_PCMP_OUTPUT_BN1, /* invert all = 1 */
		/* PinMaskStaticLevelActive */0, /* actual level high = 1 */ /* enable static level = 1 */
};

static uint8_t bSoundEnable; /* sound is allowed to play */
static uint8_t bSoundEnablePlay; /* sound has been triggered */
static uint8_t bSoundOngoing; /* sound is being played */
static r_dma_TransEndStatType_t trans_end_stat;
static uint8_t stop_bit =0;

#define SOUND_DEFAULT_PCM_SIZE_WORDS  (1024 / 2)

static r_g3dma_ChCfg_t  Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_welcome,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_DEFAULT_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		0,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

extern r_pcmp_Error_t R_PCMP_SetActiveLevel(uint32_t Unit, r_pcmp_Parameters_t *Config);
/*******************************************************************************
  loc_DmaCh0Isr

    ISR callback when DMA transfer complete or count compare match
    (depends on the DMA config setting, InterruptAct)
 */
static void loc_DmaCh0Isr(void)
{
	trans_end_stat = R_DMA_TRANS_COMP;
	chime_start1 = 0;		/* Flag to denote that chime is over */
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param_OFF);
	Sound_playing = 0;
}

static void loc_RPAS_DmaCh0Isr(void)
{
	if(stop_bit==1)
	{
		PDMAMLE = 0;
		PDMADTE = 0;
		trans_end_stat = R_DMA_TRANS_COMP;
		chime_start1 = 0;		/* Flag to denote that chime is over */
		R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param_OFF);
		Sound_playing=0;
		//R_GPIO_WritePin(17, 6, 0);//Audio amplifier
	}
}

static void loc_DmaSeatBeltIsr(void);
uint8_t Sound_Play_Completion_Status(void);
/************************************************************
  Function: PCM_FFIL
 */
static void _PcmFfilIsr(void)
{
	static uint32_t pcm_word_count = 0; /* number of word counted */

	/* test code only will not run at full rate */
	pcm_word_count++;
}


static void _DMAInit(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_LoadChannel(&Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

/* the sound is stored in 16 bit but is transfered via DMA in 32 bit */

/* ***************  WELCOME SOUND START*******************************/
#define SOUND_WELCOME_PCM_SIZE_WORDS  (WELCOME_ELEMENTS / 2)

static r_g3dma_ChCfg_t  welcome_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_welcome,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_WELCOME_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		0,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_welcome(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_LoadChannel(&welcome_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_Welcome(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	/* set up the DMA */
	_DMAInit_welcome();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_welcome[0], sound_array_welcome[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_welcome[0], sound_array_welcome[1]);
	R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}

void SoundPCM_Welcome(void)
{
	/* configure the pins */
	//R_DEV_PinInit(SgPcmPinConfigEnable);
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	_SoundPCMDMAEnable_Welcome();
}

/* ***************  WELCOME SOUND END *******************************/


/* ***************  MESSAGE SOUND START*******************************/
#define SOUND_MESSAGE_PCM_SIZE_WORDS  (MESSAGE_ELEMENTS / 2)

static r_g3dma_ChCfg_t  message_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_message,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_MESSAGE_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		0,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_message(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_LoadChannel(&message_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_message(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	/* set up the DMA */
	_DMAInit_message();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_message[0], sound_array_message[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_message[0], sound_array_message[1]);
	R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}

void SoundPCM_message(void)
{
	/* configure the pins */
	//R_DEV_PinInit(SgPcmPinConfigEnable);
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	_SoundPCMDMAEnable_message();
}

/* ***************  MESSAGE SOUND END *******************************/

/* ***************  RPAS_St3 SOUND START*******************************/
#define SOUND_RPAS_ST3_PCM_SIZE_WORDS  (RPAS_ST3_ELEMENTS / 2)

static r_g3dma_ChCfg_t  RPAS_St3_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_RPAS_St3,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_RPAS_ST3_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		0,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_RPAS_St3(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_LoadChannel(&RPAS_St3_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_RPAS_St3(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	/* set up the DMA */
	_DMAInit_RPAS_St3();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_RPAS_St3[0], sound_array_RPAS_St3[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_RPAS_St3[0], sound_array_RPAS_St3[1]);
	R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}

void SoundPCM_RPAS_St3(void)
{
	/* configure the pins */
	//R_DEV_PinInit(SgPcmPinConfigEnable);
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	_SoundPCMDMAEnable_RPAS_St3();
}

/* ***************  RPAS_St3 SOUND END *******************************/

/* ***************  Overspeed SOUND START*******************************/
#define SOUND_OVERSPEED_PCM_SIZE_WORDS  (OVERSPEED_ELEMENTS / 2)

static r_g3dma_ChCfg_t  OverSpeed_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_OverSpeed,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_OVERSPEED_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		0,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_OverSpeed(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_LoadChannel(&OverSpeed_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_OverSpeed(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	/* set up the DMA */
	_DMAInit_OverSpeed();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_OverSpeed[0], sound_array_OverSpeed[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_OverSpeed[0], sound_array_OverSpeed[1]);
	R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}

void SoundPCM_OverSpeed(void)
{
	/* configure the pins */
	//R_DEV_PinInit(SgPcmPinConfigEnable);
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	_SoundPCMDMAEnable_OverSpeed();
}

/* ***************  Seatbelt_Lv0cont SOUND END *******************************/

/* ***************  Seatbelt_Lv0cont SOUND START*******************************/
#define SOUND_SEATBELT_LV0CONT_PCM_SIZE_WORDS  (SEATBELT_LV0CONT_ELEMENTS / 2)

static r_g3dma_ChCfg_t  Seatbelt_Lv0cont_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_Seatbelt_Lv0cont,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_SEATBELT_LV0CONT_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		1,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_BOTH,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_Seatbelt_Lv0cont(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_SetReloadConfig(DMA_UNIT, DMA_CHANNEL_ID,(uint32_t) sound_array_Seatbelt_Lv0cont, PCMP0AUSA, SOUND_SEATBELT_LV0CONT_PCM_SIZE_WORDS);
	R_G3DMA_LoadChannel(&Seatbelt_Lv0cont_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_DmaSeatBeltIsr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_Seatbelt_Lv0cont(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	/* set up the DMA */
	_DMAInit_Seatbelt_Lv0cont();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_Seatbelt_Lv0cont[0], sound_array_Seatbelt_Lv0cont[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_Seatbelt_Lv0cont[0], sound_array_Seatbelt_Lv0cont[1]);
	R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}

extern uint8_t cycle133_cntr;

uint8_t seat_belt_sound_cycles,cycle_count=0;
void SoundPCM_Seatbelt_Lv0cont(uint8_t cycles)
{
	/* configure the pins */
	//R_DEV_PinInit(SgPcmPinConfigEnable);
	seat_belt_sound_cycles = cycles;
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	_SoundPCMDMAEnable_Seatbelt_Lv0cont();
}

void SoundPCM_Seatbelt_Lv0cont_STOP(void)
{
	/* channel operation disable, Transfer Abort */
	PDMAMLE = 0;
	PDMADTE = 0;
	cycle_count = 0;
	chime_start1 = 0;

}

static void loc_DmaSeatBeltIsr(void)
{
	trans_end_stat = R_DMA_TRANS_COMP;
	cycle_count++;
	//if(cycle_count>=seat_belt_sound_cycles)
	if(cycle133_cntr>=seat_belt_sound_cycles)
	{
		SoundPCM_Seatbelt_Lv0cont_STOP();
		cycle_count = 0;
		chime_start1 = 0;
		R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param_OFF);
		Sound_playing = 0;
		//R_GPIO_WritePin(17, 6, 0);//Audio amplifier
	}



}
/* ***************  Seatbelt_Lv0cont SOUND END *******************************/


/* ***************  Reverse SOUND START*******************************/
#define SOUND_REVERSE_SELFCHECK_PCM_SIZE_WORDS  (REVERSE_SELFCHECK_ELEMENTS / 2)

static r_g3dma_ChCfg_t  Reverse_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_Reverse_self_check,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_REVERSE_SELFCHECK_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		0,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_Reverse(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_LoadChannel(&Reverse_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_Reverse(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	/* set up the DMA */
	_DMAInit_Reverse();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_Reverse_self_check[0], sound_array_Reverse_self_check[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_Reverse_self_check[0], sound_array_Reverse_self_check[1]);
	R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}

void SoundPCM_Reverse(void)
{
	/* configure the pins */
	//R_DEV_PinInit(SgPcmPinConfigEnable);
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	_SoundPCMDMAEnable_Reverse();
}

/* ***************  Reverse SOUND END *******************************/

/* ***************  TICK SOUND START*******************************/
#define SOUND_TICK_PCM_SIZE_WORDS  (TICK_ELEMENTS / 2)

static r_g3dma_ChCfg_t  tick_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_tick,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_TICK_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		0,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_tick(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_LoadChannel(&tick_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_tick(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	uint16_t timeout = 0xFFFF;
	/* set up the DMA */
	_DMAInit_tick();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_tick[0], sound_array_tick[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_tick[0], sound_array_tick[1]);
	R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
	while(PDMADRQ == 0 && timeout )
	{
		timeout--;
		WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
	}
	if(timeout == 0)
	{
		error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_tock[0], sound_array_tock[1]);
		error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_tock[0], sound_array_tock[1]);
	}
}

void SoundPCM_tick(void)
{
	/* configure the pins */
	//	R_DEV_PinInit(SgPcmPinConfigEnable);
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	_SoundPCMDMAEnable_tick();
}

/* ***************  TICK SOUND END *******************************/

/* ***************  TOCK SOUND START*******************************/
#define SOUND_TOCK_PCM_SIZE_WORDS  (TOCK_ELEMENTS / 2)

static r_g3dma_ChCfg_t  tock_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_tock,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_TOCK_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		0,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_tock(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_LoadChannel(&tock_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_tock(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	uint16_t timeout = 0xFFFF;
	/* set up the DMA */
	_DMAInit_tock();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_tock[0], sound_array_tock[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_tock[0], sound_array_tock[1]);
	R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
	while(PDMADRQ == 0 && timeout )
	{
		timeout--;
		WDTA1WDTE = 0xAC;  //wATCHdOG CLEAR
	}
	if(timeout == 0)
	{
		error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_tock[0], sound_array_tock[1]);
		error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_tock[0], sound_array_tock[1]);
	}
}

void SoundPCM_tock(void)
{
	/* configure the pins */
	//	R_DEV_PinInit(SgPcmPinConfigEnable);
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	_SoundPCMDMAEnable_tock();
}

/* ***************  TOCK SOUND END *******************************/



/* ***************  RPAS_ST1_ST2  SOUND START*******************************/
#define SOUND_RPAS_ST1_ST2_PCM_SIZE_WORDS  (RPAS_ST1_ST2_ELEMENTS / 2)

static r_g3dma_ChCfg_t  RPAS_ST1_ST2_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_Reverse_self_check,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_REVERSE_SELFCHECK_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		0,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_RPAS_ST1_ST2(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_LoadChannel(&RPAS_ST1_ST2_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_RPAS_ST1_ST2(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	/* set up the DMA */
	_DMAInit_RPAS_ST1_ST2();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_Reverse_self_check[0], sound_array_Reverse_self_check[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_Reverse_self_check[0], sound_array_Reverse_self_check[1]);
	R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}

void SoundPCM_RPAS_ST1_ST2(void)
{
	/* configure the pins */
	//R_DEV_PinInit(SgPcmPinConfigEnable);
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	_SoundPCMDMAEnable_RPAS_ST1_ST2();
}

/* ***************  Reverse SOUND END *******************************/
/* ***************  sound_keyinremind SOUND START*******************************/
#define SOUND_KEYIN_SUNROOF_PCM_SIZE_WORDS  (KEYIN_SUNROOF_ELEMENTS / 2)

static r_g3dma_ChCfg_t  SOUND_KEYIN_SUNROOF_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_keyinremind,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_KEYIN_SUNROOF_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		0,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_KEYIN_SUNROOF(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_LoadChannel(&SOUND_KEYIN_SUNROOF_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_KEYIN_SUNROOF(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	/* set up the DMA */
	_DMAInit_KEYIN_SUNROOF();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_keyinremind[0], sound_array_keyinremind[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_keyinremind[0], sound_array_keyinremind[1]);
	R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}

void SoundPCM_KEYIN_SUNROOF(void)
{
	/* configure the pins */
	//R_DEV_PinInit(SgPcmPinConfigEnable);
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	_SoundPCMDMAEnable_KEYIN_SUNROOF();
}

/* ***************  sound_keyinremind SOUND END *******************************/
/* ***************  Reverse SOUND START*******************************/
#define SOUND_REVERSE_PCM_SIZE_WORDS  (REVERSE_ELEMENTS / 2)
#define SOUND_RPAS_LV1_BEGIN_PCM_SIZE_WORDS  (RPAS_LV1_BEGIN_ELEMENTS / 2)

static r_g3dma_ChCfg_t  RPAS_Cont_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_RPAS_LV1_BEGIN,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_RPAS_LV1_BEGIN_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		1,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_BOTH,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_RPAS_Cont(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_SetReloadConfig(DMA_UNIT, DMA_CHANNEL_ID,(uint32_t) sound_array_Reverse, PCMP0AUSA, SOUND_REVERSE_PCM_SIZE_WORDS);
	R_G3DMA_LoadChannel(&RPAS_Cont_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_RPAS_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_RPAS_Cont(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	/* set up the DMA */
	_DMAInit_RPAS_Cont();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_RPAS_LV1_BEGIN[0], sound_array_RPAS_LV1_BEGIN[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_RPAS_LV1_BEGIN[0], sound_array_RPAS_LV1_BEGIN[1]);
	R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}

void SoundPCM_4_rec_RPAS_St1_St2_cont(void)
{
	/* configure the pins */
	//R_DEV_PinInit(SgPcmPinConfigEnable);
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	_SoundPCMDMAEnable_RPAS_Cont();
	stop_bit=0;
	Sound_playing = 1;
}

void SoundPCM_4_rec_RPAS_St1_St2_cont_STOP(void)
{
	/* channel operation disable, Transfer Abort */
	/*PDMAMLE = 0;
    PDMADTE = 0;*/
	//PDMAMLE = 0;
	//PDMADTE = 0;
	//trans_end_stat = R_DMA_TRANS_COMP;
	//chime_start1 = 0;		/* Flag to denote that chime is over */
	//R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param_OFF);
	Sound_playing=0;
	stop_bit=1;

}

/* ***************  Reverse SOUND END *******************************/

/* ***************  RPAS LV2 SOUND START*******************************/
#define SOUND_RPAS_LV2_PCM_SIZE_WORDS  (RPAS_LV2_ELEMENTS / 2)

static r_g3dma_ChCfg_t  RPAS_LV2_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_RPAS_LV1_BEGIN,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_RPAS_LV1_BEGIN_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		1,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_BOTH,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_RPAS_LV2(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_SetReloadConfig(DMA_UNIT, DMA_CHANNEL_ID,(uint32_t) sound_array_RPAS_LV2, PCMP0AUSA, SOUND_RPAS_LV2_PCM_SIZE_WORDS);
	R_G3DMA_LoadChannel(&RPAS_LV2_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_RPAS_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_RPAS_LV2(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	/* set up the DMA */
	_DMAInit_RPAS_LV2();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_RPAS_LV1_BEGIN[0], sound_array_RPAS_LV1_BEGIN[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_RPAS_LV1_BEGIN[0], sound_array_RPAS_LV1_BEGIN[1]);
	R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}

void SoundPCM_RPAS_LV2(void)
{
	/* configure the pins */
	//R_DEV_PinInit(SgPcmPinConfigEnable);
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	_SoundPCMDMAEnable_RPAS_LV2();
	stop_bit=0;
	Sound_playing = 1;
}

void SoundPCM_RPAS_LV2_STOP(void)
{
	/* channel operation disable, Transfer Abort */
	//PDMAMLE = 0;
	//PDMADTE = 0;
	//trans_end_stat = R_DMA_TRANS_COMP;
	//chime_start1 = 0;		/* Flag to denote that chime is over */
	//R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param_OFF);
	Sound_playing=0;
	stop_bit=1;


}

/* ***************  RPAS LV2 SOUND END *******************************/


/* ***************  RPAS LV3 SOUND START*******************************/
#define SOUND_RPAS_LV3_PCM_SIZE_WORDS  (RPAS_LV3_ELEMENTS / 2)

static r_g3dma_ChCfg_t  RPAS_LV3_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_RPAS_LV1_BEGIN,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_RPAS_LV1_BEGIN_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		1,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_BOTH,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_RPAS_LV3(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_SetReloadConfig(DMA_UNIT, DMA_CHANNEL_ID,(uint32_t) sound_array_RPAS_LV3, PCMP0AUSA, SOUND_RPAS_LV3_PCM_SIZE_WORDS);
	R_G3DMA_LoadChannel(&RPAS_LV3_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_RPAS_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_RPAS_LV3(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	/* set up the DMA */
	_DMAInit_RPAS_LV3();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_RPAS_LV1_BEGIN[0], sound_array_RPAS_LV1_BEGIN[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_RPAS_LV1_BEGIN[0], sound_array_RPAS_LV1_BEGIN[1]);
	R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}

void SoundPCM_RPAS_LV3(void)
{
	/* configure the pins */
	//R_DEV_PinInit(SgPcmPinConfigEnable);
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	_SoundPCMDMAEnable_RPAS_LV3();
	stop_bit=0;
	Sound_playing = 1;
}

void SoundPCM_RPAS_LV3_STOP(void)
{
	/* channel operation disable, Transfer Abort */
	//PDMAMLE = 0;
	//PDMADTE = 0;
	//trans_end_stat = R_DMA_TRANS_COMP;
	//chime_start1 = 0;		/* Flag to denote that chime is over */
	//R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param_OFF);
	Sound_playing=0;
	stop_bit=1;


}

/* ***************  RPAS LV3 SOUND END *******************************/


/* ***************  RPAS LV4 SOUND START*******************************/
#define SOUND_RPAS_LV4_PCM_SIZE_WORDS  (RPAS_LV4_ELEMENTS / 2)

static r_g3dma_ChCfg_t  RPAS_LV4_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_RPAS_LV1_BEGIN,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_RPAS_LV1_BEGIN_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		1,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_BOTH,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_RPAS_LV4(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_SetReloadConfig(DMA_UNIT, DMA_CHANNEL_ID,(uint32_t) sound_array_RPAS_LV4, PCMP0AUSA, SOUND_RPAS_LV4_PCM_SIZE_WORDS);
	R_G3DMA_LoadChannel(&RPAS_LV4_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_RPAS_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_RPAS_LV4(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	/* set up the DMA */

	_DMAInit_RPAS_LV4();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_RPAS_LV1_BEGIN[0], sound_array_RPAS_LV1_BEGIN[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_RPAS_LV1_BEGIN[0], sound_array_RPAS_LV1_BEGIN[1]);
	R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}

void SoundPCM_RPAS_LV4(void)
{
	/* configure the pins */
	//R_DEV_PinInit(SgPcmPinConfigEnable);
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	_SoundPCMDMAEnable_RPAS_LV4();
	stop_bit=0;
	Sound_playing = 1;
}

void SoundPCM_RPAS_LV4_STOP(void)
{
	/* channel operation disable, Transfer Abort */
	//PDMAMLE = 0;
	//PDMADTE = 0;
	//trans_end_stat = R_DMA_TRANS_COMP;
	//chime_start1 = 0;		/* Flag to denote that chime is over */
	//R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param_OFF);
	Sound_playing=0;
	stop_bit =1;


}

/* ***************  RPAS LV4 SOUND END *******************************/

static void loc_DmaReverseIsr(void);
/* ***************  Reverse_cont SOUND START*******************************/
#define SOUND_REVERSE_PCM_SIZE_WORDS  (REVERSE_ELEMENTS / 2)

static r_g3dma_ChCfg_t  Reverse_cont_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_RPAS_LV1_BEGIN,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_RPAS_LV1_BEGIN_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		1,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_BOTH,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_Reverse_cont(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_SetReloadConfig(DMA_UNIT, DMA_CHANNEL_ID,(uint32_t) sound_array_Reverse, PCMP0AUSA, SOUND_REVERSE_PCM_SIZE_WORDS);
	R_G3DMA_LoadChannel(&Reverse_cont_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_DmaReverseIsr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_Reverse_cont(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	/* set up the DMA */
	_DMAInit_Reverse_cont();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_RPAS_LV1_BEGIN[0], sound_array_RPAS_LV1_BEGIN[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_RPAS_LV1_BEGIN[0], sound_array_RPAS_LV1_BEGIN[1]);
	R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}


uint8_t Reverse_sound_cycles,reverse_cycle_count=0;
void SoundPCM_Reverse_cont(uint8_t cycles)
{
	/* configure the pins */
	//R_DEV_PinInit(SgPcmPinConfigEnable);
	Reverse_sound_cycles = cycles;
	reverse_cycle_count = 0;
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	_SoundPCMDMAEnable_Reverse_cont();
}

void SoundPCM_Reverse_cont_STOP(void)
{
	/* channel operation disable, Transfer Abort */
	PDMAMLE = 0;
	PDMADTE = 0;
	reverse_cycle_count = 0;
	chime_start1 = 0;

}

static void loc_DmaReverseIsr(void)
{
	trans_end_stat = R_DMA_TRANS_COMP;
	//cycle133_cntr++;
	reverse_cycle_count++;
	if(reverse_cycle_count>=Reverse_sound_cycles)
	{
		SoundPCM_Reverse_cont_STOP();
		reverse_cycle_count = 0;
		chime_start1 = 0;
		R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param_OFF);
		Sound_playing = 0;
		//R_GPIO_WritePin(17, 6, 0);//Audio amplifier
	}



}
/* ***************  Reverse_cont SOUND END *******************************/






static void loc_buzz_DmaCh0Isr(void);
/* ***************  TURN BUZZ SOUND START*******************************/
#define SOUND_TURN_BUZZ_PCM_SIZE_WORDS  (TURN_BUZZ_ELEMENTS)

static r_g3dma_ChCfg_t  turn_buzz_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_turn_buzz,    /* SrcAddr */
		0xFFE3205C,   /* DstAddr */
		SOUND_TURN_BUZZ_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_16BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		0,   /* LoopEnable */
		52,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_turn_buzz(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_LoadChannel(&turn_buzz_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_buzz_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_Turn_Buzz(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	/* set up the DMA */
	_DMAInit_turn_buzz();

	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);


}

void SoundPCM_Turn_Buzz(void)
{

	_SoundPCMDMAEnable_Turn_Buzz();
}

static void loc_buzz_DmaCh0Isr(void)
{

	trans_end_stat = R_DMA_TRANS_COMP;
	chime_start1 = 0;		/* Flag to denote that chime is over */

}
/* ***************  TURN BUZZ SOUND END *******************************/

/* ***************  FIRST SOUND START*******************************/
#define SOUND_FIRST_PCM_SIZE_WORDS  (FIRST_ELEMENTS / 2)

static r_g3dma_ChCfg_t  SOUND_FIRST_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_first,    /* SrcAddr */
		PCMP0AUSA,   /* DstAddr */
		SOUND_FIRST_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_32BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		0,   /* LoopEnable */
		117,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};

static void _DMAInit_FIRST(void)
{
	/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_LoadChannel(&SOUND_FIRST_Dma_ChCfg, 1);
	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
}

static void _SoundPCMDMAEnable_FIRST(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;
	/* set up the DMA */
	_DMAInit_FIRST();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_first[0], sound_array_first[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_first[0], sound_array_first[1]);
	R_GPIO_WritePin(17, 6, 1);//Audio amplifier
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}

void SoundPCM_FIRST(void)
{
	/* configure the pins */
	//R_DEV_PinInit(SgPcmPinConfigEnable);
	R_PCMP_SetActiveLevel(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	_SoundPCMDMAEnable_FIRST();
}

/* ***************  FIRST SOUND END *******************************/


static void loc_RPAS_buzz_DmaCh0Isr(void);
/* ***************  RPAS_LV1 BUZZ SOUND START*******************************/
#define SOUND_RPAS_LV1_BUZZ_PCM_SIZE_WORDS  (RPAS_LV1_BUZZ_ELEMENTS)

static r_g3dma_ChCfg_t  RPAS_LV1_buzz_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_rpas_lv1_buzz,    /* SrcAddr */
		0xFFE3205C,   /* DstAddr */
		SOUND_RPAS_LV1_BUZZ_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_16BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		1,   /* LoopEnable */
		69,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_BOTH,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};


void SoundPCM_RPAS_LV1_Buzz(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;


/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_SetReloadConfig(DMA_UNIT, DMA_CHANNEL_ID,(uint32_t) sound_array_rpas_lv1_buzz, 0xFFE3205C, SOUND_RPAS_LV1_BUZZ_PCM_SIZE_WORDS);
	R_G3DMA_LoadChannel(&RPAS_LV1_buzz_Dma_ChCfg, 1);

	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_RPAS_buzz_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

}
void SoundPCM_RPAS_buzz_LV1_STOP(void)
{
		PDMAMLE = 0;
	    PDMADTE = 0;
	    chime_start1 = 0;

		r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
		trans_end_stat = R_DMA_TRANS_COMP;
}
static void loc_RPAS_buzz_DmaCh0Isr(void)
{

	//trans_end_stat = R_DMA_TRANS_COMP;
	chime_start1 = 0;		/* Flag to denote that chime is over */

}
/* ***************  RPAS LV1 BUZZ SOUND END *******************************/

/* ***************  RPAS_LV2 BUZZ SOUND START*******************************/
#define SOUND_RPAS_LV2_BUZZ_PCM_SIZE_WORDS  (RPAS_LV2_BUZZ_ELEMENTS)

static r_g3dma_ChCfg_t  RPAS_LV2_buzz_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_rpas_lv2_buzz,    /* SrcAddr */
		0xFFE3205C,   /* DstAddr */
		SOUND_RPAS_LV2_BUZZ_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_16BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		1,   /* LoopEnable */
		69,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_BOTH,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};


void SoundPCM_RPAS_LV2_Buzz(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;


/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_SetReloadConfig(DMA_UNIT, DMA_CHANNEL_ID,(uint32_t) sound_array_rpas_lv2_buzz, 0xFFE3205C, SOUND_RPAS_LV2_BUZZ_PCM_SIZE_WORDS);
	R_G3DMA_LoadChannel(&RPAS_LV2_buzz_Dma_ChCfg, 1);

	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_RPAS_buzz_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

}
void SoundPCM_RPAS_buzz_LV2_STOP(void)
{
		PDMAMLE = 0;
	    PDMADTE = 0;
	    chime_start1 = 0;

		r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
		trans_end_stat = R_DMA_TRANS_COMP;
}

/* ***************  RPAS LV2 BUZZ SOUND END *******************************/

/* ***************  RPAS_LV3 BUZZ SOUND START*******************************/
#define SOUND_RPAS_LV3_BUZZ_PCM_SIZE_WORDS  (RPAS_LV3_BUZZ_ELEMENTS)

static r_g3dma_ChCfg_t  RPAS_LV3_buzz_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_rpas_lv3_buzz,    /* SrcAddr */
		0xFFE3205C,   /* DstAddr */
		SOUND_RPAS_LV3_BUZZ_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_16BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		1,   /* LoopEnable */
		69,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_BOTH,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};


void SoundPCM_RPAS_LV3_Buzz(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;


/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_SetReloadConfig(DMA_UNIT, DMA_CHANNEL_ID,(uint32_t) sound_array_rpas_lv3_buzz, 0xFFE3205C, SOUND_RPAS_LV3_BUZZ_PCM_SIZE_WORDS);
	R_G3DMA_LoadChannel(&RPAS_LV3_buzz_Dma_ChCfg, 1);

	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_RPAS_buzz_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

}
void SoundPCM_RPAS_buzz_LV3_STOP(void)
{
		PDMAMLE = 0;
	    PDMADTE = 0;
	    chime_start1 = 0;

		r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
		trans_end_stat = R_DMA_TRANS_COMP;
}

/* ***************  RPAS LV3 BUZZ SOUND END *******************************/

/* ***************  RPAS_LV4 BUZZ SOUND START*******************************/
#define SOUND_RPAS_LV4_BUZZ_PCM_SIZE_WORDS  (RPAS_LV4_BUZZ_ELEMENTS)

static r_g3dma_ChCfg_t  RPAS_LV4_buzz_Dma_ChCfg = {
		0,   /* Unit */
		DMA_CHANNEL_ID,   /* ChannelId */
		(uint32_t) sound_array_rpas_lv4_buzz,    /* SrcAddr */
		0xFFE3205C,   /* DstAddr */
		SOUND_RPAS_LV4_BUZZ_PCM_SIZE_WORDS,   /* TransferCount this is word size */
		R_G3DMA_16BIT_T,   /* TransferDataSize */
		R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
		R_G3DMA_FIXED_T,   /* DstAddrCntDir */
		1,   /* LoopEnable */
		69,   /* INTPCMP0FFIL G3DMATriggerFactor */
		1,   /* InterruptAct */
		R_G3DMA_MODE_S,    /* Mode */
		0,   /* Cpompare Count */
		0,   /* Next Channel in Chain */
		R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
		R_G3DMA_RELOAD_BOTH,   /* reload function 1 disable */
		R_G3DMA_RELOAD_OFF,   /* reload function 2 disable */
};


void SoundPCM_RPAS_LV4_Buzz(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;


/* config the DMA */
	R_G3DMA_Init(DMA_UNIT);
	R_G3DMA_SetReloadConfig(DMA_UNIT, DMA_CHANNEL_ID,(uint32_t) sound_array_rpas_lv4_buzz, 0xFFE3205C, SOUND_RPAS_LV4_BUZZ_PCM_SIZE_WORDS);
	R_G3DMA_LoadChannel(&RPAS_LV4_buzz_Dma_ChCfg, 1);

	R_DMA_SetIsrCallback(DMA_UNIT, DMA_CHANNEL_ID, loc_RPAS_buzz_DmaCh0Isr);
	R_G3DMA_EnableInt(DMA_UNIT, DMA_CHANNEL_ID);

	trans_end_stat = R_DMA_TRANS_NOT_COMP;
	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

}
void SoundPCM_RPAS_buzz_LV4_STOP(void)
{
		PDMAMLE = 0;
	    PDMADTE = 0;
	    chime_start1 = 0;

		r_taub_OutputDisable(2,R_TAUB_CHANNEL_13);
		trans_end_stat = R_DMA_TRANS_COMP;
}

/* ***************  RPAS LV4 BUZZ SOUND END *******************************/


/*********LOW END BUZZER FILES END *********************LOW END BUZZER FILES END *********************LOW END BUZZER FILES END**************
**********LOW END BUZZER FILES END *********************LOW END BUZZER FILES END *********************LOW END BUZZER FILES END**************
**********LOW END BUZZER FILES END *********************LOW END BUZZER FILES END *********************LOW END BUZZER FILES END**************
**********LOW END BUZZER FILES END *********************LOW END BUZZER FILES END *********************LOW END BUZZER FILES END**************
 */


/************************************************************
  Function: _SoundPCMDMAEnable
 */
#if 0
static void _SoundPCMDMAEnable_message(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;

	/* set up the DMA */
	_DMAInit();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */

	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	//    error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_pcm[0], sound_array_pcm[1]);
	//    error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_pcm[0], sound_array_pcm[1]);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_message[0], sound_array_message[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_message[0], sound_array_message[1]);

	if(error)
	{
		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}
#endif

static void _SoundPCMDMAEnable(void)
{
	r_pcmp_Error_t error = R_PCMP_ERR_OK;

	/* set up the DMA */
	_DMAInit();

	error = R_PCMP_Enable(PCM_UNIT, R_TRUE);

	/* kick it off to generate the interrupt, two samples are required to
       fill the input FIFO, as the first one goes out the FILL interrupt will
       be generated
	 */

	/* enable DMA, do the actual transfer */
	R_G3DMA_HwEnable(DMA_UNIT, DMA_CHANNEL_ID);

	//    error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_pcm[0], sound_array_pcm[1]);
	//    error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_pcm[0], sound_array_pcm[1]);

	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_message[0], sound_array_message[1]);
	error |= R_PCMP_WriteSamples(PCM_UNIT, sound_array_message[0], sound_array_message[1]);

	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}

/************************************************************
  Function: SoundPCM_Init
 */
void SoundPCM_Init(uint8_t Wakeup)
{
	uint32_t u32_temp;
	r_pcmp_Error_t error = R_PCMP_ERR_OK;

	/* no sound running so indicate */
	bSoundEnable = R_FALSE;
	bSoundEnablePlay = R_FALSE;
	bSoundOngoing = R_FALSE;

	/* if reset then display settings */
	if(!Wakeup)
	{
		//		R_BSP_STDIO_Printf("PCM PWM:\n");

		/* clock for the PCMP CLKFIX */
		u32_temp = R_DEV_ClkFrequencyHz(R_DEV_CKS_PCMP);
#if 0
		R_BSP_STDIO_Printf("PCMP clock frequency '%dHz'\n", u32_temp);
		R_BSP_STDIO_Printf(" - PCM PWM right speaker driven by the H-Bridge:\n");
		R_BSP_STDIO_Printf("     close jumper JP55\n");
		R_BSP_STDIO_Printf("     close jumper JP53\n");
		R_BSP_STDIO_Printf("     open  jumper JP52\n");
		R_BSP_STDIO_Printf("     open  jumper JP54\n");
		R_BSP_STDIO_Printf("     close jumper JP46 3 & 5\n");
		R_BSP_STDIO_Printf("     close jumper JP46 4 & 6\n");
#endif
	}

	/* configure the pins */
	//  R_DEV_PinInit(SgPcmPinConfigEnable);


	/* the sample is actually a stereo one but we will just feed one channel out for now
       (left) and allow the sound generator to use the other
	 */

	error |= R_PCMP_Init(PCM_UNIT, (r_pcmp_Parameters_t *) &Pcm_Param);
	error |= R_PCMP_SetPwmPeriod(PCM_UNIT, Period);
	error |= R_PCMP_SetOffset(PCM_UNIT, Offset);

	error |= R_PCMP_SetIsrCallback(PCM_UNIT, R_PCMP_INT_FFIL, _PcmFfilIsr);
	/* used for testing only, its too fast when using the current period, slow
       it down if you want to see interrupts, i.e. Period = 0x077F; 
    error |= R_PCMP_EnableInt(PCM_UNIT, R_PCMP_INT_FFIL);*/
	// R_SG_SetVolume(1,90);
	if(error)
	{
		//		R_BSP_STDIO_Printf("Error with sound configuration\n");
	}
}


/************************************************************
  Function: SoundPCM_Play
 */
uint8_t SoundPCM_Play(void)
{
	/* see if we are allowed to play any sounds */
	if(bSoundEnable)
	{
		bSoundEnablePlay = R_TRUE;
	}
	return bSoundEnable;
}


/************************************************************
  Function: SoundPCM_Enable
 */
void SoundPCM_Enable(void)
{
	bSoundEnable = R_TRUE;
}


/************************************************************
  Function: SoundDisable
 */
void SoundPCM_Disable(void)
{
	bSoundEnable = R_FALSE;
}


/************************************************************
  Function: SoundProcessing
 */
#if 1
void SoundPCM_Processing(void)
{
	/* check the sound is not ongoing */
	if(!bSoundOngoing)
	{
		/* trigger the sound to play */
		if(bSoundEnablePlay)
		{
			bSoundEnablePlay = R_FALSE;
			bSoundOngoing = R_TRUE;

			/* configure the pins */
			//R_DEV_PinInit(SgPcmPinConfigEnable);
			_SoundPCMDMAEnable();
		}
		else
		{
			/* disable the drive static levels */
		}
	}
	else
	{

#ifdef DMA_POLL_COMPLETE

		/* a sound has started now check if the DMA has completed */
		trans_end_stat = R_G3DMA_TransEndStat(DMA_UNIT, DMA_CHANNEL_ID);

#else /* use ISR transfer complete */

		/* status set in ISR */

#endif

		/* are we done ? */
		/*   if(R_DMA_TRANS_COMP == trans_end_stat)
        {
            bSoundOngoing = R_FALSE;*/

		/* inform the state manager of a sound complete event */
		/* eeStateManager_Processing(E_SOUND_COMP);
        }*/
	}
}
#endif

/************************************************************
  Function: SoundPCM_IsBusy
 */
uint8_t SoundPCM_IsBusy(void)
{
	/* also if a pending sound is about to be activated */
	return (bSoundOngoing || bSoundEnablePlay);
}

uint8_t Sound_Play_Completion_Status(void)
{
	return (trans_end_stat);
}

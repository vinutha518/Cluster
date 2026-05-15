/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/******************************************************************************

/* Standard Includes */
#include "r_typedefs.h"
#include "my_can.h"
#include "serial_to_parallel.h"
#include "parallel_to_serial.h"
#include "TLC6C_S_P.h"
#include "dbgmacro.h"
#include "errmgr.h"
#include "dr7f701461.dvf.h"
#include "r_typedefs.h"        /* Renesas basic types, e.g. uint32_t */
#include "r_bsp_api.h"         /* Board support package */
#include "sound_pcm.h"
#include "UDSS.h"
#include "r_cg_timer.h"
#include "r_gpio_api.h"
#include "UDS_DTC.h"
#include "UDSS_APP.h"
#include "sound_pcm.h"
#include "r_dev_api.h"
#include "r_bsp_api.h"
#include "r_bsp_stdio_api.h"
#include "r_tick_api.h"
#include "init.h"
#include "r_gpio_api.h"
/*#include "r_taub_isr.h"*/
/*#include "r_taub_main.h"*/
#include "r_taub_api.h"
/*#include "r_taub_sys.h"*/
/*Drome Header Files*/
/*#include "drconf.h"
#include "druserconf.h"
#include "drapp.h"
#include "gui.h"
#include "drome.h"
 */

#define CPU_F ((double)25000000)

#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define IGN 	R_GPIO_PinRead(0,1)//	PORT_AWOP0_1  /* Ignition */

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*Narendran*/
#define ILLUM_SCALE 3	/* Scaling Value is Taken from Existing Code */
#define ILLUM_DIVIDENT 1	/* Divident Value is Taken From Existing Code */
/* THis Macro is used find Value for  the Percentage of Illumination Needed */
#define ILLUM_PER(x)	\
			(((x)*ILLUM_SCALE)/ILLUM_DIVIDENT)
uint8_t currIllumination;	/* This Variable has the Current Value of Illumination Status from NVM*/
uint8_t firstTimeIn = 0 ;
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


extern uint16_t gcsk_flag;
extern DTC_ID;
extern _Bool Off_diagTTflag;
_Bool DC_data_process_flag;

uint8_t Diag_Error,diagTTflag,Illumn_percent;
_Bool tst_tt_flag;
_Bool tst_tt_flag_1;
_Bool tst_tt_flag_2;
_Bool tst_tt_flag_3;
_Bool tst_tt_flag_4;
_Bool tst_tt_flag_5;
_Bool tst_tt_flag_6;
_Bool tst_tt_flag_7;
_Bool tst_tt_flag_8;
_Bool tst_tt_flag_9;
_Bool tst_tt_flag_10;		/* @@@ New Flag for extra condition */
_Bool gs_complete;
_Bool gs_complete_test;
uint32_t GS_DATA_1SEC_COUNTER;
//char ERROR_Clear[]={ 0xA5,0x30
//};
/*
char GLOBAL_RESET[]={0x00,0x00
		,0x00,0x00,0x00,0x00
		,0x00,0x00,0x00,0x00
		,0x00,0x00,0x00,0x00
		,0x00,0x00,0x00,0x00
		,0x00,0x00,0x00,0x00
		,0x00,0x00,0x00,0x00
		,0x00,0x00,0x00,0x00
		,0x00,0x00,0x00,0x00,0x0A,0x5C
};
 */

//char read_SID[50];
/*
char DCdata0[]={
		0x00,0x00,0x00,//R0 G0 B0
		0x00,0x00,0x00,//R1 G1 B1
		0x00,0x00,0x00,//R2 G2 B2
		0x00,0x00,0x00,//R3 G3 B3
		0x00,0x00,0x00,//R4 G4 B4
		0x00,0x00,0x00,//R5 G5 B5
		0x00,0x00,0x00,//R6 G6 B6
		0x00,0x00,0x00,//R7 G7 B7
};
 */
char DCdata1[]={
		0x7f,0x7f,0x7f,//R0 G0 B0
		0x7f,0x7f,0x7f,//R1 G1 B1
		0x7f,0x7f,0x7f,//R2 G2 B2
		0x7f,0x7f,0x7f,//R3 G3 B3
		0x7f,0x7f,0x7f,//R4 G4 B4
		0x7f,0x7f,0x7f,//R5 G5 B5
		0x7f,0x7f,0x7f,//R6 G6 B6
		0x7f,0x7f,0x7f//R7 G7 B7
};									//Q2
char DCdata2[]={
		0x7f,0x7f,0x7f,//R0 G0 B0
		0x7f,0x7f,0x7f,//R1 G1 B1
		0x7f,0x7f,0x7f,//R2 G2 B2
		0x7f,0x7f,0x7f,//R3 G3 B3
		0x7f,0x7f,0x7f,//R4 G4 B4
		0x7f,0x7f,0x7f,//R5 G5 B5
		0x7f,0x7f,0x7f,//R6 G6 B6
		0x7f,0x7f,0x7f//R7 G7 B7
};									//Q1
#ifdef PCBrevB
int read_SID[288];

int GSdata0[]={
		0x000,0x000,0x000,//R0 G0 B0
		0x000,0x000,0x000,//R1 G1 B1
		0x000,0x000,0x000,//R2 G2 B2
		0x000,0x000,0x000,//R3 G3 B3
		0x000,0x000,0x000,//R4 G4 B4
		0x000,0x000,0x000,//R5 G5 B5
		0x000,0x000,0x000,//R6 G6 B6
		0x000,0x000,0x000//R7 G7 B7
};
/*
int GSdataON[]={
		0xfff,0xfff,0xfff,//R0 G0 B0
		0xfff,0xfff,0xfff,//R1 G1 B1
		0xfff,0xfff,0xfff,//R2 G2 B2
		0xfff,0xfff,0x000,//R3 G3 B3
		0xfff,0x000,0x000,//R4 G4 B4
		0x000,0x000,0x000,//R5 G5 B5
		0x000,0xfff,0xfff,//R6 G6 B6
		0x000,0x000,0x000 //R7 G7 B7
};
 */
int GSdataON[]={
		0,0,0,//R0 G0 B0
		0,0,0,//R1 G1 B1
		0,0,0,//R2 G2 B2
		0,0,0,//R3 G3 B3
		0,0,0,//R4 G4 B4
		0,0,0,//R5 G5 B5
		0,0,0,//R6 G6 B6
		0,0,0 //R7 G7 B7
};
/*
int GSdata1[]={
		0xfff,0xfff,0xfff,//R0 G0 B0
		0xfff,0xfff,0xfff,//R1 G1 B1
		0xfff,0xfff,0xfff,//R2 G2 B2
		0xfff,0xfff,0xfff,//R3 G3 B3
		0xfff,0xfff,0xfff,//R4 G4 B4
		0xfff,0x000,0xfff,//R5 G5 B5
		0xfff,0xfff,0xfff,//R6 G6 B6
		0xfff,0x000,0xfff //R7 G7 B7
};
 */
int GSdata1[]={	0,0,0,//R0 G0 B0
		0,0,0,//R1 G1 B1
		0,0,0,//R2 G2 B2
		0,0,0,//R3 G3 B3
		0,0,0,//R4 G4 B4
		0,0,0,//R5 G5 B5
		0,0,0,//R6 G6 B6
		0,0,0//R7 G7 B7
};
/*
int color2pwm[]={0,1,1,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,14,15,16,17,18,20,
		21,23,25,26,28,30,32,34,36,38,40,43,45,48,50,53,56,59,62,65,68,71,75,78,82,85,89,93,97,
		101,105,110,114,119,123,128,133,138,143,149,154,159,165,171,177,183,189,195,202,208,215,
		222,229,236,243,250,258,266,273,281,290,298,306,315,324,332,341,351,360,369,379,389,399,
		409,419,430,440,451,462,473,485,496,508,520,532,544,556,569,582,594,608,621,634,648,662,
		676,690,704,719,734,749,764,779,795,811,827,843,859,876,893,910,927,944,962,980,998,1016,
		1034,1053,1072,1091,1110,1130,1150,1170,1190,1210,1231,1252,1273,1294,1316,1338,1360,1382,
		1404,1427,1450,1473,1497,1520,1544,1568,1593,1617,1642,1667,1693,1718,1744,1770,1797,1823,
		1850,1877,1905,1932,1960,1988,2017,2045,2074,2103,2133,2162,2192,2223,2253,2284,2315,2346,
		2378,2410,2442,2474,2507,2540,2573,2606,2640,2674,2708,2743,2778,2813,2849,2884,2920,2957,
		2993,3030,3067,3105,3143,3181,3219,3258,3297,3336,3376,3416,3456,3496,3537,3578,3619,3661,
		3703,3745,3788,3831,3874,3918,3962,4006,4010,4015};// Gamma table
 */
void SendDCData(int CMD, int UD, char GS_MODE, char DSTMR, char ADSRPM, char DC_Range_Blue, char DC_Range_Green, char DC_Range_Red, char BCB, char BCG, char BCR, char *DCdata)//Send DC/BC/FC data thru GS Data Path
{
	R_GPIO_WritePin(1, 4, 1);		/*DLED_LATCH=1 GSLAT-High for DC Mode*/
	//	R_GPIO_WritePin(17, 7, 1);

	int k,temp1,temp2;
	/*Command Code BITS 287-276*/
	for(k=287;k>=276;k--)
	{
		temp1=1<<(k-276);
		temp2=CMD&temp1;
		if(temp2==temp1)
		{
			R_GPIO_WritePin(1, 1, 1);		/*DLED_SDI=1;//GSSIN - High*/
		}
		else
		{
			R_GPIO_WritePin(1, 1, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
		}
		//DLED_SDI=0;//GSSIN - Low
		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		R_TICK_WaitUS(0u, 1u);
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}
	/*No Value BITS 275-216*/
	R_GPIO_WritePin(1, 1, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
	for(k=275;k>=216;k--)
	{
		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		IP_SHIFT_DELAY();   // R_TICK_WaitUS(0u, 3u);
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}
	/*User Define BITS 215-199*/
	for(k=215;k>=199;k--)
	{
		temp1=1<<(k-199);
		temp2=UD&temp1;
		if(temp2==temp1)
		{
			R_GPIO_WritePin(1, 1, 1);		/*DLED_SDI=1;//GSSIN - High*/
		}
		else
		{
			R_GPIO_WritePin(1, 1, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
		}

		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		R_TICK_WaitUS(0u, 1u);   // R_TICK_WaitUS(0u, 3u);
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}
	/*GS Counter Mode BITS 198-197*/
	for(k=198;k>=197;k--)
	{
		temp1=1<<(k-197);
		temp2=GS_MODE&temp1;
		if(temp2==temp1)
		{
			R_GPIO_WritePin(1, 1, 1);		/*DLED_SDI=1;//GSSIN - High*/
		}
		else
		{
			R_GPIO_WritePin(1, 1, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
		}
		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		R_TICK_WaitUS(0u, 1u);   // R_TICK_WaitUS(0u, 3u);
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}

	/*Display Timing Reset Mode BITS 196*/
	if(DSTMR==1)
	{
		R_GPIO_WritePin(1, 1, 1);		/*DLED_SDI=1;//GSSIN - High*/
	}
	else
	{
		R_GPIO_WritePin(1, 1, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
	}
	R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
	IP_SHIFT_DELAY();   // R_TICK_WaitUS(0u, 3u);
	R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/

	/*Auto Display Repeat Mode BITS 195*/
	if(ADSRPM==1)
	{
		R_GPIO_WritePin(1, 1, 1);		/*DLED_SDI=1;//GSSIN - High*/
	}
	else
	{
		R_GPIO_WritePin(1, 1, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
	}
	R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
	IP_SHIFT_DELAY();   // R_TICK_WaitUS(0u, 3u);
	R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/

	/*Dot correction adjustment range for the BLUE color output BITS 194*/
	if(DC_Range_Blue==1)
	{
		R_GPIO_WritePin(1, 1, 1);		/*DLED_SDI=1;//GSSIN - High*/
	}
	else
	{
		R_GPIO_WritePin(1, 1, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
	}
	R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
	IP_SHIFT_DELAY();   // R_TICK_WaitUS(0u, 3u);
	R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/

	/*Dot correction adjustment range for the GREEN color output BITS 193*/
	if(DC_Range_Green==1)
	{
		R_GPIO_WritePin(1, 1, 1);		/*DLED_SDI=1;//GSSIN - High*/
	}
	else
	{
		R_GPIO_WritePin(1, 1, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
	}

	R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
	IP_SHIFT_DELAY();   // R_TICK_WaitUS(0u, 3u);
	R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/

	/*Dot correction adjustment range for the RED color output BITS 192*/
	if(DC_Range_Red==1)
	{
		R_GPIO_WritePin(1, 1, 1);		/*DLED_SDI=1;//GSSIN - High*/
	}
	else
	{
		R_GPIO_WritePin(1, 1, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
	}
	R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
	IP_SHIFT_DELAY();   // R_TICK_WaitUS(0u, 3u);
	R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/

	/*Global Brightness Control Blue BITS 191-184*/
	for(k=191;k>=184;k--)
	{
		temp1=1<<(k-184);
		temp2=BCB&temp1;
		if(temp2==temp1)
		{
			R_GPIO_WritePin(1, 1, 1);		/*DLED_SDI=1;//GSSIN - High*/
		}
		else
		{
			R_GPIO_WritePin(1, 1, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
		}
		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		R_TICK_WaitUS(0u, 1u);   // R_TICK_WaitUS(0u, 3u);
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}

	/*Global Brightness Control Green BITS 183-176*/
	for(k=183;k>=176;k--)
	{
		temp1=1<<(k-176);
		temp2=BCG&temp1;
		if(temp2==temp1)
		{
			R_GPIO_WritePin(1, 1, 1);		/*DLED_SDI=1;//GSSIN - High*/
		}
		else
		{
			R_GPIO_WritePin(1, 1, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
		}
		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		R_TICK_WaitUS(0u, 1u);   // R_TICK_WaitUS(0u, 3u);
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}

	/*Global Brightness Control Red BITS 175-168*/
	for(k=175;k>=168;k--)
	{
		temp1=1<<(k-168);
		temp2=BCR&temp1;
		if(temp2==temp1)
		{
			R_GPIO_WritePin(1, 1, 1);		/*DLED_SDI=1;//GSSIN - High*/
		}
		else
		{
			R_GPIO_WritePin(1, 1, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
		}
		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		R_TICK_WaitUS(0u, 1u);   // R_TICK_WaitUS(0u, 3u);
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}

	/*Dot Correction Data BITS 167-0*/
	for(k=167;k>=0;k--)
	{
		temp1=1<<(k%7);
		temp2=DCdata[k/7]&temp1;
		if(temp2==temp1)
		{
			R_GPIO_WritePin(1, 1, 1);		/*DLED_SDI=1;//GSSIN - High*/
		}
		else
		{
			R_GPIO_WritePin(1, 1, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
		}
		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		R_TICK_WaitUS(0u, 1u);   // R_TICK_WaitUS(0u, 3u);
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}
	R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
	R_TICK_WaitUS(0u, 1u);   // R_TICK_WaitUS(0u, 3u);
	R_GPIO_WritePin(1, 4, 0);		/*DLED_LATCH=0;  //GSLAT-Low*/
	R_TICK_WaitUS(0u, 1u);   // R_TICK_WaitUS(0u, 3u);
	R_GPIO_WritePin(1, 4, 1);		/*DLED_LATCH=1 GSLAT-High for DC Mode*/
	R_TICK_WaitUS(0u, 1u);   // R_TICK_WaitUS(0u, 3u);
}

void SendGSData(int *temp)// Send Gray Scale data
{
	R_GPIO_WritePin(1, 4, 0); //GSLAT-Low for GS Mode
	int k,temp1,temp2;
	for(k=287;k>=0;k--)
	{
		temp1=1<<(k%12);
		temp2=temp[k/12]&temp1;
		if(temp2==temp1)
		{
			R_GPIO_WritePin(1, 1, 1);		/*DLED_SDI=1;//GSSIN - High*/
		}
		else
		{
			R_GPIO_WritePin(1, 1, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
		}
		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		IP_SHIFT_DELAY();
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}



	R_GPIO_WritePin(17, 7, 0);//0 blank low

	R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
	IP_SHIFT_DELAY();
	R_GPIO_WritePin(1, 4, 1);		/*DLED_LATCH=1 GSLAT-High for DC Mode*/

	R_GPIO_WritePin(17, 7, 1);

	IP_SHIFT_DELAY();
	R_GPIO_WritePin(1, 4, 0);		/*DLED_LATCH=0;  //GSLAT-Low*/
	IP_SHIFT_DELAY();
	if(gcsk_flag == 0)
		gcsk_flag=1;
}

void ReadDCData()
{
	_Bool l;
	int m;
	R_GPIO_WritePin(1, 4, 1);		/*DLED_LATCH=1 GSLAT-High for DC Mode*/



	for(m=287;m>=0;m--)
	{
		l=R_GPIO_PinRead(1,1);

		read_SID[m]= l;

		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		IP_SHIFT_DELAY();
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}

	R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
	IP_SHIFT_DELAY();

}

void DIAG_TT_init(void)
{
	diagTTflag=1;


	SendGSData(GSdata1);


}

#else
int read_SID[576];
int GSdata0[]={
		0x000,0x000,0x000,//R0 G0 B0
		0x000,0x000,0x000,//R1 G1 B1
		0x000,0x000,0x000,//R2 G2 B2
		0x000,0x000,0x000,//R3 G3 B3
		0x000,0x000,0x000,//R4 G4 B4
		0x000,0x000,0x000,//R5 G5 B5
		0x000,0x000,0x000,//R6 G6 B6
		0x000,0x000,0x000,//R7 G7 B7
		0x000,0x000,0x000,
		0x000,0x000,0x000,
		0x000,0x000,0x000,
		0x000,0x000,0x000,
		0x000,0x000,0x000,
		0x000,0x000,0x000,
		0x000,0x000,0x000,
		0x000,0x000,0x000,
};

int GSdataON[]={
		0xfff,0xfff,0xfff,//R0 G0 B0
		0xfff,0xfff,0xfff,//R1 G1 B1
		0xfff,0xfff,0xfff,//R2 G2 B2
		0xfff,0xfff,0xfff,//R3 G3 B3
		0xfff,0xfff,0xfff,//R4 G4 B4
		0xfff,0xfff,0xfff,//R5 G5 B5
		0xfff,0xfff,0xfff,//R6 G6 B6
		0xfff,0xfff,0xfff, //R7 G7 B7
		0xfff,0xfff,0xfff,//R0 G0 B0
		0xfff,0xfff,0xfff,//R1 G1 B1
		0xfff,0xfff,0xfff,//R2 G2 B2
		0xfff,0xfff,0xfff,//R3 G3 B3
		0xfff,0xfff,0xfff,//R4 G4 B4
		0xfff,0xfff,0xfff,//R5 G5 B5
		0xfff,0xfff,0xfff,//R6 G6 B6
		0xfff,0xfff,0xfff//R7 G7 B7
};
/*
int GSdata1[]={
		0xfff,0xfff,0xfff,//R0 G0 B0
		0xfff,0xfff,0xfff,//R1 G1 B1
		0xfff,0xfff,0xfff,//R2 G2 B2
		0xfff,0xfff,0xfff,//R3 G3 B3
		0xfff,0xfff,0xfff,//R4 G4 B4
		0xfff,0x000,0xfff,//R5 G5 B5
		0xfff,0xfff,0xfff,//R6 G6 B6
		0xfff,0x000,0xfff //R7 G7 B7
};
 */
int GSdata1[]={	0,0,0,//R0 G0 B0
		0,0,0,//R1 G1 B1
		0,0,0,//R2 G2 B2
		0,0,0,//R3 G3 B3
		0,0,0,//R4 G4 B4
		0,0,0,//R5 G5 B5
		0,0,0,//R6 G6 B6
		0,0,0,//R7 G7 B7
		0,0,0,//R0 G0 B0
		0,0,0,//R1 G1 B1
		0,0,0,//R2 G2 B2
		0,0,0,//R3 G3 B3
		0,0,0,//R4 G4 B4
		0,0,0,//R5 G5 B5
		0,0,0,//R6 G6 B6
		0,0,0,};//R7 G7 B7
/*
int color2pwm[]={0,1,1,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,14,15,16,17,18,20,
		21,23,25,26,28,30,32,34,36,38,40,43,45,48,50,53,56,59,62,65,68,71,75,78,82,85,89,93,97,
		101,105,110,114,119,123,128,133,138,143,149,154,159,165,171,177,183,189,195,202,208,215,
		222,229,236,243,250,258,266,273,281,290,298,306,315,324,332,341,351,360,369,379,389,399,
		409,419,430,440,451,462,473,485,496,508,520,532,544,556,569,582,594,608,621,634,648,662,
		676,690,704,719,734,749,764,779,795,811,827,843,859,876,893,910,927,944,962,980,998,1016,
		1034,1053,1072,1091,1110,1130,1150,1170,1190,1210,1231,1252,1273,1294,1316,1338,1360,1382,
		1404,1427,1450,1473,1497,1520,1544,1568,1593,1617,1642,1667,1693,1718,1744,1770,1797,1823,
		1850,1877,1905,1932,1960,1988,2017,2045,2074,2103,2133,2162,2192,2223,2253,2284,2315,2346,
		2378,2410,2442,2474,2507,2540,2573,2606,2640,2674,2708,2743,2778,2813,2849,2884,2920,2957,
		2993,3030,3067,3105,3143,3181,3219,3258,3297,3336,3376,3416,3456,3496,3537,3578,3619,3661,
		3703,3745,3788,3831,3874,3918,3962,4006,4010,4015};// Gamma table
 */

void IP_SHIFT_DELAY_Alt (void)
{
	uint8_t cnts=8;
	for(cnts=8;cnts>0;cnts--);
}

void SendDCData(int CMD, int UD, char GS_MODE, char DSTMR, char ADSRPM, char DC_Range_Blue, char DC_Range_Green, char DC_Range_Red, char BCB, char BCG, char BCR, char *DCdata)//Send DC/BC/FC data thru GS Data Path
{
	DC_data_process_flag=1;
	R_GPIO_WritePin(17, 7, 0);//0 blank low

	R_GPIO_WritePin(1, 4, 1);		/*DLED_LATCH=1 GSLAT-High for DC Mode*/


	int k,temp1,temp2;
	/*Command Code BITS 287-276*/
	for(k=287;k>=276;k--)
	{
		temp1=1<<(k-276);
		temp2=CMD&temp1;
		if(temp2==temp1)
		{
			R_GPIO_WritePin(1, 0, 1);		/*DLED_SDI=1;//GSSIN - High*/
		}
		else
		{
			R_GPIO_WritePin(1, 0, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
		}
		//DLED_SDI=0;//GSSIN - Low
		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		IP_SHIFT_DELAY_Alt();
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}
	/*No Value BITS 275-216*/
	R_GPIO_WritePin(1, 0, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
	for(k=275;k>=216;k--)
	{
		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		IP_SHIFT_DELAY_Alt();   // R_TICK_WaitUS(0u, 3u);
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}
	/*User Define BITS 215-199*/
	for(k=215;k>=199;k--)
	{
		temp1=1<<(k-199);
		temp2=UD&temp1;
		if(temp2==temp1)
		{
			R_GPIO_WritePin(1, 0, 1);		/*DLED_SDI=1;//GSSIN - High*/
		}
		else
		{
			R_GPIO_WritePin(1, 0, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
		}

		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		IP_SHIFT_DELAY_Alt();  // R_TICK_WaitUS(0u, 3u);
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}
	/*GS Counter Mode BITS 198-197*/
	for(k=198;k>=197;k--)
	{
		temp1=1<<(k-197);
		temp2=GS_MODE&temp1;
		if(temp2==temp1)
		{
			R_GPIO_WritePin(1, 0, 1);		/*DLED_SDI=1;//GSSIN - High*/
		}
		else
		{
			R_GPIO_WritePin(1, 0, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
		}
		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		IP_SHIFT_DELAY_Alt();    // R_TICK_WaitUS(0u, 3u);
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}

	/*Display Timing Reset Mode BITS 196*/
	if(DSTMR==1)
	{
		R_GPIO_WritePin(1, 0, 1);		/*DLED_SDI=1;//GSSIN - High*/
	}
	else
	{
		R_GPIO_WritePin(1, 0, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
	}
	R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
	IP_SHIFT_DELAY_Alt();  // R_TICK_WaitUS(0u, 3u);
	R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/

	/*Auto Display Repeat Mode BITS 195*/
	if(ADSRPM==1)
	{
		R_GPIO_WritePin(1, 0, 1);		/*DLED_SDI=1;//GSSIN - High*/
	}
	else
	{
		R_GPIO_WritePin(1, 0, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
	}
	R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
	IP_SHIFT_DELAY_Alt();  // R_TICK_WaitUS(0u, 3u);
	R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/

	/*Dot correction adjustment range for the BLUE color output BITS 194*/
	if(DC_Range_Blue==1)
	{
		R_GPIO_WritePin(1, 0, 1);		/*DLED_SDI=1;//GSSIN - High*/
	}
	else
	{
		R_GPIO_WritePin(1, 0, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
	}
	R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
	IP_SHIFT_DELAY_Alt(); // R_TICK_WaitUS(0u, 3u);
	R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/

	/*Dot correction adjustment range for the GREEN color output BITS 193*/
	if(DC_Range_Green==1)
	{
		R_GPIO_WritePin(1, 0, 1);		/*DLED_SDI=1;//GSSIN - High*/
	}
	else
	{
		R_GPIO_WritePin(1, 0, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
	}

	R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
	IP_SHIFT_DELAY_Alt();   // R_TICK_WaitUS(0u, 3u);
	R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/

	/*Dot correction adjustment range for the RED color output BITS 192*/
	if(DC_Range_Red==1)
	{
		R_GPIO_WritePin(1, 0, 1);		/*DLED_SDI=1;//GSSIN - High*/
	}
	else
	{
		R_GPIO_WritePin(1, 0, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
	}
	R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
	IP_SHIFT_DELAY_Alt();  // R_TICK_WaitUS(0u, 3u);
	R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/

	/*Global Brightness Control Blue BITS 191-184*/
	for(k=191;k>=184;k--)
	{
		temp1=1<<(k-184);
		temp2=BCB&temp1;
		if(temp2==temp1)
		{
			R_GPIO_WritePin(1, 0, 1);		/*DLED_SDI=1;//GSSIN - High*/
		}
		else
		{
			R_GPIO_WritePin(1, 0, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
		}
		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		IP_SHIFT_DELAY_Alt();   // R_TICK_WaitUS(0u, 3u);
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}

	/*Global Brightness Control Green BITS 183-176*/
	for(k=183;k>=176;k--)
	{
		temp1=1<<(k-176);
		temp2=BCG&temp1;
		if(temp2==temp1)
		{
			R_GPIO_WritePin(1, 0, 1);		/*DLED_SDI=1;//GSSIN - High*/
		}
		else
		{
			R_GPIO_WritePin(1, 0, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
		}
		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		IP_SHIFT_DELAY_Alt(); // R_TICK_WaitUS(0u, 3u);
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}

	/*Global Brightness Control Red BITS 175-168*/
	for(k=175;k>=168;k--)
	{
		temp1=1<<(k-168);
		temp2=BCR&temp1;
		if(temp2==temp1)
		{
			R_GPIO_WritePin(1, 0, 1);		/*DLED_SDI=1;//GSSIN - High*/
		}
		else
		{
			R_GPIO_WritePin(1, 0, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
		}
		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		IP_SHIFT_DELAY_Alt();   // R_TICK_WaitUS(0u, 3u);
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}

	/*Dot Correction Data BITS 167-0*/
	for(k=167;k>=0;k--)
	{
		temp1=1<<(k%7);
		temp2=DCdata[k/7]&temp1;
		if(temp2==temp1)
		{
			R_GPIO_WritePin(1, 0, 1);		/*DLED_SDI=1;//GSSIN - High*/
		}
		else
		{
			R_GPIO_WritePin(1, 0, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
		}
		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		IP_SHIFT_DELAY_Alt();   // R_TICK_WaitUS(0u, 3u);
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}

}

void SendGSData(int *temp)// Send Gray Scale data
{
	R_GPIO_WritePin(1, 4, 0); //GSLAT-Low for GS Mode
	int k,temp1,temp2;
	for(k=575;k>=0;k--)
	{
		temp1=1<<(k%12);
		temp2=temp[k/12]&temp1;
		if(temp2==temp1)
		{
			R_GPIO_WritePin(1, 0, 1);		/*DLED_SDI=1;//GSSIN - High*/
		}
		else
		{
			R_GPIO_WritePin(1, 0, 0);		 /*DLED_SDI=0;//GSSIN - Low*/
		}
		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		IP_SHIFT_DELAY_Alt();
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}



	R_GPIO_WritePin(17, 7, 0);//0 blank low

	R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
	IP_SHIFT_DELAY_Alt();
	R_GPIO_WritePin(1, 4, 1);		/*DLED_LATCH=1 GSLAT-High for DC Mode*/

	R_GPIO_WritePin(17, 7, 1);

	//IP_SHIFT_DELAY();
	IP_SHIFT_DELAY_Alt();
	R_GPIO_WritePin(1, 4, 0);		/*DLED_LATCH=0;  //GSLAT-Low*/
	//IP_SHIFT_DELAY();
	IP_SHIFT_DELAY_Alt();
	if(gcsk_flag == 0)
		gcsk_flag=1;
	GS_DATA_1SEC_COUNTER++;
	if(GS_DATA_1SEC_COUNTER >= 541)
	{
		gs_complete_test=1;
		GS_DATA_1SEC_COUNTER=0;
	}
	gs_complete=1;
	//IMMOBIIZER_TT=0;

}

void ReadDCData()
{
	_Bool l;
	int m;
	DC_data_process_flag=1;
	R_GPIO_WritePin(17, 7, 0);//0 blank low

	R_GPIO_WritePin(1, 4, 1);		/*DLED_LATCH=1 GSLAT-High for DC Mode*/



	for(m=575;m>=0;m--)
	{
		l=R_GPIO_PinRead(1,1);

		read_SID[m]= l;

		R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
		//IP_SHIFT_DELAY();
		IP_SHIFT_DELAY_Alt();
		R_GPIO_WritePin(1, 2, 1);        /*DLED_SCL=1;//GSSCK-High*/
	}

	R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
	//IP_SHIFT_DELAY();
	IP_SHIFT_DELAY_Alt();
	SendGSData(GSdata1);
	DC_data_process_flag=0;
	/*System Check Engine TT Failure*/
	if(read_SID[548]==1 || read_SID[452]==1)  /*Check engine telltale feedback*/
	{
		telltale_failure_DTC_Set(DTC_SYSTEM_CHECK_ENGINE_TT_STB_A002);		/*Set DTC*/
	}
	else
	{
		telltale_failure_DTC_Clear(DTC_SYSTEM_CHECK_ENGINE_TT_STB_A002);    /*Clear DTC*/
	}

	//MIL TT Failure
	if(read_SID[248]==1 ||read_SID[152]==1)
	{
		telltale_failure_DTC_Set(DTC_MIL_TT_STB_A003); /*Set DTC*/
	}
	else
	{
		telltale_failure_DTC_Clear(DTC_MIL_TT_STB_A003); /*Clear DTC*/
	}

	/*Engine Start Stop Failure telltale feedback*/
	if(read_SID[540]==1 || read_SID[444]==1)
	{
		telltale_failure_DTC_Set(DTC_ENG_ST_SP_TT_STB_A009); /*Set DTC*/
	}
	else
	{
		telltale_failure_DTC_Clear(DTC_ENG_ST_SP_TT_STB_A009); /*Clear DTC*/
	}

	/*ABS telltale feedback*/
	if(read_SID[550]==1 || read_SID[454]==1)
	{
		telltale_failure_DTC_Set(DTC_ABS_TT_STB_A00A); /*Set DTC*/
	}
	else
	{
		telltale_failure_DTC_Clear(DTC_ABS_TT_STB_A00A); /*Clear DTC*/
	}

	/*Cruise telltale feedback*/
	if(read_SID[241]==1 || read_SID[145]==1)
	{
		telltale_failure_DTC_Set(DTC_CRUISE_TT_STB_A00B); /*Set DTC*/
	}
	else
	{
		telltale_failure_DTC_Clear(DTC_CRUISE_TT_STB_A00B); /*Clear DTC*/
	}

	/*Airbag telltale feedback*/
	if(read_SID[533]==1 ||read_SID[437]==1)
	{
		telltale_failure_DTC_Set(DTC_AIRBAG_TT_STB_A00C); /*Set DTC*/
	}
	else
	{
		telltale_failure_DTC_Clear(DTC_AIRBAG_TT_STB_A00C); /*Clear DTC*/
	}

	/*PARK_BRAKE telltale feedback*/
	if(read_SID[258]==1 || read_SID[162]==1)
	{
		telltale_failure_DTC_Set(DTC_PARKBRAKE_TT_STB_A00D); /*Set DTC*/
	}
	else
	{
		telltale_failure_DTC_Clear(DTC_PARKBRAKE_TT_STB_A00D); /*Clear DTC*/
	}

	/*AT_FAIL telltale feedback*/
	if(read_SID[549]==1 || read_SID[453]==1)
	{
		telltale_failure_DTC_Set(DTC_ATFAIL_TT_STB_A010); /*Set DTC*/
	}
	else
	{
		telltale_failure_DTC_Clear(DTC_ATFAIL_TT_STB_A010); /*Clear DTC*/
	}

	/*ESP_MULFUNCTION telltale feedback*/
	if(read_SID[251]==1 || read_SID[155]==1)
	{
		telltale_failure_DTC_Set(DTC_ESP_TT_STB_A01A); /*Set DTC*/
	}
	else
	{
		telltale_failure_DTC_Clear(DTC_ESP_TT_STB_A01A); /*Clear DTC*/
	}

}


void DIAG_TT_init(void)
{
	diagTTflag=1;
	//	SendDCData(0x000, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1);

	SendGSData(GSdata1);

}

void cascade_TT_send_data(void)
{

	R_GPIO_WritePin(1, 2, 0);		/*DLED_SCL=0;//GSSCK-Low*/
	IP_SHIFT_DELAY_Alt();  // R_TICK_WaitUS(0u, 3u);
	R_GPIO_WritePin(1, 4, 0);		/*DLED_LATCH=0;  //GSLAT-Low*/
	IP_SHIFT_DELAY_Alt();  // R_TICK_WaitUS(0u, 3u);
	R_GPIO_WritePin(1, 4, 1);		/*DLED_LATCH=1 GSLAT-High for DC Mode*/
	IP_SHIFT_DELAY_Alt();  // R_TICK_WaitUS(0u, 3u);
	SendGSData(GSdata1);
	DC_data_process_flag=0;
}
#endif
/*SendDCData(double CMD, int UD, char GS_MODE, char DSTMR, char ADSRPM, char DC_Range_Blue, char DC_Range_Green, char DC_Range_Red, char BCB, char BCG, char BCR, char *DCdata)*/

void Illumination_steps(void)
{
	static uint8_t t;
	static union
	{
		struct{
			unsigned char  Rx_1:1;
			unsigned char  Rx_2:1;
			unsigned char  Rx_3:1;
			unsigned char  Rx_4:1;
		}bits;
		unsigned char byte;

	}four_bit_data;



	four_bit_data.bits.Rx_1 = CAN_IS1_100[3].bits.Rx_1;
	four_bit_data.bits.Rx_2 = CAN_IS1_100[3].bits.Rx_2;
	four_bit_data.bits.Rx_3 = CAN_IS1_100[3].bits.Rx_3;
	four_bit_data.bits.Rx_4 = CAN_IS1_100[3].bits.Rx_4;

	/*
	 * HIGH END -> 150 (base value)
	 * LOW END -> 10(base value)
	 * Keep base value as common factor and Calculate Illumination
	 * 10% of HIGH END = (150+10) in decimal
	 * 15% of LOW END = (10+15) in decimal
	 * Only for Gauge, C ring and Pointer Illumination */

	if(PARK_LAMP_TT==0 && IGN_ON_flag==SET && gs_complete==1)				//condition c
	{

		/* Condition c			Gauge,C-ring,Pointer		TFT
			Colour TFT ->				90					90
			Gray Scale ->				85					75
		 * */

#ifdef GRAYSCALE_TFT
		//t=0x65; 	/* 90% */
		t=0x5F;		/*85%*/
#else
		t=0xF0;	/*90%*/
#endif

		DCdata1[0]=t;
		DCdata1[1]=t;
		DCdata1[2]=t;
		DCdata1[3]=t;
		DCdata1[4]=t;
		DCdata1[5]=t;
		DCdata1[6]=t;
		DCdata1[7]=t;
		DCdata1[8]=t;

		DCdata2[15]=t;
		DCdata2[16]=t;
		DCdata2[18]=t;
		DCdata2[19]=t;
		DCdata2[20]=t;
		DCdata2[21]=t;
		DCdata2[22]=t;
		DCdata2[23]=t;
		DCdata2[12]=t;
		DCdata2[13]=t;
		DCdata2[17]=t;
		if((tst_tt_flag==0)&&(Digital_Speedo_Guage_off==0))
		{
			tst_tt_flag=1;
			SendDCData(0x000, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1);
			SendDCData(0x000, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata2);

			cascade_TT_send_data();
			SP_LD1_TT=0xfff;
			SP_LD2_TT=0xfff;
			SP_LD3_TT=0xfff;
			RP_LD1_TT=0xfff;
			RP_LD2_TT=0xfff;
			RP_LD3_TT=0xfff;

			S_LD1_TT=0xfff;
			S_LD2_TT=0xfff;
			S_LD3_TT=0xfff;
			S_LD4_TT=0xfff;
			S_LD5_TT=0xfff;
			S_LD6_TT=0xfff;
			R_LD1_TT=0xfff;
			R_LD2_TT=0xfff;
			R_LD3_TT=0xfff;
			R_LD4_TT=0xfff;
			R_LD5_TT=0xfff;
			R_LD6_TT=0xfff;
			if(FEATURE_CODE_FUEL_TYPE == 1)
			{
				/* Diesel*/
				R_LD8_TT=0xfff;
				R_LD7_TT=0x000;

			}
			else
			{
				/* GAsoline*/
				R_LD8_TT=0;
				R_LD7_TT=0xfff;
			}
			SendGSData(GSdata1);

		}
		gs_complete=0;

		tst_tt_flag_1=0;
		tst_tt_flag_2=0;
		tst_tt_flag_3=0;
		tst_tt_flag_4=0;
		tst_tt_flag_5=0;
		tst_tt_flag_6=0;
		tst_tt_flag_7=0;
		tst_tt_flag_8=0;
		tst_tt_flag_9=0;
		tst_tt_flag_10 = 0;



#ifdef GRAYSCALE_TFT
			Illumn_percent = 75;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//75% TFT_ILLUMINATION
#else
			Illumn_percent = 90;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//90% TFT_ILLUMINATION
#endif




		r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);


		currIllumination = 0;	/* To be Stored in EEPROM when IGN is OFF */
		firstTimeIn = 1;	/* Setting One Time In Flag */


		DID_ILLUMINATION_STATUS_0xF084_arr[0] = Illumn_percent;
		DID_ILLUMINATION_STATUS_0xF084_arr[1] = Illumn_percent;
		DID_ILLUMINATION_STATUS_0xF084_arr[2] = Illumn_percent;
	}
	else if(PARK_LAMP_TT==1 && IGN_ON_flag == SET && gs_complete==1)
	{

		if((FEATURE_CODE_IS_ILLUMINATION_CONFIG==0)
				||((FEATURE_CODE_IS_ILLUMINATION_CONFIG==1) &&(VARIANT_CODING_INFOTAINMENT_SYSTEM==1)&&(IS1_100_message_timeout_flag != 0))
				||((FEATURE_CODE_IS_ILLUMINATION_CONFIG==1) &&(VARIANT_CODING_INFOTAINMENT_SYSTEM==0))
				||((FEATURE_CODE_IS_ILLUMINATION_CONFIG==1) &&(VARIANT_CODING_INFOTAINMENT_SYSTEM==1)&&(four_bit_data.byte == 0)&&(IS1_100_message_timeout_flag ==0))
				/*|| ((FEATURE_CODE_IS_ILLUMINATION_CONFIG==1) &&(VARIANT_CODING_INFOTAINMENT_SYSTEM==1)&&(four_bit_data.byte == 1)&&(IS1_100_message_timeout_flag ==0))*/
				||((FEATURE_CODE_IS_ILLUMINATION_CONFIG==1) &&(VARIANT_CODING_INFOTAINMENT_SYSTEM==1)&&(four_bit_data.byte ==0xF)&&(IS1_100_message_timeout_flag ==0)))
			/* Condition d,e,f,g,p
			 						Gauge,C-ring,Pointer		TFT
				Colour TFT ->				26					24
				Gray Scale ->				30					20
			 * */


		{
#ifdef GRAYSCALE_TFT
		//	t=0x0A;		/* 7%*/
			t=0x28;		/*30%*/
#else
			t=0XB0;	/*26*/
#endif
			DCdata1[0]=t;
			DCdata1[1]=t;
			DCdata1[2]=t;
			DCdata1[3]=t;
			DCdata1[4]=t;
			DCdata1[5]=t;
			DCdata1[6]=t;
			DCdata1[7]=t;
			DCdata1[8]=t;

			DCdata2[15]=t;
			DCdata2[16]=t;
			DCdata2[18]=t;
			DCdata2[19]=t;
			DCdata2[20]=t;
			DCdata2[21]=t;
			DCdata2[22]=t;
			DCdata2[23]=t;
			DCdata2[12]=t;
			DCdata2[13]=t;
			DCdata2[17]=t;

			if((tst_tt_flag_1==0)&&(Digital_Speedo_Guage_off==0))
			{
				tst_tt_flag_1=1;
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata1);
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata2);

				cascade_TT_send_data();


				SP_LD1_TT=0xfff;
				SP_LD2_TT=0xfff;
				SP_LD3_TT=0xfff;
				RP_LD1_TT=0xfff;
				RP_LD2_TT=0xfff;
				RP_LD3_TT=0xfff;

				S_LD1_TT=0xfff;
				S_LD2_TT=0xfff;
				S_LD3_TT=0xfff;
				S_LD4_TT=0xfff;
				S_LD5_TT=0xfff;
				S_LD6_TT=0xfff;
				R_LD1_TT=0xfff;
				R_LD2_TT=0xfff;
				R_LD3_TT=0xfff;
				R_LD4_TT=0xfff;
				R_LD5_TT=0xfff;
				R_LD6_TT=0xfff;
				if(FEATURE_CODE_FUEL_TYPE == 1)
				{
					/* Diesel*/
					R_LD8_TT=0xfff;
					R_LD7_TT=0x000;

				}
				else
				{
					/* GAsoline*/
					R_LD8_TT=0;
					R_LD7_TT=0xfff;
				}
				SendGSData(GSdata1);


			}
			gs_complete=0;

			tst_tt_flag=0;
			tst_tt_flag_2=0;
			tst_tt_flag_3=0;
			tst_tt_flag_4=0;
			tst_tt_flag_5=0;
			tst_tt_flag_6=0;
			tst_tt_flag_7=0;
			tst_tt_flag_8=0;
			tst_tt_flag_9=0;
			tst_tt_flag_10 = 0;

#ifdef GRAYSCALE_TFT
			Illumn_percent = 20;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,21/1);			//20%TFT_ILLUMINATION

#else
			Illumn_percent = 24;								//24%TFT_ILLUMINATION
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));

#endif

			r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);
			currIllumination = 1;	/* To be Stored in EEPROM when IGN is OFF */
			firstTimeIn = 1;	/* Setting One Time In Flag */


			DID_ILLUMINATION_STATUS_0xF084_arr[0] = Illumn_percent;
			DID_ILLUMINATION_STATUS_0xF084_arr[1] = 5;
			DID_ILLUMINATION_STATUS_0xF084_arr[2] = 5;
		}

		else if((FEATURE_CODE_IS_ILLUMINATION_CONFIG==1) &&(VARIANT_CODING_INFOTAINMENT_SYSTEM==1)&&(four_bit_data.byte == 2)&&(IS1_100_message_timeout_flag == 0))
		{
			/* Condition i
			 						Gauge,C-ring,Pointer		TFT
				Colour TFT ->				40					33
				Gray Scale ->				40					30
			 * */

			//t=0x0E;
#ifdef GRAYSCALE_TFT
			t=0x32;		/* 40 % */
#else
			t=0XBE;
#endif
			DCdata1[0]=t;
			DCdata1[1]=t;
			DCdata1[2]=t;
			DCdata1[3]=t;
			DCdata1[4]=t;
			DCdata1[5]=t;
			DCdata1[6]=t;
			DCdata1[7]=t;
			DCdata1[8]=t;

			DCdata2[15]=t;
			DCdata2[16]=t;
			DCdata2[18]=t;
			DCdata2[19]=t;
			DCdata2[20]=t;
			DCdata2[21]=t;
			DCdata2[22]=t;
			DCdata2[23]=t;
			DCdata2[12]=t;
			DCdata2[13]=t;
			DCdata2[17]=t;
			if((tst_tt_flag_2==0)&&(Digital_Speedo_Guage_off==0))
			{
				tst_tt_flag_2=1;
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata1);
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata2);

				cascade_TT_send_data();

				SP_LD1_TT=0xfff;
				SP_LD2_TT=0xfff;
				SP_LD3_TT=0xfff;
				RP_LD1_TT=0xfff;
				RP_LD2_TT=0xfff;
				RP_LD3_TT=0xfff;

				S_LD1_TT=0xfff;
				S_LD2_TT=0xfff;
				S_LD3_TT=0xfff;
				S_LD4_TT=0xfff;
				S_LD5_TT=0xfff;
				S_LD6_TT=0xfff;
				R_LD1_TT=0xfff;
				R_LD2_TT=0xfff;
				R_LD3_TT=0xfff;
				R_LD4_TT=0xfff;
				R_LD5_TT=0xfff;
				R_LD6_TT=0xfff;
				if(FEATURE_CODE_FUEL_TYPE == 1)
				{
					/* Diesel*/
					R_LD8_TT=0xfff;
					R_LD7_TT=0x000;

				}
				else
				{
					/* GAsoline*/
					R_LD8_TT=0;
					R_LD7_TT=0xfff;
				}
				SendGSData(GSdata1);

			}
			gs_complete=0;
			tst_tt_flag=0;
			tst_tt_flag_1=0;
			tst_tt_flag_3=0;
			tst_tt_flag_4=0;
			tst_tt_flag_5=0;
			tst_tt_flag_6=0;
			tst_tt_flag_7=0;
			tst_tt_flag_8=0;
			tst_tt_flag_9=0;
			tst_tt_flag_10 = 0;

#ifdef GRAYSCALE_TFT
			Illumn_percent = 30;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//30% TFT_ILLUMINATION
#else
			Illumn_percent = 33;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//33% TFT_ILLUMINATION
#endif
			r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);

			currIllumination = 2;	/* To be Stored in EEPROM when IGN is OFF */
			firstTimeIn = 1;	/* Setting One Time In Flag */

			DID_ILLUMINATION_STATUS_0xF084_arr[0] = Illumn_percent;
			DID_ILLUMINATION_STATUS_0xF084_arr[1] = 7;
			DID_ILLUMINATION_STATUS_0xF084_arr[2] = 7;
		}

		else if((FEATURE_CODE_IS_ILLUMINATION_CONFIG==1) &&(VARIANT_CODING_INFOTAINMENT_SYSTEM==1)&&(four_bit_data.byte == 3)&&(IS1_100_message_timeout_flag == 0))
		{
			/* Condition j
			 						Gauge,C-ring,Pointer		TFT
				Colour TFT ->				47					38
				Gray Scale ->				45					35
			 * */

		//	t=0x12;
#ifdef GRAYSCALE_TFT
			t=0x37;	/* 45% */
#else
			t=0XC5;
#endif
			DCdata1[0]=t;
			DCdata1[1]=t;
			DCdata1[2]=t;
			DCdata1[3]=t;
			DCdata1[4]=t;
			DCdata1[5]=t;
			DCdata1[6]=t;
			DCdata1[7]=t;
			DCdata1[8]=t;

			DCdata2[15]=t;
			DCdata2[16]=t;
			DCdata2[18]=t;
			DCdata2[19]=t;
			DCdata2[20]=t;
			DCdata2[21]=t;
			DCdata2[22]=t;
			DCdata2[23]=t;
			DCdata2[12]=t;
			DCdata2[13]=t;
			DCdata2[17]=t;
			if((tst_tt_flag_3==0)&&(Digital_Speedo_Guage_off==0))
			{
				tst_tt_flag_3=1;
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata1);
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata2);

				cascade_TT_send_data();

				SP_LD1_TT=0xfff;
				SP_LD2_TT=0xfff;
				SP_LD3_TT=0xfff;
				RP_LD1_TT=0xfff;
				RP_LD2_TT=0xfff;
				RP_LD3_TT=0xfff;

				S_LD1_TT=0xfff;
				S_LD2_TT=0xfff;
				S_LD3_TT=0xfff;
				S_LD4_TT=0xfff;
				S_LD5_TT=0xfff;
				S_LD6_TT=0xfff;
				R_LD1_TT=0xfff;
				R_LD2_TT=0xfff;
				R_LD3_TT=0xfff;
				R_LD4_TT=0xfff;
				R_LD5_TT=0xfff;
				R_LD6_TT=0xfff;
				if(FEATURE_CODE_FUEL_TYPE == 1)
				{
					/* Diesel*/
					R_LD8_TT=0xfff;
					R_LD7_TT=0x000;

				}
				else
				{
					/* GAsoline*/
					R_LD8_TT=0;
					R_LD7_TT=0xfff;
				}
				SendGSData(GSdata1);


			}
			gs_complete=0;
			tst_tt_flag=0;
			tst_tt_flag_1=0;
			tst_tt_flag_2=0;
			tst_tt_flag_4=0;
			tst_tt_flag_5=0;
			tst_tt_flag_6=0;
			tst_tt_flag_7=0;
			tst_tt_flag_8=0;
			tst_tt_flag_9=0;
			tst_tt_flag_10 = 0;

#ifdef GRAYSCALE_TFT
			Illumn_percent = 35;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//35% TFT_ILLUMINATION
#else
			Illumn_percent = 38;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//38% TFT_ILLUMINATION
#endif
			r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);

			currIllumination = 3;	/* To be Stored in EEPROM when IGN is OFF */
			firstTimeIn = 1;	/* Setting One Time In Flag */


			DID_ILLUMINATION_STATUS_0xF084_arr[0] = Illumn_percent;
			DID_ILLUMINATION_STATUS_0xF084_arr[1] = 9;
			DID_ILLUMINATION_STATUS_0xF084_arr[2] = 9;
		}
		else if((FEATURE_CODE_IS_ILLUMINATION_CONFIG==1) &&(VARIANT_CODING_INFOTAINMENT_SYSTEM==1)&&(four_bit_data.byte == 4)&&(IS1_100_message_timeout_flag == 0))
		{

			/* Condition k
			 						Gauge,C-ring,Pointer		TFT
				Colour TFT ->				54					43
				Gray Scale ->				50					40
			 * */
			//t=0x16;
#ifdef GRAYSCALE_TFT
			t=0x3C;
#else
			t=0XCC;
#endif
			DCdata1[0]=t;
			DCdata1[1]=t;
			DCdata1[2]=t;
			DCdata1[3]=t;
			DCdata1[4]=t;
			DCdata1[5]=t;
			DCdata1[6]=t;
			DCdata1[7]=t;
			DCdata1[8]=t;

			DCdata2[15]=t;
			DCdata2[16]=t;
			DCdata2[18]=t;
			DCdata2[19]=t;
			DCdata2[20]=t;
			DCdata2[21]=t;
			DCdata2[22]=t;
			DCdata2[23]=t;
			DCdata2[12]=t;
			DCdata2[13]=t;
			DCdata2[17]=t;
			if((tst_tt_flag_4==0)&&(Digital_Speedo_Guage_off==0))
			{
				tst_tt_flag_4=1;
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata1);
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata2);

				cascade_TT_send_data();
				SP_LD1_TT=0xfff;
				SP_LD2_TT=0xfff;
				SP_LD3_TT=0xfff;
				RP_LD1_TT=0xfff;
				RP_LD2_TT=0xfff;
				RP_LD3_TT=0xfff;

				S_LD1_TT=0xfff;
				S_LD2_TT=0xfff;
				S_LD3_TT=0xfff;
				S_LD4_TT=0xfff;
				S_LD5_TT=0xfff;
				S_LD6_TT=0xfff;
				R_LD1_TT=0xfff;
				R_LD2_TT=0xfff;
				R_LD3_TT=0xfff;
				R_LD4_TT=0xfff;
				R_LD5_TT=0xfff;
				R_LD6_TT=0xfff;
				if(FEATURE_CODE_FUEL_TYPE == 1)
				{
					/* Diesel*/
					R_LD8_TT=0xfff;
					R_LD7_TT=0x000;

				}
				else
				{
					/* GAsoline*/
					R_LD8_TT=0;
					R_LD7_TT=0xfff;
				}
				SendGSData(GSdata1);

			}
			gs_complete=0;
			tst_tt_flag=0;
			tst_tt_flag_1=0;
			tst_tt_flag_2=0;
			tst_tt_flag_3=1;
			tst_tt_flag_5=0;
			tst_tt_flag_6=0;
			tst_tt_flag_7=0;
			tst_tt_flag_8=0;
			tst_tt_flag_9=0;
			tst_tt_flag_10 = 0;

#ifdef GRAYSCALE_TFT
			Illumn_percent = 40;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//40% TFT_ILLUMINATION
#else
			Illumn_percent = 43;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//43% TFT_ILLUMINATION
#endif

			r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);
			currIllumination = 4;	/* To be Stored in EEPROM when IGN is OFF */
			firstTimeIn = 1;	/* Setting One Time In Flag */


			DID_ILLUMINATION_STATUS_0xF084_arr[0] = Illumn_percent;
			DID_ILLUMINATION_STATUS_0xF084_arr[1] = 11;
			DID_ILLUMINATION_STATUS_0xF084_arr[2] = 11;
		}
		else if((FEATURE_CODE_IS_ILLUMINATION_CONFIG==1) &&(VARIANT_CODING_INFOTAINMENT_SYSTEM==1)&&(four_bit_data.byte == 5)&&(IS1_100_message_timeout_flag == 0))
		{

			/* Condition l
			 						Gauge,C-ring,Pointer		TFT
				Colour TFT ->				61					48
				Gray Scale ->				55					45
			 * */
			//t=0x1A;
			#ifdef GRAYSCALE_TFT
						t=0x41;
			#else
						t=0XD3;
			#endif
			DCdata1[0]=t;
			DCdata1[1]=t;
			DCdata1[2]=t;
			DCdata1[3]=t;
			DCdata1[4]=t;
			DCdata1[5]=t;
			DCdata1[6]=t;
			DCdata1[7]=t;
			DCdata1[8]=t;

			DCdata2[15]=t;
			DCdata2[16]=t;
			DCdata2[18]=t;
			DCdata2[19]=t;
			DCdata2[20]=t;
			DCdata2[21]=t;
			DCdata2[22]=t;
			DCdata2[23]=t;
			DCdata2[12]=t;
			DCdata2[13]=t;
			DCdata2[17]=t;
			if((tst_tt_flag_5==0)&&(Digital_Speedo_Guage_off==0))
			{
				tst_tt_flag_5=1;
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata1);
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata2);

				cascade_TT_send_data();

				SP_LD1_TT=0xfff;
				SP_LD2_TT=0xfff;
				SP_LD3_TT=0xfff;
				RP_LD1_TT=0xfff;
				RP_LD2_TT=0xfff;
				RP_LD3_TT=0xfff;

				S_LD1_TT=0xfff;
				S_LD2_TT=0xfff;
				S_LD3_TT=0xfff;
				S_LD4_TT=0xfff;
				S_LD5_TT=0xfff;
				S_LD6_TT=0xfff;
				R_LD1_TT=0xfff;
				R_LD2_TT=0xfff;
				R_LD3_TT=0xfff;
				R_LD4_TT=0xfff;
				R_LD5_TT=0xfff;
				R_LD6_TT=0xfff;
				if(FEATURE_CODE_FUEL_TYPE == 1)
				{
					/* Diesel*/
					R_LD8_TT=0xfff;
					R_LD7_TT=0x000;

				}
				else
				{
					/* GAsoline*/
					R_LD8_TT=0;
					R_LD7_TT=0xfff;
				}
				SendGSData(GSdata1);

			}
			gs_complete=0;
			tst_tt_flag=0;
			tst_tt_flag_1=0;
			tst_tt_flag_2=0;
			tst_tt_flag_3=0;
			tst_tt_flag_4=0;
			tst_tt_flag_6=0;
			tst_tt_flag_7=0;
			tst_tt_flag_8=0;
			tst_tt_flag_9=0;
			tst_tt_flag_10 = 0;

#ifdef GRAYSCALE_TFT
			Illumn_percent = 45;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//45% TFT_ILLUMINATION
#else
			Illumn_percent = 48;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//30% TFT_ILLUMINATION
#endif

			r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);

			currIllumination = 5;	/* To be Stored in EEPROM when IGN is OFF */
			firstTimeIn = 1;	/* Setting One Time In Flag */


			DID_ILLUMINATION_STATUS_0xF084_arr[0] = Illumn_percent;
			DID_ILLUMINATION_STATUS_0xF084_arr[1] = 13;
			DID_ILLUMINATION_STATUS_0xF084_arr[2] = 13;

		}
		else if((FEATURE_CODE_IS_ILLUMINATION_CONFIG==1) &&(VARIANT_CODING_INFOTAINMENT_SYSTEM==1)&&(four_bit_data.byte == 6)&&(IS1_100_message_timeout_flag == 0))
		{

			/* Condition m
			 						Gauge,C-ring,Pointer		TFT
				Colour TFT ->				68					53
				Gray Scale ->				60					50
			 * */
			//t=0x1E;
			#ifdef GRAYSCALE_TFT
						t=0x46;		/* 60% */
			#else
						t=0XDA;
			#endif
			DCdata1[0]=t;
			DCdata1[1]=t;
			DCdata1[2]=t;
			DCdata1[3]=t;
			DCdata1[4]=t;
			DCdata1[5]=t;
			DCdata1[6]=t;
			DCdata1[7]=t;
			DCdata1[8]=t;

			DCdata2[15]=t;
			DCdata2[16]=t;
			DCdata2[18]=t;
			DCdata2[19]=t;
			DCdata2[20]=t;
			DCdata2[21]=t;
			DCdata2[22]=t;
			DCdata2[23]=t;
			DCdata2[12]=t;
			DCdata2[13]=t;
			DCdata2[17]=t;
			if((tst_tt_flag_6==0)&&(Digital_Speedo_Guage_off==0))
			{
				tst_tt_flag_6=1;
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata1);
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata2);

				cascade_TT_send_data();
				SP_LD1_TT=0xfff;
				SP_LD2_TT=0xfff;
				SP_LD3_TT=0xfff;
				RP_LD1_TT=0xfff;
				RP_LD2_TT=0xfff;
				RP_LD3_TT=0xfff;

				S_LD1_TT=0xfff;
				S_LD2_TT=0xfff;
				S_LD3_TT=0xfff;
				S_LD4_TT=0xfff;
				S_LD5_TT=0xfff;
				S_LD6_TT=0xfff;
				R_LD1_TT=0xfff;
				R_LD2_TT=0xfff;
				R_LD3_TT=0xfff;
				R_LD4_TT=0xfff;
				R_LD5_TT=0xfff;
				R_LD6_TT=0xfff;
				if(FEATURE_CODE_FUEL_TYPE == 1)
				{
					/* Diesel*/
					R_LD8_TT=0xfff;
					R_LD7_TT=0x000;

				}
				else
				{
					/* GAsoline*/
					R_LD8_TT=0;
					R_LD7_TT=0xfff;
				}
				SendGSData(GSdata1);

			}
			gs_complete=0;
			tst_tt_flag=0;
			tst_tt_flag_1=0;
			tst_tt_flag_2=0;
			tst_tt_flag_3=0;
			tst_tt_flag_4=0;
			tst_tt_flag_5=1;
			tst_tt_flag_7=0;
			tst_tt_flag_8=0;
			tst_tt_flag_9=0;
			tst_tt_flag_10 = 0;

#ifdef GRAYSCALE_TFT
			Illumn_percent = 50;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//50% TFT_ILLUMINATION
#else
			Illumn_percent = 53;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//53% TFT_ILLUMINATION
#endif

			r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);

			currIllumination = 6;	/* To be Stored in EEPROM when IGN is OFF */
			firstTimeIn = 1;	/* Setting One Time In Flag */


			DID_ILLUMINATION_STATUS_0xF084_arr[0] = Illumn_percent;
			DID_ILLUMINATION_STATUS_0xF084_arr[1] = 15;
			DID_ILLUMINATION_STATUS_0xF084_arr[2] = 15;
		}
		else if((FEATURE_CODE_IS_ILLUMINATION_CONFIG==1) &&(VARIANT_CODING_INFOTAINMENT_SYSTEM==1)&&(four_bit_data.byte == 7)&&(IS1_100_message_timeout_flag == 0))
		{

			/* Condition n
			 						Gauge,C-ring,Pointer		TFT
				Colour TFT ->				75					58
				Gray Scale ->				65					55
			 * */
			//t=0x22;
			#ifdef GRAYSCALE_TFT
						t=0x4B;
			#else
						t=0XE1;									/* 34% Gauge*/
			#endif
			DCdata1[0]=t;
			DCdata1[1]=t;
			DCdata1[2]=t;
			DCdata1[3]=t;
			DCdata1[4]=t;
			DCdata1[5]=t;
			DCdata1[6]=t;
			DCdata1[7]=t;
			DCdata1[8]=t;

			DCdata2[15]=t;
			DCdata2[16]=t;
			DCdata2[18]=t;
			DCdata2[19]=t;
			DCdata2[20]=t;
			DCdata2[21]=t;
			DCdata2[22]=t;
			DCdata2[23]=t;
			DCdata2[12]=t;
			DCdata2[13]=t;
			DCdata2[17]=t;
			if((tst_tt_flag_7==0)&&(Digital_Speedo_Guage_off==0))
			{
				tst_tt_flag_7=1;
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata1);
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata2);

				cascade_TT_send_data();
				SP_LD1_TT=0xfff;
				SP_LD2_TT=0xfff;
				SP_LD3_TT=0xfff;
				RP_LD1_TT=0xfff;
				RP_LD2_TT=0xfff;
				RP_LD3_TT=0xfff;

				S_LD1_TT=0xfff;
				S_LD2_TT=0xfff;
				S_LD3_TT=0xfff;
				S_LD4_TT=0xfff;
				S_LD5_TT=0xfff;
				S_LD6_TT=0xfff;
				R_LD1_TT=0xfff;
				R_LD2_TT=0xfff;
				R_LD3_TT=0xfff;
				R_LD4_TT=0xfff;
				R_LD5_TT=0xfff;
				R_LD6_TT=0xfff;
				if(FEATURE_CODE_FUEL_TYPE == 1)
				{
					/* Diesel*/
					R_LD8_TT=0xfff;
					R_LD7_TT=0x000;

				}
				else
				{
					/* GAsoline*/
					R_LD8_TT=0;
					R_LD7_TT=0xfff;
				}
				SendGSData(GSdata1);

			}
			gs_complete=0;
			tst_tt_flag=0;
			tst_tt_flag_1=0;
			tst_tt_flag_2=0;
			tst_tt_flag_3=0;
			tst_tt_flag_4=0;
			tst_tt_flag_5=0;
			tst_tt_flag_6=0;
			tst_tt_flag_8=0;
			tst_tt_flag_9=0;
			tst_tt_flag_10 = 0;

#ifdef GRAYSCALE_TFT
			Illumn_percent = 55;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//55% TFT_ILLUMINATION
#else
			Illumn_percent = 58;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//58% TFT_ILLUMINATION
#endif
			r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);
			currIllumination = 7;	/* To be Stored in EEPROM when IGN is OFF */
			firstTimeIn = 1;	/* Setting One Time In Flag */



			DID_ILLUMINATION_STATUS_0xF084_arr[0] = Illumn_percent;
			DID_ILLUMINATION_STATUS_0xF084_arr[1] = 17;
			DID_ILLUMINATION_STATUS_0xF084_arr[2] = 17;
		}
		else if((FEATURE_CODE_IS_ILLUMINATION_CONFIG==1) &&(VARIANT_CODING_INFOTAINMENT_SYSTEM==1)&&(four_bit_data.byte == 8)&&(IS1_100_message_timeout_flag == 0))
		{

			/* Condition o
			 						Gauge,C-ring,Pointer		TFT
				Colour TFT ->				82					63
				Gray Scale ->				70					60
			 * */
		//	t=0x26;
				#ifdef GRAYSCALE_TFT
							t=0x50;	/*70%*/
				#else
							t=0XE8;
				#endif
			DCdata1[0]=t;
			DCdata1[1]=t;
			DCdata1[2]=t;
			DCdata1[3]=t;
			DCdata1[4]=t;
			DCdata1[5]=t;
			DCdata1[6]=t;
			DCdata1[7]=t;
			DCdata1[8]=t;

			DCdata2[15]=t;
			DCdata2[16]=t;
			DCdata2[18]=t;
			DCdata2[19]=t;
			DCdata2[20]=t;
			DCdata2[21]=t;
			DCdata2[22]=t;
			DCdata2[23]=t;
			DCdata2[12]=t;
			DCdata2[13]=t;
			DCdata2[17]=t;
			if((tst_tt_flag_8==0)&&(Digital_Speedo_Guage_off==0))
			{
				tst_tt_flag_8=1;
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata1);
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata2);

				cascade_TT_send_data();
				SP_LD1_TT=0xfff;
				SP_LD2_TT=0xfff;
				SP_LD3_TT=0xfff;
				RP_LD1_TT=0xfff;
				RP_LD2_TT=0xfff;
				RP_LD3_TT=0xfff;

				S_LD1_TT=0xfff;
				S_LD2_TT=0xfff;
				S_LD3_TT=0xfff;
				S_LD4_TT=0xfff;
				S_LD5_TT=0xfff;
				S_LD6_TT=0xfff;
				R_LD1_TT=0xfff;
				R_LD2_TT=0xfff;
				R_LD3_TT=0xfff;
				R_LD4_TT=0xfff;
				R_LD5_TT=0xfff;
				R_LD6_TT=0xfff;
				if(FEATURE_CODE_FUEL_TYPE == 1)
				{
					/* Diesel*/
					R_LD8_TT=0xfff;
					R_LD7_TT=0x000;

				}
				else
				{
					/* GAsoline*/
					R_LD8_TT=0;
					R_LD7_TT=0xfff;
				}
				SendGSData(GSdata1);

			}
			gs_complete=0;
			tst_tt_flag=0;
			tst_tt_flag_1=0;
			tst_tt_flag_2=0;
			tst_tt_flag_3=0;
			tst_tt_flag_4=0;
			tst_tt_flag_5=0;
			tst_tt_flag_6=0;
			tst_tt_flag_7=0;
			tst_tt_flag_9=0;
			tst_tt_flag_10 = 0;

#ifdef GRAYSCALE_TFT
			Illumn_percent = 60;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//60% TFT_ILLUMINATION
#else
			Illumn_percent = 63;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//63% TFT_ILLUMINATION
#endif
			r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);

			currIllumination = 8;	/* To be Stored in EEPROM when IGN is OFF */
			firstTimeIn = 1;	/* Setting One Time In Flag */


			DID_ILLUMINATION_STATUS_0xF084_arr[0] = Illumn_percent;
			DID_ILLUMINATION_STATUS_0xF084_arr[1] = 19;
			DID_ILLUMINATION_STATUS_0xF084_arr[2] = 19;

		}
		else if((FEATURE_CODE_IS_ILLUMINATION_CONFIG==1) &&(VARIANT_CODING_INFOTAINMENT_SYSTEM==1)&&(four_bit_data.byte == 1)&&(IS1_100_message_timeout_flag == 0))
		{

			/* Condition h
			 						Gauge,C-ring,Pointer		TFT
				Colour TFT ->				33					29
				Gray Scale ->				35					25
			 * */
				#ifdef GRAYSCALE_TFT
							t=0x2D;		/*35%*/
				#else
							t=0XB7;
				#endif
			DCdata1[0]=t;
			DCdata1[1]=t;
			DCdata1[2]=t;
			DCdata1[3]=t;
			DCdata1[4]=t;
			DCdata1[5]=t;
			DCdata1[6]=t;
			DCdata1[7]=t;
			DCdata1[8]=t;

			DCdata2[15]=t;
			DCdata2[16]=t;
			DCdata2[18]=t;
			DCdata2[19]=t;
			DCdata2[20]=t;
			DCdata2[21]=t;
			DCdata2[22]=t;
			DCdata2[23]=t;
			DCdata2[12]=t;
			DCdata2[13]=t;
			DCdata2[17]=t;
			if((tst_tt_flag_9==0)&&(Digital_Speedo_Guage_off==0))
			{
				tst_tt_flag_9=1;
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata1);
				SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata2);

				cascade_TT_send_data();
				SP_LD1_TT=0xfff;
				SP_LD2_TT=0xfff;
				SP_LD3_TT=0xfff;
				RP_LD1_TT=0xfff;
				RP_LD2_TT=0xfff;
				RP_LD3_TT=0xfff;

				S_LD1_TT=0xfff;
				S_LD2_TT=0xfff;
				S_LD3_TT=0xfff;
				S_LD4_TT=0xfff;
				S_LD5_TT=0xfff;
				S_LD6_TT=0xfff;
				R_LD1_TT=0xfff;
				R_LD2_TT=0xfff;
				R_LD3_TT=0xfff;
				R_LD4_TT=0xfff;
				R_LD5_TT=0xfff;
				R_LD6_TT=0xfff;
				if(FEATURE_CODE_FUEL_TYPE == 1)
				{
					/* Diesel*/
					R_LD8_TT=0xfff;
					R_LD7_TT=0x000;

				}
				else
				{
					/* GAsoline*/
					R_LD8_TT=0;
					R_LD7_TT=0xfff;
				}
				SendGSData(GSdata1);

			}
			gs_complete=0;
			tst_tt_flag=0;
			tst_tt_flag_1=0;
			tst_tt_flag_2=0;
			tst_tt_flag_3=0;
			tst_tt_flag_4=0;
			tst_tt_flag_5=0;
			tst_tt_flag_6=0;
			tst_tt_flag_7=0;
			tst_tt_flag_8=0;
			tst_tt_flag_10 = 0;

#ifdef GRAYSCALE_TFT
			Illumn_percent = 25;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//20% TFT_ILLUMINATION
#else
			Illumn_percent = 29;
			r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//29% TFT_ILLUMINATION
#endif
			r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);

			currIllumination = 9;	/* To be Stored in EEPROM when IGN is OFF */
			firstTimeIn = 1;	/* Setting One Time In Flag */


			DID_ILLUMINATION_STATUS_0xF084_arr[0] = Illumn_percent;
			DID_ILLUMINATION_STATUS_0xF084_arr[1] = 19;
			DID_ILLUMINATION_STATUS_0xF084_arr[2] = 19;

		}
		else if((FEATURE_CODE_IS_ILLUMINATION_CONFIG==1) &&(VARIANT_CODING_INFOTAINMENT_SYSTEM==1)&&(IS1_100_message_timeout_flag == 0)&&(Digital_Speedo_Guage_off==0)/*&&(gs_complete==1)*/
				&&((four_bit_data.byte == 9)||(four_bit_data.byte == 0XA)||(four_bit_data.byte == 0XB)||(four_bit_data.byte == 0XC)||(four_bit_data.byte ==0XD)||(four_bit_data.byte ==0XE)))
		{
				/*condition o*/
				/* Condition o -->
				 * Styling Ring Illumination	Pointer Illumination	Gauge Illumination		TFT Illumination
					Use last valid value		Use last valid value	Use last valid value	Use last valid value	--> Colour TFT
				 * */
				if(0 == firstTimeIn)	/* if at IGN ON CAN Message is Use Last valid value ,then load EEPROM Data*/
				{
					firstTimeIn = 1;	/* Setting One Time In Flag */
					load_tst_tt_flags(currIllumination); /* @@@ Load Last used Values to Illumination Flags from NVM  */
				}else
				{
					/* Do nothing */
				}

				if(tst_tt_flag==1)
				{

					//r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,270/1);   //90%TFT_ILLUMINATION

					#ifdef GRAYSCALE_TFT
					t=0x5F;		/*85%*/
					Illumn_percent = 75;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//75% TFT_ILLUMINATION
					#else
					t=0xF0;
					Illumn_percent = 90;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//90% TFT_ILLUMINATION
					#endif
					r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);

					currIllumination = 0;	/* To be Stored in EEPROM when IGN is OFF */
				}
				else if(tst_tt_flag_1==1)
				{

					#ifdef GRAYSCALE_TFT
					t=0x28;/*30%*/
					Illumn_percent = 20;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//20% TFT_ILLUMINATION
					#else
					t=0xB0;
					Illumn_percent = 24;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//24% TFT_ILLUMINATION
					#endif
					r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);
					currIllumination = 1;	/* To be Stored in EEPROM when IGN is OFF */

				}
				else if(tst_tt_flag_2==1)
				{

					#ifdef GRAYSCALE_TFT
					t=0x32;		/*40%*/
					Illumn_percent = 30;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//30% TFT_ILLUMINATION
					#else
					t=0xBE;
					Illumn_percent = 33;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//33% TFT_ILLUMINATION
					#endif
					r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);
					currIllumination = 2;	/* To be Stored in EEPROM when IGN is OFF */

				}
				else if(tst_tt_flag_3==1)
				{

					#ifdef GRAYSCALE_TFT
					t=0x37;/*45%*/
					Illumn_percent = 35;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//35% TFT_ILLUMINATION
					#else
					t=0xC5;
					Illumn_percent = 38;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//38% TFT_ILLUMINATION
					#endif
					r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);
					currIllumination = 3;	/* To be Stored in EEPROM when IGN is OFF */

				}
				else if(tst_tt_flag_4==1)
				{

					#ifdef GRAYSCALE_TFT
					t=0x3C;/*50%*/
					Illumn_percent = 40;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//40% TFT_ILLUMINATION
					#else
					t=0xCC;
					Illumn_percent = 43;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//43% TFT_ILLUMINATION
					#endif
					r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);
					currIllumination = 4;	/* To be Stored in EEPROM when IGN is OFF */

				}
				else if(tst_tt_flag_5==1)
				{

					#ifdef GRAYSCALE_TFT
					t=0x41;/*55%*/
					Illumn_percent = 45;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//45% TFT_ILLUMINATION
					#else
					t=0xD3;
					Illumn_percent = 48;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//48% TFT_ILLUMINATION
					#endif
					r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);
					currIllumination = 5;	/* To be Stored in EEPROM when IGN is OFF */

				}

				else if(tst_tt_flag_6==1)
				{

					#ifdef GRAYSCALE_TFT
					t=0x46;/*60%*/
					Illumn_percent = 50;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//50% TFT_ILLUMINATION
					#else
					t=0xDA;
					Illumn_percent = 53;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//53% TFT_ILLUMINATION
					#endif
				//	r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,54/1);			//18%TFT_ILLUMINATION
					r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);
					currIllumination = 6;	/* To be Stored in EEPROM when IGN is OFF */

				}

				else if(tst_tt_flag_7==1)
				{

					#ifdef GRAYSCALE_TFT
					t=0x4B;/*65%*/
					Illumn_percent = 55;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//55% TFT_ILLUMINATION
					#else
					t=0xE1;
					Illumn_percent = 58;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//58% TFT_ILLUMINATION
					#endif
				//	r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,63/1);			//21%TFT_ILLUMINATION
					r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);
					currIllumination = 7;	/* To be Stored in EEPROM when IGN is OFF */

				}
				else if(tst_tt_flag_8==1)
				{

					#ifdef GRAYSCALE_TFT
					t=0x50;/*70%*/
					Illumn_percent = 60;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//60% TFT_ILLUMINATION
					#else
					t=0XE8;
					Illumn_percent = 63;
					r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//63% TFT_ILLUMINATION
					#endif
					r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);
					currIllumination = 8;	/* To be Stored in EEPROM when IGN is OFF */

				}else if(tst_tt_flag_9==1)
				{

					#ifdef GRAYSCALE_TFT
								t=0X2D;/*35%*/
								Illumn_percent = 25;
								r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//25% TFT_ILLUMINATION
					#else
								t=0XB7;
								Illumn_percent = 29;
								r_taub_SetPeriod(2,R_TAUB_CHANNEL_5,ILLUM_PER(Illumn_percent));			//29% TFT_ILLUMINATION
					#endif
								r_taub_OutputEnable	(2,R_TAUB_CHANNEL_5);

								currIllumination = 9;	/* To be Stored in EEPROM when IGN is OFF */
				}

				DCdata1[0]=t;
				DCdata1[1]=t;
				DCdata1[2]=t;
				DCdata1[3]=t;
				DCdata1[4]=t;
				DCdata1[5]=t;
				DCdata1[6]=t;
				DCdata1[7]=t;
				DCdata1[8]=t;

				DCdata2[15]=t;
				DCdata2[16]=t;
				DCdata2[18]=t;
				DCdata2[19]=t;
				DCdata2[20]=t;
				DCdata2[21]=t;
				DCdata2[22]=t;
				DCdata2[23]=t;
				DCdata2[12]=t;
				DCdata2[13]=t;
				DCdata2[17]=t;
				if(tst_tt_flag_10==0)
				{
					tst_tt_flag_10=1;
					SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata1);
					SendDCData(0x000, 0x3, 0x00, 0, 1, 0, 0, 0, 255, 255, 255, DCdata2);

					cascade_TT_send_data();
					SP_LD1_TT=0xfff;
					SP_LD2_TT=0xfff;
					SP_LD3_TT=0xfff;
					RP_LD1_TT=0xfff;
					RP_LD2_TT=0xfff;
					RP_LD3_TT=0xfff;

					S_LD1_TT=0xfff;
					S_LD2_TT=0xfff;
					S_LD3_TT=0xfff;
					S_LD4_TT=0xfff;
					S_LD5_TT=0xfff;
					S_LD6_TT=0xfff;
					R_LD1_TT=0xfff;
					R_LD2_TT=0xfff;
					R_LD3_TT=0xfff;
					R_LD4_TT=0xfff;
					R_LD5_TT=0xfff;
					R_LD6_TT=0xfff;
					if(FEATURE_CODE_FUEL_TYPE == 1)
					{
						/* Diesel*/
						R_LD8_TT=0xfff;
						R_LD7_TT=0x000;

					}
					else
					{
						/* GAsoline*/
						R_LD8_TT=0;
						R_LD7_TT=0xfff;
					}
					SendGSData(GSdata1);

				}
				tst_tt_flag=0;
				tst_tt_flag_1=0;
				tst_tt_flag_2=0;
				tst_tt_flag_3=0;
				tst_tt_flag_4=0;
				tst_tt_flag_5=0;
				tst_tt_flag_6=0;
				tst_tt_flag_7=0;
				tst_tt_flag_8=0;
				tst_tt_flag_9=0;
				gs_complete=0;
			}

	}
#if 0
	if(gs_complete==1 && IGN_ON_flag == SET)
		{
			SendDCData(0x5A3, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1);
			SendDCData(0x5A3, 0x3, 0x00, 0, 1, 1, 1, 1, 255, 255, 255, DCdata1);
			cascade_TT_send_data();
			ReadDCData();
			gs_complete=0;
		}
#endif


}

/******************************************************************************
  * Function Name	: load_tst_tt_flags
  * Scope		    : Global
  * Arguments	  	: uint8_t
  * Returns 	  	: None
  * Purpose 	  	: This Function is used to load last used
  	  	  	  	  	   Illumination value read from EEPROM after
  	  	  	  	  	   IGN ON to Global Flags
******************************************************************************/


void load_tst_tt_flags(uint8_t Illumination)
{
	/* Set to default values */
	tst_tt_flag=0; 	 	tst_tt_flag_1=0;	tst_tt_flag_2=0; 	tst_tt_flag_3=0;
	tst_tt_flag_4=0; 	tst_tt_flag_5=0; 	tst_tt_flag_6=0; 	tst_tt_flag_7=0;
	tst_tt_flag_8=0; 	tst_tt_flag_9=0;	tst_tt_flag_10=0;

	if(Illumination >= 255)	/* Very First Time NVM Read , so load Default Value */
	{
		tst_tt_flag=1;		/* Setting to Condition C */
	}else if(0 == Illumination)
	{
		tst_tt_flag =1;
	}else if(1 == Illumination)
	{
		tst_tt_flag_1 =1;
	}else if(2 == Illumination)
	{
		tst_tt_flag_2 =1;
	}else if(3 == Illumination)
	{
		tst_tt_flag_3 =1;
	}else if(4 == Illumination)
	{
		tst_tt_flag_4 =1;
	}else if(5 == Illumination)
	{
		tst_tt_flag_5 =1;
	}else if(6 == Illumination)
	{
		tst_tt_flag_6 =1;
	}else if(7 == Illumination)
	{
		tst_tt_flag_7 =1;
	}else if(8 == Illumination)
	{
		tst_tt_flag_8 =1;
	}else if(9 == Illumination)
	{
		tst_tt_flag_9 =1;
	}else
	{
		/* if incorrrect value loaded from EEPROM , Set Default Value*/
		tst_tt_flag = 1;
	}

}







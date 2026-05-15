/****************************************************************************
Project : d1l1_display_connectivity
FILE    : $Id: hmi.c 5069 2015-05-04 15:40:54Z golczewskim $
============================================================================
DESCRIPTION

D1L1 display connectivity demo
============================================================================
                            C O P Y R I G H T
============================================================================
                           Copyright (c) 2016
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
Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.

****************************************************************************
*/


/*******************************************************************************
  Title: LCBI non TFT module

  Implementation of the LCBI non TFT mode 8 bit Adafruit MI0283QT. 

*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */

#include "r_bsp_api.h"
#include "r_bsp_stdio_api.h"
#include "r_dev_api.h"
#include "r_ddb_api.h"      /* display database with display timings */
#include "r_lcbi_api.h"
#include "r_cdi_api.h"
#include "r_tick_api.h"
#include "lcbi_non_tft.h"
#include "r_dma_api.h"
#include "r_g3dma_api.h"
#include "r_gpio_api.h"
#include "config.h"
#include <stdio.h>
#include "gui.h"
#include "mahindratext.h"
//#include "dr7f701461.dvf.h"


//#ifdef DISPLAY_MI0283QT_8BIT /* only one display can be used */

/*******************************************************************************
  Section: local Constants
*/


/* DMA Mode is only provided as a skeleton to demonstrate the work principle. 
   It may not produce correct output */
//#define USE_DMA_FOR_IMG_DATA



#define LCBI_NUM 0
#define DISPLAY_WIDTH           240
#define DISPLAY_HEIGHT          320
/* driver ILI9341 regs */
#define ILI9341_SOFTRESET          0x01
#define ILI9341_DISPLAY_ID         0x04
#define ILI9341_SLEEPIN            0x10
#define ILI9341_SLEEPOUT           0x11
#define ILI9341_NORMALDISP         0x13
#define ILI9341_INVERTOFF          0x20
#define ILI9341_INVERTON           0x21
#define ILI9341_GAMMASET           0x26
#define ILI9341_DISPLAYOFF         0x28
#define ILI9341_DISPLAYON          0x29
#define ILI9341_COLADDRSET         0x2A
#define ILI9341_PAGEADDRSET        0x2B
#define ILI9341_MEMORYWRITE        0x2C
#define ILI9341_MEMORYREAD         0x2E

#define ILI9341_PIXELFORMAT        0x3A
#define ILI9341_FRAMECONTROL       0xB1
#define ILI9341_DISPLAYFUNC        0xB6
#define ILI9341_ENTRYMODE          0xB7
#define ILI9341_POWERCONTROL1      0xC0
#define ILI9341_POWERCONTROL2      0xC1
#define ILI9341_VCOMCONTROL1       0xC5
#define ILI9341_VCOMCONTROL2       0xC7
#define ILI9341_MADCTL             0x36

#define ILI9341_ID4                0xD3

#define ILI9341_MADCTL_MY          0x80
#define ILI9341_MADCTL_MX          0x40
#define ILI9341_MADCTL_MV          0x20
#define ILI9341_MADCTL_ML          0x10
#define ILI9341_MADCTL_RGB         0x00
#define ILI9341_MADCTL_BGR         0x08
#define ILI9341_MADCTL_MH          0x04


/* RST pin just use a GPIO for this */
#define RST_PORT                 (44)
#define RST_PIN                  (5/*3*/)

/* 25ns per cycle 
 Read Strobe Low Width:  45ns (DisplaySpec), 50ns (NextPossibleValue) => 0x01
 Write Strobe Low Width: 15ns (DisplaySpec), 25ns (NextPossibleValue) => 0x00
 Reserved => 0x0
 Activating read/write strobe (UNUSED) => 0x0
 End of Cycle (Strobe High Width): 90ns (DisplaySpec-Read), 15ns (DisplaySpec-Write), 50ns (NextPossibleValue) => 0x1
 Reserved => 0x0
 */
/* Original setting:  #define R_LCBI_NONTFT_CYCLE   0x04040444 */
#ifdef USE_DMA_FOR_IMG_DATA
  #define R_LCBI_NONTFT_CYCLE   0x04040444
#else
  /*  */
  #define R_LCBI_NONTFT_CYCLE   0x04040444/*0x01020210*//*0x01000010*/
#endif

/* image data to be displayed defined externally... see image0.c & image1.c */
extern const uint32_t Image0[];
extern const uint32_t Image1[];

/*******************************************************************************
  Constant: loc_LcbiClkSelCfg

  LCBI clock
*/

static const r_dev_ClkSelConfig_t loc_LcbiClkSelCfg[] = 
/*   Cks                   SrcId                Divider     tpReqMsk      */
{
    {R_DEV_CKS_LCBI ,      R_DEV_CKS_SRC_CLKJIT,    0,   0},  
    /* delimiter - do not remove */
    {R_DEV_CKS_LAST  ,     R_DEV_CKS_SRC_MOSC,      0,   0}
};


/*******************************************************************************
  Constant: loc_LcbiTftMode_PinConfig

  LCBI pin config
*/

static const r_dev_PinConfig_t loc_LcbiNonTftMode_PinConfig[] = 
/*  Port Pin Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    /* LCBI0 Non-TFT mode (18bit) - port setup 0 */
#if 1
    /* alternative LCBI control pin location 0 */
    {43, 0,  4u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},     /* LCBI0CSZ (P43_0) */
    {43, 1,  4u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},     /* LCBI0WRRWZ (P43_1) */
    {44, 1,  3u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},     /* LCBI0RDEZ (P44_1) */
    {44, 2,  3u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},     /* LCBI0A0DE (P44_2) */

    {44, 3,  0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},     /* GPIO (P44_3) */

#else
    /* alternative LCBI control pin location 1 */
    {44, 3,  3u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},     /* LCBI0CSZ (P44_3) */
    {44, 4,  3u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},     /* LCBI0WRRWZ (P44_4) */
    {45, 0,  3u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},     /* LCBI0RDEZ (P45_0) */
    {45, 1,  3u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},     /* LCBI0A0DE (P45_1) */
#endif
    {44, 6,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},  /* LCBI0 InOut Data 0 */
    {44, 7,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {44, 8,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {44, 9,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {44, 10, 3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {44, 11, 3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},  /* LCBI0 InOut Data 5 */
    {45, 2,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},  /* LCBI0 InOut Data 6 */
    {45, 3,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},

#if 0 /* this interface is 8 bit only */
    {45, 4,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 5,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 6,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 7,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 8,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 9,  3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 10, 3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 11, 3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 12, 3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 13, 3u, R_DEV_PIN_DIRECT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},  /* LCBI0 InOut Data 17 */
#endif
    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,   0u, 0u, 0u, R_DEV_PIN_CMOS1} 
};

#ifdef USE_DMA_FOR_IMG_DATA
    /* See lcbi_tft.c for a detailed description! */
    /* DMA */
    #define DMA_UNIT                    0
    #define DMA_CHANNEL_ID_0            0
    /* Since we are using the CLUT we have 4 pixels per word */
    #define PIXELS_PER_WORD             (4u)
    /* Images are stored in ROM at 0x00100000 in order */
    #define IMAGE_WIDTH                 (240u)
    #define IMAGE_HEIGHT                (320u)
    #define IMAGE_SIZE_4                (((IMAGE_WIDTH) * (IMAGE_HEIGHT)) / (PIXELS_PER_WORD)) /* pixel size, 1 byte per pixel */
    /* Number of words written per LCBI DMA trigger. */
    #define DMA_BURST_LENGTH            (8u)
    /*  Number of times the DMA_BURST_LENGTH is written during one DMA cycle. */
    #define DMA_BURST_REPEAT            (((128u) / PIXELS_PER_WORD)) 
    //#define DMA_BURST_REPEAT             (16)
    /* initial number of manual writes to kick the LCBI off */
    #define INITIAL_WRITE_SIZE          (DMA_BURST_LENGTH)
#endif

/*******************************************************************************
  Section: local Variables
*/

#ifdef USE_DMA_FOR_IMG_DATA
    /* DMA config with reload */
    static r_g3dma_ChCfg_t  Dma_ChCfg = {
                                             0,   /* Unit */
                              DMA_CHANNEL_ID_0,   /* ChannelId */
                             (uint32_t) Image0,   /* SrcAddr */
                             LCBI_DATA_A0S_W32,   /* DstAddr LCBI data register */
     (DMA_BURST_REPEAT<<16) + DMA_BURST_LENGTH,   /* MSB reload count, LSB TransferCount */
                               R_G3DMA_32BIT_T,   /* TransferDataSize */
                           R_G3DMA_INCREMENT_T,   /* SrcAddrCntDir */
                               R_G3DMA_FIXED_T,   /* DstAddrCntDir */
                                             1,   /* LoopEnable */
                                            16,   /* INTLCBI0QTR G3DMATriggerFactor (INTLCBI0EMPTY 13, INTLCBI0QTR 16) */
                                             1,   /* InterruptAct, 1 = transfer complete, 2 = count compare */
                                R_G3DMA_MODE_S,   /* Mode */ 
     (DMA_BURST_REPEAT * DMA_BURST_LENGTH) / 2,   /* Compare Count, not used but useful */   
                                             0,   /* Next Channel in Chain */
                             R_G3DMA_CHAIN_OFF,   /* Chain mode disable */
                           R_G3DMA_RELOAD_BOTH,   /* reload function 1 */
                           R_G3DMA_RELOAD_BOTH    /* reload function 2 */
    };
    
    /* DMA source addresss pointer */
    static uint32_t  loc_Dma_Src_Addr;
    
    /* DMA frame display finished */
    static uint32_t loc_Frame_sync = 0;
#endif

/* Pointer to current image */
static uint32_t *ptrImage;

/* User requested image */
static uint8_t loc_Requested_Img;

/* used for timing for the RW cycle, depends on noisy cables and CPU speed 
   try out at run time, 1us default
*/
static uint32_t loc_delay = 1; /* uS */

/* Signal that the LCBI FIFO is full 
   "Schmitt trigger" behaviour:
   Set by:   3QTR, FULL
   Clear by: QTR, EMPTY 
*/
static volatile uint32_t lcbi_fifo_full_flag = 0;

/*******************************************************************************
  Section: Local functions
*/
#ifdef USE_DMA_FOR_IMG_DATA
static void loc_DmaCh0Isr(void);
#endif
static void loc_lcbi_isr_qtr(void);
static void loc_lcbi_isr_full(void);

static uint8_t Lcbi_ReadData(void);
static uint32_t Lcbi_Read32(uint8_t control_id);
static void Lcbi_WriteControl(uint8_t Reg);
static void Lcbi_WriteData8(uint8_t Value);
static void Lcbi_WriteData16(uint16_t Value);
static void Lcbi_WriteControlData(uint8_t Reg, uint8_t Value);

void loc_WriteImage(uint16_t *ptrImage);
void loc_WriteImage_fast(uint32_t y,uint32_t x,uint16_t *ptrImage);
void BOOTUP_WriteImage(void);
void Mahindra_TextImage(void);
static void loc_SetAddrWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
static void loc_DrawPixel(int16_t x, int16_t y, uint16_t color);
static void LCD_Setup(void);
void Mode_Select(uint8_t mode);


#ifdef USE_DMA_FOR_IMG_DATA
/*******************************************************************************
  loc_DmaCh0Isr

    ISR callback when DMA transfer complete or count compare match
    (depends on the DMA config setting, InterruptAct)
*/
static void loc_DmaCh0Isr(void)
{
    /* each time we get a DMA transfer complete we need to set up the reload address
       DMA tracsfers are 

        LCBI DMA trigger INTLCBI0QTR will cause DMA_BURST_LENGTH to be written each time 
        This is then multiplied by DMA_BURST_REPEAT before we have a complete DMA transfer
        Since we have to set up the reload address we can prepare it early by using the
        transfer count set to half
    */

    SET_TP4();  /* Set GPIO for time measurements */

    /* following code is only required when multiple DMA cycles make up a single frame
       this allows us flexibility and allows us to generate a sync signal for display update:
         loc_Frame_sync   
    */ 

    loc_Dma_Src_Addr += (DMA_BURST_REPEAT * DMA_BURST_LENGTH) * 4;

    /* roll over */
    if(loc_Dma_Src_Addr >= (ptrImage + (IMAGE_SIZE_4 * 4)) )
    {
        loc_Dma_Src_Addr = (uint32_t)ptrImage;
        loc_Frame_sync = R_TRUE;
        Dma_ChCfg.LoopEnable = 0;
        R_G3DMA_LoadChannel(&Dma_ChCfg, 1);
        Dma_ChCfg.LoopEnable = 1;
    }
    else
    {
        loc_Frame_sync = R_FALSE;
    }
    
    /* transfer complete reload the src addr + DMA_BURST_REPEAT */
    R_G3DMA_SetReloadConfig(DMA_UNIT, DMA_CHANNEL_ID_0, loc_Dma_Src_Addr, LCBI_DATA_A0S_W16 , (DMA_BURST_REPEAT * DMA_BURST_LENGTH));

    CLR_TP4();  /* Clear GPIO for time measurements */
}
#endif

/*******************************************************************************
  loc_lcbi_isr_qtr

    ISR callback when LCBI is quarter empty
*/
static void loc_lcbi_isr_qtr(void)
{
   lcbi_fifo_full_flag = 0;
}

/*******************************************************************************
  loc_lcbi_isr_full

    ISR callback when LCBI is full
*/
static void loc_lcbi_isr_full(void)
{
   lcbi_fifo_full_flag = 1;
}

/*******************************************************************************
  Function: Lcbi_Non_Tft_Image

*/
void Lcbi_Non_Tft_Image(uint8_t Img)
{
    /* grab a copy */
    loc_Requested_Img = Img;

    //loc_WriteImage();
}


/*******************************************************************************
  Function: Lcbi_Processing

*/
void Lcbi_Non_Tft_Processing(void)
{
    /* nothing for now */

}


/*******************************************************************************
  Function: Lcbi_ReadData

*/
static uint8_t Lcbi_ReadData(void)
{
uint16_t val;

    /* read data */
    val = R_LCBI_ReadData8_A0S(LCBI_NUM);
    R_TICK_WaitUS(0, loc_delay);
    return val;
}


/*******************************************************************************
  Function: Lcbi_Read32

*/
static uint32_t Lcbi_Read32(uint8_t control_id)
{
uint8_t u8_temp0;
uint8_t u8_temp1;
uint8_t u8_temp2;
uint8_t u8_temp3;
uint32_t u32_temp;

    Lcbi_WriteControl(control_id);
    Lcbi_ReadData(); /* dummy */

    /* read 4 bytes */
    u8_temp0 = Lcbi_ReadData();
    u8_temp1 = Lcbi_ReadData();
    u8_temp2 = Lcbi_ReadData();
    u8_temp3 = Lcbi_ReadData();

    u32_temp = (u8_temp0 << 24) + (u8_temp1 << 16) + (u8_temp2 << 8) + u8_temp3;

    //R_BSP_STDIO_Printf("Index 0x%02x, data = 0x%08x\n", control_id, u32_temp);
    return u32_temp;
}


/*******************************************************************************
  Function: Lcbi_WriteControl

*/
static void Lcbi_WriteControl(uint8_t Reg)
{
    /* control */
   // R_TICK_WaitUS(0, 1u);
    while (1 == lcbi_fifo_full_flag);
    R_LCBI_WriteData8_A0C(LCBI_NUM, Reg);
}


/*******************************************************************************
  Function: Lcbi_WriteData8

*/
static void Lcbi_WriteData8(uint8_t Value)
{
    /* data */
   // R_TICK_WaitUS(0, 1u);
    while (1 == lcbi_fifo_full_flag);
    
    R_LCBI_WriteData8_A0S(LCBI_NUM, Value);
}

void clear_screen(void)
{
	Mode_Select(4);
	uint16_t x,y;
    for (y = 0; y <16 ; y++)
       {
           x = 0;
           for (x = 0; x < 2400; x++)
           {
        	   Lcbi_WriteData8(0x00);/*all black on TFT*/
           }
       }
}
static void Lcbi_WriteData16(uint16_t Value)
{
    /* data */
    while (1 == lcbi_fifo_full_flag);
    
    /* This display has a different endianness */
    Value = (Value<<8) | (Value>>8);
    R_LCBI_WriteData16_A0S(LCBI_NUM, Value);
}


/*******************************************************************************
  Function: Lcbi_WriteControlData

*/
static void Lcbi_WriteControlData(uint8_t Reg, uint8_t Value)
{
    /* control and data */
    Lcbi_WriteControl(Reg);
    Lcbi_WriteData8(Value);
}


/*******************************************************************************
  Function: loc_WriteImage

*/
void BOOTUP_WriteImage(void)
{
	uint32_t x,y,i,j;
	uint16_t img1,img2,img3,img4;
	uint16_t img_r,img_g,img_b;
	uint8_t img;

	Mode_Select(4);
	for(y=0;y<320/2;y++)
	{
		x=0;
		for(x=0;x<240;x++)
		{
			if((y == 79)||(y==80))
			{
				img1= 0x7bef;

				img2= 0x7bef;
			}
			else
			{
				img1= 0x0;

				img2= 0x0;
			}
			/*img1=(*ptrImage+x);
			img2=(*ptrImage+x+240);
			img3=img1+img2;*/

//			img1=//ptrImage[x+240];
			img_r=(img1 & 0xF800)>>8;
			img_g=(img1 & 0x07e0)>>3;
			//			img_g=(img1 & 0x03e0)>>2;
			img_b=(img1 & 0x001F)<<3;
			img1= (img_r*0.3 + img_g *0.59 + img_b * 0.11);
			//			img1= (img_r+img_g+img_b)/3;
			if(img1>0xFF)
			{
				img1=0xFF;
			}
			img1 = img1 & 0xF0;

//			img2=//ptrImage[x/*+240*/];
					img_r=(img2 & 0xF800)>>8;
			img_g=(img2 & 0x07e0)>>3;
			//			img_g=(img2 & 0x03e0)>>2;
			img_b=(img2 & 0x001F)<<3;
			img2= (img_r*0.3 + img_g *0.59 + img_b * 0.11);
			//			img2= (img_r+img_g+img_b)/3;
			if(img2>0xFF)
			{
				img2=0xFF;
			}
			img2=img2 >> 4;

			img=(uint8_t)(img1+img2);
			Lcbi_WriteData8(img);

		}
		//ptrImage+=480;
	}
}

void Mahindra_TextImage(void)
{
    uint32_t x,y,i,j,p = 0;
    uint16_t img1,img2,img3,img4;
    uint16_t img_r,img_g,img_b;
    uint8_t img;



	Mode_Select(4);
for(y=0;y<320/2;y++)
{
    	x=0;
    	for(x=0;x<240;x++)
    	{
			/*img1=(*ptrImage+x);
			img2=(*ptrImage+x+240);
			img3=img1+img2;*/
    		if((y >= 69)&&(y<=90))
    		{
    			img1=mahindra_textlogo[p+x+240];

    			img2=mahindra_textlogo[p+x];
    		}
    		else
    		{
    			img1= 0x0;

    			img2= 0x0;
			p = 0;
    		}
			//img1=ptrImage[x+240];
			img_r=(img1 & 0xF800)>>8;
			img_g=(img1 & 0x07e0)>>3;
//			img_g=(img1 & 0x03e0)>>2;
			img_b=(img1 & 0x001F)<<3;
			img1= (img_r*0.3 + img_g *0.59 + img_b * 0.11);
//			img1= (img_r+img_g+img_b)/3;
			if(img1>0xFF)
			{
				img1=0xFF;
			}
			img1 = img1 & 0xF0;

			//img2=ptrImage[x/*+240*/];
			img_r=(img2 & 0xF800)>>8;
			img_g=(img2 & 0x07e0)>>3;
//			img_g=(img2 & 0x03e0)>>2;
			img_b=(img2 & 0x001F)<<3;
			img2= (img_r*0.3 + img_g *0.59 + img_b * 0.11);
//			img2= (img_r+img_g+img_b)/3;
			if(img2>0xFF)
			{
				img2=0xFF;
			}
			img2=img2 >> 4;

			img=(uint8_t)(img1+img2);
			Lcbi_WriteData8(img);

    	}
	if((y >= 69)&&(y<=91)) p+=480;
    	
    	}
}

void loc_WriteImage_fast(uint32_t y,uint32_t x,uint16_t *ptrImage)
{
    uint32_t x1;
    uint16_t img1,img2;
    uint16_t img_g;
    uint8_t img;

    if((y==0)&&(x==0))
    {
    	Mode_Select(4);
    }

//for(y=0;y<320/2;y++)
//{
    x=0;
    y=0;
    x1=0;
    	for(x1=x;x1<(IMG_STEP+x);x1++)
    	{

			img1=ptrImage[x1+240+(y*480)];
			img_g=(img1 & 0x07e0)>>3;

			img1= img_g *1.0;
			if(img1>0xFF)
			{
				img1=0xFF;
			}
			img1 = img1 & 0xF0;

			img2=ptrImage[x1+(y*480)];
			img_g=(img2 & 0x07e0)>>3;

			img2= img_g *1.0;
			if(img2>0xFF)
			{
				img2=0xFF;
			}
			img2=img2 >> 4;

			img=(uint8_t)(img1+img2);
			Lcbi_WriteData8(img);

    	}

    	//}

}

void loc_WriteImage(uint16_t *ptrImage)
{
    uint32_t x,y,i,j;
    uint16_t img1,img2,img3,img4;
    uint16_t img_r,img_g,img_b;
    uint8_t img;

	Mode_Select(4);
for(y=0;y<320/2;y++)
{
    	x=0;
    	for(x=0;x<240;x++)
    	{
			/*img1=(*ptrImage+x);
			img2=(*ptrImage+x+240);
			img3=img1+img2;*/

			img1=ptrImage[x+240];
			//img_r=(img1 & 0xF800)>>8;
			img_g=(img1 & 0x07e0)>>3;
//			img_g=(img1 & 0x03e0)>>2;
			//img_b=(img1 & 0x001F)<<3;
			img1= img_g *1.0;//(img_r*0.3 + img_g *0.59 + img_b * 0.11);
//			img1= (img_r+img_g+img_b)/3;
			if(img1>0xFF)
			{
				img1=0xFF;
			}
			img1 = img1 & 0xF0;

			img2=ptrImage[x/*+240*/];
			//img_r=(img2 & 0xF800)>>8;
			img_g=(img2 & 0x07e0)>>3;
//			img_g=(img2 & 0x03e0)>>2;
			//img_b=(img2 & 0x001F)<<3;
			img2= img_g *1.0;//(img_r*0.3 + img_g *0.59 + img_b * 0.11);
//			img2= (img_r+img_g+img_b)/3;
			if(img2>0xFF)
			{
				img2=0xFF;
			}
			img2=img2 >> 4;

			img=(uint8_t)(img1+img2);
			Lcbi_WriteData8(img);

    	}
    	ptrImage+=480;
    	}

   /* for (y = 0; y <320/2 ; y++)
        {
            x = 0;
            for (x = 0; x < 240; x++)
            {
            	if(*ptrImage!=0)
            	{
            		img1=0;
            	}
            	if(next_row==0)
            	{
					img1=(*ptrImage+0)>>8;
					img1=img1 & 0xF0;
            	}
            	else
            	{
            		img1=(*ptrImage+240)>>12;
            	}

            	ptrImage+=1;


            	Lcbi_WriteData8(img1);

            }
            ptrImage+=240;

        }
    if(next_row==0)
    {
    	next_row=1;
    }
    else
    {
    	next_row=0;
    }
*/
#if 0
    for (y = 0; y <16 ; y++)
       {
           x = 0;
           //loc_SetAddrWindow(x, y, DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1);

           for (x = 0; x < 2400; x++)
           {

           Lcbi_WriteData8(0x00);
               //loc_DrawPixel(x, y, *ptrImage++);
           }
       }

    //Lcbi_WriteControl( 0x3A);
    //Lcbi_WriteData8( 0xA5);
    /* write the data */
    for (y = 0; y <320/2 ; y++)
    {
        x = 0;

        //loc_SetAddrWindow(x, y, DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1);

        for (x = 0; x < 240; x++)
        {
        	if(*ptrImage!=0)
        	{
        		img1=0;
        	}
        	img1=(*ptrImage+0)*0.11;
        	img2=(*ptrImage+1)*0.59;
        	img3=(*ptrImage+2)*0.3;
        	img4 = img1+img2+img3;
        	img4 = img4/16;
        	img4 = img4<<4;
/*
        	img1=(*ptrImage+4)*0.3;
        	img2=(*ptrImage+5)*0.59;
        	img3=(*ptrImage+6)*0.11;
        	img = img/16;
        	img = img + img4;
*/
        	//img =img+img1+img2+img3;

        	//img = (((*ptrImage)/16)<<4)+(((*(ptrImage+1))/16));
        	//img = (((*ptrImage)/16)<<4)+(((*(ptrImage+1))/16));/**/
        	ptrImage+=4;
        	//Lcbi_WriteData8((y<<4)+y);
        	Lcbi_WriteData8(img4);
            //loc_DrawPixel(x, y, *ptrImage++);
        }
        ptrImage+=4*240;
    }

    time = R_TICK_GetTimeUS(0) - time;
    R_BSP_STDIO_Printf("Frame Update: %.1f ms (%.1f Hz)\n", time/1000.0f, 1000000.0f/time);
//    while(1);
#endif

#if 0
    while(1)
    {

    	     Mode_Select(4);

             //Display_Black();
     		for(i=0;i<240*80;i++)//display black
     	      {
     			Lcbi_WriteData8(0x00);
     	        Lcbi_WriteData8(0x00);
     	       }
     		  // WaitKey();
     		         		R_TICK_WaitMS(0, 500u);
    	     //Display_White();
    			for(i=0;i<240*80;i++)//display white
    		      {
    				Lcbi_WriteData8(0xff);
    		        Lcbi_WriteData8(0xff);
    		       }
                 //WaitKey();
    			R_TICK_WaitMS(0, 500u);




    }
#endif
}

void Mode_Select(uint8_t mode)
{
		uint8_t param,param1;
		param=(mode==1?0:mode/2);
		param1=(mode==4?159:(mode==2?79:39));

		//param=0x02;
		//param1=0x9F;
		param=0x02;
		//param1=0x9F;

		/*Lcbi_WriteControl( 0x25);
		Lcbi_WriteData8( param);
		Lcbi_WriteData8( 0xA5);
		Lcbi_WriteData8( 0xA5);
		Lcbi_WriteData8( 0xA5);*/

		/*Lcbi_WriteControl( 0x49);
		Lcbi_WriteData8( 0x00);//0x0A
		Lcbi_WriteData8( 0x00);
		Lcbi_WriteData8( 0x00);
		Lcbi_WriteData8( 0xA5);*/

		Lcbi_WriteControl( 0x22);
		Lcbi_WriteData8(0);
		Lcbi_WriteData8(param1);
		Lcbi_WriteData8(0);
		Lcbi_WriteData8( 0xA5);

		Lcbi_WriteControl( 0x23);
		Lcbi_WriteData8(0);
		Lcbi_WriteData8(0);
		Lcbi_WriteData8(0x00);
		Lcbi_WriteData8(0xef);

		Lcbi_WriteControl( 0x12);
		Lcbi_WriteData8( 0xA5);

		Lcbi_WriteControl( 0x3A);
		Lcbi_WriteData8( 0xA5);
}

/*******************************************************************************
  Function: loc_SetAddrWindow

*/
static void loc_SetAddrWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  
    Lcbi_WriteControlData(ILI9341_COLADDRSET, x1>>8);
    Lcbi_WriteData8(x1);
    Lcbi_WriteData8(x2>>8);
    Lcbi_WriteData8(x2);

    Lcbi_WriteControlData(ILI9341_PAGEADDRSET, y1>>8);
    Lcbi_WriteData8(y1);
    Lcbi_WriteData8(y2>>8);
    Lcbi_WriteData8(y2);

    Lcbi_WriteControl(ILI9341_MEMORYWRITE);
}


/*******************************************************************************
  Function: loc_DrawPixel

*/
static void loc_DrawPixel(int16_t x, int16_t y, uint16_t color) 
{
    //TODO: For consecutive pixel writes, setting the address should only be required once
    Lcbi_WriteData8(color>>8);
    Lcbi_WriteData8(color);
    
    //Lcbi_WriteData16(color);
}


/*******************************************************************************
  Function: LCD_Setup

*/
static void LCD_Setup(void)
{
    uint32_t i;
#if 0
    Lcbi_WriteControlData(ILI9341_SOFTRESET, 0);
    R_TICK_WaitMS(0, 5u);
    Lcbi_WriteControl(ILI9341_DISPLAYOFF);
    Lcbi_WriteControl(ILI9341_SLEEPOUT);
    R_TICK_WaitMS(0, 5u);
    Lcbi_WriteControlData(ILI9341_POWERCONTROL1, 0x23);
    Lcbi_WriteControlData(ILI9341_POWERCONTROL2, 0x10);

    Lcbi_WriteControlData(ILI9341_VCOMCONTROL1, 0x2B);
    Lcbi_WriteData8(0x2B);

    Lcbi_WriteControlData(ILI9341_VCOMCONTROL2, 0xC0);
    Lcbi_WriteControlData(ILI9341_MADCTL, ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
    Lcbi_WriteControlData(ILI9341_PIXELFORMAT, 0x55);

    Lcbi_WriteControlData(ILI9341_FRAMECONTROL, 0x00);
    Lcbi_WriteData8(0x1B);  

    Lcbi_WriteControlData(ILI9341_ENTRYMODE, 0x07);

    Lcbi_WriteControlData(ILI9341_SLEEPOUT, 0);
    R_TICK_WaitMS(0, 150u);
    Lcbi_WriteControlData(ILI9341_DISPLAYON, 0);
    R_TICK_WaitMS(0, 500u);
    loc_SetAddrWindow(0, 0, DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1);
#endif
	   /* PORT_ISOPMC44_5 = 0;
		PORT_ISOPM44_5 = 0;
		PORT_ISOP44_5 = 0;
		R_TICK_WaitMS(0u, 50u); //DrvSYS_Delay(50);    //Delay 50us
		//RESET=1;
		PORT_ISOPMC44_5 = 0;
		PORT_ISOPM44_5 = 0;
		PORT_ISOP44_5 = 1;*/
    	/*for(i = 0; i < 0xffff; i++);*/R_TICK_WaitMS(0u, 50u);	//Hardware reset
		Lcbi_WriteControl( 0x5E);	// Software reset
		Lcbi_WriteData8( 0xA5);//
		/*for(i = 0; i < 0xffff; i++);*/R_TICK_WaitMS(0u, 50u);

		Lcbi_WriteControl( 0x11);	// Display OFF
		Lcbi_WriteData8( 0xA5);	//

		Lcbi_WriteControl(0x91);	//DISAR (Display Area: 91h)
		Lcbi_WriteData8( 0x01);	//Parameter 1(P1) number of display lines (MSB)
		Lcbi_WriteData8( 0x3F);	//Parameter 2(P2) number of display lines (LSB)
		Lcbi_WriteData8( 0x77);	//Parameter 3(P3) number of display columns (MSB)
		Lcbi_WriteData8( 0xA5);	//Parameter 3(P3) number of display columns (LSB)

		Lcbi_WriteControl ( 0x92);	//DISSET1 (Display Set 1: 92h)
		Lcbi_WriteData8( 0X32);	//Parameter 1 (P1) number of clocks in 1H
		Lcbi_WriteData8( 0x02);	//Parameter 2 (P2) number of back porch set ¡°number of back porch¡± ¨C 1.
		Lcbi_WriteData8( 0x80);	/*Normally black*///Parameter 3 (P3) LCD type setting, OSCO output setting,number of front porch (MSB)
		Lcbi_WriteData8( 0x01);	//Parameter 4 (P4) number of front porch (LSB)set ¡°number of front porch lines¡± -1.

		Lcbi_WriteControl( 0x93);	//DISSET2 (Display  Set 2: 93h)
		Lcbi_WriteData8( 0x0A);	//Parameter 1 (P1) Source output ON timing
		Lcbi_WriteData8( 0x28);	//Parameter 2 (P2) Source output OFF timing
		Lcbi_WriteData8( 0x0C);	//Parameter 3 (P3) Gate output ON timing
		Lcbi_WriteData8( 0x26);	//Parameter 4 (P4) Gate output OFF timing    23  TYP

		Lcbi_WriteControl( 0x21);	//(Memory Address Control: 21h)
		Lcbi_WriteData8( 0x00); //0x00	//Parameter 1 (P1) Display data RAM control Increment column address direction  (S1: address 0, S320: address 319)
		Lcbi_WriteData8( 0xA5);	//
		Lcbi_WriteData8( 0xA5);	//
		Lcbi_WriteData8( 0xA5);	//

		Lcbi_WriteControl( 0x23);	//caset
		Lcbi_WriteData8( 0x00);	//
		Lcbi_WriteData8( 0x00);	//
		Lcbi_WriteData8( 0x01);	//column start address
		Lcbi_WriteData8( 0x73);	//

	        Lcbi_WriteControl( 0x22);	//STFRAME (Start Frame Address: 24h)
		Lcbi_WriteData8( 0x00);	//start frame address:0
		Lcbi_WriteData8( 0x9F);	//
		Lcbi_WriteData8( 0x00);	//
		Lcbi_WriteData8( 0xA5);	//

		Lcbi_WriteControl( 0x24);	//STFRAME (Start Frame Address: 24h)
		Lcbi_WriteData8( 0x00);	//start frame address:0
		Lcbi_WriteData8( 0xA5);	//
		Lcbi_WriteData8( 0xA5);	//
		Lcbi_WriteData8( 0xA5);	//

		Lcbi_WriteControl( 0x25);	//BPPSEL (Bpp Sel: 25h)
		Lcbi_WriteData8( 0x02);	//Parameter 1 (P1) Data format P11-P10 1,2,4bit/pixel 0x00:1bit,0x01:2bit,0x02,0x03:3bit
		Lcbi_WriteData8( 0xA5);
		Lcbi_WriteData8( 0xA5);
		Lcbi_WriteData8( 0xA5);

		Lcbi_WriteControl( 0x26);	//MDCTL (Memory Data Control: 26h)
		Lcbi_WriteData8( 0x00);	//Parameter 1 (P1) Data mask option no bit operation
		Lcbi_WriteData8( 0x00);	//Parameter 2 (P2) Mask at start page address
		Lcbi_WriteData8( 0x00);	//Parameter 3 (P3) Mask at end page address
		Lcbi_WriteData8( 0xA5);	//Parameter 4 (P4) Dummy

		Lcbi_WriteControl( 0xA2);	//Command GATESET
		Lcbi_WriteData8( 0x00);	//Parameter 1 (P1) Scan mode 2side Interlace driving 1, Normal scan direction
		Lcbi_WriteData8( 0xA5);	//
		Lcbi_WriteData8( 0xA5);	//
		Lcbi_WriteData8( 0xA5);	//

		Lcbi_WriteControl( 0xA1);	//ACSET
		Lcbi_WriteData8( 0x00);	//1 line inversion driving
		Lcbi_WriteData8( 0xA5);	//
		Lcbi_WriteData8( 0xA5);	//
		Lcbi_WriteData8( 0xA5);	//




		Lcbi_WriteControl( 0x81);	// GAMSET4P1
		Lcbi_WriteData8( 0x00);	// P6
		Lcbi_WriteData8( 0x09);	// P7
		Lcbi_WriteData8( 0x10);	// P8
		Lcbi_WriteData8( 0x13);	// P9

		Lcbi_WriteControl( 0x82);		// GAMSET4P2
		Lcbi_WriteData8( 0x16);	// P6
		Lcbi_WriteData8( 0x18);	// P7
		Lcbi_WriteData8( 0x1a);	// P8
		Lcbi_WriteData8( 0x1c);	// P9

		Lcbi_WriteControl( 0x83);		// GAMSET4P3
		Lcbi_WriteData8( 0x1e);	// P6
		Lcbi_WriteData8( 0x20);	// P7
		Lcbi_WriteData8( 0x22);	// P8
		Lcbi_WriteData8( 0x24);	// P9

		Lcbi_WriteControl( 0x84);		// GAMSET4P4
		Lcbi_WriteData8( 0x27);	// P6
		Lcbi_WriteData8( 0x29);	// P7
		Lcbi_WriteData8( 0x2d);	// P8
		Lcbi_WriteData8( 0x3F);	// P9



	//////////////////////////////////////////
	/////////////////////////////////////////

		Lcbi_WriteControl( 0x71);	//Electronic Volume Set 1
		Lcbi_WriteData8( 0x15);	//VDDHS P10-P13 0-FH 4.1 -5.6V Step 0.1V 0x04 is 4.5V(VDDHS) Sourch high voltage
		Lcbi_WriteData8( 0X0D);	//VCOMH P20-P27 00-98H 2.46-5.5V Step 0.02V 0x34 is 3.5V  VCOMH=3.5V   60 TYP  4c
		Lcbi_WriteData8( 0x0B);	//VCA P30-P34  00-1EH 1.50-3.00V Step is 0.05V 0x0A is 2.0V VCOML=VCOMH-2*VCA=3.5-2*2=-0.5V
		Lcbi_WriteData8( 0x0B);	//VDDRH P40-P44 00-19H  3.00-5.50V Step is 0.1V 0x0A is 4.0V(VDDRH) Maximum Gray scale voltage(V0) VDD  HS-0.1V>VDDRH

		Lcbi_WriteControl( 0x72);	//Electronic Volume Set 2
		Lcbi_WriteData8( 0x09);	//VOFREG P10-P14 00-1FH 2.4-5.5V Step 0.1V 0x01 is 2.5V(VOFREG) VEE=VOUTM-2*VOFREG=-3-2*VOFREG =-8V
		Lcbi_WriteData8( 0x06);	//VONREG P20-P25 01-24H 1.5-4.5V Step 0.1V 0x1A is 4.0V  VDDHG=2*VONREG-VEE=16V
		Lcbi_WriteData8( 0x30);	//Vcom adjust
		Lcbi_WriteData8( 0xA5);	//


		Lcbi_WriteControl( 0x61);	//Command PWRCTL
		Lcbi_WriteData8( 0x00);	//Parameter 1(P1) Booster circuit control, VCOMH and VCOML control 3rd booster *(-2)
		Lcbi_WriteData8( 0x55);	//Parameter 2(P2) Source amp. Setting, Display wait and VCOMH regulator drivability setting
		Lcbi_WriteData8( 0xD0);	//Parameter 3(P3) Booster clock frequency setting in normal display
		Lcbi_WriteData8( 0xA5);	//Parameter 4(P4) Booster clock frequency setting in partial display

		Lcbi_WriteControl( 0x49);	//VCOMH Offset Data
		Lcbi_WriteData8( 0x00);	//No offset adjustment of VCOMH
		Lcbi_WriteData8( 0x00);	//
		Lcbi_WriteData8( 0x00);	//
		Lcbi_WriteData8( 0xA5);	//


		/*for(i = 0; i < 0xffff; i++);*/R_TICK_WaitMS(0u, 50u);	//delay more than 1ms ??

		Lcbi_WriteControl( 0x13);	//SLEEP OUT
		Lcbi_WriteData8( 0xA5);	//
		/*for(i = 0; i < 0xffff; i++);*/R_TICK_WaitMS(0u, 50u);

		Lcbi_WriteControl( 0x12);	// Display ON
		Lcbi_WriteData8( 0xA5);	//
		clear_screen();
}


/*******************************************************************************
  Function: Lcbi_Init

*/
void Lcbi_Non_Tft_Init(void)
{
uint32_t u32_temp,i;
r_lcbi_Parameter_t  loc_r_lcbi_Param;
r_lcbi_Output_Active_t loc_r_lcbi_Out_Act;

    /* default user requested image and the current active image */
    loc_Requested_Img = 0;

    /* Select LCBI clock source */
    R_DEV_ClkSelection(loc_LcbiClkSelCfg);

    /* what is the current frequency */
    u32_temp = R_DEV_ClkFrequencyHz(R_DEV_CKS_LCBI);

    if (u32_temp == 0)
    {
        // error
        while(1);
    }

    /* Select LCBI ports for TFT mode */
    //R_DEV_PinInit (loc_LcbiNonTftMode_PinConfig);/*already done in init.c*/

    /* Setup Video Input and Video Output port pins */

    R_LCBI_Init(LCBI_NUM);

    R_LCBI_DisableInt(LCBI_NUM, R_LCBI_INT_RDY);
    R_LCBI_DisableInt(LCBI_NUM, R_LCBI_INT_EMPT);
    R_LCBI_DisableInt(LCBI_NUM, R_LCBI_INT_QTR);
    R_LCBI_DisableInt(LCBI_NUM, R_LCBI_INT_HALF);
    R_LCBI_DisableInt(LCBI_NUM, R_LCBI_INT_3QTR);
    R_LCBI_DisableInt(LCBI_NUM, R_LCBI_INT_FULL);

    R_LCBI_SetIsrCallback(LCBI_NUM, R_LCBI_INT_QTR, loc_lcbi_isr_qtr);
    R_LCBI_SetIsrCallback(LCBI_NUM, R_LCBI_INT_FULL, loc_lcbi_isr_full);
    R_LCBI_EnableInt(LCBI_NUM, R_LCBI_INT_QTR);
    R_LCBI_EnableInt(LCBI_NUM, R_LCBI_INT_FULL);

    /* LCBI - Set non-TFT Mode */
    loc_r_lcbi_Param.WorkMode   = R_LCBI_WM_NONTFT;
    loc_r_lcbi_Param.CycleSpeed = R_LCBI_BUSCYCLE_SLOW;/*R_LCBI_BUSCYCLE_FAST*/;
    loc_r_lcbi_Param.CycleType  = R_LCBI_BUSCYCLE_RAM;
    loc_r_lcbi_Param.TransMode  = R_LCBI_DIRECT;/*CLUT disable*/
    loc_r_lcbi_Param.AccessBit  = R_LCBI_8BIT;
    loc_r_lcbi_Param.ShiftBit   = R_LCBI_SHIFT0;/*R_LCBI_SHIFT3;*/
    
    R_LCBI_StopLcdBIF(LCBI_NUM);
    
#ifdef USE_DMA_FOR_IMG_DATA
    /* Workaround to use QTR and FULL ISRs from LCBI, otherwise, the LCBI FIFO never gets full */
    R_LCBI_SetClock(LCBI_NUM, R_LCBI_CLKDIV_16);
#else
    /* Set the divider for the operation clock. Bus clock is 80MHz.
       The fastest possible bus cycle needs 3 clocks, which means the transfer speed is 
       the operation clock divided by 3.

       The display takes transfer speeds up to 12.5 MHz.
       Setting the operation clock to 40MHz and adding an additional bus cycles gives:
       40MHz / 4 clock bus cycles = 10Mhz transfer speed */
    R_LCBI_SetClock(LCBI_NUM,/*R_LCBI_CLKDIV_8*/ R_LCBI_CLKDIV_2);
#endif
    R_LCBI_SetTConParam(LCBI_NUM, &loc_r_lcbi_Param);
    R_LCBI_SetBusCycle(LCBI_NUM, loc_r_lcbi_Param.CycleType);
    R_LCBI_SetNonTFTCycleSpec(LCBI_NUM, R_LCBI_NONTFT_CYCLE);
    R_LCBI_SetWorkingParam(LCBI_NUM, &loc_r_lcbi_Param);

    //R_LCBI_InvertOutput(LCBI_NUM, &loc_r_lcbi_Out_Act);

    /* reset the display */
    /* set the LCD display _RST inactive */
    R_GPIO_WritePin(RST_PORT, RST_PIN, 1);
    /*for(i = 0; i < 0xffff; i++);*/R_TICK_WaitMS(0, 50u);
    R_GPIO_WritePin(RST_PORT, RST_PIN, 0);
    /*for(i = 0; i < 0xffff; i++);*/R_TICK_WaitMS(0, 50u);
    R_GPIO_WritePin(RST_PORT, RST_PIN, 1);

    /* allow some time to settle */
    /*for(i = 0; i < 0xffff; i++);*/R_TICK_WaitMS(0, 50u);


    LCD_Setup();

    /* get the driver version of the display */
    u32_temp = Lcbi_Read32(ILI9341_ID4);


    /* draw the welcome image */
   // loc_WriteImage();
    clear_screen();
}


/*******************************************************************************
  Function: Lcbi_DeInit

*/
void Lcbi_Non_Tft_DeInit(void)
{
    /* LCBI - Stop and de-Init */    
    R_LCBI_StopLcdBIF(LCBI_NUM);
    R_LCBI_DeInit(LCBI_NUM);
}

//#endif /*DISPLAY_MI0283QT_8BIT*/

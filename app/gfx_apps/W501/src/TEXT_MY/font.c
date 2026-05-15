/*
****************************************************************************
PROJECT : DRW2D test
FILE    : $Id: font.c 11006 2016-11-07 16:32:44Z michael.golczewski $
============================================================================ 
DESCRIPTION
DRW2D test
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

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

****************************************************************************
*/

/***********************************************************
  Title: test_022.c  (cube3d_solid)
*/

#include "r_typedefs.h"

#include "r_cdi_api.h"
#include "r_drw2d_os.h"
#include "r_drw2d_api.h"
#include "r_config_drw2d.h"
#include "r_drw2d_ctx_dhd.h"
#include "r_ddb_api.h"
#include "r_wm_api.h"
#include "scenes.h"
#include "drw_wrap.h"
#include "mem_init.h"
#include "sprite.h"
#include "wm.h"
#include "fs_data.h"
#include "fs_data_res.h"
#include "r_bsp_stdio_api.h"
#ifndef _WINDOWS
#include "r_gfxutils_api.h"
#endif
#include "r_util_dhd.h"
#include "load_to_ram.h"
#include "math.h"
#include "jpeg.h"
#include "r_tick_api.h"
#include "bench.h"
#include "car_model.h"
#include "main_scene.h"
#include "davehd_types.h"          /* Required for error-free inclusion of "davehd_registertypes.h" */
#include "davehd_registertypes.h"  /* Required for profiling */
#include "davehd_driver.h"
#include "state_machine.h"
#include "font.h"

extern r_drw2d_Texture_t font_fixedsys12;

extern r_drw2d_Texture_t my_odo_font_texture;
extern uint8_t odo_string;

#if 1
static const FontPos_t _FontStatusPos[] = {
    { 0, 4}, /*   */
    { 4, 5}, /* ! */
    { 9, 6}, /* " */
    { 15, 10}, /* # */
    { 25, 8}, /* $ */
    { 33, 13}, /* % */
    { 46, 9}, /* & */
    { 55, 3}, /* ' */
    { 58, 5}, /* ( */
    { 63, 5}, /* ) */
    { 68, 8}, /* * */
    { 76, 9}, /* + */
    { 85, 4}, /* , */
    { 89, 5}, /* - */
    { 94, 4}, /* . */
    { 98, 5}, /* / */

	{ 0, 40}, /* 0 */
    { 40, 30}, /* 1 */
    { 72, 40}, /* 2 */
    { 116, 42}, /* 3 */
    { 166, 40}, /* 4 */
    { 212, 42}, /* 5 */
    { 258, 42}, /* 6 */
    { 306, 40}, /* 7 */
    { 352, 42}, /* 8 */
    { 400, 42}, /* 9 */
    

};
#endif

#if 1
static const FontPos_t my_odo_font_info[] = {
    { 0, 14}, /*   */
    { 32, 20}, /* ! */
    { 58, 20}, /* " */
    { 84, 28}, /* # */
    { 124, 22}, /* $ */
    { 160, 30}, /* % */
    { 202, 26}, /* & */
    { 234, 16}, /* ' */
    { 258, 18}, /* ( */
    { 288, 18}, /* ) */
    { 314, 22}, /* * */
    { 348, 20}, /* + */
    { 380, 18}, /* , */
    { 404, 20}, /* - */
    { 432, 18}, /* . */
    { 456, 26}, /* / */

#if 0
	{ 516+0, 22}, /* 0 */
    { 516+24, 14}, /* 1 */
    { 516+48, 22}, /* 2 */
    { 516+76, 22}, /* 3 */
    { 516+104, 22}, /* 4 */
    { 516+134, 22}, /* 5 */
    { 516+162, 22}, /* 6 */
    { 516+192, 22}, /* 7 */
    { 516+220, 22}, /* 8 */
    { 516+250, 22}, /* 9 */
#endif 

	{ 516, 22}, /* 0 */
    { 540, 14}, /* 1 */
    { 554, 22}, /* 2 */
    { 580, 22}, /* 3 */
    { 605, 22}, /* 4 */
    { 630, 22}, /* 5 */
    { 655, 22}, /* 6 */
    { 680, 22}, /* 7 */
    { 705, 22}, /* 8 */
    { 730, 22}, /* 9 */
	
	{ 492, 14}, /* : */
	
	{ 760, 12}, /* Degree Symbol */
	{ 778, 12}, /* C Symbol */
	{ 790, 24}, /* km Symbol */
	{ 814, 24}, /* am Symbol */
	{ 838, 24}, /* pm Symbol */
	
	
};
#endif


/***********************************************************
  Function: locGetCharRemapping
*/

int locGetCharRemapping(unsigned char Character) {
    int c;
    
    /* Character remapping */
    if ((Character >= 32) && (Character < 128)) {
        /* visible ASCII characters start with 32 (blank)
           not visible chars 0-31 and will be omitted */
        c = (Character - 32);
    } else {
        switch (Character) {
            case 0xB0:
            	  /* special remapping for non ASCII character 'degree' (° or 0xB0)
            	     non visible char DEL (127) will be used for 'degree' */
                c = (127 - 32);
                break;
            default:
                /* use blank as default */
                c = 0;
                break;
        }
    }
    return c;
}


/***********************************************************
  Function: locWriteChar

  Write a single character.

  Use the currently selected buffer, font and color to draw
  a character at the given position.

  Parameters:
  Dev        - D/AVE 2D device structure
  PosX       - Display X position (not turned)
  PosY       - Display Y position (not turned)
  Character  - ASCII value of the character

  Returns:
  void
*/
uint32_t my_color = 0xffff0000;
static void locWriteChar(int PosX, int PosY, unsigned char Character, unsigned char change_color, unsigned char label) 
{
    static r_drw2d_Rect_t srcRect;
    static r_drw2d_Rect_t dstRect;

    char      c;

    c = locGetCharRemapping(Character);
    
	switch(label)
	{
		case 0:
			srcRect.Pos.X       = R_DRW2D_2X(_FontStatusPos[c].Offset);
			srcRect.Pos.Y       = 0; 
			srcRect.Size.Width  = R_DRW2D_2X(_FontStatusPos[c].Width); 
			srcRect.Size.Height = R_DRW2D_2X(64); 	// For Speed fonts
			 
			
			break;
			
		case 1:
			srcRect.Pos.X       = R_DRW2D_2X(my_odo_font_info[c].Offset);
			srcRect.Pos.Y       = 0;
			srcRect.Size.Width  = R_DRW2D_2X(my_odo_font_info[c].Width); 
			srcRect.Size.Height = R_DRW2D_2X(34); 	// For Odo fonts
			break;
	
	}
	
/*	
    srcRect.Pos.X       = R_DRW2D_2X(_FontStatusPos[c].Offset);
	//srcRect.Pos.X       = R_DRW2D_2X(my_odo_font_info[c].Offset);
	
    srcRect.Pos.Y       = 0; 
    srcRect.Size.Width  = R_DRW2D_2X(_FontStatusPos[c].Width); 
	//srcRect.Size.Width  = R_DRW2D_2X(my_odo_font_info[c].Width); 
	
    //srcRect.Size.Height = R_DRW2D_2X(16); 
	srcRect.Size.Height = R_DRW2D_2X(64); 	// For Speed fonts
	//srcRect.Size.Height = R_DRW2D_2X(34); 	// For Odo fonts
*/    
    dstRect.Pos.X       = R_DRW2D_2X(PosX);
    dstRect.Pos.Y       = R_DRW2D_2X(PosY); 
    dstRect.Size.Width  = srcRect.Size.Width; 
    dstRect.Size.Height = srcRect.Size.Height;     
    
    if(change_color)
	{
		//================= START: ONLY RED effect =================================	

		static r_drw2d_EffectStage_t Effects[1];
		static unsigned char needle_blink_count;

		Effects[0].Name = R_DRW2D_EFFECT_MODULATE;
		Effects[0].Args[0].Source = R_DRW2D_EFFECT_SOURCE_TEXTURE_UNIT;
		Effects[0].Args[0].Param.Color.Source.TextureUnit = 0;
		Effects[0].Args[0].Param.Color.Operand = R_DRW2D_EFFECT_COLOR_OPERAND_RGBA;
		Effects[0].Args[1].Source = R_DRW2D_EFFECT_SOURCE_CONSTANT_COLOR;
		Effects[0].Args[1].Param.Color.Source.ConstantColor = my_color;//0xffff0000;//my_color;
		Effects[0].Args[1].Param.Color.Operand = R_DRW2D_EFFECT_COLOR_OPERAND_RGBA;

		
		R_DRW2D_CtxEffectsSet(g_Drw2dDev, Effects, 1);
			
	//================= END: ONLY RED effect =================================	
		R_DRW2D_TextureBlit(g_Drw2dDev,&srcRect,&dstRect);
		R_DRW2D_CtxEffectsDelete(g_Drw2dDev); 
	
	}
	
	else
	{
		R_DRW2D_TextureBlit(g_Drw2dDev,&srcRect,&dstRect);
	
	}
		
}

/***********************************************************
  Section: Global Functions

  Description see: <write.h>
*/


/***********************************************************
  Function: GetCharWidth
*/

int GetCharWidth(char Character, unsigned char label) {
    int c;

    c = locGetCharRemapping(Character);
    if ((c < 0) || (c > 95)) {
        return 0;
    }
	
	switch(label)
	{
		case 0:
			 return _FontStatusPos[c].Width;	// For Speed fonts
			break;
	
		case 1:
			return my_odo_font_info[c].Width;	// For Odo fonts
			break;
	
	}
   
	//
	
}


/***********************************************************
  Function: GetTextWidth
*/

int GetTextWidth(char *Text) {
    int w;

    w = 0;
    while (*Text != 0) {
        w += GetCharWidth(*Text, 0);
        Text++;
    }
    return w;
}

/***********************************************************
  Function: GetTextHeight
*/

int GetTextHeight(char *Text) {
    int h;

    h = 1; /* To include the last line */
    while (*Text != 0) {
        if (*Text == '\n') {
            h += 16;
        }
        Text++;
    }
    return h;
}





/***********************************************************
  Function: Write
*/

void FlashDash_WriteString_Speed(int PosX, int PosY, char *Text, unsigned char change_color, unsigned char label) 
{
    int       startx;
	unsigned char my_digit_count = 0;
	static long color = 0xffffffff;
    R_DRW2D_CtxBgColor(g_Drw2dDev, color);
	
	switch(label)
	{
		case 0:
			R_DRW2D_CtxTextureSet(g_Drw2dDev, 0u, &font_fixedsys12);   
			break;
			
		case 1:
			R_DRW2D_CtxTextureSet(g_Drw2dDev, 0u, &my_odo_font_texture);   
			break;
	}
    
	startx = PosX;
    while (*Text != 0) {
        if (*Text == '\n') {
            PosX  = startx;
            PosY += 16;
        } else {
			if((label == 1) && (odo_string == 1))
			{
				my_digit_count++;
				if(my_digit_count == 7)
				{
					my_digit_count = 0;
					my_color = 0xff9f6110;//0xffff7f00;
					change_color = 1;
				}
				else
				{
					my_color = 0xffff0000;
					change_color = 0;
				}
			}
            locWriteChar(PosX, PosY, *Text, change_color, label);
            PosX += GetCharWidth(*Text, label);
        }
        Text++;
    } 
	

}
/*
void FlashDash_WriteString_Odo(int PosX, int PosY, char *Text, unsigned char change_color) 
{
    int       startx;

	static long color = 0xffffffff;
    R_DRW2D_CtxBgColor(g_Drw2dDev, color);
    
	//R_DRW2D_CtxTextureSet(g_Drw2dDev, 0u, &font_fixedsys12);   
	R_DRW2D_CtxTextureSet(g_Drw2dDev, 0u, &my_odo_font_texture);   
	                                          
    startx = PosX;
    while (*Text != 0) {
        if (*Text == '\n') {
            PosX  = startx;
            PosY += 16;
        } else {
            locWriteChar(PosX, PosY, *Text, change_color);
            PosX += GetCharWidth(*Text);
        }
        Text++;
    } 
	

}
*/
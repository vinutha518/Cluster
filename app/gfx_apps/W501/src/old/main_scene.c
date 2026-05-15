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
//#include "r_gfxutils_api.h"
#include "r_util_dhd.h"
#include "load_to_ram.h"
#include "math.h"
#include "jpeg.h"
#include "r_tick_api.h"
#include "bench.h"
#include "car_model.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "state_machine.h"
#include "font.h"
//#include "my_text_print.h"


static int movie_count=0;

unsigned int my_L_X = 150;//160;
unsigned int my_L_Y = 240;//290;

unsigned int my_R_X = 400;//500;//400;
unsigned int my_R_Y = 240;//210;//248;

unsigned char my_move = 0;
unsigned char move_done = 0;

extern unsigned char my_speed_val;
extern unsigned int my_rpm_val;
	
#define CENTER_L_X (234.5 + 2 + .25 + 160)
#define CENTER_L_Y (239.5 + .25)

#define DEBUG_SPRITES 0
#define KPH_FACTOR 1.7

extern unsigned char my_angle_val;

r_drw2d_Framebuffer_t drw2d_fb;
extern r_wm_Window_t speed_Window;
extern r_wm_Window_t g_Window;
extern r_wm_Window_t odo_Window;
extern MultiBufferSprite_t Speed_Sprite;
extern MultiBufferSprite_t Odo_Sprite;
extern MultiBufferSprite_t Time_Sprite;
extern MultiBufferSprite_t Temp_Sprite;
extern MultiBufferSprite_t Mode_Sprite;

float32_t kph, rpm;
extern r_drw2d_Texture_t my_odo_font_texture;

uint8_t odo_string = 0;
//======================================================================================

void Init_LeftNeedle(void){
    r_drw2d_Framebuffer_t *draw_fb;
    draw_fb = MbSpriteNewDrawBufGet(&LeftNeedle);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);
#if DEBUG_SPRITES
    R_DRW2D_CtxBgColor(g_Drw2dDev, 0x8000ffff);
#else
    R_DRW2D_CtxBgColor(g_Drw2dDev, 0x00000000);
#endif
    R_DRW2D_FramebufferClear(g_Drw2dDev);    
}

void Init_SpeedSprite(void){
    r_drw2d_Framebuffer_t *draw_fb;
    draw_fb = MbSpriteNewDrawBufGet(&Speed_Sprite);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);
#if DEBUG_SPRITES
    R_DRW2D_CtxBgColor(g_Drw2dDev, 0x8000ffff);
#else
    R_DRW2D_CtxBgColor(g_Drw2dDev, 0x00000000);
#endif
    R_DRW2D_FramebufferClear(g_Drw2dDev);    
}

void Init_OdoSprite(void)
{
    r_drw2d_Framebuffer_t *draw_fb;
    draw_fb = MbSpriteNewDrawBufGet(&Odo_Sprite);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);
#if DEBUG_SPRITES
    R_DRW2D_CtxBgColor(g_Drw2dDev, 0x8000ffff);
#else
    R_DRW2D_CtxBgColor(g_Drw2dDev, 0x00000000);
#endif
    R_DRW2D_FramebufferClear(g_Drw2dDev);    
}

void Init_TimeSprite(void)
{
    r_drw2d_Framebuffer_t *draw_fb;
    draw_fb = MbSpriteNewDrawBufGet(&Time_Sprite);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);
#if DEBUG_SPRITES
    R_DRW2D_CtxBgColor(g_Drw2dDev, 0x8000ffff);
#else
    R_DRW2D_CtxBgColor(g_Drw2dDev, 0x00000000);
#endif
    R_DRW2D_FramebufferClear(g_Drw2dDev);    
}

void Init_TempSprite(void)
{
    r_drw2d_Framebuffer_t *draw_fb;
    draw_fb = MbSpriteNewDrawBufGet(&Temp_Sprite);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);
#if DEBUG_SPRITES
    R_DRW2D_CtxBgColor(g_Drw2dDev, 0x8000ffff);
#else
    R_DRW2D_CtxBgColor(g_Drw2dDev, 0x00000000);
#endif
    R_DRW2D_FramebufferClear(g_Drw2dDev);    
}

void Init_ModeSprite(void)
{
    r_drw2d_Framebuffer_t *draw_fb;
    draw_fb = MbSpriteNewDrawBufGet(&Mode_Sprite);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);
#if DEBUG_SPRITES
    R_DRW2D_CtxBgColor(g_Drw2dDev, 0x8000ffff);
#else
    R_DRW2D_CtxBgColor(g_Drw2dDev, 0x00000000);
#endif
    R_DRW2D_FramebufferClear(g_Drw2dDev);    
}



void Init_RightNeedle(void){
    r_drw2d_Framebuffer_t *draw_fb;
    draw_fb = MbSpriteNewDrawBufGet(&RightNeedle);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);
	
#if DEBUG_SPRITES
    R_DRW2D_CtxBgColor(g_Drw2dDev, 0x80ff00ff);
#else
    R_DRW2D_CtxBgColor(g_Drw2dDev, 0x00000000);
#endif
    R_DRW2D_FramebufferClear(g_Drw2dDev);    
}

void Init_MainScene(void) {
    /* Lowest Layer */
    R_DRW2D_CtxImgQuality(g_Drw2dDev, R_DRW2D_IMGQUALITY_HIGH);
   
   //Init_Background();
   R_DRW2D_GpuFinish(g_Drw2dDev, R_DRW2D_FINISH_WAIT);
   
   
    Init_LeftNeedle();
	Init_SpeedSprite();
	Init_OdoSprite();
	Init_TimeSprite();
	Init_TempSprite();
	Init_ModeSprite();
	
	
	//Init_RightNeedle();

    R_DRW2D_GpuFinish(g_Drw2dDev, R_DRW2D_FINISH_WAIT);
    /* Sprite swap */

    MbSpriteSwap(&LeftNeedle);
	MbSpriteSwap(&Speed_Sprite);
	MbSpriteSwap(&Odo_Sprite);
	MbSpriteSwap(&Time_Sprite);
	MbSpriteSwap(&Temp_Sprite);
	MbSpriteSwap(&Mode_Sprite);
    
	//R_WM_WindowSwap(FLASHDASH_WM_UNIT, &g_Window);
	
    R_WM_FrameEndMark(FLASHDASH_WM_UNIT, 0);
    R_WM_FrameWait(FLASHDASH_WM_UNIT, 0);
    srand(R_TICK_GetTimeMS(0));
}

float64_t CalculateAngleLeft(float32_t kph)
{
	float64_t final_angle = -49;
	float64_t angle_min = -49;//-45.44470326;
    float64_t angle_max = 230;//180 + 29.48890781;
	
	final_angle = ((kph/255)  * 1 * (angle_max - angle_min)) + angle_min;
	
	if(final_angle > angle_max)
		final_angle = angle_max;
	if(final_angle < angle_min)
		final_angle = angle_min;
	
	return final_angle;
    //return kph/220  * KPH_FACTOR * (angle_max - angle_min) + angle_min;
	
	
}

float64_t CalculateAngleRight(float32_t rpm)
{
	float64_t final_angle = -21;
    float64_t angle_min = -45;//-21;//-96.44470326;
    float64_t angle_max = 135;//132;
	
	final_angle = ((rpm /8000) * (angle_max - angle_min)) + angle_min;
	
	if(final_angle > angle_max)
		final_angle = angle_max;
	if(final_angle < angle_min)
		final_angle = angle_min;
	
	return final_angle;
	
    //return rpm /8000 * (angle_max - angle_min) + angle_min;    
}

uint32_t IncIndex(uint32_t index){
    index+=1;
    if(index >= 2) index = 0;
    return index;
}




void Setup_LeftNeedleParameters(r_drw2d_Point_t *rot_center_p,
                                r_drw2d_Point_t *tex_rot_center_p)
{
    r_drw2d_Point_t tmp;
    
    tmp.X = R_DRW2D_2X(WindowNeedle.PosX + MbSpriteDrawPosXGet(&LeftNeedle) - SpeedSprite.PosX );
    tmp.Y = R_DRW2D_2X(WindowNeedle.PosY + MbSpriteDrawPosYGet(&LeftNeedle) - SpeedSprite.PosY );
    SetNeedleRefreshSpritePos(tmp);

    tmp.X = R_DRW2D_2X(WindowNeedle.PosX + MbSpriteDrawPosXGet(&LeftNeedle));
    tmp.Y = R_DRW2D_2X(WindowNeedle.PosY + MbSpriteDrawPosYGet(&LeftNeedle));

	rot_center_p->X     = R_DRW2D_2X(my_R_X) - tmp.X;
    rot_center_p->Y     = R_DRW2D_2X(my_R_Y) - tmp.Y;
	
	//tex_rot_center_p->X = R_DRW2D_2X(151);//168.93168-.5);
    //tex_rot_center_p->Y = R_DRW2D_2X( 48);// 17);
	
	tex_rot_center_p->X = R_DRW2D_2X(169);
    tex_rot_center_p->Y = R_DRW2D_2X( 39);
	
}

void Setup_RightNeedleParameters(r_drw2d_Point_t *rot_center_p,
                                r_drw2d_Point_t *tex_rot_center_p)
{
    r_drw2d_Point_t tmp;
    
    tmp.X = R_DRW2D_2X(WindowNeedle.PosX + MbSpriteDrawPosXGet(&RightNeedle) - TachoSprite.PosX );
    tmp.Y = R_DRW2D_2X(WindowNeedle.PosY + MbSpriteDrawPosYGet(&RightNeedle) - TachoSprite.PosY );
    SetNeedleRefreshSpritePos(tmp);

    tmp.X = R_DRW2D_2X(WindowNeedle.PosX + MbSpriteDrawPosXGet(&RightNeedle));
    tmp.Y = R_DRW2D_2X(WindowNeedle.PosY + MbSpriteDrawPosYGet(&RightNeedle));

	rot_center_p->X     = R_DRW2D_2X(my_L_X) - tmp.X;
    rot_center_p->Y     = R_DRW2D_2X(my_L_Y) - tmp.Y;
	
	tex_rot_center_p->X = R_DRW2D_2X(129);//159);//168.93168-.5);
    tex_rot_center_p->Y = R_DRW2D_2X( 26);//32);// 17);
	
}


void Clear_Needle_Left(float64_t angle, r_drw2d_Point_t rot_center, r_drw2d_Point_t tex_rot_center,
                  MultiBufferSprite_t *MBSprite, uint32_t is_left) {
    r_drw2d_Framebuffer_t *draw_fb;
    static r_drw2d_Texture_t *needle = &gauge_needle2_texture;
    
    draw_fb = MbSpriteNewDrawBufGet(MBSprite);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);
    
   
    Clear_RotationObject(angle, rot_center, tex_rot_center, 
                             needle, 4);
}

void Clear_Needle_Right(float64_t angle, r_drw2d_Point_t rot_center, r_drw2d_Point_t tex_rot_center,
                  MultiBufferSprite_t *MBSprite, uint32_t is_left) {
    r_drw2d_Framebuffer_t *draw_fb;
    static r_drw2d_Texture_t *needle = &gauge_needle1_texture;
    
    draw_fb = MbSpriteNewDrawBufGet(MBSprite);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);
    
    Clear_RotationObject(angle, rot_center, tex_rot_center, 
                             needle, 4);
}

r_drw2d_IntRect_t Calc_Rect_Left(float64_t angle,
                    r_drw2d_Point_t rot_center, 
                    r_drw2d_Point_t tex_rot_center,
               MultiBufferSprite_t *MBSprite) {
    r_drw2d_Framebuffer_t *draw_fb;
    r_drw2d_Point_t shadow_rot_center;
    r_drw2d_Point_t shadow_tex_rot_center;
    r_drw2d_IntRect_t rect_light, rect_shadow, rect_needle, rect_max;
    static r_drw2d_Texture_t *needle = &gauge_needle2_texture;
    const uint32_t max_pixels = 190 * 190;

    draw_fb = MbSpriteNewDrawBufGet(MBSprite);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);

/*     static r_drw2d_Texture_t *needle = &gauge_needle_texture; */
    shadow_rot_center.X = rot_center.X - R_DRW2D_2X(4);
    shadow_rot_center.Y = rot_center.Y + R_DRW2D_2X(4);
    shadow_tex_rot_center.X = tex_rot_center.X + R_DRW2D_2X(1);
    shadow_tex_rot_center.Y = tex_rot_center.Y + R_DRW2D_2X(3);
    
    //rect_light  = CalcRect_Light(angle);
    //rect_shadow = CalcRect_RotationObject(angle, shadow_rot_center, shadow_tex_rot_center,    &gauge_shadow_texture);
    rect_needle = CalcRect_RotationObject(angle,
                                          rot_center, tex_rot_center,
                                          needle);
		
	rect_max = MaximizeBoundingBox( &rect_needle, &rect_needle);
    //rect_max = MaximizeBoundingBox( &rect_needle, &rect_shadow);
    //rect_max = MaximizeBoundingBox( &rect_max   , &rect_light);

	/*     rect_max.Pos.X -= WindowGears.PosX; */
/*     rect_max.Pos.Y -= WindowGears.PosY; */
    if(rect_max.Pos.X & 1) { /* Odd sprite positions are not allowed. */
        rect_max.Pos.X--; /* Shift left by 1 */
        rect_max.Size.Width++; /* Increase width by 1 */
    }
    if(rect_max.Size.Width & 1) { /* Odd width is not allowed. */
        rect_max.Size.Width++;
    }

    //R_GFXUTILS_ASSERT((rect_max.Size.Width * rect_max.Size.Height) <= max_pixels);
    
    return rect_max;
}

r_drw2d_IntRect_t Calc_Rect_Right(float64_t angle,
                    r_drw2d_Point_t rot_center, 
                    r_drw2d_Point_t tex_rot_center,
               MultiBufferSprite_t *MBSprite) {
    r_drw2d_Framebuffer_t *draw_fb;
    r_drw2d_Point_t shadow_rot_center;
    r_drw2d_Point_t shadow_tex_rot_center;
    r_drw2d_IntRect_t rect_light, rect_shadow, rect_needle, rect_max;
    static r_drw2d_Texture_t *needle = &gauge_needle1_texture;
    const uint32_t max_pixels = 190 * 190;

    draw_fb = MbSpriteNewDrawBufGet(MBSprite);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);

/*     static r_drw2d_Texture_t *needle = &gauge_needle_texture; */
    shadow_rot_center.X = rot_center.X - R_DRW2D_2X(4);
    shadow_rot_center.Y = rot_center.Y + R_DRW2D_2X(4);
    shadow_tex_rot_center.X = tex_rot_center.X + R_DRW2D_2X(1);
    shadow_tex_rot_center.Y = tex_rot_center.Y + R_DRW2D_2X(3);
    
    //rect_light  = CalcRect_Light(angle);
    //rect_shadow = CalcRect_RotationObject(angle, shadow_rot_center, shadow_tex_rot_center,    &gauge_shadow_texture);
    rect_needle = CalcRect_RotationObject(angle,
                                          rot_center, tex_rot_center,
                                          needle);
		
	rect_max = MaximizeBoundingBox( &rect_needle, &rect_needle);
    //rect_max = MaximizeBoundingBox( &rect_needle, &rect_shadow);
    //rect_max = MaximizeBoundingBox( &rect_max   , &rect_light);

	/*     rect_max.Pos.X -= WindowGears.PosX; */
/*     rect_max.Pos.Y -= WindowGears.PosY; */
    if(rect_max.Pos.X & 1) { /* Odd sprite positions are not allowed. */
        rect_max.Pos.X--; /* Shift left by 1 */
        rect_max.Size.Width++; /* Increase width by 1 */
    }
    if(rect_max.Size.Width & 1) { /* Odd width is not allowed. */
        rect_max.Size.Width++;
    }

    //R_GFXUTILS_ASSERT((rect_max.Size.Width * rect_max.Size.Height) <= max_pixels);
    
    return rect_max;
}

void Refresh_Needle_Left(float64_t angle,float64_t old_angle,
                    r_drw2d_Point_t rot_center, 
                    r_drw2d_Point_t tex_rot_center,
                    r_drw2d_Texture_t *background,
                    MultiBufferSprite_t *MBSprite,
                    uint32_t is_left) {
    r_drw2d_Framebuffer_t *draw_fb;
    r_drw2d_Point_t shadow_rot_center;
    r_drw2d_Point_t shadow_tex_rot_center;
    static r_drw2d_Texture_t *needle = &gauge_needle2_texture;

    draw_fb = MbSpriteNewDrawBufGet(MBSprite);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);

/*     static r_drw2d_Texture_t *needle = &gauge_needle_texture; */
    shadow_rot_center.X = rot_center.X - R_DRW2D_2X(4);
    shadow_rot_center.Y = rot_center.Y + R_DRW2D_2X(4);
    shadow_tex_rot_center.X = tex_rot_center.X + R_DRW2D_2X(1);
    shadow_tex_rot_center.Y = tex_rot_center.Y + R_DRW2D_2X(3);
    
    //Refresh_Light(angle, background, is_left);
    //Refresh_RotationObject(angle, shadow_rot_center, shadow_tex_rot_center,      &gauge_shadow_texture, 0);
    Refresh_RotationObject(angle,
                           rot_center, tex_rot_center,
                           needle, 0);

}
uint8_t myspeedString[32];
#define MY_FONT_BG_COLOR             0x00000000
void Refresh_Speed_Text(MultiBufferSprite_t *MBSprite, unsigned char change_color)
{
	r_drw2d_Framebuffer_t *draw_fb;
	draw_fb = MbSpriteNewDrawBufGet(MBSprite);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);
	
	R_DRW2D_CtxBgColor(g_Drw2dDev, MY_FONT_BG_COLOR);            
    
    R_DRW2D_CtxTransformMode(g_Drw2dDev, R_DRW2D_TRANSFORM_2D);
    R_DRW2D_CtxIdentity(g_Drw2dDev);
   // R_DRW2D_CtxFillMode(g_Drw2dDev, R_DRW2D_FILLMODE_TEXTURE);	 
	 
    R_DRW2D_CtxBlendMode(g_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
	
	R_DRW2D_FramebufferClear(g_Drw2dDev);
	FlashDash_WriteString_Speed(0, 0, (char*)myspeedString, change_color, 0);
			

}


float64_t CalculateDistance(float32_t kph){
/*     const float64_t factor = 1 / 60.0 / 3600.0;w */
    const float64_t factor = 1 / 30.0 / 3600.0;
    static float64_t distance = 21543.5;
    float64_t delta;
    delta = factor * kph * KPH_FACTOR * 2;
    distance += delta;
    return distance;
}


uint8_t myodoString[10]="2254380";
uint8_t mytripString[10] = "1892 ";

float64_t distance;
void Refresh_Odo_Text(MultiBufferSprite_t *MBSprite, r_drw2d_Texture_t *background)
{
	r_drw2d_Rect_t draw_clear_rect;
	
	
	r_drw2d_Framebuffer_t *draw_fb;
	draw_fb = MbSpriteNewDrawBufGet(MBSprite);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);
	
	R_DRW2D_CtxBgColor(g_Drw2dDev, MY_FONT_BG_COLOR);   
		
	R_DRW2D_CtxTextureTransformMode(g_Drw2dDev, R_DRW2D_TEX_TRANSFORM_2D);
    R_DRW2D_CtxTextureIdentity(g_Drw2dDev);   
	
	R_DRW2D_FramebufferClear(g_Drw2dDev);
	
	
	distance = CalculateDistance(kph);
	
	sprintf((char*)myodoString,"%0.1f",distance);
	myodoString[7] = 0x20;		// space
	myodoString[8] = 0x3D;		// KM symbol
	
	
	mytripString[5] = 0x3D;		// KM symbol
	
	
	//odo_string = 0;
	//FlashDash_WriteString_Speed(0, 0, (char*)my_time_string, 0, 1);
	
	odo_string = 0;
	FlashDash_WriteString_Speed(0, 20, (char*)mytripString, 0, 1);
	
	odo_string = 1;
	FlashDash_WriteString_Speed(0, 80, (char*)myodoString, 0, 1);			

}

uint8_t my_time_string[10]="12:30 ";
uint8_t my_sec_count = 0;
uint8_t my_min_cnt = 0x30;
uint8_t my_hour_cnt = 0x12;


void Refresh_Time_Text(MultiBufferSprite_t *MBSprite)
{
	r_drw2d_Rect_t draw_clear_rect;
	static uint32_t my_last_time = 0;
	static uint8_t my_cnt = 0;
	
	r_drw2d_Framebuffer_t *draw_fb;
	draw_fb = MbSpriteNewDrawBufGet(MBSprite);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);
	
	R_DRW2D_CtxBgColor(g_Drw2dDev, MY_FONT_BG_COLOR);   
		
	R_DRW2D_CtxTextureTransformMode(g_Drw2dDev, R_DRW2D_TEX_TRANSFORM_2D);
    R_DRW2D_CtxTextureIdentity(g_Drw2dDev);   
	
	R_DRW2D_FramebufferClear(g_Drw2dDev);
	
	if ((R_TICK_GetTimeMS(0) - my_last_time) > 500)
	{
		my_sec_count++;
		
		if(my_sec_count >= 120)
		{
			my_sec_count = 0;
			my_time_string[4]++;
			if(my_time_string[4] > '9')
			{
				my_time_string[4] = '0';
				my_time_string[3]++;
				if(my_time_string[3] > '5')
				{
					my_time_string[3] = '0';
					my_time_string[1]++;
					if(my_time_string[1] > '9')
					{
						my_time_string[1] = '0';
						my_time_string[0]++;
						if(my_time_string[0] > '1')
						{
							my_time_string[0] = '0';
							my_time_string[1]++;
						
						}
					
					}
				
				}
			
			
			}
			
		
		}
		
		my_last_time = R_TICK_GetTimeMS(0);
		my_cnt++;
		if(my_cnt >1)
		{
			my_time_string[2] = 0x20;	// Space
			my_cnt = 0;
		}
		else
		{
			my_time_string[2] = 0x3A;	// :		
		}
	}	
	
	// my_time_string[0] = my_hour_cnt >> 4;
	// my_time_string[1] = my_hour_cnt && 0x0F;
	
	// my_time_string[3] = my_min_cnt >> 4;
	// my_time_string[4] = my_min_cnt && 0x0F;
	
	
	my_time_string[6] = 0x3F;	// PM symbol
	//my_time_string[5] = 0x3E;	// AM symbol
	
	odo_string = 0;
	FlashDash_WriteString_Speed(0, 0, (char*)my_time_string, 0, 1);
	

}
uint8_t mytempString[10]="25.6  ";
void Refresh_Temp_Text(MultiBufferSprite_t *MBSprite)
{
	r_drw2d_Rect_t draw_clear_rect;
	
	
	r_drw2d_Framebuffer_t *draw_fb;
	draw_fb = MbSpriteNewDrawBufGet(MBSprite);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);
	
	R_DRW2D_CtxBgColor(g_Drw2dDev, MY_FONT_BG_COLOR);   
		
	R_DRW2D_CtxTextureTransformMode(g_Drw2dDev, R_DRW2D_TEX_TRANSFORM_2D);
    R_DRW2D_CtxTextureIdentity(g_Drw2dDev);   
	
	R_DRW2D_FramebufferClear(g_Drw2dDev);
	
	mytempString[4] = 0x3B;		// for degree symbol
	mytempString[5] = 0x3C;		// for C symbol
	
	odo_string = 0;
	FlashDash_WriteString_Speed(0, 0, (char*)mytempString, 0, 1);
	

}

uint8_t mymodeString[10]="ECO";
//uint8_t mymodeString[10]="123";

void Refresh_Mode_Text(MultiBufferSprite_t *MBSprite)
{
	r_drw2d_Rect_t draw_clear_rect;
	
	
	r_drw2d_Framebuffer_t *draw_fb;
	draw_fb = MbSpriteNewDrawBufGet(MBSprite);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);
	
	R_DRW2D_CtxBgColor(g_Drw2dDev, MY_FONT_BG_COLOR);   
		
	R_DRW2D_CtxTextureTransformMode(g_Drw2dDev, R_DRW2D_TEX_TRANSFORM_2D);
    R_DRW2D_CtxTextureIdentity(g_Drw2dDev);   
	
	R_DRW2D_FramebufferClear(g_Drw2dDev);
	
	odo_string = 0;
	FlashDash_WriteString_Speed(0, 0, (char*)mymodeString, 0, 1);
	

}



void Refresh_Needle_Right(float64_t angle,float64_t old_angle,
                    r_drw2d_Point_t rot_center, 
                    r_drw2d_Point_t tex_rot_center,
                    r_drw2d_Texture_t *background,
                    MultiBufferSprite_t *MBSprite,
                    uint32_t is_left) {
    r_drw2d_Framebuffer_t *draw_fb;
    r_drw2d_Point_t shadow_rot_center;
    r_drw2d_Point_t shadow_tex_rot_center;
    static r_drw2d_Texture_t *needle = &gauge_needle1_texture;

    draw_fb = MbSpriteNewDrawBufGet(MBSprite);
    R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);

/*     static r_drw2d_Texture_t *needle = &gauge_needle_texture; */
    shadow_rot_center.X = rot_center.X - R_DRW2D_2X(4);
    shadow_rot_center.Y = rot_center.Y + R_DRW2D_2X(4);
    shadow_tex_rot_center.X = tex_rot_center.X + R_DRW2D_2X(1);
    shadow_tex_rot_center.Y = tex_rot_center.Y + R_DRW2D_2X(3);
    
    //Refresh_Light(angle, background, is_left);
    //Refresh_RotationObject(angle, shadow_rot_center, shadow_tex_rot_center,      &gauge_shadow_texture, 0);
    Refresh_RotationObject(angle,
                           rot_center, tex_rot_center,
                           needle, 0);

}




void Refresh_LeftNeedle(float32_t kph,
                        r_drw2d_Texture_t *background){
    r_drw2d_Point_t rot_center, tex_rot_center;
    volatile static float64_t angle = -32.44470326;
    static float64_t old_angle[2] = { -90, -90};
    static uint32_t  old_angle_index=0;
    r_drw2d_IntRect_t rect;
#if 1
    //angle = CalculateAngleLeft(kph);
	angle = CalculateAngleRight(kph);
#else
    angle += .5;
    if(angle == 180.0 && angle <= 180.0001){
        angle = angle;
    }
/*     if(angle >= 240) angle = 120; */
    if(angle >= 360) angle -= 360;
#endif
    Setup_LeftNeedleParameters(&rot_center, &tex_rot_center);
    Clear_Needle_Left(old_angle[old_angle_index], rot_center, tex_rot_center, &LeftNeedle, 1);
    MbSpriteDrawPosSet(&LeftNeedle, 0, 0);
    Setup_LeftNeedleParameters(&rot_center, &tex_rot_center);
    rect = Calc_Rect_Left(angle, rot_center, tex_rot_center, &LeftNeedle);
    MbSpriteDrawGeometrySet(&LeftNeedle, rect);    
    Setup_LeftNeedleParameters(&rot_center, &tex_rot_center);
    Refresh_Needle_Left(angle, old_angle[old_angle_index], rot_center, tex_rot_center, background, 
                   &LeftNeedle, 1);
    old_angle[old_angle_index] = angle;
    old_angle_index = IncIndex(old_angle_index);
}

void Refresh_RightNeedle(float32_t rpm,
                        r_drw2d_Texture_t *background){
    r_drw2d_Point_t rot_center, tex_rot_center;
    volatile static float64_t angle = -32.44470326;
    static float64_t old_angle[2] = { -90, -90};
    static uint32_t  old_angle_index=0;
    r_drw2d_IntRect_t rect;
#if 1
    angle = CalculateAngleRight(rpm);
#else
    angle += .5;
    if(angle == 180.0 && angle <= 180.0001){
        angle = angle;
    }
/*     if(angle >= 240) angle = 120; */
    if(angle >= 360) angle -= 360;
#endif
    Setup_RightNeedleParameters(&rot_center, &tex_rot_center);
    Clear_Needle_Right(old_angle[old_angle_index], rot_center, tex_rot_center, &RightNeedle, 1);
    MbSpriteDrawPosSet(&RightNeedle, 0, 0);
    Setup_RightNeedleParameters(&rot_center, &tex_rot_center);
    rect = Calc_Rect_Right(angle, rot_center, tex_rot_center, &RightNeedle);
    MbSpriteDrawGeometrySet(&RightNeedle, rect);    
    Setup_RightNeedleParameters(&rot_center, &tex_rot_center);
    Refresh_Needle_Right(angle, old_angle[old_angle_index], rot_center, tex_rot_center, background, 
                   &RightNeedle, 1);
    old_angle[old_angle_index] = angle;
    old_angle_index = IncIndex(old_angle_index);
}



static uint32_t volatile  frame_count=0;

uint32_t even_frame(void){
    return (0 == frame_count%2);
}

uint32_t odd_frame(void){
    return (1 == frame_count%2);
}

void break_frame_debug(void){
    if(frame_count%20 == 0){
        frame_count = frame_count;
    }
}



r_drw2d_Rect_t GetDrawRect(r_drw2d_IntRect_t int_rect){
    r_drw2d_Rect_t tmp;
    tmp.Pos.X       = R_DRW2D_2X(int_rect.Pos.X);
    tmp.Pos.Y       = R_DRW2D_2X(int_rect.Pos.Y);
    tmp.Size.Width  = R_DRW2D_2X(int_rect.Size.Width);
    tmp.Size.Height = R_DRW2D_2X(int_rect.Size.Height);
    return tmp;
}



unsigned int my_x = 280;
unsigned int my_y = 0;
unsigned int my_z = 0;
unsigned int my_x_fuel = 320;

extern r_drw2d_Texture_t background_texture;
extern unsigned char my_menu_cmd;
void Refresh_MainScene(void) 
{
	
	static unsigned char my_text = 0;
	static unsigned long my_odo_text = 0;
    static unsigned char change_color = 0;
/*	
	static uint32_t swap_done=1;   

	if(swap_done) {
        drw2d_fb.Buffer.Data = R_WM_WindowNewDrawBufGet(FLASHDASH_WM_UNIT, &g_Window);
        R_DRW2D_FramebufferSet(g_Drw2dDev, &drw2d_fb);

        swap_done = 0;
    }
*/	
    bench_start(BENCH_CPU_60);
    bench_start(BENCH_DRW2D_60);
    
	CarModelGetData(&kph, &rpm);		// simulate speed & RPM
	
	//---------- get real speed & rpm value -------------------
	//kph = my_speed_val;
	//rpm = my_rpm_val;
	
    Refresh_LeftNeedle(rpm, &my_speed_texture);//&background_texture);
	
	//--------------------------- Speed text related --------------------	
	my_text++;
	if(my_text >= 200)
		my_text = 0;
	
	my_text = (unsigned char) kph;
	
	if(my_text > 50)
	{
		change_color = 1;
	}
	else
	{
		change_color = 0;
	}
	sprintf((char*)myspeedString,"%d",my_text);
	Refresh_Speed_Text(&Speed_Sprite, change_color);
	//--------------------------- Odo & Trip text related --------------------
	
	Refresh_Odo_Text(&Odo_Sprite, &background_texture);
	
	//--------------------------- Time text related --------------------
	Refresh_Temp_Text(&Temp_Sprite);
	
	//--------------------------- Temperature text related --------------------
	Refresh_Time_Text(&Time_Sprite);
	
	//--------------------------- Mode text related --------------------
	//Refresh_Mode_Text(&Mode_Sprite);
	
	//Refresh_LeftNeedle((my_rpm_val >> 1), &my_speed_texture);
	//Refresh_RightNeedle(rpm, &my_tacho_texture);
    //Refresh_LeftGear(kph);
    //Refresh_DotMatrixDisplay();
    //Refresh_Ecometer(kph, rpm);
    bench_stop(BENCH_CPU_60);

    R_DRW2D_GpuFinish(g_Drw2dDev, R_DRW2D_FINISH_WAIT);
    bench_stop(BENCH_DRW2D_60);

    MbSpriteSwap(&LeftNeedle);
	MbSpriteSwap(&Speed_Sprite);
	MbSpriteSwap(&Odo_Sprite);
	MbSpriteSwap(&Temp_Sprite);
	MbSpriteSwap(&Time_Sprite);
	
	//MbSpriteSwap(&Mode_Sprite);
    //MbSpriteSwap(&LeftGear);
    //MbSpriteSwap(&Ecometer);
    if(even_frame()) {
        movie_count++; /* needed for blink sequence of turn indicators */
        if(movie_count > 1266) movie_count = 0;
        bench_start(BENCH_CPU_30);
#if DO_BENCH
        bench_start(BENCH_DRW2D_30);
#endif
        //Refresh_Indicators();
        //Refresh_Odometer(kph);
        bench_stop(BENCH_CPU_30);
#if DO_BENCH
        R_DRW2D_GpuFinish(g_Drw2dDev, R_DRW2D_FINISH_WAIT);
        bench_stop(BENCH_DRW2D_30);
#endif
    } else {
        //MbSpriteSwap(&Odometer);
    }    

	
	if(my_menu_cmd == 1)
	{
		my_move = 1;
		move_done = 0;
	}
	else if(my_menu_cmd == 2)
	{
		my_move = 0;
		move_done = 1;
	}

	

	if(my_move)
	{
	
			my_move = 0;
			R_WM_SpriteDisable(FLASHDASH_WM_UNIT, &Odo_Sprite.Sprite);
			R_WM_SpriteDisable(FLASHDASH_WM_UNIT, &Time_Sprite.Sprite);
			R_WM_SpriteEnable(FLASHDASH_WM_UNIT, &MenuSprite);
			

	}
	if(move_done)
	{
			move_done = 0;
			R_WM_SpriteDisable(FLASHDASH_WM_UNIT, &MenuSprite);
			R_WM_SpriteEnable(FLASHDASH_WM_UNIT, &Odo_Sprite.Sprite);
			R_WM_SpriteEnable(FLASHDASH_WM_UNIT, &Time_Sprite.Sprite);
			
		
	
	}

	 //FlashDash_DrawPerfomance(&drw2d_fb);
	 
	//my_write_text(0);
	//R_DRW2D_GpuFinish(g_Drw2dDev, R_DRW2D_FINISH_WAIT);
	//R_WM_WindowSwap(FLASHDASH_WM_UNIT, &speed_Window);

	//my_write_text(1);
	//R_DRW2D_GpuFinish(g_Drw2dDev, R_DRW2D_FINISH_WAIT);
	//R_WM_WindowSwap(FLASHDASH_WM_UNIT, &odo_Window);
	
	
    R_WM_FrameEndMark(FLASHDASH_WM_UNIT, 0);
    R_WM_FrameWait(FLASHDASH_WM_UNIT, 0);
    frame_count++;
}


void Init_Background(void)
{   
    /* LeftGear, RightGear, CenterMovie; */
    r_drw2d_Rect_t rect = { { 0, 0 }, { R_DRW2D_2X(800), R_DRW2D_2X(480) } };    

	R_DRW2D_CtxImgQuality(g_Drw2dDev, R_DRW2D_IMGQUALITY_HIGH);

    drw2d_fb.Flags = (r_drw2d_FramebufferFlags_t)0;
    drw2d_fb.Handle = 0; /* not used currently */
    drw2d_fb.Buffer.Pitch = g_Window.Pitch;
    drw2d_fb.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB8888;
    drw2d_fb.Buffer.Size.Width = g_Window.Width;
    drw2d_fb.Buffer.Size.Height = g_Window.Height;
    drw2d_fb.Buffer.Data = R_WM_WindowNewDrawBufGet(FLASHDASH_WM_UNIT, &g_Window);
    R_DRW2D_FramebufferSet(g_Drw2dDev, &drw2d_fb);
#ifdef R_USE_FASTSOFTDAVE
    R_UTIL_DHD_FSDRegBuf(drw2d_fb.Buffer.Data, drw2d_fb.Buffer.Pitch * drw2d_fb.Buffer.Size.Height * 4);
#endif
    /* draw the bkg */
    R_DRW2D_CtxFillMode(g_Drw2dDev, R_DRW2D_FILLMODE_TEXTURE);
    R_DRW2D_CtxTextureTransformMode(g_Drw2dDev, R_DRW2D_TEX_TRANSFORM_NONE);
    R_DRW2D_CtxTextureSet(g_Drw2dDev, 0u, &background_texture);
    R_DRW2D_CtxBlendMode(g_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
    R_DRW2D_DrawRect(g_Drw2dDev, &rect);    
	
	R_DRW2D_GpuFinish(g_Drw2dDev, R_DRW2D_FINISH_WAIT);
	
	R_WM_WindowSwap(FLASHDASH_WM_UNIT, &g_Window); 
	
	R_WM_FrameEndMark(FLASHDASH_WM_UNIT, 0); 
    R_WM_FrameWait(FLASHDASH_WM_UNIT, 0); 
	
	
}


void FlashDash_OpeningScene(void)
{
	 void *fb;
    int size;
    fb = R_WM_WindowNewDrawBufGet(FLASHDASH_WM_UNIT, &g_Window);
    drw2d_fb.Flags = (r_drw2d_FramebufferFlags_t)0;
    drw2d_fb.Handle = 0; /* not used currently */
    drw2d_fb.Buffer.Pitch = g_Window.Pitch;
    drw2d_fb.Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB8888;
    drw2d_fb.Buffer.Size.Width = g_Window.Width;
    drw2d_fb.Buffer.Size.Height = g_Window.Height;
    drw2d_fb.Buffer.Data = fb;
    R_DRW2D_FramebufferSet(g_Drw2dDev, &drw2d_fb);

    R_DRW2D_CtxBgColor(g_Drw2dDev, 0xff0000ff);
    R_DRW2D_FramebufferClear(g_Drw2dDev);

	R_DRW2D_GpuFinish(g_Drw2dDev, R_DRW2D_FINISH_WAIT);
    R_WM_WindowSwap(FLASHDASH_WM_UNIT, &g_Window);
    R_WM_FrameEndMark(FLASHDASH_WM_UNIT, 0);
    R_WM_FrameWait(FLASHDASH_WM_UNIT, 0);


}

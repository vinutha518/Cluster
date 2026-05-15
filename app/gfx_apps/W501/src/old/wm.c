#include "r_typedefs.h"
#include "r_ddb_api.h"
#include "r_dev_api.h"  /* For PixelClock/FPS Information */
#include "r_bsp_stdio_api.h"
#include "r_cdi_api.h"
#include "r_wm_api.h"
#include "mem_init.h"
#include "r_drw2d_os.h"
#include "r_drw2d_api.h"
#include "r_config_drw2d.h"
#include "r_drw2d_ctx_dhd.h"
#include "sprite.h"
#include "wm.h"
#include "init.h"
#include "main_scene.h"
#include "load_to_ram.h"
#include "bench.h"
#include "r_vdce_api.h"
//#include "r_gfxutils_api.h"


#ifdef _WINDOWS
//#include <r_config_d1mxsim.h>
//#include <r_d1mxsim_api.h>
#endif

/*******************************************************************************
Section: Local defines
*/
#define WM_MSG_QUEUE_LEN 32

//#define LOC_DISPLAY_NAME   "CCT_FCA_TFT_800_480"
// #define LOC_DISPLAY_NAME   "1280x480_60Hz"
//#define LOC_DISPLAY_NAME   "TIANMA_TFT_800_480"
#define LOC_DISPLAY_NAME   "KOE_TFT_800_480"
/* #define LOC_DISPLAY_NAME   "VESA_640x480_60Hz" */
/* #define LOC_DISPLAY_NAME "VESA_640x480_60Hz" */
/*******************************************************************************
Section: Local variables

*/
static r_wm_Msg_t loc_WmMsgQueue[WM_MSG_QUEUE_LEN];
uint32_t g_ScrWidth;
uint32_t g_ScrHeight;

r_wm_Window_t g_Window;
r_wm_Window_t WindowNeedle;
r_wm_Window_t WindowMiddle;
r_wm_Window_t speed_Window;

r_drw2d_Buffer_t dst;

MultiBufferSprite_t LeftNeedle, RightNeedle;

static uint32_t Layer2UnderFlow = 0;

r_wm_Sprite_t MenuSprite;
r_wm_Sprite_t CitySprite;
r_wm_Sprite_t SpeedSprite;
r_wm_Sprite_t TachoSprite;

extern const unsigned char locImgData[];
extern const unsigned char my_new_bg[];
extern const unsigned char my_middle_patch[];
extern const unsigned char race_on[];
extern const unsigned char high_beam_on[];
extern const unsigned char engine_on[];
extern const unsigned char eco_on[];
extern const unsigned char bt_on[];
extern const unsigned char alert_right[];
extern const unsigned char alert_left[];

extern const unsigned char my_menu_patch[];

extern r_drw2d_Texture_t my_middle_patch_texture;
extern r_drw2d_Texture_t my_menu_patch_texture;
extern r_drw2d_Texture_t background_texture;

r_drw2d_Texture_t my_speed_texture;


void Layer2UnderFlowCB(void){
    Layer2UnderFlow++;
}

void InitInterrupt(void) {
    R_VDCE_IntcCallbackSet(0, R_VDCE_INTC_ERR_LAYER2_UNDERFLOW, &Layer2UnderFlowCB);
    R_VDCE_IntcEnable(0, R_VDCE_INTC_ERR_LAYER2_UNDERFLOW);
}

void WM_Error (uint32_t Unit, r_wm_Error_t Error){
    static uint32_t block = 1;
    for(;block;);
}


/*******************************************************************************
Function: FlashDash_InitWm
*/


uint64_t FrameCount = 0;
void WmCallBack(uint32_t Unit, const r_wm_Event_t* Event){
    switch (Event->Id){
    case R_WM_EVENT_VBLANK:
        FrameCount++;
        break;
    case R_WM_EVENT_SCANLINE:
        break;
    }
}


void FlashDash_InitWm(void)
{
    static r_ddb_Timing_t *timing;
#if 1
#if 0 /* For D1M1 (no H)*/
    r_ddb_Timing_t custom_timing=
        {"TRULY_WVGA_TFT_800x480",
         800u,480u,
         {952u-80,952-800u-80,40u-30,48u},   /* Changed from 928 to 952 */
         {525u-25,525-480u-25,13u, 3u},
         ((uint32_t)R_DDB_DISP_FLAG_VOEN | (uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
         26600000u};    /* Will have no effect, PLL will align this to 30MHz */
#else
    /* For d1m1h timing is a little bit different because of pixelclock divider */
    r_ddb_Timing_t custom_timing=
        {"TRULY_WVGA_TFT_800x480",
         800u,480u,
         {928,128u,40u,48u},
         {525u,45u,13u,3u},
         ((uint32_t)R_DDB_DISP_FLAG_VOEN | (uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
         30000000u};
		 
		 /* {"HITACHI_WVGA_TFT_800x480",
     800u,480u,
     {1056u,47u,40u,3u},
     {525u,15u,11u,2u},
     ((uint32_t)R_DDB_DISP_FLAG_VOEN | (uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
     25264000u}; */
#endif
/*     r_ddb_Timing_t *timing, custom_timing= */
/*     {"VESA_640x480_60Hz", */
/*       640u, 480u, */
/*       {800u, 160u, 16u, 96u}, */
/*       {525u,  45u, 10u, 2u}, */
/*       ((uint32_t)R_DDB_DISP_FLAG_VOEN */
/*       |(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI), */
/*      27175000u}; */
#endif
#if 0
    r_ddb_Timing_t custom_timing=
        {"NEC_QVGA_TOUCH_240x320",
         240u, 320u,
         {256u+16, 16u+16, 4u, 8u},
         {324u,  4u, 1u, 2u},
         R_DDB_DISP_FLAG_NONE,
/*          5000000u}; */
         10000000u};
#endif

    r_wm_Error_t wmErr;
 
    wmErr = R_WM_DevInit(FLASHDASH_WM_UNIT, loc_WmMsgQueue, WM_MSG_QUEUE_LEN, WmCallBack,
                         &IramHeap, AppHeap_p);
    
    R_WM_DevEventRegister  (FLASHDASH_WM_UNIT, R_WM_EVENT_VBLANK, 0);

    if (R_WM_ERR_OK != wmErr)
    {
    	FlashDash_Error();
    }

    R_WM_ErrorCallbackSet(FLASHDASH_WM_UNIT, WM_Error);


    timing = R_DDB_GetDisplayTiming((const char*)LOC_DISPLAY_NAME);
/*     timing = &custom_timing; */
    g_ScrWidth  = timing->ScreenWidth;
    g_ScrHeight = timing->ScreenHeight;
/*     g_ScrWidth =  240; */
/*     g_ScrHeight = 320; */

    //timing = &custom_timing;
/*     timing = R_DDB_GetDisplayTiming((const char*)"NEC_QVGA_TOUCH_240x320"); */
/*     timing = R_DDB_GetDisplayTiming((const char*)"VESA_640x480_60Hz"); */


    R_WM_ScreenTimingSet(FLASHDASH_WM_UNIT, timing);

#if 0
    // ----------- for KOE ---------------------------
    wmErr = R_WM_ScreenColorFormatSet(FLASHDASH_WM_UNIT, (R_WM_OUTCOLORFMT_RGB666| R_WM_OUTCOLORFMT_FLAG_SWAP_BR));	// output is 666 and RGB is swapped to become BGR
    if(R_WM_ERR_OK != wmErr)
    {
       FlashDash_Error();
    }

#endif    

#if 0
    // ----------- for TIANMA ---------------------------
    wmErr = R_WM_ScreenColorFormatSet(FLASHDASH_WM_UNIT, (R_WM_OUTCOLORFMT_RGB888| R_WM_OUTCOLORFMT_FLAG_ENDIAN));	// output is 666 and RGB is swapped to become BGR
    if(R_WM_ERR_OK != wmErr)
    {
       FlashDash_Error();
    }
#endif

    // for FCA TFT no manipulation of RGB bits required
    
    R_WM_ScreenDisable(FLASHDASH_WM_UNIT);
    R_WM_ScreenEnable(FLASHDASH_WM_UNIT);
    R_WM_ScreenBgColorSet(FLASHDASH_WM_UNIT, 0, 0, 0);
    
    /* Screen Refresh Rate Debug 
        
       The EIZO monitor seems to drop frames, when the refresh rate 
       leaves its specified range of 59Hz - 61Hz.
       
       The following lines of code shown the "real" frequency rather the the requested frequency.
       These values may differ due to quantisation caused by the PLL.
       
       The problem could be solved my settings the refresh rate from 61.6Hz to 60Hz.
    */
    if (0) {
        uint32_t f_bus = R_DEV_BusFrequencyHz(R_DEV_CLK_PLL2);
        uint32_t f_ddb = timing->PixelClock;
        
        uint32_t div = f_bus/f_ddb;
        
        uint32_t f_pix_0 = f_bus/div;
        uint32_t f_pix_1 = f_bus/(div+1);
        
        uint32_t f_pix = ((f_pix_0-f_ddb)<(f_ddb-f_pix_1))?f_pix_0:f_pix_1;
        
        float32_t fps = ((float32_t)f_pix / (float32_t)timing->H.Total) / (float32_t)timing->V.Total;
        
        R_BSP_STDIO_Printf("PixelClock PLL: %u Hz\n", f_pix);
        R_BSP_STDIO_Printf("Screen Refresh: %3.2f Hz\n", fps);
    }
}

//======================= MAIN Background Window =========================================

void CreateSpeedWin(r_wm_Window_t *Window)
{
    Window->Mode = R_WM_WINMODE_FB;//R_WM_WINMODE_SPRITES;
    Window->ColorFmt = R_WM_COLORFMT_ARGB8888;
    Window->Alpha = 0xff;
    Window->PosX = 0;//160;
    Window->PosY = 0;
    Window->PosZ = 0;
    Window->Width = 800;//g_ScrWidth;
    Window->Height = 480;//g_ScrHeight;
    Window->Pitch = 800;//2048;

	Window->Surface.Fb.BufNum = 2; 
    Window->Surface.Fb.Buffer = 0; 
    Window->Surface.Fb.BufMode = R_WM_WINBUF_ALLOC_INTERNAL; 
	
    R_WM_WindowCreate(FLASHDASH_WM_UNIT, Window);
    R_WM_WindowEnable(FLASHDASH_WM_UNIT, Window);
}

void CreateSpeedTextWin(r_wm_Window_t *Window)
{

    Window->Mode = R_WM_WINMODE_SPRITES;
    Window->ColorFmt = R_WM_COLORFMT_ARGB8888;
    Window->Alpha = 0xff;
    Window->PosX = 0;
    Window->PosY = 0;
    Window->PosZ = 3;
    Window->Width = 800;
    Window->Height = 480;
    Window->Pitch = 2048;
	Window->UsePremultipliedAlpha = 1;


    R_WM_WindowCreate(FLASHDASH_WM_UNIT, Window); 
    R_WM_WindowEnable(FLASHDASH_WM_UNIT, Window); 

}
MultiBufferSprite_t Speed_Sprite;
MultiBufferSprite_t Odo_Sprite;
MultiBufferSprite_t Time_Sprite;
MultiBufferSprite_t Temp_Sprite;
MultiBufferSprite_t Mode_Sprite;

void CreateSpeedTextSprite(r_wm_Window_t *Window)
{
	r_wm_Sprite_t tmp;
	tmp.PosX   = 360;
    tmp.PosY   = 192;
    tmp.PosZ   = 0;
    tmp.Width  = 128; /* User smaller needle win and change geometry all the time */
    tmp.Height = 80;
    tmp.Window = Window;
    InitMultiBufferSprite(&Speed_Sprite, tmp, 2);
}

void CreateOdoTripSprite(r_wm_Window_t *Window)
{
	r_wm_Sprite_t tmp;
	tmp.PosX   = 30;//50;
    tmp.PosY   = 180;//192;
    tmp.PosZ   = 0;
    tmp.Width  = 200;
    tmp.Height = 120;
    tmp.Window = Window;
    InitMultiBufferSprite(&Odo_Sprite, tmp, 2);
}

void CreateTimeSprite(r_wm_Window_t *Window)
{
	r_wm_Sprite_t tmp;
	tmp.PosX   = 30;//0;//50;
    tmp.PosY   = 10;//0;//192;
    tmp.PosZ   = 0;
    tmp.Width  = 140;
    tmp.Height = 40;
    tmp.Window = Window;
    InitMultiBufferSprite(&Time_Sprite, tmp, 2);
}

void CreateTempSprite(r_wm_Window_t *Window)
{
	r_wm_Sprite_t tmp;
	tmp.PosX   = 680;//50;
    tmp.PosY   = 10;//0;//192;
    tmp.PosZ   = 0;
    tmp.Width  = 120;
    tmp.Height = 40;
    tmp.Window = Window;
    InitMultiBufferSprite(&Temp_Sprite, tmp, 2);
}

void CreateModeSprite(r_wm_Window_t *Window)
{
	r_wm_Sprite_t tmp;
	tmp.PosX   = 360;
    tmp.PosY   = 360;//0;//192;
    tmp.PosZ   = 0;
    tmp.Width  = 100;
    tmp.Height = 40;
    tmp.Window = Window;
    InitMultiBufferSprite(&Mode_Sprite, tmp, 2);
}


void CreateMenuWin(r_wm_Window_t *Window)
{

    Window->Mode = R_WM_WINMODE_SPRITES;
    Window->ColorFmt = R_WM_COLORFMT_ARGB8888;
    Window->Alpha = 0xff;
    Window->PosX = 0;
    Window->PosY = 0;
    Window->PosZ = 3;
    Window->Width = 240;//64;//192;
    Window->Height = 350;//96;//32;//128;
    Window->Pitch = 2048;

    Window->UsePremultipliedAlpha = 1; 
     
    R_WM_WindowCreate(FLASHDASH_WM_UNIT, Window); 
    R_WM_WindowEnable(FLASHDASH_WM_UNIT, Window); 

}

void CreateMenuSprite(r_wm_Window_t *Window)
{

	r_wm_Sprite_t tmp; 

	 
    tmp.PosX   = 0;
    tmp.PosY   = 0;
    tmp.PosZ   = 0; 
    tmp.Width  = 800; 
    tmp.Height = 480; 
    tmp.Window = Window;     
    tmp.Data   = background_texture.Buffer.Data;//&locImgData; 
    MenuSprite = tmp; 
	 
	R_WM_SpriteCreate(FLASHDASH_WM_UNIT, &MenuSprite); 
    R_WM_SpriteEnable(FLASHDASH_WM_UNIT, &MenuSprite);
	// R_WM_SpriteDisable(FLASHDASH_WM_UNIT, &MenuSprite);

}

void CreateNeedleWin(r_wm_Window_t *Window)
{
    /* Contains left and right needle */
    Window->Mode     = R_WM_WINMODE_SPRITES;
    Window->ColorFmt = R_WM_COLORFMT_ARGB8888;
    Window->Alpha    = 0xff;
    Window->PosX     = 0;//165;
    Window->PosY     = 0;//10;
    Window->PosZ     = 1;
    Window->Width    = 800;//464;
    Window->Height   = 480;//380;
    Window->Pitch    = 2048;
    Window->UsePremultipliedAlpha = 1;
    
    R_WM_WindowCreate(FLASHDASH_WM_UNIT, Window);
    R_WM_WindowEnable(FLASHDASH_WM_UNIT, Window);

}

void CreateNeedle1Sprites(r_wm_Window_t *Window) {
    r_wm_Sprite_t tmp;
    /* MultiBufferSprite_t LeftNeedle, RightNeedle */

    tmp.PosX   = 0;
    tmp.PosY   = 0;
    tmp.PosZ   = 0;
    tmp.Width  = 220; /* User smaller needle win and change geometry all the time */
    tmp.Height = 220;
    tmp.Window = Window;
    InitMultiBufferSprite(&LeftNeedle, tmp, 2);

}

void CreateMiddlePatch(r_wm_Window_t *Window)
{
	r_wm_Error_t wmErr;
	
    /* Contains left and right needle */
    Window->Mode     = R_WM_WINMODE_SPRITES;
    Window->ColorFmt = R_WM_COLORFMT_ARGB8888;
    Window->Alpha    = 0xff;
    Window->PosX     = 0;//295;
    Window->PosY     = 0;//135;
    Window->PosZ     = 2;
    Window->Width    = 800;//210;//464;
    Window->Height   = 480;//210;//380;
    Window->Pitch    = 2048;
    Window->UsePremultipliedAlpha = 1;
    
	
    wmErr = R_WM_WindowCreate(FLASHDASH_WM_UNIT, Window);
	 if(R_WM_ERR_OK != wmErr)
    {
       FlashDash_Error();
    }
	
    wmErr = R_WM_WindowEnable(FLASHDASH_WM_UNIT, Window);
	if(R_WM_ERR_OK != wmErr)
    {
       FlashDash_Error();
    }
}


void CreateCityModeSprites(r_wm_Window_t *Window)
{
    r_wm_Sprite_t tmp;
	r_wm_Error_t wmErr;
	//dst.Data     = &my_mode_city;
	//dst.Data     = &my_middle_patch;
	
	//my_middle_patch_texture.Buffer = dst;
	
    tmp.PosX   = 295;//0;//102;//0;//MIDDLE_BKG_LEFT - WIN_MAIN_LEFT;
    tmp.PosY   = 135;//0;//322;//0;//MIDDLE_BKG_TOP  - WIN_MAIN_TOP;
    tmp.PosZ   = 0;
    tmp.Width  = 210;
    tmp.Height = 210;
    tmp.Window = Window;    
    tmp.Data   = my_middle_patch_texture.Buffer.Data;
    CitySprite = tmp;
	
	wmErr = R_WM_SpriteCreate(FLASHDASH_WM_UNIT, &CitySprite);
	if(R_WM_ERR_OK != wmErr)
    {
       FlashDash_Error();
    }
    wmErr = R_WM_SpriteEnable(FLASHDASH_WM_UNIT, &CitySprite);
	if(R_WM_ERR_OK != wmErr)
    {
       FlashDash_Error();
    }
}

void FlashDash_CreateUi(void)
{
	//CarModelInit();
    InitInterrupt();
	
	//-------------- Main Background ------------------------------------------------
//	CreateSpeedWin(&g_Window);
//	R_WM_FrameEndMark(FLASHDASH_WM_UNIT, 0);
//	R_WM_FrameWait(FLASHDASH_WM_UNIT, 0);
	
	//----------------- Window for Needle -------------------------------------------
	
	CreateNeedleWin(&WindowNeedle);
	// CreateNeedle1Sprites(&WindowNeedle);
	//	CreateNeedle2Sprites(&WindowNeedle);
	R_WM_FrameEndMark(FLASHDASH_WM_UNIT, 0);
	R_WM_FrameWait(FLASHDASH_WM_UNIT, 0);
    
	//------------------ Window for Center Patch --------------------------------------
	
//	CreateMiddlePatch(&WindowMiddle);
//	CreateCityModeSprites(&WindowMiddle);
	
//	R_WM_FrameEndMark(FLASHDASH_WM_UNIT, 0);
 //   R_WM_FrameWait(FLASHDASH_WM_UNIT, 0);
    
	//------------------ Window for TEXTs ----------------------------------------------
	
 //   CreateSpeedTextWin(&speed_Window);
//	CreateSpeedTextSprite(&speed_Window);
//	CreateOdoTripSprite(&speed_Window);
//	CreateTempSprite(&speed_Window);
//	CreateTimeSprite(&speed_Window);
	CreateMenuSprite(&WindowNeedle);
//	CreateModeSprite(&speed_Window);
	
	R_WM_FrameEndMark(FLASHDASH_WM_UNIT, 0);
	R_WM_FrameWait(FLASHDASH_WM_UNIT, 0);
	while(1);
	Init_MainScene();
	Init_MainScene();
	
}

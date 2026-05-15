#ifndef WM_INIT_1H_
#define WM_INIT_1H_


#ifdef __cplusplus
extern "C"
{
#endif

#define FLASHDASH_WM_UNIT 0

extern uint32_t g_ScrWidth;
extern uint32_t g_ScrHeight;

extern r_wm_Window_t WindowMain;
extern r_wm_Window_t WindowNeedle;
extern r_wm_Window_t WindowGears;

extern r_wm_Sprite_t MiddleBkg, LeftBkg, RightBkg;
extern r_wm_Sprite_t Abs, Battery, TurnLeft, TurnRight, Damage, Tank, GlowPlug, Oil;

    extern MultiBufferSprite_t LeftNeedle, LeftGear, Ecometer, Odometer;

extern r_drw2d_Texture_t current_cache_texture;
extern r_drw2d_Texture_t next_cache_texture;

extern r_drw2d_Texture_t my_speed_texture;
extern r_drw2d_Texture_t my_tacho_texture;

extern MultiBufferSprite_t LeftNeedle, RightNeedle;

extern r_wm_Sprite_t SpeedSprite;
extern r_wm_Sprite_t TachoSprite;
extern r_wm_Sprite_t MenuSprite;

void FlashDash_CreateUi(void);
void FlashDash_InitWm(void);

extern uint64_t FrameCount;

#ifdef __cplusplus
}
#endif

#endif

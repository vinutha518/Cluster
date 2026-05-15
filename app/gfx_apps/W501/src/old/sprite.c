#include "r_typedefs.h"
#include "r_ddb_api.h"
#include "r_cdi_api.h"
#include "r_wm_api.h"

#include "mem_init.h"
#include "r_drw2d_os.h"
#include "r_drw2d_api.h"
#include "r_config_drw2d.h"
#include "r_drw2d_ctx_dhd.h"
//#include "r_gfxutils_api.h"
#include "r_bsp_stdio_api.h"
#include "sprite.h"
#include "wm.h"


void InitMultiBufferSprite(MultiBufferSprite_t *MBSprite, r_wm_Sprite_t Sprite,
                           uint32_t Count) {
    int i;
    r_drw2d_Framebuffer_t *DrawFb_p;
    MBSprite->Count = Count;
    MBSprite->VoIndex = 0;
    MBSprite->DrawIndex = MBSprite->VoIndex + 1;
    if (MBSprite->DrawIndex > (MBSprite->Count-1)) {
        MBSprite->DrawIndex = 0;
    }
    
    MBSprite->Sprite = Sprite;

    //R_GFXUTILS_ASSERT(Count <= 4);

    for(i = 0;i < MBSprite->Count;i++) {
        MBSprite->Buffer[i] = R_CDI_Alloc(AppHeap_p,
                                          MBSprite->Sprite.Width * MBSprite->Sprite.Height * 4);
        //R_GFXUTILS_ASSERT(MBSprite->Buffer[i]);
        MBSprite->Pos[i].X       = MBSprite->Sprite.PosX;
        MBSprite->Pos[i].Y       = MBSprite->Sprite.PosY;
        MBSprite->Size[i].Width  = MBSprite->Sprite.Width;
        MBSprite->Size[i].Height = MBSprite->Sprite.Height;
    }
    MBSprite->Sprite.Data = MBSprite->Buffer[MBSprite->VoIndex];

    DrawFb_p = &MBSprite->DrawFb;

    DrawFb_p->Flags = (r_drw2d_FramebufferFlags_t)0;
    DrawFb_p->Handle = 0; /* not used currently */
    DrawFb_p->Buffer.Pitch = MBSprite->Sprite.Width;
    DrawFb_p->Buffer.PixelFormat = R_DRW2D_PIXELFORMAT_ARGB8888; /* Currently only ARGB8888 */
    DrawFb_p->Buffer.Size.Width = MBSprite->Sprite.Width;
    DrawFb_p->Buffer.Size.Height = MBSprite->Sprite.Height;
    DrawFb_p->Buffer.Data = MBSprite->Buffer[MBSprite->DrawIndex];;

    R_WM_SpriteCreate(FLASHDASH_WM_UNIT, &MBSprite->Sprite);
    R_WM_SpriteEnable(FLASHDASH_WM_UNIT, &MBSprite->Sprite);
}


r_drw2d_Framebuffer_t *MbSpriteNewDrawBufGet(MultiBufferSprite_t *MBSprite){
    uint32_t cur;
    r_drw2d_Framebuffer_t *DrawFb_p;
    DrawFb_p = &MBSprite->DrawFb;
    cur = MBSprite->DrawIndex;
    MBSprite->DrawFb.Buffer.Data = MBSprite->Buffer[cur];
    DrawFb_p->Buffer.Pitch       = MBSprite->Size[cur].Width;
    DrawFb_p->Buffer.Size.Width  = MBSprite->Size[cur].Width;
    DrawFb_p->Buffer.Size.Height = MBSprite->Size[cur].Height;
    return &MBSprite->DrawFb;
}

void MbSpriteDrawPosSet(MultiBufferSprite_t *MBSprite,uint32_t x, uint32_t y) {
    uint32_t cur;
    cur = MBSprite->DrawIndex;
    MBSprite->Pos[cur].X = x;
    MBSprite->Pos[cur].Y = y;
}

void MbSpriteDrawGeometrySet(MultiBufferSprite_t *MBSprite,r_drw2d_IntRect_t rect) {
    uint32_t cur;
    cur = MBSprite->DrawIndex;
    MBSprite->Pos[cur]  = rect.Pos;
    MBSprite->Size[cur] = rect.Size;
}

void MbSpriteSwap(MultiBufferSprite_t *MBSprite){
    r_wm_Sprite_t *cur_sprite;
    uint32_t cur;
    MBSprite->VoIndex++;
    if(MBSprite->VoIndex >= MBSprite->Count) MBSprite->VoIndex = 0;
    MBSprite->DrawIndex++;
    if(MBSprite->DrawIndex >= MBSprite->Count) MBSprite->DrawIndex = 0;
    cur = MBSprite->VoIndex;
    cur_sprite = &MBSprite->Sprite;
    cur_sprite->PosX   = MBSprite->Pos[cur].X;
    cur_sprite->PosY   = MBSprite->Pos[cur].Y;
    cur_sprite->Width  = MBSprite->Size[cur].Width;
    cur_sprite->Height = MBSprite->Size[cur].Height;
    R_WM_SpriteBufSet(FLASHDASH_WM_UNIT, cur_sprite,
                      MBSprite->Buffer[cur]);
}

void MbSpriteWaitForSwap(MultiBufferSprite_t *MBSprite){
    /* Check if address is already taken by system */
    while(MBSprite->Buffer[MBSprite->VoIndex] != MBSprite->Sprite.Data);
    /* There must be a wait for VSYNC also ? Not necessary as it would not be visible anymore ? */
}

uint32_t MbSpriteDrawPosXGet(MultiBufferSprite_t *MBSprite){
    uint32_t cur;
    cur = MBSprite->DrawIndex;    
    return MBSprite->Pos[cur].X;
}
uint32_t MbSpriteDrawPosYGet(MultiBufferSprite_t *MBSprite){
    uint32_t cur;
    cur = MBSprite->DrawIndex;    
    return MBSprite->Pos[cur].Y;
}

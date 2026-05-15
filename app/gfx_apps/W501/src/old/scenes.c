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
//#include "fs_data_movie.h"
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



/* static r_drw2d_Framebuffer_t drw2d_fb; */
static r_drw2d_Buffer_t      conv_buf;


r_drw2d_Rect_t CreateRectFromTexture(r_drw2d_Texture_t *texture_p, int32_t x,int32_t y){
    r_drw2d_Rect_t tmp;
    tmp.Pos.X = x;
    tmp.Pos.Y = y;
/*     tmp.Size.Width  = texture_p->Buffer.Size.Width  * rect_scale_factor; */
/*     tmp.Size.Height = texture_p->Buffer.Size.Height * rect_scale_factor; */
    tmp.Size.Width  = R_DRW2D_2X(texture_p->Buffer.Size.Width);
    tmp.Size.Height = R_DRW2D_2X(texture_p->Buffer.Size.Height);
    return tmp;
}


void CopyTextureFrb(r_drw2d_Texture_t *texture_p, uint32_t PosX, uint32_t PosY) {
    r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
    r_drw2d_Rect_t  rect;


    ret |= R_DRW2D_CtxTextureIdentity(g_Drw2dDev);
    ret |= R_DRW2D_CtxIdentity(g_Drw2dDev);
    ret |= R_DRW2D_CtxTextureSet(g_Drw2dDev, 0, texture_p);

    ret |= R_DRW2D_CtxFillMode(g_Drw2dDev,  R_DRW2D_FILLMODE_TEXTURE);
    ret |= R_DRW2D_CtxBlendMode(g_Drw2dDev, R_DRW2D_BLENDMODE_SRC_OVER);

    rect.Pos.X       = PosX;
    rect.Pos.Y       = PosY;
    rect.Size.Width  = R_DRW2D_2X(texture_p->Buffer.Size.Width);
    rect.Size.Height = R_DRW2D_2X(texture_p->Buffer.Size.Height);
    
    ret |= R_DRW2D_DrawRect(g_Drw2dDev, &rect);

    //R_GFXUTILS_ASSERT(R_DRW2D_ERR_OK == ret);    
}

void BlurTextureFrb(r_drw2d_Texture_t *texture_p, r_drw2d_Framebuffer_t *frb_p, uint32_t is_x);

void Blur_Buffer(r_drw2d_Buffer_t *src_p, r_drw2d_Buffer_t *dst_p){
    r_drw2d_Texture_t     texture;
    r_drw2d_Framebuffer_t frb;

    R_DRW2D_CtxStripingEnable(g_Drw2dDev);

    texture.Handle = R_NULL;
    texture.Flags  = R_DRW2D_TEX_VT;
    texture.Buffer = *src_p;

    frb.Handle = R_NULL;
    frb.Flags  = (r_drw2d_FramebufferFlags_t)0;
    frb.Buffer = conv_buf;
#if 1
    BlurTextureFrb(&texture,&frb,1);
#endif
#if 1
    texture.Buffer = conv_buf;
    frb.Buffer = *dst_p;
    BlurTextureFrb(&texture,&frb,0);
    R_DRW2D_CtxStripingDisable(g_Drw2dDev);
#endif
}


void BlurTextureFrb(r_drw2d_Texture_t *texture_p, r_drw2d_Framebuffer_t *frb_p, uint32_t is_x) {
    r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
    r_drw2d_IntRect_t  rect;


    ret |= R_DRW2D_CtxTextureIdentity(g_Drw2dDev);
    ret |= R_DRW2D_CtxIdentity(g_Drw2dDev);
    ret |= R_DRW2D_CtxTextureSet(g_Drw2dDev, 0, texture_p);

    ret |= R_DRW2D_CtxFillMode(g_Drw2dDev,  R_DRW2D_FILLMODE_TEXTURE);
    ret |= R_DRW2D_CtxBlendMode(g_Drw2dDev, R_DRW2D_BLENDMODE_SRC_OVER);

    rect.Pos.X       = 0;
    rect.Pos.Y       = 0;
    rect.Size.Width  = texture_p->Buffer.Size.Width;
    rect.Size.Height = texture_p->Buffer.Size.Height;
    
    R_DRW2D_FramebufferSet(g_Drw2dDev, frb_p);
    if(is_x) {
        R_DRW2D_DrawRectConvolve1dx(g_Drw2dDev, &rect, 0,0);
    } else {
        R_DRW2D_DrawRectConvolve1dy(g_Drw2dDev, &rect, 0,0);
    }

    //R_GFXUTILS_ASSERT(R_DRW2D_ERR_OK == ret);    
}

void InitJpegDemo(void){
    SetupJCUA();
}

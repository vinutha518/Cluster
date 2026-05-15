#include "r_typedefs.h"

#include "r_cdi_api.h"
#include "r_config_drw2d.h"
#include "r_drw2d_os.h"
#include "r_drw2d_api.h"
#include "r_drw2d_sys.h"
#include "r_drw2d_priv.h"
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
#include "limits.h"
#include "r_dev_api.h"

/* XC1 QoS Registers (D1M1(H) only) */

#define QOS_BASE       (0xffcd2300u)

#define QOS_MODE       (QOS_BASE + 0x0000u)
#define QOS_MAX_COUNT  (QOS_BASE + 0x0004u)
#define QOS_MAX_RACC   (QOS_BASE + 0x0008u)
#define QOS_MAX_WACC   (QOS_BASE + 0x000cu)


#define QOS_PORT_CPU0   0x0
#define QOS_PORT_CPU1   0x1
#define QOS_PORT_VACC0  0x2
#define QOS_PORT_VACC1  0x3
#define QOS_PORT_JCUA   0x4
#define QOS_PORT_GPU2D  0x5
#define QOS_PORT_ETNB   0x6


uint32_t my_read(uint32_t address){
    return *((uint32_t *)address);
}

void  my_write(uint32_t address, uint32_t value){
     *((uint32_t *)address) = value;
}


typedef struct {
    uint32_t mode; 
    uint32_t max_cnt; 
    uint32_t max_racc;
    uint32_t max_wacc;
} r_xc1_qos_config_t;

static const r_xc1_qos_config_t qos_config_slow = 
{
	0x3u,	/* Threshold mode */
/* 	0xe6u, */   /* Initial cycle count: e6 cycles */
	0xfau,  /* Initial cycle count: e6 cycles */
	0x0u,	/* 1 read bursts permitted within e6 cycles */
	0x7u,	/* 8 write bursts permitted within e6 cycles */
};

static const r_xc1_qos_config_t qos_config_max = 
{
	0x3u,	/* Threshold mode */
	0x7u,   /* Initial cycle count: 8 cycles */
	0x7u,	/* 8 read bursts permitted within 8 cycles */
	0x7u,	/* 8 write bursts permitted within 8 cycles */
};

static void loc_XC1_ConfigureQoS(uint32_t port, r_xc1_qos_config_t *config)
{
    uint32_t addr_offset = 0x10 * port;
	
	R_DEV_WRITE_REG(32, QOS_MODE      + addr_offset, config->mode);
	R_DEV_WRITE_REG(32, QOS_MAX_COUNT + addr_offset, config->max_cnt);	
	R_DEV_WRITE_REG(32, QOS_MAX_RACC  + addr_offset, config->max_racc);	
	R_DEV_WRITE_REG(32, QOS_MAX_WACC  + addr_offset, config->max_wacc);
}

/* Slow Dave will prevent buffer underrun during concurrent access of Dave HD and 
   VDCE to serial flash. */
void QosSlowDave(void ){
    loc_XC1_ConfigureQoS(QOS_PORT_GPU2D,  (r_xc1_qos_config_t *) &qos_config_slow);
}

void QosFastDave(void ){
    loc_XC1_ConfigureQoS(QOS_PORT_GPU2D, (r_xc1_qos_config_t *)&qos_config_max);
}



static r_drw2d_Point_t needle_refresh_sprite_pos;

void SetNeedleRefreshSpritePos(r_drw2d_Point_t pos){
    needle_refresh_sprite_pos = pos;
}

r_drw2d_Point_t GetNeedleRefreshSpritePos(void) {
    return needle_refresh_sprite_pos;
}


#define MIN(x,y) (((x) < (y)) ? x : y)
#define MAX(x,y) (((x) > (y)) ? x : y)

r_drw2d_IntRect_t CalcBoundingBoxRect2D( r_drw2d_Rect_t *Rect ){
    r_drw2d_Vec4_t vtx[4];
    r_drw2d_IntRect_t tmp;
    int32_t min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN;
    uint32_t i;
    r_drw2d_DeviceBase_t    *devBase   = (r_drw2d_DeviceBase_t *) g_Drw2dDev;
    vtx[0].X = Rect->Pos.X;
    vtx[0].Y = Rect->Pos.Y;
    vtx[0].Z = R_DRW2D_2X(0);
    vtx[0].W = R_DRW2D_2X(1);
    
    vtx[1].X = Rect->Pos.X + Rect->Size.Width;
    vtx[1].Y = Rect->Pos.Y;
    vtx[1].Z = R_DRW2D_2X(0);
    vtx[1].W = R_DRW2D_2X(1);
    
    vtx[2].X = Rect->Pos.X + Rect->Size.Width;
    vtx[2].Y = Rect->Pos.Y + Rect->Size.Height;
    vtx[2].Z = R_DRW2D_2X(0);
    vtx[2].W = R_DRW2D_2X(1);
    
    vtx[3].X = Rect->Pos.X;
    vtx[3].Y = Rect->Pos.Y + Rect->Size.Height;
    vtx[3].Z = R_DRW2D_2X(0);
    vtx[3].W = R_DRW2D_2X(1);
    drw2d_VtxTransform(devBase, vtx, 4);
    for(i = 0;i < 4;i++){
        min_x = MIN(vtx[i].X, min_x);
        min_y = MIN(vtx[i].Y, min_y);
        max_x = MAX(vtx[i].X, max_x);
        max_y = MAX(vtx[i].Y, max_y);
    }

    min_x = R_DRW2D_2I(min_x);
    min_y = R_DRW2D_2I(min_y);
    max_x = R_DRW2D_2I(max_x + 65535);
    max_y = R_DRW2D_2I(max_y + 65535);
    tmp.Pos.X       = min_x;
    tmp.Pos.Y       = min_y;
    tmp.Size.Width  = max_x - min_x /* + 1 , right and bottom is not drawn. */;
    tmp.Size.Height = max_y - min_y /* + 1 , right and bottom is not drawn. */;
    return tmp;
}

r_drw2d_IntRect_t MaximizeBoundingBox( r_drw2d_IntRect_t *Rect1, r_drw2d_IntRect_t *Rect2 ){
    r_drw2d_IntRect_t tmp;
    uint32_t min_x[3], max_x[3], min_y[3], max_y[3];
    uint32_t i;
    r_drw2d_IntRect_t *Rects[3];
    Rects[1] = Rect1;
    Rects[2] = Rect2;

    for(i = 1;i <= 2;i++) {
        min_x[i] = Rects[i]->Pos.X;
        min_y[i] = Rects[i]->Pos.Y;
        max_x[i] = Rects[i]->Pos.X + Rects[i]->Size.Width  - 1;
        max_y[i] = Rects[i]->Pos.Y + Rects[i]->Size.Height - 1;
    }

    min_x[0] = MIN(min_x[1], min_x[2]);
    min_y[0] = MIN(min_y[1], min_y[2]);

    max_x[0] = MAX(max_x[1], max_x[2]);
    max_y[0] = MAX(max_y[1], max_y[2]);

    tmp.Pos.X       = min_x[0];
    tmp.Pos.Y       = min_y[0];
    tmp.Size.Width  = max_x[0] - min_x[0] + 1;
    tmp.Size.Height = max_y[0] - min_y[0] + 1;
    return tmp;
}


static uint32_t rotation_object_scale_factor  =  R_DRW2D_2X(1);
static uint32_t rotation_texture_scale_factor = R_DRW2D_2X(1);

void ActivateRotationObjectScaleing(void){
    rotation_object_scale_factor  = R_DRW2D_2X(25);
    rotation_texture_scale_factor = R_DRW2D_2X(1.0/25);
/*     SetRectScaleFactor(rotation_object_scale_factor); */
}

void DeactivateRotationObjectScaleing(void){
    rotation_object_scale_factor  = R_DRW2D_2X(1);
    rotation_texture_scale_factor = R_DRW2D_2X(1);
/*     SetRectScaleFactor(rotation_object_scale_factor); */
}

void SetNeedleRotationTransformationMatrix(float64_t angle, 
                                           r_drw2d_Point_t rot_center,
                                           r_drw2d_Point_t tex_rot_center,
                                           uint32_t use_texture) {

    if (use_texture) {
        R_DRW2D_CtxTextureTransformMode(g_Drw2dDev, R_DRW2D_TEX_TRANSFORM_2D);
        R_DRW2D_CtxTextureIdentity(g_Drw2dDev);
        R_DRW2D_CtxTextureTranslate(g_Drw2dDev, rot_center.X , rot_center.Y);
        R_DRW2D_CtxTextureRotate(g_Drw2dDev, R_DRW2D_2X(angle));
        R_DRW2D_CtxTextureTranslate(g_Drw2dDev, -tex_rot_center.X , -tex_rot_center.Y);
        R_DRW2D_CtxTextureScale(g_Drw2dDev, 
                                rotation_texture_scale_factor, 
                                rotation_texture_scale_factor);
/*     R_DRW2D_CtxTextureTranslate(g_Drw2dDev, R_DRW2D_2X(1) , R_DRW2D_2X(1)); */
    }

    R_DRW2D_CtxIdentity(g_Drw2dDev);
    R_DRW2D_CtxTranslate(g_Drw2dDev, rot_center.X , rot_center.Y, 0);
    R_DRW2D_CtxRotate(g_Drw2dDev, R_DRW2D_2X(angle));
    R_DRW2D_CtxTranslate(g_Drw2dDev, -tex_rot_center.X , -tex_rot_center.Y, 0);    
    R_DRW2D_CtxScale(g_Drw2dDev, 
                     rotation_object_scale_factor, 
                     rotation_object_scale_factor, R_DRW2D_2X(1));
}


    
/* void Clear_LeftNeedle(float64_t angle){ */

void Clear_RotationObject(float64_t angle,
                          r_drw2d_Point_t rot_center,
                          r_drw2d_Point_t tex_rot_center,
                          r_drw2d_Texture_t *texture,
                          uint32_t border){
    r_drw2d_Rect_t clear_needle_rect;    
    if( angle == -90) return;   
    clear_needle_rect = CreateRectFromTexture(texture, 0, 0);
    
    tex_rot_center.X += R_DRW2D_2X(2) + R_DRW2D_2X(border);
    tex_rot_center.Y += R_DRW2D_2X(2) + R_DRW2D_2X(border);
    clear_needle_rect.Size.Width  += R_DRW2D_2X(4) + R_DRW2D_2X(border * 2);
    clear_needle_rect.Size.Height += R_DRW2D_2X(4) + R_DRW2D_2X(border * 2);

    R_DRW2D_CtxFillMode(g_Drw2dDev, R_DRW2D_FILLMODE_SOLID);
    /* R_DRW2D_CtxFgColor(g_Drw2dDev, 0xff00ffffu); */
    R_DRW2D_CtxFgColor(g_Drw2dDev, 0x00000000u);

    R_DRW2D_CtxBlendMode(g_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
    SetNeedleRotationTransformationMatrix(angle,  rot_center, tex_rot_center, 0);
    R_DRW2D_CtxImgQuality(g_Drw2dDev, R_DRW2D_IMGQUALITY_LOW);
    R_DRW2D_DrawRect(g_Drw2dDev, &clear_needle_rect);
    R_DRW2D_CtxImgQuality(g_Drw2dDev, R_DRW2D_IMGQUALITY_HIGH);
}

void Refresh_RotationObject(float64_t angle,
                            r_drw2d_Point_t rot_center,
                            r_drw2d_Point_t tex_rot_center,
                            r_drw2d_Texture_t *texture, uint32_t blendmode )
{
    r_drw2d_Rect_t draw_rect;
    
    draw_rect = CreateRectFromTexture(texture, 0, 0);

    R_DRW2D_CtxStripingEnable(g_Drw2dDev);
    

    /* draw the needle */
/*     if (blendmode == 3) { */
/*         texture->Flags = R_DRW2D_TEX_BILINEAR; */
/*     } */
    R_DRW2D_CtxTextureSet(g_Drw2dDev, 0u, texture);
    R_DRW2D_CtxFillMode(g_Drw2dDev,  R_DRW2D_FILLMODE_TEXTURE);

    /* R_DRW2D_CtxFillMode(g_Drw2dDev, R_DRW2D_FILLMODE_SOLID); */
/*     R_DRW2D_CtxFgColor(g_Drw2dDev, 0xffff0000u); */

/*     R_DRW2D_CtxBlendMode(g_Drw2dDev, R_DRW2D_BLENDMODE_SRC); */
    switch (blendmode) {
    case 0: /* For needle */
        R_DRW2D_CtxBlendMode(g_Drw2dDev, R_DRW2D_BLENDMODE_SRC_OVER);
        break;
    case 1: /* For gears */
        R_DRW2D_CtxBlendMode(g_Drw2dDev, R_DRW2D_BLENDMODE_SRC_OVER);
        break;
    case 2: /* For color_wheel */
        R_DRW2D_CtxBlendMode(g_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
        break;
    case 3: /* For Dot Matrix display */
#if  1
        R_DRW2D_CtxBlendFactors (   g_Drw2dDev,
                                    R_DRW2D_BLEND_SRC_ALPHA,
                                    R_DRW2D_BLEND_ONE_MINUS_SRC_ALPHA,
                                    R_DRW2D_BLEND_SRC_ALPHA,
                                    R_DRW2D_BLEND_ONE_MINUS_SRC_ALPHA);
        R_DRW2D_CtxBlendMode(g_Drw2dDev, R_DRW2D_BLENDMODE_CUSTOM);
#else
        R_DRW2D_CtxBlendMode(g_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
#endif
        break;
    }    
    
    SetNeedleRotationTransformationMatrix(angle,  rot_center, tex_rot_center, 1);
	
	
//================= START: ONLY RED effect on pointer =================================	

static r_drw2d_EffectStage_t Effects[1];
static unsigned char needle_blink_count;

	Effects[0].Name = R_DRW2D_EFFECT_MODULATE;
	Effects[0].Args[0].Source = R_DRW2D_EFFECT_SOURCE_TEXTURE_UNIT;
	Effects[0].Args[0].Param.Color.Source.TextureUnit = 0;
	Effects[0].Args[0].Param.Color.Operand = R_DRW2D_EFFECT_COLOR_OPERAND_RGBA;
	Effects[0].Args[1].Source = R_DRW2D_EFFECT_SOURCE_CONSTANT_COLOR;
	Effects[0].Args[1].Param.Color.Source.ConstantColor = 0xffff0000;//my_color;
	Effects[0].Args[1].Param.Color.Operand = R_DRW2D_EFFECT_COLOR_OPERAND_RGBA;

	
	R_DRW2D_CtxEffectsSet(g_Drw2dDev, Effects, 1);
		
//================= END: ONLY RED effect on pointer =================================	

    R_DRW2D_DrawRect(g_Drw2dDev, &draw_rect);
    R_DRW2D_CtxStripingDisable(g_Drw2dDev);
	
	
	R_DRW2D_CtxEffectsDelete(g_Drw2dDev);
	 

}

r_drw2d_IntRect_t CalcRect_RotationObject(float64_t angle,
                                       r_drw2d_Point_t rot_center,
                                       r_drw2d_Point_t tex_rot_center,
                                       r_drw2d_Texture_t *texture)
{
    r_drw2d_Rect_t draw_rect;    
    draw_rect = CreateRectFromTexture(texture, 0, 0);
    SetNeedleRotationTransformationMatrix(angle,  rot_center, tex_rot_center, 0);
    return CalcBoundingBoxRect2D(&draw_rect);
}


void Refresh_ParallelRotatedObject(float64_t angle,
                                   r_drw2d_Point_t rot_center,
                                   r_drw2d_Point_t tex_rot_center,
                                   r_drw2d_Point_t tex_offset,
                                   r_drw2d_Texture_t *texture )
{
    r_drw2d_Rect_t draw_light_rect;
    //R_GFXUTILS_ASSERT(texture);
    draw_light_rect      = CreateRectFromTexture(texture, 0, 0);
    R_DRW2D_CtxStripingDisable(g_Drw2dDev);

    R_DRW2D_CtxTextureSet(g_Drw2dDev, 0u, texture);
    R_DRW2D_CtxFillMode(g_Drw2dDev,  R_DRW2D_FILLMODE_TEXTURE);
    
    SetNeedleRotationTransformationMatrix(angle,  rot_center, tex_rot_center, 1);
    R_DRW2D_CtxRotate(g_Drw2dDev, R_DRW2D_2X(-angle));
    R_DRW2D_CtxTextureRotate(g_Drw2dDev, R_DRW2D_2X(-angle)); /* Neg angle needed here ?*/
    R_DRW2D_CtxTextureTranslate(g_Drw2dDev, -tex_offset.X, -tex_offset.Y);
    R_DRW2D_CtxTranslate(g_Drw2dDev, -tex_offset.X , -tex_offset.Y, 0);
    R_DRW2D_DrawRect(g_Drw2dDev, &draw_light_rect);
}

r_drw2d_IntRect_t CalcRect_ParallelRotatedObject(float64_t angle,
                                                 r_drw2d_Point_t rot_center,
                                                 r_drw2d_Point_t tex_rot_center,
                                                 r_drw2d_Point_t tex_offset,
                                              r_drw2d_Texture_t *texture )
{
    r_drw2d_Rect_t draw_light_rect;
    //R_GFXUTILS_ASSERT(texture);
    draw_light_rect      = CreateRectFromTexture(texture, 0, 0);
    SetNeedleRotationTransformationMatrix(angle,  rot_center, tex_rot_center, 0);
    R_DRW2D_CtxRotate(g_Drw2dDev, R_DRW2D_2X(-angle));
    R_DRW2D_CtxTranslate(g_Drw2dDev, -tex_offset.X , -tex_offset.Y, 0);
    return CalcBoundingBoxRect2D(&draw_light_rect);
}


void Clear_ParallelRotatedObject(float64_t angle,
                                 r_drw2d_Point_t rot_center,
                                 r_drw2d_Point_t tex_rot_center,
                                 r_drw2d_Point_t tex_offset,
                                 r_drw2d_Texture_t *texture,                                 
                                 r_drw2d_Texture_t *background)
{
    r_drw2d_Rect_t draw_light_clear_rect;
    
    SetNeedleRotationTransformationMatrix(angle,  rot_center, tex_rot_center, 0);
    R_DRW2D_CtxRotate(g_Drw2dDev, R_DRW2D_2X(-angle));
    R_DRW2D_CtxTranslate(g_Drw2dDev, -tex_offset.X , -tex_offset.Y, 0);

    draw_light_clear_rect = CreateRectFromTexture(texture, 0, 0);
    if (background) { /* If background copy use done is rectangle that has original size */
/*         draw_light_clear_rect.Size.Width +=R_DRW2D_2X(30); */
/*         draw_light_clear_rect.Size.Height+=R_DRW2D_2X(30); */
        R_DRW2D_CtxTextureTransformMode(g_Drw2dDev, R_DRW2D_TEX_TRANSFORM_2D);
        R_DRW2D_CtxTextureIdentity(g_Drw2dDev);        
        R_DRW2D_CtxTextureTranslate(g_Drw2dDev,
                                    -needle_refresh_sprite_pos.X,
                                    -needle_refresh_sprite_pos.Y);
        R_DRW2D_CtxTextureSet(g_Drw2dDev, 0u, background);
        R_DRW2D_CtxFillMode(g_Drw2dDev,  R_DRW2D_FILLMODE_TEXTURE);
    } else { /* If clear use rectanlge that is one larger */
        draw_light_clear_rect.Size.Width  += R_DRW2D_2X(2);
        draw_light_clear_rect.Size.Height += R_DRW2D_2X(2);
        R_DRW2D_CtxFillMode(g_Drw2dDev, R_DRW2D_FILLMODE_SOLID);
        R_DRW2D_CtxFgColor(g_Drw2dDev, 0);
/*         R_DRW2D_CtxFgColor(g_Drw2dDev, 0xff00ffffu); */
        R_DRW2D_CtxTranslate(g_Drw2dDev, R_DRW2D_2X(-1) , R_DRW2D_2X(-1), 0);
    }
    R_DRW2D_CtxImgQuality(g_Drw2dDev, R_DRW2D_IMGQUALITY_LOW); 
    R_DRW2D_DrawRect(g_Drw2dDev, &draw_light_clear_rect);
    R_DRW2D_CtxImgQuality(g_Drw2dDev, R_DRW2D_IMGQUALITY_HIGH); 
}


r_drw2d_Point_t IntPoint2Point(r_drw2d_IntPoint_t * IntPoint) {
    r_drw2d_Point_t tmp_point;
    tmp_point.X = R_DRW2D_2X(IntPoint->X);
    tmp_point.Y = R_DRW2D_2X(IntPoint->Y);
    return tmp_point;
}

r_drw2d_Rect_t IntRect2Rect(r_drw2d_IntRect_t * IntRect) {
    r_drw2d_Rect_t tmp_rect;
    tmp_rect.Pos = IntPoint2Point(&IntRect->Pos);
    tmp_rect.Size.Width  = R_DRW2D_2X(IntRect->Size.Width);
    tmp_rect.Size.Height = R_DRW2D_2X(IntRect->Size.Height);
    return tmp_rect;
}

void Copy_Rect(r_drw2d_Texture_t *src_p, r_drw2d_Texture_t  *dst_p,
               r_drw2d_IntRect_t *src_rect_p, r_drw2d_IntPoint_t *dst_pos_p) {
    r_drw2d_Rect_t  src_rect, rect;
    r_drw2d_Point_t  dst_point;
    r_drw2d_Point_t  src_translation;
    r_drw2d_Framebuffer_t frb;
    r_drw2d_TextureFlags_t old_flags;
    static uint32_t use_needle_buffer = 0;

    frb.Handle = R_NULL;
    frb.Flags  = (r_drw2d_FramebufferFlags_t)0;
    frb.Buffer = dst_p->Buffer;

    src_rect  = IntRect2Rect(src_rect_p);
    dst_point = IntPoint2Point(dst_pos_p);
    src_translation.X = -src_rect.Pos.X + dst_point.X;
    src_translation.Y = -src_rect.Pos.Y + dst_point.Y;

    old_flags = src_p->Flags;
    src_p->Flags = 0;
    R_DRW2D_CtxTextureSet(g_Drw2dDev, 0, src_p);
    
    R_DRW2D_CtxTextureIdentity(g_Drw2dDev);
    R_DRW2D_CtxTextureTranslate(g_Drw2dDev, src_translation.X, src_translation.Y);

    R_DRW2D_CtxIdentity(g_Drw2dDev);

    R_DRW2D_CtxFillMode(g_Drw2dDev,  R_DRW2D_FILLMODE_TEXTURE);
    R_DRW2D_CtxBlendMode(g_Drw2dDev, R_DRW2D_BLENDMODE_SRC);

    rect.Pos         = dst_point;
    rect.Size        = src_rect.Size;


    if(use_needle_buffer){
        r_drw2d_Framebuffer_t *draw_fb;
        draw_fb = MbSpriteNewDrawBufGet(&LeftNeedle);
        R_DRW2D_FramebufferSet(g_Drw2dDev, draw_fb);
        R_DRW2D_CtxBgColor(g_Drw2dDev, 0xff0000ff);
        R_DRW2D_FramebufferClear(g_Drw2dDev);
    } else {
        R_DRW2D_FramebufferSet(g_Drw2dDev, &frb);
    }
    R_DRW2D_CtxImgQuality(g_Drw2dDev, R_DRW2D_IMGQUALITY_LOW);
    R_DRW2D_DrawRect(g_Drw2dDev, &rect);
    src_p->Flags = old_flags;
    R_DRW2D_CtxImgQuality(g_Drw2dDev, R_DRW2D_IMGQUALITY_HIGH);
}

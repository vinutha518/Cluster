#include "r_typedefs.h"

#include "r_cdi_api.h"
#include "r_drw2d_os.h"
#include "r_drw2d_api.h"
#include "r_config_drw2d.h"
#include "r_drw2d_ctx_dhd.h"
#include "mem_init.h"
#include "fs_data.h"
#include "fs_data_res.h"
#include "r_bsp_stdio_api.h"
//#include "r_gfxutils_api.h"
#include "r_util_dhd.h"
#include "drw_wrap.h"

#ifdef _WINDOWS
#include <stdio.h>
#endif

r_drw2d_Texture_t background_texture;
r_drw2d_Texture_t gauge_needle2_texture;
r_drw2d_Texture_t gauge_needle1_texture;

r_drw2d_Texture_t font_fixedsys12;
r_drw2d_Texture_t my_odo_font_texture;

r_drw2d_Texture_t my_middle_patch_texture;
r_drw2d_Texture_t my_menu_patch_texture;

//void *car_model_data;

typedef struct {
    uint32_t image_id;
    r_drw2d_Buffer_t buffer;
    uint32_t do_copy;
} copy_t;

const copy_t rom_data_geometry[] = {
    {ROM_FS_BKG,                 {0, 800, { 800,480}, R_DRW2D_PIXELFORMAT_ARGB8888}, 0},
	
	{ROM_FS_POINTER_NEEDLE1,     {0, 160 ,{ 160, 64}, R_DRW2D_PIXELFORMAT_ARGB8888}, 1},
	
	
	{ROM_FS_POINTER_NEEDLE2,     {0, 188 ,{ 188, 96}, R_DRW2D_PIXELFORMAT_ARGB8888}, 1},

	//{ROM_FS_POINTER_NEEDLE2,     {0, 212 ,{ 212, 84}, R_DRW2D_PIXELFORMAT_ARGB8888}, 1},
	
	//{ROM_FS_FIXEDSYS_12_708X16,     {0, 708,{ 708,  16},R_DRW2D_PIXELFORMAT_ARGB8888}},
	{ROM_FS_SPEED_FONT,     {0, 448,{ 448,  68},R_DRW2D_PIXELFORMAT_ARGB8888}},		// for speed fonts
	
	//{ROM_FS_ODO_FONT,     {0, 274,{ 274,  36},R_DRW2D_PIXELFORMAT_ARGB8888}},		// for odo fonts
//	{ROM_FS_ODO_FONT,     {0, 756,{ 756,  36},R_DRW2D_PIXELFORMAT_ARGB8888}},		// for odo all fonts
//	{ROM_FS_ODO_FONT,     {0, 776,{ 776,  36},R_DRW2D_PIXELFORMAT_ARGB8888}},		// for odo all fonts new
	{ROM_FS_ODO_FONT,     {0, 862,{ 862,  36},R_DRW2D_PIXELFORMAT_ARGB8888}},		// for odo all fonts with symbols
	
	{ROM_FS_MIDDLE_PATCH,     {0, 210,{ 210,  210},R_DRW2D_PIXELFORMAT_ARGB8888}},		// for middle patch	
	{ROM_FS_MENU_PATCH,     {0, 192,{ 192,  280},R_DRW2D_PIXELFORMAT_ARGB8888}},		// for menu	
	
	
};

uint32_t rom_data_geometry_size = sizeof(rom_data_geometry) / sizeof(rom_data_geometry[0]);

extern const uint8_t my_bkg[480*480];
extern const unsigned char my_pointer[];
extern const unsigned char my_pointer_big[];
extern const unsigned char locImgData[];

#pragma ghs startdata
#pragma ghs section bss = ".my_section"
unsigned char my_dst[1536000];
#pragma ghs section bss = default
#pragma ghs enddata

unsigned long m;
extern const unsigned char my_new_bg[];

extern const unsigned char my_font_array[];
extern const unsigned char Font_consolas_B36[];

extern const unsigned char my_odo_font_array[];
extern const unsigned char my_odo_font_info[];
extern const unsigned char my_middle_patch[];
extern const unsigned char my_menu_patch[];

r_drw2d_Texture_t texture_template = { R_NULL, 0,0,0};

uint32_t use_premultiplied_alpha=0;

void Copy(r_drw2d_Texture_t *texture_p, r_drw2d_Framebuffer_t *frb_p) {
    r_drw2d_Error_t ret = R_DRW2D_ERR_OK;
    r_drw2d_Rect_t  rect;


    ret |= R_DRW2D_CtxTextureIdentity(g_Drw2dDev);
    ret |= R_DRW2D_CtxIdentity(g_Drw2dDev);
    ret |= R_DRW2D_CtxTextureSet(g_Drw2dDev, 0, texture_p);

    ret |= R_DRW2D_CtxFillMode(g_Drw2dDev,  R_DRW2D_FILLMODE_TEXTURE);

    if (use_premultiplied_alpha) {
        ret |= R_DRW2D_CtxBlendMode(g_Drw2dDev, R_DRW2D_BLENDMODE_CUSTOM);
        ret |= R_DRW2D_CtxBlendFactors(g_Drw2dDev,
                                       R_DRW2D_BLEND_SRC_ALPHA,
                                       /* R_DRW2D_BLEND_ZERO, */
                                       R_DRW2D_BLEND_ZERO,
                                       R_DRW2D_BLEND_ONE,
                                       R_DRW2D_BLEND_ZERO);
    } else {
        ret |= R_DRW2D_CtxBlendMode(g_Drw2dDev, R_DRW2D_BLENDMODE_SRC);
    }

    rect.Pos.X       = rect.Pos.Y = 0;
    rect.Size.Width  = R_DRW2D_2X(frb_p->Buffer.Size.Width);
    rect.Size.Height = R_DRW2D_2X(frb_p->Buffer.Size.Height);
    
    ret = R_DRW2D_FramebufferSet(g_Drw2dDev, frb_p);
    ret |= R_DRW2D_DrawRect(g_Drw2dDev, &rect);

    //R_GFXUTILS_ASSERT(R_DRW2D_ERR_OK == ret);    
}

void Copy_Buffer(r_drw2d_Buffer_t *src_p, r_drw2d_Buffer_t *dst_p){
    r_drw2d_Texture_t     texture;
    r_drw2d_Framebuffer_t frb;

    texture.Handle = R_NULL;
    texture.Flags  = (r_drw2d_TextureFlags_t)0;
    texture.Buffer = *src_p;

    frb.Handle = R_NULL;
    frb.Flags  = (r_drw2d_FramebufferFlags_t)0;
    frb.Buffer = *dst_p;
    Copy(&texture, &frb);
    
}

void LoadToRam(void)
{
    int i;

    r_drw2d_Buffer_t src,dst;
    const copy_t *cur_geometry;
    uint32_t cur_image_id, cur_do_copy;
    const r_fs_data_t *cur_rom_fs;
    
    for(i = 0;i < rom_data_geometry_size; i++) 
	{
        r_drw2d_Texture_t *cur_texture;
        cur_geometry = rom_data_geometry + i;
        cur_image_id = cur_geometry->image_id;
        cur_do_copy  = cur_geometry->do_copy;
        //cur_rom_fs   = RomFileSystemData_res + cur_image_id;
        src          = dst = cur_geometry->buffer;
        src.Data     = cur_rom_fs->Address;
        use_premultiplied_alpha = 0;
        switch(cur_image_id)
		{
			case ROM_FS_SPEED_FONT:
				src.Data     = &my_font_array;		// Speed Font
				//src.Data     = &my_odo_font_array;	// Odo font
				
				dst.Data     = R_CDI_Alloc(AppHeap_p, 121856);	// Speed Fonts
				//dst.Data     = R_CDI_Alloc(AppHeap_p, 39456);	// Odo Fonts
							
				font_fixedsys12.Buffer = dst;
				font_fixedsys12.Flags = R_DRW2D_TEX_VT|R_DRW2D_TEX_BILINEAR|R_DRW2D_TEX_PERSPECTIVE;
				
				Copy_Buffer( &src, &dst);
				R_DRW2D_GpuFinish(g_Drw2dDev, R_DRW2D_FINISH_WAIT);	
				
				break;
			
			case ROM_FS_ODO_FONT:
				src.Data     = &my_odo_font_array;	// Odo font
				
				//dst.Data     = R_CDI_Alloc(AppHeap_p, 39456);	// Odo Fonts
				//dst.Data     = R_CDI_Alloc(AppHeap_p, 108864);	// Odo all Fonts
				//dst.Data     = R_CDI_Alloc(AppHeap_p, 111744);	// Odo all Fonts new
				dst.Data     = R_CDI_Alloc(AppHeap_p, 124128);	// Odo all Fonts with symbols
							
				my_odo_font_texture.Buffer = dst;
				my_odo_font_texture.Flags = R_DRW2D_TEX_VT|R_DRW2D_TEX_BILINEAR|R_DRW2D_TEX_PERSPECTIVE;
				
				Copy_Buffer( &src, &dst);
				R_DRW2D_GpuFinish(g_Drw2dDev, R_DRW2D_FINISH_WAIT);	
				
				break;
				
			case ROM_FS_BKG:
				src.Data     = &locImgData;//&my_menu_patch;

				for(m=0; m<1536000; m++)
				{
				    my_dst[m] = locImgData[m] >> 2;
				}

				src.Data     = &my_dst;
				dst.Data     = R_CDI_Alloc(AppHeap_p, 1536000);
				
				background_texture.Buffer = dst;
			
				Copy_Buffer( &src, &dst);
				R_DRW2D_GpuFinish(g_Drw2dDev, R_DRW2D_FINISH_WAIT);
				break;

			
			case ROM_FS_POINTER_NEEDLE2:
			#if 1
				src.Data     = &my_pointer_big;	//cur_rom_fs->Address;
				dst.Data     = R_CDI_Alloc(AppHeap_p, 72192);	// for Altia my big needle
				//dst.Data     = R_CDI_Alloc(AppHeap_p, 71232);	// for Ketan big needle
			#endif
			
			#if 0
				src.Data     = &my_pointer;	//cur_rom_fs->Address;
				dst.Data     = R_CDI_Alloc(AppHeap_p, 48128);	// for Altia my small needle
			#endif
			
			#if 0
				src.Data     = &my_pointer;	//cur_rom_fs->Address;
				dst.Data     = R_CDI_Alloc(AppHeap_p, 27072);	 //for REE needle
			#endif
				
				use_premultiplied_alpha = 1;
				gauge_needle2_texture.Buffer = dst;
				gauge_needle2_texture.Flags  = R_DRW2D_TEX_SWIZZLE|R_DRW2D_TEX_BILINEAR;
					
				Copy_Buffer( &src, &dst);
				R_DRW2D_GpuFinish(g_Drw2dDev, R_DRW2D_FINISH_WAIT);	
				
				break;

				
			case ROM_FS_POINTER_NEEDLE1:
				//src.Data     = &my_pointer;
				//dst.Data     = R_CDI_Alloc(AppHeap_p, 40960);
				
				use_premultiplied_alpha = 1;
				gauge_needle1_texture.Buffer = dst;
				gauge_needle1_texture.Flags  = R_DRW2D_TEX_SWIZZLE|R_DRW2D_TEX_BILINEAR;
					
				//Copy_Buffer( &src, &dst);
				//R_DRW2D_GpuFinish(g_Drw2dDev, R_DRW2D_FINISH_WAIT);	
				break;
			
			case ROM_FS_MIDDLE_PATCH:
				src.Data     = &my_middle_patch;
				dst.Data     = R_CDI_Alloc(AppHeap_p, 176400);
				
				use_premultiplied_alpha = 1;
				my_middle_patch_texture.Buffer = dst;
				//my_middle_patch_texture.Flags  = R_DRW2D_TEX_SWIZZLE|R_DRW2D_TEX_BILINEAR;
					
				Copy_Buffer( &src, &dst);
				R_DRW2D_GpuFinish(g_Drw2dDev, R_DRW2D_FINISH_WAIT);	
				break;
				
			case ROM_FS_MENU_PATCH:
				// src.Data     = &locImgData;//&my_menu_patch;
				// dst.Data     = R_CDI_Alloc(AppHeap_p, 1536000);
				
				// use_premultiplied_alpha = 1;
				// my_menu_patch_texture.Buffer = dst;
				//my_menu_patch_texture.Flags  = R_DRW2D_TEX_SWIZZLE|R_DRW2D_TEX_BILINEAR;
					
				// Copy_Buffer( &src, &dst);
				// R_DRW2D_GpuFinish(g_Drw2dDev, R_DRW2D_FINISH_WAIT);	
				break;	
			
			
			if(0)//cur_do_copy)
			{
				Copy_Buffer( &src, &dst);
				R_DRW2D_GpuFinish(g_Drw2dDev, R_DRW2D_FINISH_WAIT);
			}
		}
	}
}



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
#include "r_bsp_stdio_api.h"
//#include "r_gfxutils_api.h"
#include "r_util_dhd.h"
#include "load_to_ram.h"
#include "math.h"
#include "r_jcua_api.h"
#include "bench.h"

volatile uint32_t JCUA_running = 0;
uint32_t callback_swap_window = 0;

r_wm_Window_t cur_Window;

#define MAX_WAIT 6


void jpeg_wait_for_finish(void){    
    while(JCUA_running);
}

void callbackJCUA(uint32_t Unit, r_jcua_CallbackReason_t Factor, uint32_t Param)
{
    switch(Factor)
    {
    case R_JCUA_CB_FACTOR_DECODE_COMPLETE:
        JCUA_running = 0;
        break;
        
    case R_JCUA_CB_FACTOR_DECODE_SIZEOVER:
    case R_JCUA_CB_FACTOR_DECODE_ERRORED:
    case R_JCUA_CB_FACTOR_FATAL_ERROR:
        JCUA_running = 0;
        break;
    default:
        break;
    }

    if (callback_swap_window) {
        R_WM_WindowSwap(FLASHDASH_WM_UNIT, &cur_Window);
        R_WM_FrameEndMark(FLASHDASH_WM_UNIT, 0);
    }
}



void SetupJCUA(void)
{
    r_jcua_Error_t err;

    err = R_JCUA_Init(0);
    if (err != R_JCUA_ERR_OK)
    {
        return;
    }

    JCUA_running = 0;

    err = R_JCUA_DecoderOpen(0, callbackJCUA, 0);
    if (err != R_JCUA_ERR_OK)
    {
        return;
    }
}

r_jcua_DecodeSetting_t jpeg_decode;

void jpeg_decode_buffer(void *src, r_drw2d_Framebuffer_t*  draw_fb) {
    r_jcua_FrameBuffer_t dst_buf;
    r_jcua_Error_t err;
    volatile static int FrameWait=1;    

    
    /*  Currently hard coded */
    dst_buf.Address = draw_fb->Buffer.Data;
    dst_buf.Size    = draw_fb->Buffer.Pitch * draw_fb->Buffer.Size.Height * 4; 
    dst_buf.Stride  = draw_fb->Buffer.Pitch * 4 /* BPP */;
    dst_buf.Format  = R_JCUA_OUTPUT_FORMAT_ARGB8888; /* currently YUV 422 not support in wm */
    dst_buf.Swap    = R_JCUA_SWAP_LONG;

    jpeg_decode.OptionFlag = R_JCUA_DECODE_OPTION_NONE;
    jpeg_decode.ImgInfo    = R_NULL;

    callback_swap_window = 0;
    JCUA_running = 1;
    
    err = R_JCUA_DecoderStart(0, src, &dst_buf, &jpeg_decode);
    while (err != R_JCUA_ERR_OK)    {
	    JCUA_running = 0;
    }
}

void CallSwap(void ){
    R_WM_WindowSwap(FLASHDASH_WM_UNIT, &cur_Window);
    R_WM_FrameEndMark(FLASHDASH_WM_UNIT, 0);
}

#include "r_typedefs.h"

#include "r_drw2d_os.h"
#include "r_drw2d_api.h"
#include "r_config_drw2d.h"
#include "r_drw2d_ctx_dhd.h"
#include "r_cdi_api.h"
#include "r_util_dhd.h"

#include "mem_init.h"
#include "init.h"
#include "device_config.h"

#define LOC_DHD_UNIT    0
#define LOC_DRW2D_UNIT  0

/***********************************************************

Variable: g_Drw2dDev

Pointer to the Drw2D device handle
*/

r_drw2d_Device_t g_Drw2dDev;

/***********************************************************
Variable: loc_DHDDev

DHD Drawing handle
*/
static struct r_drw2d_DeviceDHD_s loc_DHDDev;

/***********************************************************
Function: loc_DRW2DErrorHandler

Error handler used for Drw2d errors
*/
static r_drw2d_Boolean_t loc_DRW2DErrorHandler(r_drw2d_Error_t  Error, void *UserData)
{
    FlashDash_Error();
    return R_TRUE;
}

void FlashDash_InitDrwDriver(void)
{
    R_UTIL_DHD_Init(LOC_DHD_UNIT);
    R_UTIL_DHD_Config((dhd_gpu_ptr_t)LOC_DHD_BASE, LOC_DHD_WORKING_BUFFER, &VramHeap);
    /* start up gfx engine */
    R_DRW2D_Init();
    R_DRW2D_GlobalErrCallbackSet(&loc_DRW2DErrorHandler, R_NULL);
    R_DRW2D_Open(LOC_DRW2D_UNIT, R_DRW2D_UNIT_DHD0, &loc_DHDDev, &g_Drw2dDev);
}

void FlashDash_DeinitDrwDriver(void)
{
    R_UTIL_DHD_DeInit(LOC_DHD_UNIT);
}

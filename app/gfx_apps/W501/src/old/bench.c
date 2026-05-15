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
/* #include "wm.h" */
#include "fs_data.h"
#include "fs_data_res.h"
//#include "fs_data_movie.h"
#include "r_bsp_stdio_api.h"
//#include "r_gfxutils_api.h"
#include "r_util_dhd.h"
#include "load_to_ram.h"
#include "math.h"
#include "jpeg.h"
#include "r_tick_api.h"
#include "bench.h"


double bench_ms[BENCH_SLOT_MAX][BENCH_US_MAX];
uint32_t bench_current[BENCH_SLOT_MAX];
uint32_t bench_old[BENCH_SLOT_MAX];

static uint32_t loc_bench_start[BENCH_SLOT_MAX];

void bench_start(uint32_t slot){    
    if(slot >= BENCH_SLOT_MAX){
        R_BSP_STDIO_Printf("Only %d slots available.\n",BENCH_SLOT_MAX);
    }
    //R_GFXUTILS_ASSERT(loc_bench_start[slot] == 0);
    while (0 == (loc_bench_start[slot] = R_TICK_GetTimeUS(0))) ;
}

void bench_stop(uint32_t slot){
    uint32_t bench_stop;
    if(slot >= BENCH_SLOT_MAX){
        R_BSP_STDIO_Printf("Only %d slots available.\n",BENCH_SLOT_MAX);
    }
    if(loc_bench_start[slot] == 0){
        R_BSP_STDIO_Printf("Benching stop without start\n");
    }

    bench_stop = R_TICK_GetTimeUS(0);

    if(loc_bench_start[slot] > bench_stop) {
        bench_ms[slot][bench_current[slot]] = 
            ((double) bench_stop + (((uint64_t)1)<<32) -
             (double)loc_bench_start[slot]) / 1e3;
    } else {
        bench_ms[slot][bench_current[slot]] =
            ((double)bench_stop - (double)loc_bench_start[slot]) / 1e3;
    }
    loc_bench_start[slot] = 0;  
    bench_old[slot] = bench_current[slot];
    bench_current[slot]++;
    if(bench_current[slot] >= BENCH_US_MAX){
        bench_current[slot] = 0; /* rewind measurements */
    }
    bench_ms[slot][bench_current[slot]] = 0;
}

float64_t GetCurrentDuration(uint32_t slot){
    return bench_ms[slot][bench_old[slot]];
}

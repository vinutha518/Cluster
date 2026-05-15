/************************************************************************************************
PROJECT : simple_draw app
FILE    : $Id: mem_init.c 14969 2017-11-02 14:33:54Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Init functions for memory
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2013 - 2014
                                  by 
                       Renesas Electronics (Europe) GmbH.
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing, not for mass production

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

SAMPLE CODE is not part of the licensed software, as such it must not be used in
mass-production applications. It can only be used for evaluation and
demonstration purposes at customer's premises listed in the signed license
agreement.
****************************************************************************
*/

/*******************************************************************************
  Section: Includes
*/
#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */
#include "r_config_d1x.h"
#include "r_dev_api.h"
#include "r_cdi_api.h"
#include "mem_init.h"
//#include "config_wm.h"
#include "config_mem.h"
//#include "main.h"
#include "davehd_driver.h"
#include "r_util_dhd.h"

/*******************************************************************************
  Section: Local defines
*/
#define LOC_VID_HP_BLOCK_SIZE  (0x1000u)
#define LOC_VID_HP_BLOCK_NB    (loc_SDRAM_SIZE / LOC_VID_HP_BLOCK_SIZE)
#define ALIGN(ADDR,BLOCK)      ( (ADDR+(BLOCK-1)) & ~(BLOCK-1) )
#define VRAM_HEAP_BASE         ALIGN(loc_SDRAM+DHD_MEMORY_SIZE, LOC_VID_HP_BLOCK_SIZE)

#define LOC_HEAP_SIZE           (LOC_CPU_HP_BLOCK_NB * LOC_CPU_HP_BLOCK_SIZE)
#define LOC_CPU_HP_BLOCK_NB     0x800
#define LOC_CPU_HP_BLOCK_SIZE   0x40
static uint32_t loc_LocalHeap[LOC_HEAP_SIZE >> 2];


/*******************************************************************************
  Section: Local variables
  
*/


r_cdi_Heap_t       loc_VRAM_heap;
r_cdi_Heap_t       loc_lRAM_heap;

r_cdi_Heap_t IramHeap;
r_cdi_Heap_t SdramHeap;
r_cdi_Heap_t VramHeap;
r_cdi_Heap_t *AppHeap_p;


/* This way, allocated memory is at least aligned to its own block size */
#pragma alignvar(CPU_HP_BLOCK_SIZE)
uint8_t            loc_lRAM_heapMem[CPU_HP_BLOCK_NB * CPU_HP_BLOCK_SIZE];
r_cdi_HpBlkIndex_t loc_lRAM_heapIdxList[CPU_HP_BLOCK_NB];
uint32_t loc_SDRAM               = 0;
uint32_t loc_SDRAM_SIZE          = 0;

/*******************************************************************************
  Section: Local functions

*/

/*******************************************************************************
  Section: Global functions
*/

/*******************************************************************************
  Function: SetupMemManager

    See: main.h for details
*/
//void loc_SetupMemManager(void)
void FlashDash_InitMem(void)
{
    uint32_t x;
    r_dev_Device_t dev;

    dev = R_DEV_GetDev();
    if((dev == R_DEV_R7F701404) ||
       (dev == R_DEV_R7F701405) ||
       (dev == R_DEV_R7F701406) ||
       (dev == R_DEV_R7F701407) ||
       (dev == R_DEV_R7F701418) ||
       (dev == R_DEV_R7F701442))
    {
        loc_SDRAM = (D1M_SDRAM_BASE);//(D1M_SDRAM_BASE + 0x100000);
        loc_SDRAM_SIZE = 8*1024*1024;
    }
    else if((dev == R_DEV_R7F701408) ||
            (dev == R_DEV_R7F701410) ||
            (dev == R_DEV_R7F701411) ||
            (dev == R_DEV_R7F701412) ||
            (dev == R_DEV_R7F701441) ||
            (dev == R_DEV_R7F701460))
    {
        loc_SDRAM = (D1M_SDRAM_BASE);//(D1M_SDRAM_BASE + 0x100000);
        loc_SDRAM_SIZE = 8*1024*1024;
    }

    else
    {
        while(1);
    }
    x = R_CDI_InitHeapManager((uint32_t)loc_LocalHeap,//loc_lRAM_heapMem,
                              &IramHeap,//&loc_lRAM_heap,
                              loc_lRAM_heapIdxList,
                              2048,//CPU_HP_BLOCK_NB,
                              64);//CPU_HP_BLOCK_SIZE);
    if (x == 1)
    {
        while(1);
    }

    x = R_CDI_InitHeapManager((uint32_t) 0x40000000, //VRAM_HEAP_BASE,
                              &SdramHeap,//loc_VRAM_heap,
                              0,
                              LOC_VID_HP_BLOCK_NB,
                              LOC_VID_HP_BLOCK_SIZE);
    if (x == 1)
    {
         while(1);
    }
	
	x = R_CDI_InitHeapManager((uint32_t) 0x3FE76000,
                              &VramHeap,
                              0,
                              3152,
                              512);
    if (x == 1)
    {
    	while(1);
    }
	
	//AppHeap_p = &VramHeap;
     AppHeap_p = &SdramHeap;

}



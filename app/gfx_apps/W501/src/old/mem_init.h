/***********************************************************************************************
PROJECT : d1mx_mfd_demo app
FILE   : $Id: mem_init.h 4317 2014-11-10 15:03:50Z miodrag.stankovic $
============================================================================ 
DESCRIPTION
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
#ifndef MEM_INIT_H_
#define MEM_INIT_H_


#ifdef __cplusplus
extern "C" {
#endif



/*******************************************************************************
  Section: Local Functions
*/

void FlashDash_InitMem(void);

/* Estimation:
   r_util_dhd.h
   #define DHD_USER_INSTANCES (1u)
   #define DHD_JOBSIZE      (512u)
   #define DHD_JOBCOUNT       (8u)
   #define DHD_RINGSIZE    (1024u)
   min LOC_DHD_WORKING_BUFFER = DHD_JOBSIZE * 1 + DHD_RINGSIZE = 1536 byte + some bytes for align
   avg LOC_DHD_WORKING_BUFFER = DHD_JOBSIZE * 8 + DHD_RINGSIZE = 5120 byte + some bytes for align
   choosing 8 kB is totally safe here. */
/* #define LOC_DHD_WORKING_BUFFER (0x10000) */ /* just estimated for now... */
#define LOC_DHD_WORKING_BUFFER 8192

#define LOC_DHD_BASE          LOC_VRAM1
#define LOC_VO0_BASE          (LOC_DHD_BASE + LOC_DHD_WORKING_BUFFER)
/* #define LOC_VO1_BASE          LOC_VRAM1 */
/* #define LOC_EXTRAM_BASE       LOC_SDRAM_BASE */

#define LOC_VID_FB_BASE       (LOC_VO0_BASE)
#define LOC_SDRAM_FB_BASE     (LOC_EXTRAM_BASE)

#if (LOC_VID_FB_BASE == LOC_EXTRAM_BASE)
#define LOC_VIDMEM_SIZE     (LOC_SDRAM_SIZE)
#else
#define LOC_VIDMEM_SIZE     (LOC_VRAM1_SIZE-LOC_DHD_WORKING_BUFFER)
#endif


extern r_cdi_Heap_t IramHeap;
extern r_cdi_Heap_t SdramHeap;
extern r_cdi_Heap_t VramHeap;
extern r_cdi_Heap_t *AppHeap_p;

extern r_cdi_Heap_t MyramHeap;

#ifdef __cplusplus
}
#endif

#endif /* MEM_INIT_H_ */

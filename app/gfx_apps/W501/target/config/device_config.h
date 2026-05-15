/***********************************************************************************************
PROJECT : drw2d simple app
FILE   : $Id: device_config.h 4317 2014-11-10 15:03:50Z miodrag.stankovic $
============================================================================ 
DESCRIPTION
D1m2 device config
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

#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

/*******************************************************************************
  Define: Display infos

   LOC_DISPLAY_NAME     - display string
   LOC_DISPLAY_WIDTH    - display width
   LOC_DISPLAY_HEIGHT   - display height
   LOC_DISPLAY_BPP      - bytes per pixel
*/

#define LOC_VRAM0           (0x3FCE8000u)
#define LOC_VRAM1           (0x3FE74000u)
#define LOC_SDRAM_BASE      (0x40000000u)
#define LOC_VRAM0_SIZE      (0x18C000u)
#define LOC_VRAM1_SIZE      (0x18C000u)
#define LOC_SDRAM_SIZE      (512u << 20/* MB */)

#define LOC_JCUA_UNIT (0u)
#define LOC_SFMA_UNIT (0u)
#define LOC_VDCE_UNIT (0u)

#endif /* DEVICE_CONFIG_H */

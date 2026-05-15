/*
****************************************************************************
PROJECT : DRW2D
FILE    : $Id: r_config_drw2d.h 3678 2014-08-29 10:52:42Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for DRW2D macro
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

#ifndef R_CONFIG_DRW2D_H_
#define R_CONFIG_DRW2D_H_



/***********************************************************
  Title: config_drw2d.h
  
  DRW2D configuration
*/



/***********************************************************
  Section: Includes
************************************************************/





/***********************************************************
  Section: Global Defines
************************************************************/



/***********************************************************
  Constant: R_DRW2D_UNIT_*

  Enumeration of DRW2D units.


  DRW2D units:
  R_DRW2D_UNIT_DHD0      - 0:  First DaveHD instance (#0)
*/

#define R_DRW2D_UNIT_DHD0     0u



/***********************************************************
  Constant: R_DRW2D_NUM_UNITS

  Number of DRW2D devices. see <R_DRW2D_DEV_*>.

*/

#define R_DRW2D_NUM_UNITS     1u



/***********************************************************
  Constant: R_DRW2D_MAX_BUFFER_WIDTH

  Maximum framebuffer/texture width
*/

#define R_DRW2D_MAX_BUFFER_WIDTH  (4095u)



/***********************************************************
  Constant: R_DRW2D_MAX_BUFFER_HEIGHT

  Maximum framebuffer/texture height
*/

#define R_DRW2D_MAX_BUFFER_HEIGHT  (4095u)

#endif /* R_CONFIG_DRW2D_H_ */


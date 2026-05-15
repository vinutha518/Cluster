/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                   drmathbboxif.h                                ***
***                                                                                 ***
***                                   Project Drome                                 ***
***                                                                                 ***
***          Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2018#$          ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file
  \ingroup Math

  \Subject         DROME - BBox (Bounding box).

  \Reference       None

  \Abstract
     This file implements XX.

  \Version history record:

  $Id: drmathbboxif.h 10 2018-12-03 11:36:39Z stoneridge $

\{
***************************************************************************************
*/

#ifndef DRMATHBBOXIF_H_
#define DRMATHBBOXIF_H_

/*
**=====================================================================================
** Imported definitions
**=====================================================================================
*/

#include "drome.h"
#include "drhost.h"
#include "drmathif.h"

/*
**=====================================================================================
** Public defines
**=====================================================================================
*/

/*
**=====================================================================================
** Public type definitions
**=====================================================================================
*/

/**
 *************************************************************************************
 **
 **   An axis aligned rectangle
 **
 **************************************************************************************
 **/
typedef struct{
   S16 min_x;
   S16 min_y;
   S16 max_x;
   S16 max_y;
} drmathbbox_BBox;

/*
**=====================================================================================
** Public data declarations
**=====================================================================================
*/

/*
**=====================================================================================
** Public function declarations and macros
**=====================================================================================
*/

Public void drmathbboxif_initializeToNone(drmathbbox_BBox* box_ref);
Public void drmathbboxif_initialize(drmathbbox_BBox* box_ref, const drmathif_Matrix* transform_ref, S16 min_x, S16 min_y, S16 max_x, S16 max_y);
Public Boolean drmathbboxif_intersects(const drmathbbox_BBox* box_1_ref, const drmathbbox_BBox* box_2_ref);
Public void drmathbboxif_getUnion(const drmathbbox_BBox* box_1_ref, const drmathbbox_BBox* box_2_ref, drmathbbox_BBox* result_ref);

#endif /* DRMATHBBOXIF_H_ */
/** \} 
*********************************** End of file **************************************/

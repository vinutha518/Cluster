/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                   drglif.h                                      ***
***                                                                                 ***
***                               Project Drome MCU                                 ***
***                                                                                 ***
***          Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2018#$          ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file
   \ingroup GLIF 

   \Subject    DROME - Graphics Library Interface declarations. 

   \Abstract  
       This header file contains the public declarations of the graphics library
       interface. The GLIF interface is shared between different implementations
       of the graphics library, each targeting different hardware platforms.
       See the respective implementation for usage descriptions and details.

   \Version history record: 

   $Id: drglif.h 10 2018-12-03 11:36:39Z stoneridge $

   \{ 
***************************************************************************************
*/

#ifndef DRGLIF_H
#define DRGLIF_H

/*
**=====================================================================================
** Imported definitions
**=====================================================================================
*/

#include "dr.h"
#include "drome.h"
#include "drmathif.h"
#include "drhost.h"
#include "drwidbif.h"

/*
**=====================================================================================
** Public defines
**=====================================================================================
*/

#if drome_GLIB_VARIANT == drome_GLIB_DHD

/** Special return value used by drglif_getFirmwareId() */
#define drglif_FWID_UNDEFINED       0xFFFF

#endif

/*
**=====================================================================================
** Public type definitions
**=====================================================================================
*/

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

/** See the respective implementation for documentation! */
Public void drglif_init( void * imgdb_address );
Public dr_Status drglif_shutDown(Boolean force);

Public dr_Status drglif_updateDisplay(Boolean keep);
Public dr_Status drglif_clearScreen( void );

Public dr_Status drglif_freeViewResources(void);

Public void drglif_getDisplayTransform(U16 width, U16 height, drmathif_Matrix* result_ref);

Public dr_Status drglif_drawRectangle(U16 x, U16 y, U16 dx, U16 dy,
                                      dr_Colour col,
                                      const drmathif_Matrix* transform_ref,
                                      Boolean blend);
Public dr_Status drglif_drawRectangleWithColorKey(U16 x, U16 y, U16 dx, U16 dy,
                                       dr_Colour col,
                                       const drmathif_Matrix* transform_ref);

Public dr_Status drglif_drawArc(U16 x, U16 y, U16 radius_min, U16 radius_max,
      Real angle_start, Real angle_end, const drmathif_Matrix* transform_ref, dr_Colour col);

Public dr_Status drglif_drawArcWithTexture(U16 x, U16 y, U16 radius_min, U16 radius_max,
      Real angle_start, Real angle_end, const drmathif_Matrix* transform_ref, dr_Colour col,
      const drmathif_Matrix* img_transform_ref, dr_ImageId img_id);

Public dr_Status drglif_pasteImage(U16 x, U16 y,
                                   dr_ImageId img_id,
                                   const drmathif_Matrix* transform_ref,
                                   Boolean blend,
                                   dr_Colour col);

Public dr_Status drglif_pasteImage_pre(U16 x, U16 y,
                                   dr_ImageId img_id,
                                   const drmathif_Matrix* transform_ref,
                                   Boolean blend,
                                   Boolean pre_draw,
                                   dr_Colour col);

Public dr_Status drglif_clipImage(U16 x, U16 y, dr_ImageId img_id,
                                  U16 xofs, U16 yofs, U16 dx, U16 dy,
                                  const drmathif_Matrix* transform_ref,
                                  Boolean blend, dr_Colour col);
Public dr_Status drglif_setTextFont(dr_FontId font_id);
Public dr_Status drglif_setTextColour(dr_Colour col);
Public dr_Status drglif_setTextClipBox(U16 x, U16 y, U16 dx, U16 dy);
Public dr_Status drglif_writeText(const U16 *char_index_ptr, U16 count, const drmathif_Matrix* transform_ref);
Public dr_Status drglif_sync( void );

#if (drome_GLIB_VARIANT == drome_GLIB_DHD) || (drome_GLIB_VARIANT == drome_GLIB_OPENGL)

/* DHD specific functions */

Public drome_DispFramerate drglif_getDisplayFramerate( void );
Public dr_Status drglif_changeDisplayFramerate( drome_DispFramerate framerate );

Public dr_Status drglif_pasteSpriteImage(U16 x, U16 y, const drwidbif_SpriteAnimationProps* sprite_props,
                                         const drmathif_Matrix* transform_ref, Boolean blend);

Public Boolean drglif_hasPreRenderBuffer(dr_WidgetHandle widget_hdl);
Public dr_Status drglif_startPreRender(dr_WidgetHandle widget_hdl, U16 width, U16 height, U32 color_format);
Public dr_Status drglif_stopPreRender(void);
Public void drglif_killPreRender(dr_WidgetHandle widget);
Public dr_Status drglif_pastePreRenderBuffer(dr_WidgetHandle widget_hdl, U16 x, U16 y,
                                             const drmathif_Matrix* transform_ref,
                                             Boolean blend, dr_Colour col);
Public dr_Status drglif_scaleBufferOpacity(U8 opacity_factor, U16 width, U16 height);
Public dr_Status drglif_clipPreRenderBuffer(dr_WidgetHandle widget, U16 x, U16 y, U16 xofs, U16 yofs,
                                            U16 dx, U16 dy, const drmathif_Matrix* transform_ref,
                                            Boolean blend, dr_Colour col);

#ifdef drome_CONF_WIDGET_SIMPLE_POINTER
Public dr_Status drglif_renderPolygon(U16 x, U16 y, U16 dx, U16 dy,
                                      U16 origo_x, U16 origo_y, U16 rot_angle,
                                      U16 color, const dr_PolCoord shape[], U8 count);
#endif

#ifdef drapp_DATS
Public dr_Status drglif_setScreenAreaReadOut(U16 x, U16 y, U16 dx, U16 dy, drome_Colour * buffer_ptr);
Public Boolean drglif_isDispReadOutMode(void);
#endif
Public U16 drglif_getFirmwareId( void );
Public U32 drglif_getCRCOfImageDB( void );

#endif

#ifdef drome_CONF_WIDGET_VIDEO

/* Video specific functions */
Public dr_Status drglif_setVideoParams(U16 x, U16 y,
                                       U16 w, U16 h,
                                       U16 rot,
                                       S16 x_offs, S16 y_offs,
                                       dr_VideoDeinterlaceMode deinterl,
                                       U8 scale_ix, dr_VideoSignalFormat vid_fmt,
                                       dr_Colour color_key,
                                       const drmathif_Matrix* transform_ref);
Public dr_Status drglif_setVideoCaptureEnable(Boolean enable);
Public dr_Status drglif_setVideoSyncEnable(Boolean enable);

#endif

Public dr_Status drglif_doCyclic( void );
Public Boolean drglif_isIdle( void );

Public void drglif_freeResourcesBelongingToRenderBuffer(U8 renderBufferId);
Public void drglif_freeAllResources(void);

Public Boolean drgldisp_getFrameBufferHasAlpha(void);
Public void* drglif_getCurrentFrameBuffer(void);



Public dr_Status
drglif_ctxrotate(U16 x, U16 y, S16 angle);
Public dr_Status
drglif_ctxendrotate(void);

#if 0
Public void drgl_selectContext(U8 contextid);
#endif

Public dr_Status drgldisp_setTemporaryFrameBuffer(void* disp_buffer, U16 w, U16 h, U32 color_type);


#endif /* #ifdef DRGLIF_H */
/** \} // End GLIF */
/********************************** End of file **************************************/

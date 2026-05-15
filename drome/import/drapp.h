/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                    drapp.h                                      ***
***                                                                                 ***
***                               Project Drome MCU                                 ***
***                                                                                 ***
***          Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2019#$          ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file
   \ingroup IMPORTS

   \Subject    Application specific configuration of the Drome library.

   \Reference  None

   \Abstract
      This file uses define's to configure different aspects of the Drome library
      to suite the needs of the application. Key values like buffer lengths and
      update rates are defined as well as a mapping of application defined callback
      functions to defines used by the Drome library code. \n
   \n
      NOTE: It is the responsibility of the application integrator to assert the
            correctness of each definition in this file. Values used here are
            for example only!


   \Version history record:

   $Id: drapp.h 39 2019-06-19 09:10:47Z stoneridge $

   \{
***************************************************************************************
*/

#ifndef DRAPP_H
#define DRAPP_H

/*
**=====================================================================================
** Imported definitions
**=====================================================================================
*/

#include "dr.h"
#include "drhost.h"

/*
**=====================================================================================
** Public defines
**=====================================================================================
*/

/** Either define drapp_DISPLAY_TYPE with a value from the table or return the timing
 * parameters in the getDisplayParams() callback (see drome_CallbackFunctPtrs in
 * drome.h and drappif.h). \code
 * --- Type ---------------   --- Compatible parts ------------------------------------
 * dr_DISP_TYPE_WVGA_1      = TPO TD070WGEA1,      7.0" W (800xRGBx480) TFT-LCD.
 * dr_DISP_TYPE_VX_WVGA_1   = Varitronix           7.0" W (800xRGBx480) TFT-LCD.
 * dr_DISP_TYPE_SONY_WVGA_1 = Sony                 7.0" W (800xRGBx480) TFT-LCD.
 * dr_DISP_TYPE_QWGA_VX     = Varitronix           4.0/5.0" (240xRGBx320) TFT-LCD.
 * \endcode */
#if (!defined drapp_DISPLAY_TYPE)
   /* #define drapp_DISPLAY_TYPE                dr_DISP_TYPE_WVGA_1 */
#endif

/** Select the default/startup display framerate to use  \code
 * dr_DISP_FRAMERATE_50 = 50fps progressive, use with 25fps interlaced video (PAL).
 * dr_DISP_FRAMERATE_60 = 60fps progressive, use with 29.97fps interlaced video (NTSC).
 * \endcode */
#define drapp_DISPLAY_FRAMERATE           dr_DISP_FRAMERATE_60

/** Sets the maximum number of widgets that can responds to events in a view.
  * If more widget (w events) exist than set in this define, the exceeding widgets
  * will not receive any events. */
#define drapp_MAX_WID_W_EVENT             32U

/** TODO: write description */
#define drapp_MAX_DEPTH_OF_STATES         10U

/** Sets the buffer size used for text processing by all widgets displaying text.
 ** Will allocate 2*define bytes of RAM */
#ifndef drapp_TEXT_BUFFER_LEN
   #define drapp_TEXT_BUFFER_LEN          128U
#endif
	
/** Sets the buffer size used for data text processing by the data label widget.
 ** Will allocate 2*define bytes of RAM */
#define drapp_DATA_BUFFER_LEN             50U

/** Sets size of queue storing events sent from application to drome.
 ** Size shall be an even power of 2, i.e. 2,4,8,16,32 etc */
#define drapp_EVENT_QUEUE_SIZE            32U

/** Sets the minimum cut slider length for the scrollbar widget (i.e. the slider will
  * never be cut shorter, no matter how many elements used).
  * Caution: Don't set a value bigger than the smallest slider length used in the model.
  * Note: Only even values are supported! */
#define drapp_SCROLLBAR_MIN_SLIDER_LEN    10U

/** Defines how many images that can be stored in the Drome software image cache. The
  * cache is implemented as a meta data table in CPU RAM while the cached image data is
  * stored in VIDEO RAM. A large image cache will increase performance but consume RAM.
  * Each entry in the meta data table will consume 24 bytes of CPU RAM.*/
#ifndef drapp_NUM_CACHED_IMAGES_MAX
   #define drapp_NUM_CACHED_IMAGES_MAX    60U
#endif

/** Sets the maximum number of possible telltale positions for one complex telltale
  * widget. */
#define drapp_CTT_MAX_TT_COUNT            25U

/** Sets the maximum number of possible rows for a Scrolling Text widget.
  * Will allocate 2*define bytes of RAM */
#define drapp_STW_MAX_ROW                 32U

/** Data pool handle (from drconf.h) used by complex telltale widgets to request telltale
  * status values. */
#define drapp_CTT_DP_HANDLE               drome_CTT_TELLTALE_STATUS

/** Set the refresh interval for the specified widget.
  * Only two refresh intervals will be used, one for everything that should
  * be refreshed with a high frequency and one for the rest. If there are more than two
  * refresh intervals only the shortest and next shortest will be used.
  *   - Note 1: The time counter is reset at each view redraw. To completely disable refresh,
  *     set the corresponding interval to zero
  *   - Note 2: Refresh intervals for widgets placed inside an animation container will be overridden
  *     by the refresh rate of the animation container. */
#define drapp_BARGRAPH_REFRESH_INTERVAL            (20U)
#define drapp_DATA_LABEL_REFRESH_INTERVAL          (20U)  /**< \sa drapp_BARGRAPH_REFRESH_INTERVAL*/
#define drapp_SIMPLE_POINTER_REFRESH_INTERVAL      (20U)  /**< \sa drapp_BARGRAPH_REFRESH_INTERVAL*/
#define drapp_ANIMATION_CONTAINER_REFRESH_INTERVAL (20U)  /**< \sa drapp_BARGRAPH_REFRESH_INTERVAL*/
#define drapp_OTHER_REFRESH_INTERVAL               (200U) /**< \sa drapp_BARGRAPH_REFRESH_INTERVAL*/

/** drome_doCyclic() execution time limit. If the time is exceeded before the view is
  * completely drawn, rendering is continued on next call.
  * Note: 0xFFFFFFF0 is used for debugging of Drome, real applications should
  *       probably use shorter limits, e.g. 50000 or 2000. */
  //wenyu: modified for debugging purpose
#ifndef drapp_MAX_CYCLIC_TIME
#define drapp_MAX_CYCLIC_TIME            5000 // (us)
#endif

/** The size of the glyph cache in bytes. The cache is disabled if drapp_GLYPH_CACHE_SIZE
  * is not defined or if it is defined to be zero. */
// #define drapp_GLYPH_CACHE_SIZE              480UL // (bytes) 

/** Maximum rendering time for a single view. If this time is exceeded without any
  * updates of the display, a fatal error (drome_ERR_VIEW_RENDER_TIME_OUT) is reported
  * and Drome will cease to operate until it has been reinitialized using drome_init().
  * Of course relevant hardware drivers should also be reinitialized in case they are
  * the source of the problem.
  * Note: Set value to zero to disable this check. */
#define drapp_VIEW_RENDER_TIME_OUT        0UL  // (us)

/** If the video input cannot be found, Drome does not search for a video signal until
  * this timeout has passed */
#define drapp_VIDEO_RETRY_TIMEOUT        10000000UL  // (us)

/** Delay display controller to VPU sync. This delay will be introduced just after
  * video output to screen has been enabled. The video output will be distorted during
  * this delay but will be confined to the video area of the current view (hopefully),
  * and the delay may help to enhance the subjective impression of smoothness when
  * display sync is  transfered to the VPU. */
#define drapp_VIDEO_SYNC_DELAY            80000UL     // (us)

/** The display deinitializing might need a last white or black frame right before the
  * power to the display is switched off. Define either
#define drapp_WHITE_DISPLAY_ON_SHUTDOWN
  * for normally white displays or
#define drapp_BLACK_DISPLAY_ON_SHUTDOWN
  * for normally black displays to draw a last white or black frame. */
#ifndef drapp_BLACK_DISPLAY_ON_SHUTDOWN
   #define drapp_BLACK_DISPLAY_ON_SHUTDOWN
#endif

#if drome_GLIB_VARIANT == drome_GLIB_DHD

/** The default color for pixels not covered by any graphical layer. The only time this
  * might be visible is if the size of the video widget or the scaled video frame is
  * changed or if the view size is less than the size of the display.
  *
  * Format: 0x00RRGGBB
  *
  * \note This is only applicable when building for the DHD GPU. */
#ifndef drapp_DISPLAY_BACKGROUND_RGB888
   #define drapp_DISPLAY_BACKGROUND_RGB888 0x00000000UL
#endif

#endif

/* Mono h/w driver interface */
#if drome_GLIB_VARIANT == drome_GLIB_MONO
/** TODO: write description
 * \note This is only applicable when building for a mono system. */
#define drapp_DISP_RAM                    app_display_ram

/** TODO: write description
 * \note This is only applicable when building for a mono system. */
#define drapp_DISP_BORDER_WIDTH           1

/** TODO: write description
 * \note This is only applicable when building for a mono system. */
#if (drome_DISP_ORIENT==dr_ORIENT_0DEG) || (drome_DISP_ORIENT==dr_ORIENT_180DEG)
   // Landscape orientation
   #define drapp_DISP_RAM_SIZE               \
            ( (drome_DISP_WIDTH + (2 * drapp_DISP_BORDER_WIDTH)) * \
              (((drome_DISP_HEIGHT + (2 * drapp_DISP_BORDER_WIDTH)) + 7) / 8) )
#else
   // Portrait orientation
   #define drapp_DISP_RAM_SIZE               \
            ( (drome_DISP_HEIGHT + (2 * drapp_DISP_BORDER_WIDTH)) * \
              (((drome_DISP_WIDTH + (2 * drapp_DISP_BORDER_WIDTH)) + 7) / 8) )
#endif
#endif

/* Video specific interface */
#ifdef drome_CONF_WIDGET_VIDEO
/** TODO: write description (I2C bus access interface)
  * \note This is only applicable when video widget is active. */
#ifndef drapp_VCODEC_ADDRESS
   #define drapp_VCODEC_ADDRESS           0x42U
#endif

/** The amount of VRAM dedicated to store the video frame. This number must be chosen with
  * great care. If it is too small the video might not be visible but if it is too big
  * the rest of the graphics system might run out of memory.
  * For example the value must be equal to (or slightly larger due to the MCUs scaling
  * functions) the video widgets \code
  * width x height x 2 bytes
  * \endcode (every pixel is stored
  * as 16 bit RGB565).
  * \note This is only applicable when video widget is active. */
#ifndef drapp_VIDEO_IMAGE_MEM_SIZE
   #define drapp_VIDEO_IMAGE_MEM_SIZE     (86UL * 1024UL)
#endif
#endif

/** TODO: write description */
#define drapp_FRAMERATE_BUFFER_SIZE       32U

#endif /* DRAPP_H */
/** \} // End IMPORTS */
/********************************** End of file **************************************/

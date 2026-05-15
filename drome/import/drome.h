/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                   drome.h                                       ***
***                                                                                 ***
***                               Project Drome MCU                                 ***
***                                                                                 ***
***          Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2019#$          ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file 
   \ingroup DROMEIF 

   \Subject    Drome interface to application.

   \Reference  None

   \Abstract 
      This file contains all definitions the application program needs to use the
      Drome platform.
   
   \Version history record: 

   $Id: drome.h 39 2019-06-19 09:10:47Z stoneridge $

   \{ 
***************************************************************************************
*/

#ifndef DROME_H
#define DROME_H

/*
**=====================================================================================
** Imported definitions
**=====================================================================================
*/

#include "drhost.h"

/*
**=====================================================================================
** Drome configuration
**=====================================================================================
*/

/* Valid values for drome_GLIB_VARIANT */
#define drome_GLIB_MONO       0
#define drome_GLIB_DHD      2
#define drome_GLIB_OPENGL     3

/* Include compiler generated definitions */
#include "drconf.h"

/*
**=====================================================================================
** Public defines
**=====================================================================================
*/

/// Unicode code point used together with the text label and scrolling text widgets
/// to indicate that the text has changed and Drome thus must re-break the entire
/// text from the beginning. To indicate a change, this value shall be returned as
/// the first and only code unit when Drome queries the corresponding data pool
/// handle using drapp_GET_DP_VAL_AS_STRING().
#define drome_LONG_TEXT_UPDATED     0xFFFF

/*
**=====================================================================================
** Public type definitions
**=====================================================================================
*/

typedef U16 drome_VariantId;
typedef U16 drome_LanguageId;
typedef U16 drome_Colour;
/** Used to clarify that the texts is encoded in UTF-8 format*/
typedef U8  UTF8;

/// return type for drome Cyclic call
typedef enum 
{
   drome_CYCLIC_IDLE,          ///< No ongoing activities in Drome or graphics controller.
   drome_CYCLIC_FINISHED,      ///< Cyclic call was able to return control before being out-of-time.
   drome_CYCLIC_NOT_FINISHED,  ///< Cyclic did not finish all its tasks, will continue on next cyclic.
   drome_CYCLIC_ERROR          ///< A fatal error occurred, Drome requires reinitialization.
} drome_CyclicStat;

/// Parameter in safeToUpdateGlobalProperties callback
typedef enum
{
   drome_RD_REDRAW,            ///< Next rendering will redraw all widgets in the current view.
   drome_RD_REFRESH            ///< Next rendering will only update widgets needing high refresh rate.
} drome_RedrawType;

#if (drome_GLIB_VARIANT == drome_GLIB_DHD) || (drome_GLIB_VARIANT == drome_GLIB_OPENGL)
#ifdef drapp_DATS
/// return type for display read out functions
typedef enum
{
   drome_SDROPS_OK, ///< Done, successful setting of readout params.
   drome_SDROPS_BAD_ARG, ///< Requested read out area is out of display range.
   drome_SDROPS_BAD_STATE ///< Not able to set params due to wrong state, try again.
}drome_SetDispReadOutParamStat;
/// return type for display read out functions
typedef enum
{
   drome_DROS_FINISHED, ///< Done, read out of display image is finished.
   drome_DROS_NOT_FINISHED, ///< Readout ongoing, but not finished, repeat function call.
   drome_DROS_BAD_STATE, ///< Display read out params have not been set correctly, call drome_setScreenAreaReadOut(..) again.
   drome_DROS_ERROR ///< Error occurred during read out.
}drome_DispReadOutStat;
#endif
#endif

/** Format parameters used when requesting strings from the application data pool */
typedef struct
{
   Boolean append_unit;       ///< Shall unit be added after value or not
   Boolean round;             ///< Round value (if not, truncate it)
   U8      filter;            ///< Filter to use when calculating the data value
   U8      digits_fraction;   ///< Number of decimals to use
   U8      digits_integer;    ///< Number of integers to use
} drome_DPStrFormat;

/** Error codes reported by Drome to the application. */
typedef enum
{
   /* Note: Don't change numbers (to avoid confusion with old versions)! */

   drome_ERR__FATAL = 0x8000,
      ///< This code is used to mark error codes as fatal, meaning that Drome will
      ///< not display anything, alt. cease to operate.
      ///< \n \n

   drome_ERR_INVALID_DATABASE = 0x0000 + drome_ERR__FATAL,
      ///< Invalid database detected (missing magic number).

   drome_ERR_INVALID_DATABASE_VERSION = 0x0001 + drome_ERR__FATAL,
      ///< Invalid database version detected (wrong compiler used).

   drome_ERR_INVALID_CONFIGURATION_USED = 0x0002 + drome_ERR__FATAL,
      ///< Colour or mono database used with wrong type of library/application.

   drome_ERR_INVALID_CONDITIONAL_IMAGE_INDEX = 0x0003,
      ///< Image index returned from the application was out of range.

   drome_ERR_INVALID_CONDITIONAL_REFERENCE_INDEX = 0x0004,
      ///< Conditional reference index returned from the application was out of range.

   drome_ERR_EVENT_QUEUE_OVERFLOW = 0x0005,
      ///< An event (internally generated or sent by the application) was lost because
      ///< the Drome internal event queue was full.

   drome_ERR_WIDGET_DRAWING_ERROR = 0x0006,
      ///< An error occured while drawing a view, the view may not appear as intended. \n
      ///< NOTE: This code may be reported by it self, or in addition to another code
      ///< generated during the same Drome cyclic call.

   drome_ERR_GRAPHICAL_OBJECT_INFLATE = 0x0007,
      ///< Drome failed to unpack a graphical object

   drome_ERR_INVALID_LIST_HIGHLIGHT_ROW = 0x0008,
      ///< List row highlight position returned from the application was not on a
      ///< valid/visible row.

   drome_ERR_INVALID_CONDITIONAL_TEXT_INDEX = 0x0009,
      ///< Text index returned from the application was out of range.

   drome_ERR_INVALID_UTF8_DATA_IN_DATABASE = 0x000A,
      ///< The text data read from the HMI database contained invalid UTF-8, e.g.
      ///< an UTF-16 surrogate code encoded as UTF-8 or a character above the
      ///< standard 17 Unicode code planes.

   drome_ERR_APPL_TEXT_STRING_TOO_LONG = 0x000B,
      ///< A string returned from the application was too long (no null term. found).
      ///< This is serious because it might indicate a buffer overrun!

   drome_ERR_VIEW_RENDER_TIME_OUT = 0x000C + drome_ERR__FATAL,
      ///< The maximum allowed view rendering time as configured in drapp.h has been
      ///< exceeded.

   drome_ERR_IMAGE_OBJECT_TOO_BIG = 0x000D,
      ///< An image object was too big to store in the image object cache.

   drome_ERR_FONT_OBJECT_TOO_BIG = 0x000E,
      ///< A font object was too big to store in the font object cache.

   drome_ERR_STATE_MACHINE_DB = 0x000F + drome_ERR__FATAL,
      ///< Corrupt or invalid data found in the state machine db.

   drome_ERR_CTT_WIDGET_SIZE = 0x0010,
      ///< The model contained a Complex Telltale widget with more than
      ///< drapp_MAX_CTT_MAX_TT_COUNT possible telltale positions.

   drome_ERR_APP_CONFIG_MISSMATCH = 0x0011 + drome_ERR__FATAL,
      ///< The model was compiled using config.xml information different from the
      ///< drconf.h file used to build dromelib.

   drome_ERR_DB_LIB_DISP_SIZE_MISSMATCH = 0x0012 + drome_ERR__FATAL,
      ///< The model was compiled using a display width and/or height different from
      ///< the one that was specified when dromelib was built.

   drome_ERR_DB_LIB_DISP_ORIENT_MISSMATCH = 0x0013 + drome_ERR__FATAL,
      ///< The model was compiled using a display orientation different from the one
      ///< that was specified when dromelib was built.

   drome_ERR_HMI_AND_GRAPHICS_DB_MISSMATCH = 0x0014 + drome_ERR__FATAL,
      ///< The compatibility code of the graphics database differs from the value
      ///< stored in the HMI database.

   drome_ERR_WIDGET_DB = 0x0015 + drome_ERR__FATAL,
      ///< Corrupt or invalid data found in the widget db.

   drome_ERR_TEXT_STRING_DB = 0x0016 + drome_ERR__FATAL,
      ///< Corrupt or invalid data found in the text string db.

   drome_ERR_GRAPICAL_OBJECT_DB = 0x0017 + drome_ERR__FATAL,
      ///< Corrupt or invalid data found in the graphical object db.

   drome_ERR_DATA_POOL_DB = 0x0018 + drome_ERR__FATAL,
      ///< Corrupt or invalid data found in the data pool db.

   drome_ERR_DATABASE_ALIGNMENT = 0x0019 + drome_ERR__FATAL,
      ///< A non word-aligned DB address supplied to \ref drome_init().
      ///< For speed reasons, the DB must be aligned to an even 4 byte boundary.

   drome_ERR_SPW_NR_OF_POINTS = 0x001B,
      ///< The number of points used for the polygon is greater than the defined maximum 
      ///<  nr of points specified in drapp
      
   drome_ERR_HISTORY_STATE_INCONSISTENCY = 0x001D,
      ///<  A history state point to a state that is not a child state of
      ///<  that history states compound state. If the application is storing the history
      ///< state data in non-volatile memory that memory needs to be reset when a new model
      ///< is loaded. If not this error might occur.

   drome_ERR_UNABLE_TO_INITIALIZE_GRAPHICS_SYSTEM = 0x001D + drome_ERR__FATAL,
      ///<  The graphics system could not be initialized

   drome_ERR_ANIMATION_DATA_INCONSISTENCY = 0x001E + drome_ERR__FATAL,
      ///<  The number of animations in database is more then the maximum allowed
      ///<  which indicates that that the database does not match the application.

   drome_ERR_INVALID_CALLBACK_FUNCTION = 0x001F + drome_ERR__FATAL,
      ///<  One or more of the function pointers that was submitted to initialize the Drome
      ///<  callback function interface is a bad pointer.

   drome_ERR_APP_USER_CONFIG_MISSMATCH = 0x0020 + drome_ERR__FATAL,
      ///< The model was compiled using UserConfig.xml information different from the
      ///< druserconf.h file used to build the application.

   drome_ERR_OUT_OF_VIDEO_MEMORY = 0x0021,
      ///< An allocation of video memory failed because no contiguous block of memory of
      ///< the requested size was available. This might cause some animations to stop
      ///< working or be fatal to the rendering of the entire view.

   drome_ERR_INVALID_CONDITIONAL_DATA_LABEL_INDEX = 0x0022,
      ///< Conditional Data Label index returned from the application was out of range.

   drome_ERR_GRAPICHS_SYSTEM_FATAL_CONDITION = 0x0023 + drome_ERR__FATAL,
      ///< The graphics library has reported an unrecoverable error. Drome needs to be
      ///< restarted to reset the graphics related HW and drivers.

   drome_ERR_LANGPACK_CRC_INCONSISTENT_WITH_GUIDEDB = 0x0024,
      ///< The language pack crc does not match the Guide database, hence the text string properties
      ///< differs between the Guide model used for generating the language packs and the model
      ///< used for generating the Guide database.

   drome_ERR_LANGPACK_CRC_INCONSISTENT_WITH_IMGDB = 0x0025,
      ///< The language pack crc does not match the img database, hence the graphics (fonts)
      ///< differs between the Guide Model used for generating the language packs and the model
      ///< used for generating the img db database.

   drome_ERR_EVENT_ID_OUT_OF_RANGE = 0x0026,
      ///< An event with id outside the allowed range has been passed to the event handler.

   drome_ERR_OPENGL_SHADER_COMPILE_ERROR = 0x0027 + drome_ERR__FATAL
      ///< The OpenGL Shader Compiler reported an error.

   /* Note: Add new codes here! Don't change numbers or remove codes! */
} drome_ErrorCode;

#ifdef drome_CONF_USE_PUBLIC_OBJECTS
/** Meta data for public font characters */
typedef struct
{
   U8 char_width;          /**< Width of this character (pels) */
   U8 top_offset;          /**< Distance from top of font to top of character */
   U8 char_height;         /**< Number of lines containing set pixels */
   const U8 * bmp_ptr;     /**< Pointer to character bitmap */
} drome_PubFontCharData;

/** Meta data for public images */
typedef struct
{
   U16 img_width;          /**< Width of the image */
   U16 img_height;         /**< Height of the image */
   const U8 * bmp_ptr;     /**< Pointer to the bitmap */
} drome_PubImageData;
#endif

/** Version info items available from drome_get_versionInfo().
  * The suffix of each item indicates the type of the returned value:
  * - ZSTR = Zero terminated string.
  * - U16  = 16-bit unsigned value.
  * - U32  = 32-bit unsigned value.
  *
  * NOTE: 
  * The GC variant and revision is read from the graphics controller by
  * GLIB as a part of the dromelib initialization during the first number
  * of calls to drome_doCyclic(), returns 0xFFFF if not read..
*/
typedef enum
{
   drome_VERINFO_LIB_VER_ZSTR,       ///< Dromelib version.
   drome_VERINFO_GC_VAR_REV_U16,     ///< Graphics controller variant (MSB) and rev (LSB).
   drome_VERINFO_DB_COMP_VER_ZSTR,   ///< Drome compiler version used for loaded GuideDB.
   drome_VERINFO_DB_PARTNUM_ZSTR,    ///< GuideDB part number.
   drome_VERINFO_DB_PARTVER_ZSTR,    ///< GuideDB part version.
   drome_VERINFO_DB_MODELCRC_U32,    ///< CRC32 of Guide model items (0 if not calculated).
   drome_VERINFO_DB_TEXTCRC_U32,     ///< CRC32 of text meta items (0 if not calculated).
   drome_LANGPACK_CUSTOMER_PARTNO,   ///< PARTNO of loaded language pack (NULL if not existing).
   drome_LANGPACK_SRE_PARTNO        ///< PARTNO of loaded language pack (NULL if not existing).
} drome_VerInfoID;


/** Display framerate, see drapp_DISPLAY_FRAMERATE */
typedef enum
{
   dr_DISP_FRAMERATE_50,         /**< 50fps progressive */
   dr_DISP_FRAMERATE_60,         /**< 60fps progressive */
   dr_DISP_FRAMERATE_COUNT,      /**< Number of supported framerates */
   dr_DISP_FRAMERATE_NOT_SET     /**< No framerate has been set */
} drome_DispFramerate;

/*******************************************************************************
  Section: Global Defines 
  
  R_DDB_DISP_FLAG_RSDS_SHIFT - Bit start position of RSDS flags. See <r_ddb_TimingFlags_t>. 
*/
#define R_DDB_DISP_FLAG_RSDS_SHIFT    (6u)


/*******************************************************************************
  Section: Enumerations 
*/

/*******************************************************************************
  Enum: r_ddb_TimingFlags_t

  DDB Timing Flags.
  List of all available Timing Flags of the Display DataBase

  Values:
  R_DDB_DISP_FLAG_NONE          - empty flag
  R_DDB_DISP_FLAG_VOEN          - display requires VOEN (DATA ENABLE) signal instead of VSYNC
  R_DDB_DISP_FLAG_CSYNC         - display requires CSYNC signal instead of H/VSYNC
  R_DDB_DISP_FLAG_NEGCLK        - display requires neg. DCLK (data out with falling edge,
                                                             data stable with rising edge of VOnCLK)
  R_DDB_DISP_FLAG_HSYNC_ACTHI   - use active high level for HSYNC
  R_DDB_DISP_FLAG_VSYNC_ACTHI   - use active high level for VSYNC
  R_DDB_DISP_FLAG_DESYNC_ACTHI  - use active high level for DATE ENABLE SYNC
  R_DDB_DISP_FLAG_RSDS_RBSW     - display requires bit swap (off:even bit first, on:odd bit first)
  R_DDB_DISP_FLAG_RSDS_PHSEL90  - display requires phase shift of 90-degree (default)
  R_DDB_DISP_FLAG_RSDS_PHSEL180 - display requires phase shift of 180-degree 
  R_DDB_DISP_FLAG_RSDS_PHSEL270 - display requires phase shift of 270-degree 
  R_DDB_DISP_FLAG_RSDS_PHSEL360 - display requires phase shift of 360/0-degree 
  R_DDB_DISP_FLAG_RSDS_ENABLE   - display requires RSDS mode 

  Note:
  DO NOT change the bit position relation from R_DDB_DISP_FLAG_RSDS_RBSW to 
  R_DDB_DISP_FLAG_RSDS_ENABLE.
  
  R_DDB_DISP_FLAG_RSDS_RBSW and R_DDB_DISP_FLAG_RSDS_PHSELxx are valid when 
  R_DDB_DISP_FLAG_RSDS_ENABLE is set.
*/

typedef enum
{
    R_DDB_DISP_FLAG_NONE          = (0uL << 0u),
    R_DDB_DISP_FLAG_VOEN          = (1uL << 0u),
    R_DDB_DISP_FLAG_CSYNC         = (1uL << 1u),
    R_DDB_DISP_FLAG_NEGCLK        = (1uL << 2u),
    R_DDB_DISP_FLAG_HSYNC_ACTHI   = (1uL << 3u),
    R_DDB_DISP_FLAG_VSYNC_ACTHI   = (1uL << 4u),
    R_DDB_DISP_FLAG_DESYNC_ACTHI  = (1uL << 5u),
    R_DDB_DISP_FLAG_RSDS_RBSW     = (1uL << (R_DDB_DISP_FLAG_RSDS_SHIFT + 0u)),
    R_DDB_DISP_FLAG_RSDS_PHSEL90  = (0uL << (R_DDB_DISP_FLAG_RSDS_SHIFT + 1u)),
    R_DDB_DISP_FLAG_RSDS_PHSEL180 = (1uL << (R_DDB_DISP_FLAG_RSDS_SHIFT + 1u)),
    R_DDB_DISP_FLAG_RSDS_PHSEL270 = (2uL << (R_DDB_DISP_FLAG_RSDS_SHIFT + 1u)),
    R_DDB_DISP_FLAG_RSDS_PHSEL360 = (3uL << (R_DDB_DISP_FLAG_RSDS_SHIFT + 1u)),
    R_DDB_DISP_FLAG_RSDS_ENABLE   = (1uL << (R_DDB_DISP_FLAG_RSDS_SHIFT + 3u)) 
} r_ddb_TimingFlags_t;


/*******************************************************************************
  Section: Structures
*/

/*******************************************************************************
  Typedef: r_ddb_SubTiming_t

  Sub-Timing information of horizontal / vertical display line.
 
> 
>           +-----------+
>  SYNC     |           |
>  ---------+           +------------------------------------
>                        
>                                +----------------------+
>  DATA EN                       |                      |
>  ------------------------------+                      +----
>                       
>    Front      Sync      Back
>  <-Porch-><---Width--><-Porch->
>  <---- BlankWidth ------------>
>  <------------------------- Total ------------------------>
>
>  BlankWidth = Front Porch + Sync Width + Back Porch
> 
  
  Members:
  Total          - length of total line
  BlankWidth     - length of blanking period
  FrontPorch     - length of front porch
  SyncWidth      - length of active sync pulse
*/     

typedef struct
{
    uint32_t Total;
    uint32_t BlankWidth;
    uint32_t FrontPorch;
    uint32_t SyncWidth;
} r_ddb_SubTiming_t;


/*******************************************************************************
  typedef: r_ddb_Timing_t

  Timing information of display

  Members:
  Name           - display name as unique idendifier a specific display
  ScreenWidth    - width of screen (in pixel)
  ScreenHeight   - height of screen (in pixel)
  H              - horizontal sub-timing see: <r_ddb_SubTiming_t>
  V              - vertical sub-timing see: <r_ddb_SubTiming_t>
  Flags          - Timing Flags see: <r_ddb_TimingFlags_t>
  PixelClock     - pixel clock frequency (in MHz)
*/     

typedef struct
{
    const char          *Name;
    uint32_t            ScreenWidth;
    uint32_t            ScreenHeight;
    r_ddb_SubTiming_t   Hsync;
    r_ddb_SubTiming_t   Vsync;
    uint32_t            Flags;
    uint32_t            PixelClock;
} r_ddb_Timing_t;


/***********************************************************
  Enum: r_wm_OutColorFmt_t

  Description:
  A parameter of this type specifies the colour mode of the video output. 

  Members:
  (A)RGB(w)xyz - The value specifies the number of bits for each colour.

  supported color modes :
  (code)
  R_WM_OUTCOLORFMT_RGB888             24bit           rrrrrrrrggggggggbbbbbbbb
  R_WM_OUTCOLORFMT_RGB666             18bit           ------rrrrrrggggggbbbbbb
  R_WM_OUTCOLORFMT_RGB565             16bit           --------rrrrrggggggbbbbb
  (end)
  

  ***Documentation of flags it subject to be moved to system-specific implementation***
  
  supported flags :
  R_WM_OUTCOLORFMT_FLAG_ENDIAN        change endianess of output colour
  R_WM_OUTCOLORFMT_FLAG_SWAP_BR       swap blue and red channel of output color
  R_WM_OUTCOLORFMT_FLAG_DITHER        activate dithering applied to output colour
  
  Result of flags is shown as example for RGB666.
  (code)
                                                      MSB                     LSB
  R_WM_OUTCOLORFMT_FLAG_ENDIAN                        ------ rrrrrr gggggg bbbbbb
                                    FLAG=0:           ------ 765432 765432 765432
                                    FLAG=1:           ------ 234567 234567 234567

                                                      MSB                     LSB
  R_WM_OUTCOLORFMT_FLAG_SWAP_BR                       ------ rrrrrr gggggg bbbbbb
                                    FLAG=0:           ------ hgfedc ba9876 543210
                                    FLAG=1:           ------ 543210 ba9876 hgfedc
  (end)

  Note:
  Not all displays support all formats.
*/

typedef enum
{
    R_WM_OUTCOLORFMT_RGB888 = 0u,
    R_WM_OUTCOLORFMT_RGB666 = 1u,
    R_WM_OUTCOLORFMT_RGB565 = 2u,
    R_WM_OUTCOLORFMT_LAST = 3u,
    
    /* Documentation of flags is subject to be moved to system-specific implementation */
    R_WM_OUTCOLORFMT_FLAG_DITHER  = (uint32_t) 1u<<28u,
    R_WM_OUTCOLORFMT_FLAG_SWAP_BR = (uint32_t) 1u<<29u,
    R_WM_OUTCOLORFMT_FLAG_ENDIAN  = (uint32_t) 1u<<30u,
    /* We do not use the 31th bit if not needed. Saves some signedness castings */
    R_WM_OUTCOLORFMT_FLAG_MASK = (R_WM_OUTCOLORFMT_FLAG_ENDIAN
                                    + R_WM_OUTCOLORFMT_FLAG_SWAP_BR
                                    + R_WM_OUTCOLORFMT_FLAG_DITHER),
} r_wm_OutColorFmt_t;




/** Function pointers to the drapp callback interface. This struct shall be provided
 *  to Drome in the drome_init() call. None of the function pointers can be set to
 *  NULL or Drome will return an error and will not be initialized.
 */
typedef struct
{
   /* Mandatory callback functions */
   void (*detectedDromeError)          (drome_ErrorCode);
   void (*detectedCodeError)           (U32,U32);
   void (*setDPValAsNumeric)           (U16,U32,U16);
   void (*getDPValAsNumeric)           (U16,U32*,U16);
   void (*getDPValAsString)            (U16,drome_DPStrFormat*,U16*,U16,U16);
   U32  (*getTimeInUs)                 (void);
   U32  (*getTimeInMs)                 (void);
   U32  (*getUserConfChecksum)         (void);
   /* Optional callback functions */
   void (*loadHistoryStates)           (U32*,U16);
   void (*saveHistoryStates)           (const U32*,U16);
   void (*sendEventToApplication)      (U32);
   U32  (*getNullEvent)                (void);
   U32  (*getLastInEvent)              (void);
   U32  (*getMaxEvents)                (void);
   void (*getDPValMaxValue)            (U16,U32*,U16);
   const r_ddb_Timing_t* (*getDisplayParams) (void);
   const r_wm_OutColorFmt_t (*getOutputColorFormat)(void);
   void (*getExternalText)             (U16,U16*,U16,U16);
   void (*getCTTValueVector)            (const void *,void*,U16,U16);
   void (*updateDisplay)               (const U8*);
   Boolean (*i2cOpen)                  (void);
   void    (*i2cClose)                 (void);
   Boolean (*i2cWriteStart)            (U8,const U8*,U16);
   Boolean (*i2cReadStart)             (U8,const U8*,U16,U8*,U16);
   void    (*i2cIssueStop)             (void);
   Boolean (*getScaledVideoSize)       (U8,U16*,U16*);
   void (*safeToUpdateGlobalProperties)(drome_RedrawType, Boolean*);
} drome_CallbackFunctPtrs;

/**
 *  Measured framerate data
 */
typedef struct
{
   U16 min;       /**< Min time measured for drawing a frame [ms] */
   U16 max;       /**< Max time measured for drawing a frame [ms] */
   U16 average;   /**< Average time measured for drawing a frame [ms] */
   U16 min_long;  /**< Min time measured for drawing a frame measure over long period[ms] */
   U16 max_long;  /**< Max time measured for drawing a frame measure over long period[ms] */
   U16 ave_long;  /**< Average time measured for drawing a frame measure over long period[ms] */
} drome_FrameRateData;

/**
 * Enum which list the entries of the drome_ProfilingData array
 * which is returned by function drome_getProfilingData.
 */
typedef enum
{
   DR_APP_SET_DP_VAL_AS_NUMERIC, /**< Index of profiling data for drapp_SET_DP_VAL_AS_NUMERIC*/
   DR_APP_GET_DP_VAL_AS_NUMERIC, /**< Index of profiling data for drapp_GET_DP_VAL_AS_NUMERIC*/
   DR_APP_GET_DP_VAL_MAX_VAL,    /**< Index of profiling data for drapp_GET_DP_VAL_MAX_VAL*/
   DR_APP_GET_DP_VAL_AS_STRING,  /**< Index of profiling data for drapp_GET_DP_VAL_AS_STRING*/
   DR_NUM_OF_MEASURE_POINTS
} drome_ProfilingMeasurePoints;

/**
 * Measured profiling data for a measure point.
 */
typedef struct
{
   U32 max;
   U32 count;
   U32 tot;
} drome_ProfilingData;

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

Public void drome_init(
   void * db_address,
   void * imgdb_address,
   drome_CallbackFunctPtrs * callbacks_ptr,
   U32 lang_pack_address
);   
Public drome_CyclicStat drome_doCyclic(void);
Public Boolean drome_shutDown(void);
Public Boolean drome_saveNVParams(void);

Public const void * drome_getVersionInfo(drome_VerInfoID id);
Public void    drome_drawViewWithIndex(U32 index);
Public Boolean drome_drawViewWithKey(U32 view_key);
Public Boolean drome_redrawView(void);

Public Boolean drome_setLanguage(drome_LanguageId language);
Public U16 drome_getNumberOfLanguages(void);
Public drome_LanguageId drome_getLanguageCode(U16 index);

Public U32 drome_getCurrentViewKey( void );

#ifdef drome_CONF_USE_DYNAMIC_VARIANTS
Public Boolean drome_setVariant(drome_VariantId variant);
Public U16 drome_getNumberOfVariants(void);
Public drome_VariantId drome_getVariantCode(U16 index);
#endif

#ifdef drome_CONF_USE_SM_AND_EVENTS
Public Boolean drome_returnViewControlToStateMachine(void);
Public Boolean drome_sendEventToDrome(U32 event_in);
#endif

Public void drome_resetProfiling(void);
Public const drome_ProfilingData* drome_getProfilingData(void);

#ifdef drome_CONF_USE_PUBLIC_OBJECTS
Public U16 drome_getPubFontHeight( U8 pub_font_id );
Public Boolean drome_getPubFontCharData(
   U8 pub_font_id, U32 char_code, drome_PubFontCharData * dest_ptr
);
Public Boolean drome_getPubImageData(
   U8 pub_image_id, drome_PubImageData * dest_ptr
);
#endif

#ifdef drome_CONF_WIDGET_VIDEO
Public void drome_enableVideo(
   Boolean videoEnable              /**<[in] FALSE = Video will 
                                       not be initialized or drawn
                                       in the widget drawing loop */
);
#endif



#if (drome_GLIB_VARIANT == drome_GLIB_DHD) || (drome_GLIB_VARIANT == drome_GLIB_OPENGL)
#ifdef drapp_DATS

#ifdef drapp_GREYSCALE
Public drome_SetDispReadOutParamStat drome_GreyTFTBufDump(U16 w,U16 h,U8 *buffer_ptr);
#endif
Public drome_SetDispReadOutParamStat drome_setScreenAreaReadOut(U16 x, U16 y, U16 dx, U16 dy, drome_Colour * buffer_ptr);
Public drome_DispReadOutStat drome_doScreenAreaReadOut(void);
#endif
Public void drome_getFrameRateData(drome_FrameRateData * data);
Public void drome_clearFrameRateData(void);
#endif

#endif /* ifndef DROME_H */
/** \} // End DROMEIF */
/********************************** End of file **************************************/

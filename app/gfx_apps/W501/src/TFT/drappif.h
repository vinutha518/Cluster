/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                   drappif.h                                     ***
***                                                                                 ***
***                               Project Drome MCU                                 ***
***                                                                                 ***
***          Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2018#$          ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file
   \ingroup IMPORTS 

   \Subject    Drome application call-back interface definition.

   \Reference  None

   \Abstract
      This file uses the variable and function name mapping defined by drapp.h to
      declare function prototypes for the application variables and call-back
      functions used by dromelib. \n


   \Version history record: 

   $Id: drappif.h 10 2018-12-03 11:36:39Z stoneridge $

   \{ 
***************************************************************************************
*/

#ifndef DRAPPIF_H
#define DRAPPIF_H

/*
**=====================================================================================
** Imported definitions
**=====================================================================================
*/

#include "drapp.h"
#include "drome.h"
#include "drhost.h"

/*
**=====================================================================================
** Public defines
**=====================================================================================
*/

/*
**=====================================================================================
** Public data declarations
**=====================================================================================
*/

#if drome_GLIB_VARIANT == drome_GLIB_MONO

/** Display RAM buffer */
extern U8 drapp_DISP_RAM[ drapp_DISP_RAM_SIZE ];

#endif

/*
**=====================================================================================
** Public function declarations and macros
**=====================================================================================
*/

/**
**************************************************************************************
**
**   Tells application that drome has detected an error. \n
**   The function is not used for Drome code errors but for errors in the Drome
**   environment, e.g. the HMI model, connected hardware, etc.
**   Depending on the error the application has to perform an appropriate action.
**
**   NOTE: On fatal errors, the complete Drome subsystem must be reinitialized.
**
**   NOTE: To Dromelib developers: *never* call this function directly,
**         *always* use the wrapper function dr_wrapDetectedDromeError() instead!
**
**************************************************************************************
**/
Public extern void (*drapp_DETECTED_DROME_ERROR) (
   drome_ErrorCode error_code          /**<[in] The error code */
);

/**
**************************************************************************************
**
**   Tells application that drome has detected an error. \n
**   The function is used for Drome code errors.
**
**   NOTE: To Dromelib developers: this function should be called via the drdbg debug
**   macros.
**
**************************************************************************************
**/
Public extern void (*drapp_DETECTED_CODE_ERROR) (
   U32 level_file_line,                /**< (Level << 28) + (File << 16) + Line */
   U32 data                            /**< User data */
);

/**
**************************************************************************************
**
**   Stores an integer value in a data pool parameter.
**
**************************************************************************************
**/
Public extern void (*drapp_SET_DP_VAL_AS_NUMERIC)(
   U16 data_handle,  /**< A handle to the data pool parameter. */
   U32 val,          /**< Value to store in datapool parameter */
   U16 argument      /**< Optional argument, parameter specific meaning */
);

/**
**************************************************************************************
**
**   Retrieves a data pool parameter as an integer.
**   Shall set value to zero if data_handle is invalid.
**
**************************************************************************************
**/
Public extern void (*drapp_GET_DP_VAL_AS_NUMERIC)(
   U16 data_handle,  /**< A handle to the data pool parameter. */
   U32* val_ptr,     /**< Pointer to store data pool parameter value to */
   U16 argument      /**< Optional argument, parameter specific meaning */
);

/**
**************************************************************************************
**
**   Retrieves a data pool parameters maximum value.
**   Shall set value to zero if data_handle is invalid.
**
**************************************************************************************
**/
#ifdef drome_CONF_WIDGET_BARGRAPH
Public extern void (*drapp_GET_DP_VAL_MAX_VAL)(
   U16 data_handle,  /**< A handle to the data pool parameter. */
   U32* val_ptr,     /**< Pointer to store data pool parameter value to */
   U16 argument      /**< Optional argument, parameter specific meaning */
);
#endif

/**
**************************************************************************************
**
**   Retrieves a data pool parameter as a string. \n
**   See definition of drome_DPStrFormat for details about formatting parameters.
**   The string character data shall be encoded using UTF-16. The string shall
**   always be null terminated, if the parameter is not available, the string shall
**   be empty (i.e. null only).
**
**   NOTE: The string length is in UTF-16 code units, i.e. number of U16's.
**
**************************************************************************************
**/
Public extern void (*drapp_GET_DP_VAL_AS_STRING)(
   U16 data_handle,           /**< A handle to the data pool parameter. */
   drome_DPStrFormat* format, /**< Struct with format settings for data values (may be NULL!) */
   U16* str_ptr,              /**< Pointer to where string shall be stored */
   U16 str_len,               /**< Maximum string length, including null termination */
   U16 argument               /**< Optional argument, parameter specific meaning */
);

/**
**************************************************************************************
**
**   Retrieves a data pool parameter as a vector of entries.
**
**************************************************************************************
**/
Public extern void (*drapp_GET_CTT_VALUE_VECTOR)(
   const void * set_ptr,    /**< Pointer to data set specification, parameter specific meaning. */
   void * dest_ptr,         /**< Pointer to buffer where to store data. */
   U16 dest_size,           /**< Size of destination buffer, unit is parameter specific. */
   U16 argument             /**< Optional argument, parameter specific meaning. */
);

/**
**************************************************************************************
**
**   This application function is used by DROME to load history state data stored
**   in non-volatile memory during Drome initialisation.
**
**************************************************************************************
**/
Public extern void (*drapp_LOAD_HISTORY_STATES)(
	U32* data_ptr,      /**< Pointer to the history state buffer where the app. shall get the data. */
	U16 size            /**< Number of bytes that shall be saved. */
);

/**
**************************************************************************************
**
**   This application function is used by DROME to save history state data in
**   non-volatile memory during Drome shut-down.
**
**************************************************************************************
**/
Public extern void (*drapp_SAVE_HISTORY_STATES)(
	const U32* data_ptr,      /**< Pointer to the history state buffer where the app. shall get the data. */
	U16 size                 /**< Number of bytes that shall be saved. */
);

/**
**************************************************************************************
**
**   Retrives a text string. \n
**   The text string handles are mapped to model entities using the index file
**   produced by the compiler (if the "use external texts" option is enabled).
**   The string character data shall be encoded using UTF-16. The string shall
**   always be null terminated. If the parameter is not available, the string shall
**   be empty (i.e. null only).
**
**   NOTE: The string length and offset is expressed in UTF-16 code units,
**         i.e. number of U16's.
**
**************************************************************************************
**/
Public extern void (*drapp_GET_EXTERNAL_TEXT)(
   U16 text_handle,           /**< A handle to the text string */
   U16* str_ptr,              /**< Pointer to where the text string shall be stored */
   U16 str_len,               /**< Maximum string length, including null termination */
   U16 offset                 /**< Offset into string data */
);

/**
*************************************************************************************
**
**   Retrieves the system time in microseconds (us).
**   The resolution is spec. in application.
**
**   \return U32, the time in microseconds
**   
**************************************************************************************
**/
Public extern U32 (*drapp_GET_TIME_IN_US)(void);

/**
*************************************************************************************
**
**   Retrieves the system time in microseconds (ms).
**   The resolution is spec. in application.
**
**   \return U32, the time in microseconds
**
**************************************************************************************
**/
Public extern U32 (*drapp_GET_TIME_IN_MS)(void);

/**
*************************************************************************************
**
**   Retrieves the checksum of the user configuration calculated at compile time.
**
**   \return U32, the user config checksum
**
**************************************************************************************
**/
Public extern U32 (*drapp_GET_USER_CONFIG_CHECKSUM)(void);

/**
*************************************************************************************
**
**   Sends an event to the application from Drome.
**
**************************************************************************************
**/
Public extern void (*drapp_SEND_EVENT_TO_APPLICATION)(
   U32 event                   /**<[in] The event sent to application */
);


/**
*************************************************************************************
**
**   Retrieves the id of the NULL event
**
**   \return U32, the null event
**
**************************************************************************************
**/
Public extern U32 (*drapp_GET_NULL_EVENT)(void);

/**
*************************************************************************************
**
**   Retrieves the id of the drome_LAST_IN_EVENT, i.e. the number of IN events. If
**   we have N in events and M out events, the IN events are numbered from 1 to N
**   and the out events are numbered from N+2 to N+2+M, this function returns the
**   event id N+1.
**
**   \return U32, the event id
**
**************************************************************************************
**/
Public extern U32 (*drapp_GET_LAST_IN_EVENT)(void);

/**
*************************************************************************************
**
**   Retrieves the id of the drome_MAX_EVENTS, i.e. the number of events present in
**   the system.
**
**   \return U32, the number of events
**
**************************************************************************************
**/
Public extern U32 (*drapp_GET_MAX_EVENTS)(void);

/**
*************************************************************************************
**
**   Retrieves the display timing parameters from the application.
**
**   \return drome_GCDispParams*, a pointer to the display parameters
**
**************************************************************************************
**/
#if drome_GLIB_VARIANT == drome_GLIB_DHD
#ifndef drapp_DISPLAY_TYPE
Public extern const r_ddb_Timing_t* (*drapp_GET_DISP_PARAMS)(void);
#endif
#endif

/*
**=====================================================================================
** Mono h/w driver interface
**=====================================================================================
*/

#if (drome_GLIB_VARIANT == drome_GLIB_MONO) || (drome_GLIB_VARIANT == drome_GLIB_OPENGL)

/**
 **************************************************************************************
 **
 **   Informs the application that the graphics rendering has completed and it's
 **   OK to output data to the display.
 **
 **************************************************************************************
 **/
Public extern void (*drapp_DISP_UPDATE)(
   const U8* image_ptr                 /**< Pointer to updated display image */
);

#endif

/*
**=====================================================================================
** I2C bus access interface
**=====================================================================================
*/

/**
 **************************************************************************************
 **
 **   Open I2C interface. \n
 **   The I2C interface is allocated for use by Drome.
 **
 **    \return     TRUE = Interface available, FALSE = Interface busy, try later.
 **
 **************************************************************************************
 **/
Public extern Boolean (*drapp_I2C_OPENIF)( void );

/**
 **************************************************************************************
 **
 **   Close I2C interface. \n
 **   Used by Drome to release the I2C interface for use by others.
 **
 **************************************************************************************
 **/
Public extern void (*drapp_I2C_CLOSEIF)( void );

/**
 *************************************************************************************
 **
 **   Write a stream of bytes to the I2C interface. \n
 **   The interface must first have been allocated using drapp_I2C_OPENIF().
 **
 **    \return TRUE if successful, FALSE otherwise.
 **
 **************************************************************************************
 **/
Public extern Boolean (*drapp_I2C_WRITESTART)(
   U8 adr,                             /*[in] I2C-adress */
   const U8* txbuf,                    /*[in] Data to write */
   U16 txnum                           /*[in] Number of bytes to write*/
);


/**
 *************************************************************************************
 **
 **   Read a stream of bytes from the I2C interface. \n
 **   The interface must first have been allocated using drapp_I2C_OPENIF().
 **
 **    \return TRUE if successful, FALSE otherwise.
 **
 **************************************************************************************
 **/
Public extern Boolean (*drapp_I2C_READSTART)(
   U8 adr,                           /*[in] I2C-adress */
   const U8* txbuf,                  /*[in] Data to write */
   U16 txnum,                        /*[in] Number of bytes to write*/
   U8* rxbuf,                        /*[out] Buffer for read data */
   U16 rxnum                         /*[in] Number of bytes to read*/
);

/**
 *************************************************************************************
 **
 **   Output an I2C stop condition. \n
 **   The interface must first have been allocated using drapp_I2C_OPENIF().
 **
 **************************************************************************************
 **/
Public extern void (*drapp_I2C_ISSUESTOP)( void );

/**
 *************************************************************************************
 **
 **   Retrieves the scaled width and height according to the scale property.
 **   When the scale property equals zero width and height will be unscaled.
 **
 **   \return TRUE if successful, FALSE otherwise.
 **
 **************************************************************************************
 **/
Public extern Boolean (*drapp_GET_SCALED_VIDEO_SIZE)(
      U8 scale_index,                /*[in]  Video widgets scale property */
      U16* scaled_width,             /*[out] Scaled width */
      U16* scaled_height             /*[out] Scaled height */
);

/**
**************************************************************************************
**
**   SafeToUpdateGlobalProperties callback
**
**   The redraw type tells if the next rendering is a redraw of all widgets in the
**   current view or if it only refreshes the widgets that needs a higher refresh
**   rate.
**
**   waitForPropertyUpdate is a return value. TRUE means that drome returns from
**   the drome_doCyclic and starts the rendering next time the drome_doCyclic is
**   called. FALSE means that if drome still has time left it will start rendering
**   the next frame immediately.
**
**************************************************************************************
**/
Public extern void (*drapp_SAFE_TO_UPDATE_GLOBAL_PROPERTIES)(
      drome_RedrawType redraw_type,     /**< [in]  Is the next update a redraw or a refresh. */
      Boolean* wait_for_property_update /**< [out] Return TRUE if the rendering should be postponed to the next drome_doCyclic call. */
);

#endif /* DRAPPIF_H */
/** \} // End IMPORTS */
/********************************** End of file **************************************/

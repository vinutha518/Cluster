/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                   DRDBG.H                                       ***
***                                                                                 ***
***            Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2018#$        ***
***                                                                                 ***
***************************************************************************************
***************************************************************************************

    $Id: drdbg.h 10 2018-12-03 11:36:39Z stoneridge $

    Subject: Debugging utility functions.

    Document reference: n/a

    Description of this header file:
       This header file contains all public definitions and prototypes that
       make up the interface of the DRDBG module.
       See also the source file (DRDBG.C) for further details.

***************************************************************************************
*/

#ifndef DRDBG_H
#define DRDBG_H

/*
**=====================================================================================
** Imported definitions
**=====================================================================================
*/

#include "drfileid.h"
#include "drhost.h"
#include "drappif.h"
#include "dr.h"
#include "drome.h"

#ifdef HOST_WIN32
   #include <assert.h> 
#endif

#ifdef __GNUC__
#define __huge
#endif

/*
**=====================================================================================
** Public defines
**=====================================================================================
*/

/* Error severity levels */
/* DBG_LEVEL must be #define'd by each module individually to one of these values */
#define drdbg_LEVEL_FATAL       0     /* Condition about to cause a crash */
#define drdbg_LEVEL_SEVERE      1     /* Severe functional error */
#define drdbg_LEVEL_ERROR       2     /* Other program errors */
#define drdbg_LEVEL_PROGRESS    3     /* Minimal info, e.g. program progress */
#define drdbg_LEVEL_EXTENDED    4     /* Extended status information, data dump, etc */


/*** Decode error codes ***/
#define drdbg_EC_LEVEL( ec )        ((ec)>>28)
#define drdbg_EC_FILE( ec )         (((ec)>>16) & 0x0FFF)
#define drdbg_EC_LINE( ec )         ((ec) & 0xFFFF)

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

#ifndef DBG

   /*********  R E L E A S E   B U I L D   V E R S I O N  *********/

   /* Info print-out -> silenced! */

   #define drdbg_PRINT( level, string_const )              ((void)0)
   #define drdbg_PRINT1( level, string_const, value )      ((void)0)
   #define drdbg_HEX_DUMP( level, mem_ptr, byte_count )    ((void)0)
   #define drdbg_DECODE_CODE_ERROR( level, err_code, err_data )  ((void)0)
   #define drdbg_FLUSH( level )                            ((void)0)

   /* Error handling -> only active if level is "severe" or worse */
   
   #define drdbg_ASSERT( level, test ) \
      (((drdbg_LEVEL_SEVERE < (level)) || (test)) ? ((void)0) : \
       drapp_DETECTED_CODE_ERROR(dbg_ERRCODE(level), 0))

   #define drdbg_CODE_ERROR( level ) \
      ((drdbg_LEVEL_SEVERE < (level)) ? ((void)0) : \
       drapp_DETECTED_CODE_ERROR(dbg_ERRCODE(level), (U32)0))

   #define drdbg_CODE_ERROR1( level, U32_p1 ) \
      ((drdbg_LEVEL_SEVERE < (level)) ? ((void)0) : \
       drapp_DETECTED_CODE_ERROR(dbg_ERRCODE(level), U32_p1))

#else

   /*********  D E B U G   B U I L D   V E R S I O N  *********/

   /* Info print-out */

   #if defined ( DBGFMT_UART)
   		 #define drdbg_PRINT( level, string_const ) \
         ((DBG_LEVEL < (level)) ? ((void)0) : \
          dr_wrapDetectedDromeError(drome_ERR_WIDGET_DRAWING_ERROR)) 
         #define drdbg_PRINT1( level, string_const, value ) \
         ((DBG_LEVEL < (level)) ? ((void)0) : \
          dr_wrapDetectedDromeError(drome_ERR_WIDGET_DRAWING_ERROR))
   #define drdbg_HEX_DUMP( level, mem_ptr, byte_count )    ((void)0)
   #define drdbg_DECODE_CODE_ERROR( level, err_code, err_data )  ((void)0)
   #define drdbg_FLUSH( level )                            ((void)0)

   #elif defined( DBGFMT_SMART )

      #define drdbg_PRINT( level, string_const ) \
         ((DBG_LEVEL < (level)) ? ((void)0) : \
          dbg_smartPrint( dbg_ERRCODE(level) ))

      #define drdbg_PRINT1( level, string_const, value ) \
         ((DBG_LEVEL < (level)) ? ((void)0) : \
          dbg_smartPrint1( dbg_ERRCODE(level), (U8)sizeof(value), (value) ))

      #define drdbg_HEX_DUMP( level, mem_ptr, byte_count ) \
         ((void)0) /* Not available in smart format */

      #define drdbg_DECODE_CODE_ERROR( level, err_code, err_data )
         ((void)0) /* Not available in smart format */

      #define drdbg_FLUSH( level )
         ((void)0) /* Not available in smart format */

   #elif defined( DBGFMT_TEXT )

      #define drdbg_PRINT( level, string_const ) \
         ((DBG_LEVEL < (level)) ? ((void)0) : \
          dbg_print( (const U8 *)(string_const) ))

      #define drdbg_PRINT1( level, string_const, value ) \
         ((DBG_LEVEL < (level)) ? ((void)0) : \
          dbg_print1( (const U8 *)(string_const), (U8)sizeof(value), (value) ))

      #define drdbg_HEX_DUMP( level, mem_ptr, byte_count ) \
         ((DBG_LEVEL < (level)) ? ((void)0) : \
          dbg_hexDump( (mem_ptr), (byte_count) ))

      #define drdbg_DECODE_CODE_ERROR( level, err_code, err_data ) \
         ((DBG_LEVEL < (level)) ? ((void)0) : \
          dbg_decodeCodeError( err_code, err_data ))

      #define drdbg_FLUSH( level ) \
         ((DBG_LEVEL < (level)) ? ((void)0) : dbg_flush())

   #else
      #error DBGFMT_ definition missing!
   #endif

   /* Error handling */

   #if defined(HOST_WIN32) && !defined(AUTOTEST)

      /* C assert's doesn't work well in auto tests. */
      /* do not check level in simulated environment */
      #define drdbg_ASSERT( level, test )  \
            assert(test)

      #define drdbg_CODE_ERROR( level ) \
          assert( "Code error" && !dbg_ERRCODE(level) )

      #define drdbg_CODE_ERROR1( level, U32_p1 ) \
          assert( "Code error" && !dbg_ERRCODE(level) )

   #else 

      #define drdbg_ASSERT( level, test ) \
         (((DBG_LEVEL < (level)) || (test)) ? ((void)0) : \
          drdbg_userError(dbg_ERRCODE(level), (U32)0))

      #define drdbg_CODE_ERROR( level ) \
         ((DBG_LEVEL < (level)) ? ((void)0) : \
          drdbg_userError(dbg_ERRCODE(level), (U32)0))

      #define drdbg_CODE_ERROR1( level, U32_p1 ) \
         ((DBG_LEVEL < (level)) ? ((void)0) : \
          drdbg_userError(dbg_ERRCODE(level), U32_p1))

   #endif

#endif

/*
**=====================================================================================
** Supporting function and macro declarations, NOT to be accessed directly!
**=====================================================================================
*/

/* Calculate an error code value */
/*lint -emacro( {648}, dbg_ERRCODE ) Overflow in computing const for op.: 'shift left'" */
#define dbg_ERRCODE( level ) \
           ((U32)(((level)<<28) | ((FILE_ID)<<16) | (__LINE__)))

/* Make number-string of numeric macro */
#define _STRIZE(x)   _VAL(x)
#define _VAL(x)	     #x

/* Info print-out functions */
#ifdef DBG
   #if defined( DBGFMT_SMART )

      extern void dbg_smartPrint( U32 reference );
      extern void dbg_smartPrint1( U32 reference, U8 size, U32 value );

   #elif defined( DBGFMT_TEXT )

      extern void dbg_print( const U8 * string_ptr );
      extern void dbg_print1( const U8 * string_ptr, U8 size, U32 value );
      extern void dbg_hexDump( const U8 __huge * mem_ptr, U32 byte_count );
      extern void dbg_decodeCodeError( U32 err_code, U32 err_data );
      extern void dbg_flush( void );

   #endif

#endif

/* Error functions */
extern void drdbg_userError(U32 level_file_line, U32 data); /* (Level << 28) + (File << 16) + Line */

#endif /* #ifdef DRDBG_H */
/********************************** End of file **************************************/


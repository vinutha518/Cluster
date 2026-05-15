/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                     DRGODBIF.H                                  ***
***                                                                                 ***
***                                   Project drome                                 ***
***                                                                                 ***
***          Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2018#$          ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file
   \ingroup GODBIF

   \Subject    Drome graphical object database interface.

   \Reference  n/a

   \Abstract

   This module implements the graphical object database interface (GODBIF).
   This interface is used to access the graphical object metadata and the
   graphical object data.

   \code

         Meta data chunk                  Graphical objects chunk
          +-----------------+              +----------------------+
          |  HeaderData     |       ,----->| Image 0              |
          +-----------------+       |      |                      |
          |ImageMetadata    +-------'      +----------------------+
          | (id=1)          |              | Image 1              |
          +-----------------+              |                      |
          | ...             |              +----------------------+
          +-----------------+              | ...                  |
          |ImageMetadata    |              |                      |
          | (id=num_images) |              |                      |
          +-----------------+              +----------------------+
   +------|FontMetadataOffs |      ,------>| Font 0               |
   |      |(id=num_images+1)|      |       |                      |
   |      +-----------------+      |       +----------------------+
   |      | ...             |      |       | Font 1               |
   |      +-----------------+      |       |                      |
   |      |FontMetaDataOffs |      |       +----------------------+
   |      |(id=num_images + |      |       | ...                  |
   |      |  num_fonts)     |      |       |                      |
   |      +-----------------+      |       +----------------------+
   +----->|FontMetadata 0   +------'
          |- CharMetadata 0 |
          |- CharMetadata 1 |
          |...              |
          +-----------------+
          |FontMetadata 1   |
          |- CharMetadata 0 |
          |- CharMetadata 1 |
          |...              |
          +-----------------+
          |...              |
          +-----------------+

   \endcode

   \Version history record:

   $Id: drgodbif.h 10 2018-12-03 11:36:39Z stoneridge $

\{
***************************************************************************************
*/

#ifndef DRGODBIF_H
#define DRGODBIF_H

/*
**=====================================================================================
** Imported definitions
**=====================================================================================
*/

#include "drapp.h"
#include "dr.h"
#include "drome.h"

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

typedef struct
{
   U32 crc32;
   U32 reserved;
   U32 offsetToMetaData;
   U32 offsetToImageData;
} drgodbif_ImgDatabase;

/** Image format */
typedef enum
{
   RGB565 = 0U,
   ARGB4444 = 1U,
   ALPHA_MAP_8_BITS = 2U,
   ARGB8888 = 3U,
   ALPHA_MAP_4_BITS = 4U,
   XRGB8888 = 5U,
   MAX_NO_OF_IMAGE_FORMATS = 6U
}drgodbif_ImageFormat;

/** Image meta data */
typedef struct
{
   U16 width;              /**< Image width (pels) */
   U16 height;             /**< Image height (pels) */
   U32 offset;             /**< XXFLASH offset (bytes) */
#if (drome_GLIB_VARIANT == drome_GLIB_DHD) || (drome_GLIB_VARIANT == drome_GLIB_OPENGL)
   /*lint --e{46} // Field type should be int - int is not allowed - hole in the head */
   U32 inflated_size:24;      /**< Size of inflated data (bytes) / For mono images we can store flags */
   U32 image_format:8;    /**< image format see11111 */
#else
   U32 inflated_size;      /**< Size of inflated data (bytes) / For mono images we can store flags */
#endif
} drgodbif_ImageMetaData;

typedef U32 drgodbif_FontMetadataOffset; /**< offset from header data[0] */

/** Font meta data */
typedef struct
{
   U16 char_count;         /**< Number of characters in font */
   U16 height;             /**< Font height (pels) */
   U32 offset;             /**< XXFLASH offset (bytes) */
   U32 inflated_size;      /**< Size of inflated data (bytes) / For mono fonts we can store flags*/
} drgodbif_FontMetaData;


/** Character meta data \n *
 ** Compact look-up table entry used for finding index, width or char_code
 ** from either index or char_code. For monochrome fonts additional information
 ** about the character is included.
 */
typedef struct
{
  /*lint --e{46} // Field type should be int - int is not allowed - hole in the head */
  U32 width:8;            /**< Width of this character (pels) */
  U32 char_code:24;       /**< Codepoint of this character */
#if drome_GLIB_VARIANT == drome_GLIB_MONO
  U8 offset;              /**< Distance from top of font to top of character */
  U8 char_height;         /**< Number of lines containing set pixels */
  U16 data_offset;        /**< Offset into character data for this character. */
#elif (drome_GLIB_VARIANT == drome_GLIB_DHD) || (drome_GLIB_VARIANT == drome_GLIB_OPENGL)
  U32 data_offset;        /**< Offset into character data for this character. */
#endif/* #if drome_GLIB_VARIANT */
} drgodbif_CharMetaData;



/** Sprite Animation meta data */
typedef struct
{
   U16 width;              /**< Image width (pels) */
   U16 height;             /**< Image height (pels) */
   U16 num_images;
   U16 image_format;       /**< image format see11111 */
   U32 image_offsets[1];      /**<XFLASH offsets sprite images */
} drgodbif_SpriteAnimationMetaData;



/* The Drome compiler build needs both variants since it supports both formats.
 * For this reason the resulting definitions for both MONO and DHD are supplied
 * below.
 */
#ifdef WIN32
/** Compiler MONO version */
typedef struct
{
  U32 width:8;            /**< Width of this character (pels) */
  U32 char_code:24;       /**< Codepoint of this character */
  U8 offset;              /**< Distance from top of font to top of character */
  U8 char_height;         /**< Number of lines containing set pixels */
  U16 data_offset;        /**< Offset into char data for this character. */
} drgodbif_MonoCharMetaData;

/** Compiler DHD version */
typedef struct
{
   U32 width:8;            /**< Width of this character (pels) */
   U32 char_code:24;       /**< Codepoint of this character */
   U32 data_offset;        /**< Offset into character data for this character. */
} drgodbif_ColourCharMetaData;
#endif

#ifdef drome_CONF_WIDGET_VIDEO

/** Video scaling mode */
typedef enum
{
   drgodbif_VSCMOD_PAL_FRAME,
   drgodbif_VSCMOD_PAL_FIELD,
   drgodbif_VSCMOD_NTSC_FRAME,
   drgodbif_VSCMOD_NTSC_FIELD
} drgodbif_VideoScalingMode;


#endif /* #ifdef drome_CONF_WIDGET_VIDEO */

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

void drgodbif_init(const void * db_ptr);
void drgodbif_init_2G(const void * go_db_ptr, const void * sprite_db_ptr);

const drgodbif_ImageMetaData* drgodbif_getImageMetaData(dr_ImageId image);
const drgodbif_FontMetaData* drgodbif_getFontMetaData( dr_FontId font_id );
const drgodbif_SpriteAnimationMetaData* drgodbif_getSpriteMetaData( U16 spriteId );

dr_FontId drgodbif_getFirstFontId(void);
U16 drgodbif_getFontHeight( dr_FontId font_id );
U16 drgodbif_getCharIndex( dr_FontId font_id, U32 char_code );
U8 drgodbif_getCharWidth( dr_FontId font_id, U16 char_index );


#if drome_GLIB_VARIANT == drome_GLIB_MONO

#ifdef drome_CONF_USE_PUBLIC_OBJECTS
dr_FontId drgodbif_getFontIdOfPublicFont( U8 pub_font_id );
dr_ImageId drgodbif_getImageIdOfPublicImage( U8 pub_image_id );
#endif

const U8 * drgodbif_getMonoImageData( dr_ImageId image );
const U8 * drgodbif_getCharImageData( dr_FontId font_id, U16 char_index );

#endif /* #if drome_GLIB_VARIANT */

const drgodbif_CharMetaData* drgodbif_getCharacterMetaData( dr_FontId font_id, U16 char_index );

#endif /* #ifdef DRGODBIF_H */

/*************************************************************************************/
/** \}
*********************************** End of file **************************************/

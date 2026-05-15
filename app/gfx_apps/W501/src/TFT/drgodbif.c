/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                       DRGODBIF.C                                ***
***                                                                                 ***
***                                      Project drome                              ***
***                                                                                 ***
***          Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2018#$          ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file
   \ingroup GODBIF

   \Subject    Drome graphics metadata Interface implementation

   \Reference  n/a

   \Abstract

   This module implements the graphics objects database interface
   This interface is used to access the graphics metadata


   \Version history record:

   $Id: drgodbif.c 10 2018-12-03 11:36:39Z stoneridge $

\{
***************************************************************************************
*/
#define FILE_ID                  drfileid_DRGODBIF_C

#ifdef DRGODBIF_DBG_LEVEL
#define DBG_LEVEL                DRGODBIF_DBG_LEVEL
#else
#define DBG_LEVEL                drdbg_LEVEL_ERROR
#endif


/*
**=====================================================================================
** Imported definitions
**=====================================================================================
*/

#include "drdbg.h"
#include "drapp.h"
#include "drdb.h"
#include "drgodbif.h"
#include "drome.h"
#include "drfileid.h"
#include "drhost.h"
#include "dr_profiling.h"

/*
**=====================================================================================
** Private defines
**=====================================================================================
*/

#if defined(__ICCV850__)
#define HUGE_SEG_DECL __huge
#else
#define HUGE_SEG_DECL
#endif

/*
**=====================================================================================
** Private type definitions
**=====================================================================================
*/

#if (defined(drapp_GLYPH_CACHE_SIZE)) && (drapp_GLYPH_CACHE_SIZE > 0)

/** Glyph cache element. */
typedef struct
{
   drgodbif_CharMetaData glyph_meta_data;
   U32 reference_counter;
   U16 char_index;
   dr_FontId font_id;
} GlyphCacheItem;

#define GLYPH_CACHE_SIZE               (drapp_GLYPH_CACHE_SIZE / sizeof(GlyphCacheItem))
#define GLYPH_CACHE_AGING_MAX          0xFFFFFFFFUL
#define GLYPH_CACHE_AGING_ACTIVE_BIT   0x80000000UL
#endif

/*
**=====================================================================================
** Public data declarations
**=====================================================================================
*/

/*
**=====================================================================================
** Private data declarations
**=====================================================================================
*/

#if (defined(drapp_GLYPH_CACHE_SIZE)) && (drapp_GLYPH_CACHE_SIZE > 0)
Private GlyphCacheItem priv_glyph_cache[GLYPH_CACHE_SIZE];  /**< Glyph cache */
Private U16 priv_num_items_in_glyph_cache;                  /**< Num items stored in the glyph cache */
#endif

/*
**=====================================================================================
** Private function prototypes and macros
**=====================================================================================
*/

Private const drdb_CompactImageDatabase* priv_header_data_ptr = NULL;
Private const drgodbif_ImageMetaData*    priv_image_meta_data_ptr = NULL;
Private const drdb_SpriteAnimationDatabase* priv_sprite_header_data_ptr = NULL;
Private const U16* priv_sprite_meta_data_ptr = NULL;

#if (defined(drapp_GLYPH_CACHE_SIZE)) && (drapp_GLYPH_CACHE_SIZE > 0)
Private Boolean glyphCacheGetGlyphMetaData(dr_FontId font_id, U16 char_index, const drgodbif_CharMetaData** glyph_meta_data_ref);
Private Boolean glyphCacheGetIndex(dr_FontId font_id, U16 char_code, U16* char_index_ref);
Private void glyphCacheStoreGlyph(dr_FontId font_id, U16 index, const drgodbif_CharMetaData* glyph_meta_data_ref);
#endif

/*
**=====================================================================================
** Public function definitions
**=====================================================================================
*/

/**
*************************************************************************************
**
**   Initializes drgodbif.
**   Must be called before use of any other functions
**
**************************************************************************************
**/
void drgodbif_init(const void * db_ptr)
{
  const drdb_CompactImageDatabase * cid_ptr = db_ptr;

  priv_header_data_ptr = NULL;
  priv_image_meta_data_ptr = NULL;

  if ((cid_ptr == NULL) || (cid_ptr->magic_number != drdb_GO_MAGIC_U32))
  {
    dr_wrapDetectedDromeError(drome_ERR_GRAPICAL_OBJECT_DB);
  }
  else if (cid_ptr->char_metadata_size != sizeof(drgodbif_CharMetaData)) 
  {
    /* DB / Graphics library missmatch */
    dr_wrapDetectedDromeError(drome_ERR_INVALID_CONFIGURATION_USED);
  }
  else
  {
    priv_header_data_ptr = cid_ptr;

    /*lint -e{826} area is not to small */
    priv_image_meta_data_ptr = (drgodbif_ImageMetaData*)cid_ptr->data;
  }
}

/**
*************************************************************************************
**
**   Initializes drgodbif if sprites are used.
**   Must be called before use of any other functions
**
**************************************************************************************
**/
void drgodbif_init_2G(const void * go_db_ptr, const void * sprite_db_ptr)
{
  const drdb_SpriteAnimationDatabase * cid_ptr = sprite_db_ptr;

  drgodbif_init(go_db_ptr);
  priv_sprite_header_data_ptr = NULL;
  priv_sprite_meta_data_ptr = NULL; 

#if (defined(drapp_GLYPH_CACHE_SIZE)) && (drapp_GLYPH_CACHE_SIZE > 0)
  priv_num_items_in_glyph_cache = 0;
#endif

  if ((cid_ptr == NULL) ||(cid_ptr->magic_number != drdb_SA_MAGIC_U32))
  {
    dr_wrapDetectedDromeError(drome_ERR_GRAPICAL_OBJECT_DB); //TODO: should perhaps use a different error code
  }
  else
  {
    priv_sprite_header_data_ptr = cid_ptr;

    /*lint -e{826} area is not to small */
    priv_sprite_meta_data_ptr = (U16*)cid_ptr->data;
  }
 
}


/**
 *************************************************************************************
 **
 **   Gets the image metadata for the specific id. 
 **
 **    \return NULL      If image metadata was not found
 **    \return If the image metadata is found a pointer to the metadata is returned
 **
 **************************************************************************************
 **/
const drgodbif_ImageMetaData* drgodbif_getImageMetaData(
   dr_ImageId image       /**<[in] -  image Id of the image that we want */
)
{
  const drgodbif_ImageMetaData* ret = NULL;

  if (priv_image_meta_data_ptr != NULL)
  {
    U32 image_index = image - 1;  // -1 for NULL object

    if (image_index < priv_header_data_ptr->num_images)
    {
      ret = &(priv_image_meta_data_ptr[image_index]);
    }
  }

  return(ret);
}

/**
 *************************************************************************************
 **
 **   Gets the sprite animation metadata for the specific id. 
 **
 **    \return NULL      If sprite animation metadata was not found
 **    \return If the image metadata is found a pointer to the metadata is returned
 **
 **************************************************************************************
 **/
const drgodbif_SpriteAnimationMetaData* drgodbif_getSpriteMetaData(
   U16 spriteId       /**<[in] -  Id of the sprite that we want */
)
{
  const drgodbif_SpriteAnimationMetaData* ret = NULL;

  if (priv_sprite_header_data_ptr != NULL)
  {
    U32 sprite_index = spriteId - 1;  // -1 for NULL object

    if (sprite_index < priv_sprite_header_data_ptr->num_sprites)
    {
      const U16 offset = priv_sprite_meta_data_ptr[sprite_index];
      /* Add the offset and the size of the offset table to the meta data ptr to get
       * the meta data for the actual sprite animation. */
      /*lint -e{740} // Unusual (but correct!) pointer cast */
      /*lint -e{826} area is not to small */
      ret = (drgodbif_SpriteAnimationMetaData*)&(priv_sprite_meta_data_ptr[(offset >> 1) + priv_sprite_header_data_ptr->num_sprites]);
    }
  }

  return(ret);
}



/**
*************************************************************************************
**
**   Gets the font metadata for the specific id. 
**
**    \return NULL      If image metadata was not found
**    \return If the font metadata is found a pointer to the metadata is returned
**
**************************************************************************************
**/
const drgodbif_FontMetaData* drgodbif_getFontMetaData( 
  dr_FontId font_id     /**<[in] -  font Id of the font that we want */
)
{
  const drgodbif_FontMetaData* ret_ptr = NULL;
  dr_profiling_BEGIN(DRGODBIF_GET_FONT_META_DATA);
  if (priv_header_data_ptr!=NULL)
  {
    U32 font_index =
      (font_id - (priv_header_data_ptr->num_images)) - 1;  // -1 for NULL object

    if (font_index < priv_header_data_ptr->num_fonts)
    {
      drgodbif_FontMetadataOffset font_offset;
      U32 font_offset_ix =
        ((priv_header_data_ptr->num_images * sizeof(drgodbif_ImageMetaData))) +
        (font_index * sizeof(drgodbif_FontMetadataOffset));

      /*lint -e{826} area is not too small */
      font_offset = 
         *(drgodbif_FontMetadataOffset*)&priv_header_data_ptr->data[font_offset_ix];

      /*lint -e{826} area is not to small */
      ret_ptr = (drgodbif_FontMetaData*)&priv_header_data_ptr->data[font_offset];
    }
  }
  dr_profiling_END(DRGODBIF_GET_FONT_META_DATA);
  return(ret_ptr);
}

/**
*************************************************************************************
**
**   Gets number of first font in metadata
**
**    \return The Id of the first font 
**
**    \return 0 If there is no fonts available
**
**************************************************************************************
**/
dr_FontId drgodbif_getFirstFontId(void)
{
  dr_FontId ret=(dr_FontId) 0;
  if (priv_header_data_ptr!=NULL) {
    if (priv_header_data_ptr->num_fonts>0) 
    {
      ret=(dr_FontId)priv_header_data_ptr->num_images + 1;
    }
  }
  return(ret);
}


/**
*************************************************************************************
**
**   Get character index for a given font and character code
**
**    \return Character index for the given char_code
**    \return 0 if the char code does not exist in the font.
**
**************************************************************************************
**/
U16 drgodbif_getCharIndex( dr_FontId font_id,  /**<[in] Font identifier to lookup */
                           U32 char_code       /**<[in] What character code (Unicode)*/
                          ) 
{

  U16 ret=0;
  const drgodbif_FontMetaData *font_metadata_ptr = NULL;
  const drgodbif_CharMetaData *char_metadata_ptr = NULL;
  dr_profiling_BEGIN(DRGODBIF_GET_CHAR_INDEX);

#if (defined(drapp_GLYPH_CACHE_SIZE)) && (drapp_GLYPH_CACHE_SIZE > 0)
  if (!glyphCacheGetIndex(font_id, (U16)char_code, &ret))
#endif
  {
     font_metadata_ptr = drgodbif_getFontMetaData(font_id);
     if (font_metadata_ptr!=NULL)
     {
       /*lint -e{923} cast from unsigned long to pointer */
       char_metadata_ptr= (drgodbif_CharMetaData *)((U32) font_metadata_ptr +  sizeof(drgodbif_FontMetaData));
     }
     if (priv_header_data_ptr!=NULL)
     {
       if ((char_metadata_ptr!=NULL) && (font_metadata_ptr!=NULL))
       {
         S32 low=0;
         S32 high=font_metadata_ptr->char_count-1;
         S32 mid=0;
         while (low <= high)
         {
           mid = low + ((high - low)/2);
           if (char_code > (U32)char_metadata_ptr[mid].char_code)
           {
             low = mid+1;
           }
           else if (char_code < (U32)char_metadata_ptr[mid].char_code)
           {
             high = mid-1;
           }
           else
           {
             if (char_code == (U32)char_metadata_ptr[mid].char_code)
             {
               ret=(U16)mid;
             }
             low=high+1;
           }
         }

#if (defined(drapp_GLYPH_CACHE_SIZE)) && (drapp_GLYPH_CACHE_SIZE > 0)
         glyphCacheStoreGlyph(font_id, ret, &char_metadata_ptr[ret]);
#endif
       }
     }
  }

  dr_profiling_END(DRGODBIF_GET_CHAR_INDEX);
  return (ret);
}

#if drome_GLIB_VARIANT == drome_GLIB_MONO

/**
*************************************************************************************
**
**   Returns the internal Font ID corresponding to the specified public font ID.
**
**    \return  Font ID or zero if no mapping found.
**
**************************************************************************************
**/
#ifdef drome_CONF_USE_PUBLIC_OBJECTS
dr_FontId drgodbif_getFontIdOfPublicFont( U8 pub_font_id )
{
   dr_FontId ret_val = 0;

   if (priv_header_data_ptr!=NULL)
   {
      if ((pub_font_id > 0) && (pub_font_id <= priv_header_data_ptr->num_fonts))
      {
         ret_val = (U16)(priv_header_data_ptr->num_images + pub_font_id);
      }
   }

   return ret_val;
}
#endif

/**
*************************************************************************************
**
**   Returns the internal Image ID corresponding to the specified public image ID.
**
**    \return  Image ID or zero if no mapping found.
**
**************************************************************************************
**/
#ifdef drome_CONF_USE_PUBLIC_OBJECTS
dr_ImageId drgodbif_getImageIdOfPublicImage( U8 pub_image_id )
{
   dr_ImageId ret_val = 0;

   if (priv_header_data_ptr!=NULL)
   {
      if ((pub_image_id > 0) && (pub_image_id <= priv_header_data_ptr->num_images))
      {
         ret_val = pub_image_id;
      }
   }

   return ret_val;
}
#endif

/**
*************************************************************************************
**
**   Gets pointers to image data for the monochrome display
**
**    \return pointer to image data
**    \return 0 if no such image is found.
**
**************************************************************************************
**/
const U8 * drgodbif_getMonoImageData( dr_ImageId image )
{
  const U8 *ret=NULL;
  U32 size;

  if (priv_header_data_ptr!=NULL) 
  {
    if (priv_header_data_ptr->char_metadata_size==sizeof(drgodbif_CharMetaData)) 
    {
      if (priv_image_meta_data_ptr!=NULL) 
      {
        if (image>0) 
        {
          ret=drdb_getImageData(&size);
          if (priv_image_meta_data_ptr!=0) 
          {
            ret+=priv_image_meta_data_ptr[image-1].offset;
          }
        }
      }
    }
  }

  return(ret);
}

/**
*************************************************************************************
**
**   Gets pointers to the image for the given character for the monochrome display
**
**    \return pointer to image data
**    \return 0 if no such image is found.
**
**************************************************************************************
**/
const U8 * drgodbif_getCharImageData( dr_FontId font_id, U16 char_index) 
{
  const drgodbif_FontMetaData *font_metadata = NULL;
  const drgodbif_CharMetaData *char_metadata;
  U32 size;
  const U8 *data_ptr=NULL;

  NOT_USED( char_index );
  font_metadata=drgodbif_getFontMetaData(font_id);

  if (font_metadata!=NULL) {
    data_ptr=drdb_getImageData(&size);
    if (data_ptr!=NULL)
    {
      data_ptr+=font_metadata->offset;
      char_metadata=drgodbif_getCharacterMetaData(font_id,char_index);
      data_ptr+=char_metadata->data_offset;
    }
  }

  return(data_ptr);
}



#endif /* #if drome_GLIB_VARIANT */

/**
*************************************************************************************
**
**   Returns the character metadata of the character with specified index
**
**    \return Width of character
**    \return 0 if the metadata with the specified index was not found
**
**************************************************************************************
**/
const drgodbif_CharMetaData* drgodbif_getCharacterMetaData( dr_FontId font_id, U16 char_index )
{
  const drgodbif_FontMetaData *font_metadata_ptr=NULL;
  const drgodbif_CharMetaData *char_metadata_ptr=NULL;
  dr_profiling_BEGIN(DRGODBIF_GET_CHAR_META_DATA);

#if (defined(drapp_GLYPH_CACHE_SIZE)) && (drapp_GLYPH_CACHE_SIZE > 0)
  if (!glyphCacheGetGlyphMetaData(font_id, char_index, &char_metadata_ptr))
#endif
  {
     font_metadata_ptr = drgodbif_getFontMetaData(font_id);
     if (font_metadata_ptr!=NULL)
     {
       /*lint -e{923} cast from pointer to unsigned long */
       char_metadata_ptr=
          (drgodbif_CharMetaData *)((U32)font_metadata_ptr +
                                    (U32)sizeof(drgodbif_FontMetaData) +
                                    (char_index * sizeof(drgodbif_CharMetaData)));
       drdbg_ASSERT(drdbg_LEVEL_ERROR,(font_metadata_ptr->char_count>char_index));
       if (font_metadata_ptr->char_count<char_index)
       {
         char_metadata_ptr=NULL;
       }
     }
  }

  dr_profiling_END(DRGODBIF_GET_CHAR_META_DATA);
  return(char_metadata_ptr);
}

/**
*************************************************************************************
**
**   Returns the width of the character with specified index
**
**    \return Width of character
**    \return 0 if the character was not found
**
**************************************************************************************
**/
U8 drgodbif_getCharWidth( dr_FontId font_id, U16 char_index )
{
  U8 ret = 0;
  const drgodbif_FontMetaData *font_metadata_ptr = NULL;
  const drgodbif_CharMetaData *char_metadata_ptr = NULL;

  font_metadata_ptr=drgodbif_getFontMetaData(font_id);
  if (font_metadata_ptr!=NULL) 
  {
    /*lint -e{923} cast from pointer to unsigned long & from unsigned long to pointer */
    char_metadata_ptr= (drgodbif_CharMetaData *)((U32) font_metadata_ptr + (U32) sizeof(drgodbif_FontMetaData));
    drdbg_ASSERT(drdbg_LEVEL_ERROR,(font_metadata_ptr->char_count>char_index));
  }
  if (char_metadata_ptr!=NULL) 
  {
    ret=(U8)char_metadata_ptr[char_index].width;
  }
  return(ret);
}

/**
*************************************************************************************
**
**   Get the font height
**
**    \return Height of font
**    \return 0 if the font was not found
**
**************************************************************************************
**/
U16 drgodbif_getFontHeight( dr_FontId font_id ) 
{
  U16 ret=0;
  const drgodbif_FontMetaData* font_meta_data_ptr;

  font_meta_data_ptr = drgodbif_getFontMetaData(font_id);
  if (font_meta_data_ptr!=NULL) {
    ret=font_meta_data_ptr->height;
  }

 return(ret);
}

/**
*************************************************************************************
**
**   Get glyph meta data from glyph meta data cache.
**
**    \return TRUE if the meta data exists in cache,
**            FALSE otherwise
**
**************************************************************************************
**/
#if (defined(drapp_GLYPH_CACHE_SIZE)) && (drapp_GLYPH_CACHE_SIZE > 0)
Private Boolean glyphCacheGetGlyphMetaData(
      dr_FontId font_id,                                 /**< The id of the font that the glyph belongs to */
      U16 char_index,                                    /**< The index of the glyph meta data in flash */
      const drgodbif_CharMetaData** glyph_meta_data_ref  /**< [OUT] The glyph meta data */
)
{
   Boolean ret_val = FALSE;
   U32 i;

   for (i = 0; (i < priv_num_items_in_glyph_cache) && (!ret_val); i++)
   {
      if ((priv_glyph_cache[i].font_id == font_id) && (priv_glyph_cache[i].char_index == char_index))
      {
         *glyph_meta_data_ref = &priv_glyph_cache[i].glyph_meta_data;
         ret_val = TRUE;
      }
   }

   return ret_val;
}
#endif

/**
*************************************************************************************
**
**   Get glyph index from glyph meta data cache.
**
**    \return TRUE if the meta data exists in cache,
**            FALSE otherwise
**
**************************************************************************************
**/
#if (defined(drapp_GLYPH_CACHE_SIZE)) && (drapp_GLYPH_CACHE_SIZE > 0)
Private Boolean glyphCacheGetIndex(
      dr_FontId font_id,                                 /**< The id of the font that the glyph belongs to */
      U16 char_code,                                     /**< The code point of the glyph */
      U16* char_index_ref                                /**< [OUT] The index of the glyph meta data in flash */
)
{
   Boolean ret_val = FALSE;
   U32 i;

   for (i = 0; i < priv_num_items_in_glyph_cache; i++)
   {
      /* The glyph cache uses aging as replacement algorithm. When an element is
       * referenced all elements reference counters are shifted right. The MSB of
       * the referenced elements reference_counter is set to one.
       * The element with the smallest reference_count value will be replaced
       * when a new element is added and the cache is full. */
      priv_glyph_cache[i].reference_counter >>= 1;

      if ((priv_glyph_cache[i].font_id == font_id) && (priv_glyph_cache[i].glyph_meta_data.char_code == char_code))
      {
         priv_glyph_cache[i].reference_counter |= GLYPH_CACHE_AGING_ACTIVE_BIT;
         *char_index_ref = priv_glyph_cache[i].char_index;
         ret_val = TRUE;
      }
   }

   return ret_val;
}
#endif

/**
*************************************************************************************
**
**   Store a new glyph meta data element in the glyph meta data cache.
**
**************************************************************************************
**/
#if (defined(drapp_GLYPH_CACHE_SIZE)) && (drapp_GLYPH_CACHE_SIZE > 0)
Private void glyphCacheStoreGlyph(
      dr_FontId font_id,                                 /**< The id of the font that the glyph belongs to */
      U16 char_index,                                    /**< The index of the glyph meta data in flash */
      const drgodbif_CharMetaData* glyph_meta_data_ref   /**< The glyph meta data to store in the cache */
)
{
   U32 i;
   U32 least_referenced = GLYPH_CACHE_AGING_MAX;
   U16 least_referenced_index = 0;

   if (priv_num_items_in_glyph_cache < GLYPH_CACHE_SIZE)
   {
      least_referenced_index = priv_num_items_in_glyph_cache;
      priv_num_items_in_glyph_cache++;
   }
   else
   {
      for (i = 0; i < priv_num_items_in_glyph_cache; i++)
      {
         if (priv_glyph_cache[i].reference_counter < least_referenced)
         {
            least_referenced = priv_glyph_cache[i].reference_counter;
            least_referenced_index = (U16)i;
         }
      }
   }

   priv_glyph_cache[least_referenced_index].reference_counter = GLYPH_CACHE_AGING_ACTIVE_BIT;
   priv_glyph_cache[least_referenced_index].char_index = char_index;
   priv_glyph_cache[least_referenced_index].font_id = font_id;
   priv_glyph_cache[least_referenced_index].glyph_meta_data = *glyph_meta_data_ref;
}
#endif

/** \}
*********************************** End of file **************************************/

/*
****************************************************************************
PROJECT : GFX_APPNOTE
FILE    : $Id: font.h 11006 2016-11-07 16:32:44Z michael.golczewski $
============================================================================ 
DESCRIPTION
Sample program for demonstration of the Dx4 GFX features
============================================================================
                            C O P Y R I G H T                                    
============================================================================
                           Copyright (c) 2011
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing

Warranty Disclaimer

Because the Product(s) is licensed free of charge, there is no warranty 
of any kind whatsoever and expressly disclaimed and excluded by Renesas, 
either expressed or implied, including but not limited to those for 
non-infringement of intellectual property, merchantability and/or 
fitness for the particular purpose. 
Renesas shall not have any obligation to maintain, service or provide bug 
fixes for the supplied Product(s) and/or the Application.

Each User is solely responsible for determining the appropriateness of 
using the Product(s) and assumes all risks associated with its exercise 
of rights under this Agreement, including, but not limited to the risks 
and costs of program errors, compliance with applicable laws, damage to 
or loss of data, programs or equipment, and unavailability or 
interruption of operations.

Limitation of Liability

In no event shall Renesas be liable to the User for any incidental, 
consequential, indirect, or punitive damage (including but not limited 
to lost profits) regardless of whether such liability is based on breach 
of contract, tort, strict liability, breach of warranties, failure of 
essential purpose or otherwise and even if advised of the possibility of 
such damages. Renesas shall not be liable for any services or products 
provided by third party vendors, developers or consultants identified or
referred to the User by Renesas in connection with the Product(s) and/or the 
Application.

****************************************************************************
*/

#ifndef _FONT_H_
#define _FONT_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: Font Interface
  
  This header describes the structure of a bitmap font,
  which can be used by the framework.
*/


/***********************************************************
  Section: Global Types
*/

/***********************************************************
  Type: FontPos_t

  Character position and width information.

  The type is used to specify the position of a character
  inside a font texture.

  Members:
  Offset  - Pixel position in the texture
  Width   - Pixel width in the texture
*/

typedef struct {
    int Offset;
    int Width;
} FontPos_t;


void FlashDash_WriteString_Speed(int PosX, int PosY, char *Text, unsigned char change_color, unsigned char label);



#ifdef __cplusplus
}
#endif

#endif /* _FONT_H_ */


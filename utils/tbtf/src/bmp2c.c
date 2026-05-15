
#include <stdio.h>
#include "bmp2c.h"
#include "main.h"


#define _MAX_WIDTH   4096
#define _MAX_HEIGHT  2048
#define _MAX_DEPTH   4

static unsigned char _FrameBuffer[_MAX_WIDTH * _MAX_HEIGHT * _MAX_DEPTH];

static int _MaxWidth;
static int _MaxHeight;
static int _MaxDepth;



static unsigned int _Read(void* Buffer, unsigned int Size,  unsigned int Count) {
    return fread(Buffer, Size, Count, Source);
}


static void _AdjustBufferSize(int X, int Y) {
    if (X > _MaxWidth) {
        _MaxWidth = X;
    }
    if (Y > _MaxHeight) {
        _MaxHeight = Y;
    }
}

static int _GetAlpha(int R, int G, int B) {
    int alpha;

    if (AlphaMaskValue >= 0) {
        alpha  = B & 0xff;
        alpha |= ((G & 0xff) << 8);
        alpha |= ((R & 0xff) << 16);
        if (alpha == AlphaMaskValue) {
            alpha = 0;
        } else {
            alpha = AlphaConst;
        }

    } else {
        alpha = AlphaConst;
    }
    return alpha;
}

static void _ClearBuffer(void) {
    int col;
    int row;
    int i;
    int fbindex;
   
    for (row = 0; row < _MAX_HEIGHT; row++) {
        for (col = 0; col < _MAX_WIDTH; col++) {
            fbindex = (row * _MAX_WIDTH + col) * _MAX_DEPTH;
            for (i = 0; i < _MAX_DEPTH; i++) {
                _FrameBuffer[fbindex + i] = 0x66;
            }
        }
    }
}


static void _DumpBuffer(void) {
    int col;
    int row;
    int i;
    int fbindex;
    int cc;

    cc = 0;
    for (row = 0; row <= _MaxHeight; row++) {
        for (col = 0; col <= _MaxWidth; col++) {
            fbindex = (row * _MAX_WIDTH + col) * _MAX_DEPTH;
            for (i = 0; i < _MaxDepth; i++) {
                printf("0x%2.2X, ", _FrameBuffer[fbindex + i]);
                cc++;
                if (cc >= 12) {
                    cc = 0;
                    printf("\n    ");
                }
            }
        }
    }
}


static void LoadBmp(void) {
    printf("const char %s[] = {\n    ", ArrayName);
    eeBmp_LoadBmp();
    if (NecFormat) {
        printf("0x4e, 0x45, 0x43, 0x%2.2X, 0x%2.2X, 0x%2.2X, 0x%2.2X, 0x%2.2X,\n    ",
               (_MaxWidth + 1) & 0xff, ((_MaxWidth + 1) >> 8) & 0xff,
               (_MaxHeight +1) & 0xff, ((_MaxHeight + 1) >> 8) & 0xff,
               _MaxDepth & 0xff);
    }
    _DumpBuffer();
    printf("};\n");

}

static  void _SetPixelRGBA8888(int X, int Y , int R, int G, int B) {
    int fbindex;
    int alpha;

    _AdjustBufferSize(X, Y);
    alpha = _GetAlpha(R, G, B);
    fbindex = (Y * _MAX_WIDTH + X) * _MAX_DEPTH;
    _FrameBuffer[fbindex]     = B;
    _FrameBuffer[fbindex + 1] = G;
    _FrameBuffer[fbindex + 2] = R;
    _FrameBuffer[fbindex + 3] = alpha & 0xff;
}

static void _Bmp2RGBA8888(void) {
    _ClearBuffer();
    eeBmp_Init(_Read, _SetPixelRGBA8888);
    _MaxDepth = 4;
    LoadBmp();
}




static  void _SetPixelRGB565(int X, int Y , int R, int G, int B) {
    unsigned short cw;
    int fbindex;

    _AdjustBufferSize(X, Y);
    cw  = (B >> 3) & 0x1f;
    cw |= (((G >> 2) & 0x3f) << 5);
    cw |= (((R >> 3) & 0x1f) << 11);

    fbindex = (Y * _MAX_WIDTH + X) * _MAX_DEPTH;
    _FrameBuffer[fbindex]     = cw & 0xff;
    _FrameBuffer[fbindex + 1] = (cw >> 8) & 0xff;

}

static void _Bmp2RGB565(void) {
    _ClearBuffer();
    eeBmp_Init(_Read, _SetPixelRGB565);
    _MaxDepth = 2;
    LoadBmp();
}


static  void _SetPixelRGBA4444(int X, int Y , int R, int G, int B) {
    unsigned short cw;
    int fbindex;
    int alpha;

    _AdjustBufferSize(X, Y);
    alpha = _GetAlpha(R, G, B);
    cw  = (B >> 4) & 0x0f;
    cw |= (((G >> 4) & 0x0f) << 4);
    cw |= (((R >> 4) & 0x0f) << 8);
    cw |= (((alpha >> 4) & 0x0f) << 12);

    fbindex = (Y * _MAX_WIDTH + X) * _MAX_DEPTH;
    _FrameBuffer[fbindex]     = cw & 0xff;
    _FrameBuffer[fbindex + 1] = (cw >> 8) & 0xff;
}

static void _Bmp2RGBA4444(void) {
    _ClearBuffer();
    eeBmp_Init(_Read, _SetPixelRGBA4444);
    _MaxDepth = 2;
    LoadBmp();
}


static  void _SetPixelAlpha8(int X, int Y , int R, int G, int B) {
    int fbindex;
    int alpha;

    _AdjustBufferSize(X, Y);
    alpha                   = R | G | B;
    fbindex                 = (Y * _MAX_WIDTH + X) * _MAX_DEPTH;
    alpha                  &= 0xff;
    if (InvertAlpha) {
        alpha  = 0xff - alpha;
        alpha &= 0xff;
    }
    _FrameBuffer[fbindex]   = alpha;
}

static void _Bmp2Alpha8(void) {
    _ClearBuffer();
    eeBmp_Init(_Read, _SetPixelAlpha8);
    _MaxDepth = 1;
    LoadBmp();
}


/***********************************************************
  Section: Global Functions
*/

void Bmp2RGBA8888(char *Param) {
    TransformFunc = _Bmp2RGBA8888;
}

void Bmp2RGB565(char *Param) {
    TransformFunc = _Bmp2RGB565;
}


void Bmp2RGBA4444(char *Param) {
    TransformFunc = _Bmp2RGBA4444;
}

void Bmp2Alpha8(char *Param) {
    TransformFunc = _Bmp2Alpha8;
}

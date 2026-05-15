
#include <stdio.h>
#include <stdlib.h>
#include "bmp2c.h"
#include "main.h"

typedef struct {
    void (*Func)(char *Param);
    char *Option;
    char *Help;
} CmdOption_t;

const CmdOption_t CmdOption[] = {
    Bmp2RGBA8888,      "-rgba8888",    "RGBA 8888",
    Bmp2RGB565,        "-rgb565",      "RGB 565",
    Bmp2RGBA4444,      "-rgba4444",    "RGBA 4444",
    Bmp2Alpha8,        "-alpha8",      "Alpha 8",
    SetInvertAlpha,    "-invalpha",    "Invert alpha",
    GetSrcFilename,    "-src",         "Source file",
    GetArrayName,      "-array",       "C array name",
    SetAlphaConst,     "-alphaconst",  "Alpha constant (0..255)",
    SetAlphaMaskValue, "-transparent", "Value of transparent color (hex number)",
    SetNecFormat,      "-nec",         "Use NEC output format ('NEC', W, H, D)",
    Help,              "-h",           "Help"
};

const CmdOptionNum = sizeof(CmdOption) / sizeof (CmdOption_t);

void (*TransformFunc)(void);
char  *SrcFilename;
FILE  *Source;
char  *ArrayName      = "Image";
int    AlphaConst     = 0xff;
int    AlphaMaskValue = -1;
int    NecFormat = 0;
int    InvertAlpha = 0;

void SetAlphaMaskValue(char *Param) {
    AlphaMaskValue = strtol(Param, 0, 16);
}

void SetAlphaConst(char *Param) {
    AlphaConst = atoi(Param);
}

void GetSrcFilename(char *Param) {
    SrcFilename = Param;
}

void GetArrayName(char *Param) {
    ArrayName = Param;
}

void SetNecFormat(char *Param) {
    NecFormat = 1;
}

void SetInvertAlpha(char *Param) {
    InvertAlpha = 1;
}

void Help(char *Param) {
    int i;

    printf("TBTF usage:\n");
    for (i = 0; i < CmdOptionNum; i++) {
        printf("    %16.16s - %s\n", CmdOption[i].Option, CmdOption[i].Help);
    }
}

int main(int argc, char **argv) {
    int   i;
    int   j;
    char *param;

    for (i = 0; i < argc; i++) {
        for (j = 0; j < CmdOptionNum; j ++) {
            if (strcmp(argv[i], CmdOption[j].Option) == 0) {
                if ((i + 1) < argc) {
                    param = argv[i + 1];
                } else {
                    param = 0;
                }
                CmdOption[j].Func(param);
            }
        }
    }
    if (SrcFilename) {
        Source = fopen(SrcFilename, "rb");
        if (!Source) {
            fprintf(stderr,"Cannot open %s.\n", SrcFilename);
            exit(-1);
        }
    } else {
        Source = stdin;
    }
    if (TransformFunc) {
        TransformFunc();
    }
    if (SrcFilename) {
        fclose(Source);
    }
    return 0;
}


#include <stdio.h>


typedef struct {
    void (*Func)(char *Param);
    char *Option;
    char *Help;
} CmdOption_t;


extern void Help(char *Param);
extern void GetSrcFilename(char *Param);
extern void GetArrayName(char *Param);
extern void GetFormatName(char *Param);

const CmdOption_t CmdOption[] = {
    GetSrcFilename,    "-src",          "Source file",
    GetArrayName,      "-array",       "C array name",
    GetFormatName,     "-format",      "Variable format of array",    
    Help,              "-h",           "Help"
};

const CmdOptionNum = sizeof(CmdOption) / sizeof (CmdOption_t);


char  *SrcFilename;
char  *FormatName = "char";
FILE  *Source;
char  *ArrayName = "Image";

void Help(char *Param) {
    int i;

    printf("BIN2ARRAY usage:\n");
    for (i = 0; i < CmdOptionNum; i++) {
        printf("    %16.16s - %s\n", CmdOption[i].Option, CmdOption[i].Help);
    }
    exit(0);
}

void GetFormatName(char *Param) {
    FormatName = Param;
}

void GetSrcFilename(char *Param) {
    SrcFilename = Param;
}


void GetArrayName(char *Param) {
    ArrayName = Param;
}

int main(int argc, char **argv) {
    unsigned int ch;
    unsigned int ch1;
    unsigned int ch2;
    unsigned int ch3;
    int          cc;
    int          i;
    int          j;
    char         *param; 
    int          elements;

    elements = 0;
    
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

    
    printf("const %s %s[] = {\n    ", FormatName, ArrayName);
   
    cc = 0;
    
    if ((strcmp(FormatName, "uint16_t") == 0) || (strcmp(FormatName, "int16_t") == 0)) {

        while (!feof(Source)) {
            ch = fgetc(Source);
            ch = (fgetc(Source) << 8) | ch;    // new                
            if (!feof(Source)) {
                printf("0x%4.4X, ", ch);
                cc++;
                if (cc >= 12) {
                    cc = 0;
                    printf("\n    ");
                }
                elements++;
            }
        }   
    } else if ((strcmp(FormatName, "uint32_t") == 0) || (strcmp(FormatName, "int32_t") == 0)) {

        while (!feof(Source)) {
            ch3 = fgetc(Source);
            ch2 = fgetc(Source);
            ch1 = fgetc(Source);
            ch =  (fgetc(Source)<<24) | (ch1 <<16)| (ch2 <<8)| ch3;                                        
            if (!feof(Source)) {
                printf("0x%8.8X, ", ch);
                cc++;
                if (cc >= 12) {
                    cc = 0;
                    printf("\n    ");
                }  
                elements++;
            }
        }  
    } else {
        while (!feof(Source)) {
            ch = fgetc(Source);
            if (!feof(Source)) {
                printf("0x%2.2X, ", ch);
                cc++;
                if (cc >= 12) {
                    cc = 0;
                    printf("\n    ");
                }
                elements++;
            }
        }
    }
    
    printf("};\n"); 
    printf("/*%d Elements are in the Array\n*/",elements);
    printf("\n");     
    if (SrcFilename) {
        fclose(Source);
    }

    return 0;    

}


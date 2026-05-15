
#ifndef _MAIN_H_
#define _MAIN_H_


#ifdef __cplusplus
extern "C" {
#endif

extern void (*TransformFunc)(void);
extern char  *SrcFilename;
extern FILE  *Source;
extern char  *ArrayName;
extern int    AlphaConst;
extern int    AlphaMaskValue;
extern int    NecFormat;
extern int    InvertAlpha;

extern void SetNecFormat(char *Param);
extern void SetAlphaMaskValue(char *Param);
extern void SetAlphaConst(char *Param);
extern void GetSrcFilename(char *Param);
extern void GetArrayName(char *Param);
    extern void SetInvertAlpha(char *Param);
extern void Help(char *Param);



#ifdef __cplusplus
}
#endif

#endif /* _MAIN_H_ */

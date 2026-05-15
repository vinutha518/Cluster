
#ifdef COLOR_TFT
#define PROVEOUT_UP	(uint16_t)3//33//55//33//110
#define PROVEOUT_DOWN (uint16_t)80//150//250//150//250
#define PROVEOUT_DONE PROVEOUT_DOWN
#endif

#ifdef GRAYSCALE_TFT
#define PROVEOUT_UP	(uint16_t)3//7
#define PROVEOUT_DOWN (uint16_t)70//60//80//120
#define PROVEOUT_DONE PROVEOUT_DOWN
#endif

void loc_Ism_TC0001(void);
void init_ISM(void);
void init_ISM_proveout(void);
void manual_zpd(uint8_t param);
void loc_Ism_TC0004(void);


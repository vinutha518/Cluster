#define SPRITE_MAX_BUFFERS 2

typedef struct {
    uint32_t Count;
    void *Buffer[SPRITE_MAX_BUFFERS];
    r_drw2d_IntPoint_t Pos[SPRITE_MAX_BUFFERS];
    r_drw2d_IntSize_t  Size[SPRITE_MAX_BUFFERS];
    r_wm_Sprite_t Sprite;
    uint32_t VoIndex;
    uint32_t DrawIndex;
    r_drw2d_Framebuffer_t DrawFb;
} MultiBufferSprite_t;

void InitMultiBufferSprite(MultiBufferSprite_t *MBSprite, r_wm_Sprite_t Sprite,
                           uint32_t Count);

r_drw2d_Framebuffer_t *MbSpriteNewDrawBufGet(MultiBufferSprite_t *MBSprite);

void MbSpriteSwap(MultiBufferSprite_t *MBSprite);

void MbSpriteDrawGeometrySet(MultiBufferSprite_t *MBSprite,r_drw2d_IntRect_t rect);

uint32_t MbSpriteDrawPosXGet(MultiBufferSprite_t *MBSprite);
uint32_t MbSpriteDrawPosYGet(MultiBufferSprite_t *MBSprite);
void MbSpriteDrawPosSet(MultiBufferSprite_t *MBSprite,uint32_t x, uint32_t y);

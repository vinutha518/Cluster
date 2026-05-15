
void SetupJCUA(void);

extern uint32_t JCUA_running;

void jpeg_wait_for_finish(void);

void jpeg_decode_window(void *src, r_wm_Window_t*  Window);
void jpeg_decode_buffer(void *src, r_drw2d_Framebuffer_t * draw_fb);
void CallSwap(void);

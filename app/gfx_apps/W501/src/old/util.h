r_drw2d_IntRect_t CalcBoundingBoxRect2D( r_drw2d_Rect_t *Rect );
r_drw2d_IntRect_t MaximizeBoundingBox( r_drw2d_IntRect_t *Rect1, r_drw2d_IntRect_t *Rect2 );

void ActivateRotationObjectScaleing(void);
void DeactivateRotationObjectScaleing(void);
void SetNeedleRotationTransformationMatrix(float64_t angle, 
                                           r_drw2d_Point_t rot_center,
                                           r_drw2d_Point_t tex_rot_center,
                                           uint32_t use_texture);

void Clear_RotationObject(float64_t angle,
                          r_drw2d_Point_t rot_center,
                          r_drw2d_Point_t tex_rot_center,
                          r_drw2d_Texture_t *texture,
                          uint32_t border);

void Refresh_RotationObject(float64_t angle,
                            r_drw2d_Point_t rot_center,
                            r_drw2d_Point_t tex_rot_center,
                            r_drw2d_Texture_t *texture, uint32_t blendmode );

void Refresh_ParallelRotatedObject(float64_t angle,
                                   r_drw2d_Point_t rot_center,
                                   r_drw2d_Point_t tex_rot_center,
                                   r_drw2d_Point_t tex_offset,
                                   r_drw2d_Texture_t *texture );

void Clear_ParallelRotatedObject(float64_t angle,
                                 r_drw2d_Point_t rot_center,
                                 r_drw2d_Point_t tex_rot_center,
                                 r_drw2d_Point_t tex_offset,
                                 r_drw2d_Texture_t *texture,                                 
                                 r_drw2d_Texture_t *background);

void CopyToCache_Light(float64_t angle, r_drw2d_Texture_t *background, uint32_t is_left,
                       r_drw2d_Framebuffer_t *draw_fb);

void SetNeedleRefreshSpritePos(r_drw2d_Point_t pos);
r_drw2d_Point_t GetNeedleRefreshSpritePos(void);

r_drw2d_IntRect_t CalcRect_RotationObject(float64_t angle,
                                       r_drw2d_Point_t rot_center,
                                       r_drw2d_Point_t tex_rot_center,
                                       r_drw2d_Texture_t *texture);

r_drw2d_IntRect_t CalcRect_ParallelRotatedObject(float64_t angle,
                                              r_drw2d_Point_t rot_center,
                                              r_drw2d_Point_t tex_rot_center,
                                              r_drw2d_Point_t tex_offset,
                                              r_drw2d_Texture_t *texture );


r_drw2d_Point_t IntPoint2Point(r_drw2d_IntPoint_t * IntPoint);
r_drw2d_Rect_t  IntRect2Rect(r_drw2d_IntRect_t * IntRect);

void Copy_Rect(r_drw2d_Texture_t *src_p, r_drw2d_Texture_t  *dst_p,
               r_drw2d_IntRect_t *src_rect_p, r_drw2d_IntPoint_t *dst_pos_p);

void QosSlowDave(void);
void QosFastDave(void);

uint32_t my_read(uint32_t address);
void my_write(uint32_t address, uint32_t value);

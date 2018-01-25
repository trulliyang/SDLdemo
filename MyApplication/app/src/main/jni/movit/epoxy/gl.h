#ifndef gl_define_h
#define gl_define_h 1

// Convert postmultiplied alpha to premultiplied alpha, simply by dividing.
//
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>


#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#define GL_PIXEL_PACK_BUFFER_ARB                                                   0x88EB
#define GL_BGRA                                                                    0x80E1
#define GL_READ_ONLY                                                               0x88B8
#define GL_UNSIGNED_INT_8_8_8_8_REV                                                0x8367
#define GL_FRAMEBUFFER_SRGB                                                        0x8DB9
#define GL_TIME_ELAPSED                                                            0x88BF
#define GL_R16                                                                     0x822A
#define GL_RG16                                                                    0x822C
#define GL_RGB16                                                                   0x8054
#define GL_PIXEL_UNPACK_BUFFER_ARB                                                 0x88EC
#define GL_RGBA16                                                                  0x805B
#define GL_RGBA32F_ARB                                                             0x8814
#define GL_RGBA16F_ARB                                                             0x881A
#define GL_RGB10                                                                   0x8052
#define GL_RGB32F_ARB                                                              0x8815
#define GL_RGB16F_ARB                                                              0x881B

#endif // !defined(_MOVIT_ALPHA_DIVISION_EFFECT_H)

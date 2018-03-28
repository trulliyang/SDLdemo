// center_x, center_y
// scale_x, scale_y
// rotate_degree
// translate_x, translate_y

// transform order is scale, rotate and translate

vec4 FUNCNAME(vec2 tc) {
    // do scale
    float sx = PREFIX(scale_x);
    float sy = PREFIX(scale_y);
    float sxxf = (1.0-sx)*0.5;
    float syyf = (1.0-sy)*0.5;

    vec3 tcBeforeScale = vec3(tc.x, tc.y, 1.0);
    mat3 mat_scale = mat3(  sx,    0.0,    0.0,
                           0.0,     sy,    0.0,
                          sxxf,   syyf,    1.0);

    vec3 tcScale = mat_scale*tcBeforeScale;

    // do rotate
    float w = PREFIX(width);
    float h = PREFIX(height);
    float wr = 1.0/w;
    float hr = 1.0/h;
//    float wOrigin  = w*tc.x;
//    float hOrigin  = h*tc.y;
//
//    vec3 tcOrigin = vec3(wOrigin, hOrigin, 1.0);


    vec3 tcBeforeRotate = vec3(tcScale.x*w, tcScale.y*h, 1.0);

    mat3 rtmtx = mat3(1.0, 0.0, 0.0,
                      0.0, 1.0, 0.0,
                      0.0, 0.0, 1.0);

//    float rotate_degree = 45.0;
    float rotate_x = w*0.5;
    float rotate_y = h*0.5;

//    float rotate_radius = rotate_degree * 3.1415926535897932384626 / 180.0;
    float rotate_radius = PREFIX(rotate_degree) * 0.017453292519943;
    float c = cos(rotate_radius);
    float s = sin(rotate_radius);

    rtmtx[0][0] = c;
    rtmtx[1][0] = -s;
    rtmtx[2][0] = (1.0-c)*rotate_x + rotate_y*s;

    rtmtx[0][1] = s;
    rtmtx[1][1] = c;
    rtmtx[2][1] = (1.0-c)*rotate_y - rotate_x*s;

    rtmtx[0][2] = 0.0;
    rtmtx[1][2] = 0.0;
    rtmtx[2][2] = 1.0;

    vec3 tcRotate = rtmtx*tcBeforeRotate;

    // do translate
    vec3 tcBeforeTranslate = vec3(tcRotate.x*wr, tcRotate.y*hr, 1.0);
    float tl_x = PREFIX(translate_x);
    float tl_y = PREFIX(translate_y);
    mat3 tlmtx = mat3(1.0,  0.0, 0.0,
                      0.0,  1.0, 0.0,
                      tl_x, tl_y, 1.0);

    vec3 tcTranslate = tlmtx*tcBeforeTranslate;

    // finally
    vec2 tcFinal = tcTranslate.xy;

    if (0.0 <= tcFinal.x && tcFinal.x <= 1.0 && 0.0 <= tcFinal.y && tcFinal.y <= 1.0) {
        return INPUT(tcFinal);
    } else {
        return vec4(PREFIX(out_color_red), PREFIX(out_color_green), PREFIX(out_color_blue), PREFIX(out_color_alpha));
    }
}

// generate a scaled frame

vec4 FUNCNAME(vec2 tc) {
    float sx = PREFIX(scale_x);
    float sy = PREFIX(scale_y);
    float sxxf = (1.0-sx)*0.5;
    float syyf = (1.0-sy)*0.5;

    vec3 tcOrigin = vec3(tc.x, tc.y, 1.0);

    mat3 mat_scale = mat3(  sx,    0.0,    0.0,
                           0.0,     sy,    0.0,
                          sxxf,   syyf,    1.0);

    vec3 tcNew = mat_scale*tcOrigin;
    if (0.0 < tcNew.x && tcNew.x < 1.0 && 0.0 < tcNew.y && tcNew.y < 1.0) {
        return INPUT(tcNew.xy);
    } else {
        return vec4(0.0, 0.0, 1.0, 0.8);
    }
}

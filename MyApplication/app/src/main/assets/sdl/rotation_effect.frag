vec4 FUNCNAME(vec2 tc) {
   float w = PREFIX(width);
   float h = PREFIX(height);
   float wr = 1.0/w;
   float hr = 1.0/h;
   float wOrigin  = w*tc.x;
   float hOrigin  = h*tc.y;

    vec3 tcOrigin = vec3(wOrigin, hOrigin, 1.0);

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

    vec3 tcRotate = rtmtx*tcOrigin;

    vec4 rgbaRotate = vec4(PREFIX(outColorRed), PREFIX(outColorGreen), PREFIX(outColorBlue), PREFIX(outColorAlpha));
    if (0.0 <= tcRotate.x*wr && tcRotate.x*wr <= 1.0 && 0.0 <= tcRotate.y*hr && tcRotate.y*hr <= 1.0) {
        rgbaRotate = INPUT(vec2(tcRotate.x*wr, tcRotate.y*hr));
    }

    return rgbaRotate;
}

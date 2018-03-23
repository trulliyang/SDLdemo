vec4 FUNCNAME(vec2 tc) {
//	vec4 rgba = INPUT(tc);
//	return rgba;

//    if (PREFIX(test) > 2) {
//        return vec4(1.0, 0.0, 0.0, 1.0);
//    } else {
//        return vec4(0.0, 1.0, 1.0, 1.0);
//    }


   float w = 1920.0;
   float h = 1070.0;
   float wr = 1.0/w;
   float hr = 1.0/h;
   float wOrigin  = w*tc.x;
   float hOrigin  = h*tc.y;


    vec3 tcOrigin = vec3(wOrigin, hOrigin, 1.0);

    mat3 rtmtx = mat3(1.0, 0.0, 0.0,
                      0.0, 1.0, 0.0,
                      0.0, 0.0, 1.0);


    float rotate_degree = 45.0;
    float rotate_x = w*0.5;
    float rotate_y = h*0.5;

    float rotate_radius = rotate_degree * 3.1415926535897932384626 / 180.0;

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



//    vec3 tcRotate = PREFIX(rotate_matrix)*tcOrigin;
    vec4 rgbaRotate = INPUT(vec2(tcRotate.x*wr, tcRotate.y*hr));

//	ycbcr_a.rgb = PREFIX(ycbcr_matrix) * rgba.rgb + PREFIX(offset);

	return rgbaRotate;

}

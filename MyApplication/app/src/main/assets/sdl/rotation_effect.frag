vec4 FUNCNAME(vec2 tc) {
//	vec4 rgba = INPUT(tc);
//	return rgba;

//    if (PREFIX(test) > 2) {
//        return vec4(1.0, 0.0, 0.0, 1.0);
//    } else {
//        return vec4(0.0, 1.0, 1.0, 1.0);
//    }


    vec3 tcOrigin = vec3(tc, 1.0);

    mat3 rtmtx = mat3(1.0, 0.0, 0.0,
                      0.0, 1.0, 0.0,
                      0.0, 0.0, 1.0);


//    rotate_degree = 90.0;
//    rotate_x = 0.5;
//    rotate_y = 0.5;
//
//    float rotate_radius = rotate_degree * 3.1415926535897932384626 / 180.0;
//
//    float c = cos(rotate_radius);
//    float s = sin(rotate_radius);
//
//
//
//
//    rtmtx[0][0] = c;
//    rtmtx[1][0] = -s;
//    rtmtx[2][0] = (1.0-c)*rotate_x + rotate_y*s;
//
//    rtmtx[0][1] = s;
//    rtmtx[1][1] = c;
//    rtmtx[2][1] = (1.0-c)*rotate_y - rotate_x*s;
//
//    rtmtx[0][2] = 0.0;
//    rtmtx[1][2] = 0.0;
//    rtmtx[2][2] = 1.0;


    vec3 tcRotate = rtmtx*tcOrigin;



//    vec3 tcRotate = PREFIX(rotate_matrix)*tcOrigin;
    vec4 rgbaRotate = INPUT(vec2(tcRotate.x, tcRotate.y));

//	ycbcr_a.rgb = PREFIX(ycbcr_matrix) * rgba.rgb + PREFIX(offset);

	return rgbaRotate;

}

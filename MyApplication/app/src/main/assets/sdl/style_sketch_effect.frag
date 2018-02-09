// generate a sketch style frame

vec4 FUNCNAME(vec2 tc) {
     float dx = PREFIX(widthStepSK);
     float dy = PREFIX(heightStepSK);

     float bli = INPUT(tc+vec2(-dx, -dy)).r;
     float tri = INPUT(tc+vec2( dx,  dy)).r;
     float tli = INPUT(tc+vec2(-dx,  dy)).r;
     float bri = INPUT(tc+vec2( dx, -dy)).r;
     float li  = INPUT(tc+vec2(-dx,   0)).r;
     float ri  = INPUT(tc+vec2( dx,   0)).r;
     float bi  = INPUT(tc+vec2(  0, -dy)).r;
     float ti  = INPUT(tc+vec2(  0,  dy)).r;

     float h = -tli - 2.0 * ti - tri + bli + 2.0 * bi + bri;
     float v = -bli - 2.0 * li - tli + bri + 2.0 * ri + tri;

     float mag = 1.0 - (length(vec2(h, v)) * PREFIX(edgeStrengthSK));
     vec4 x = vec4(vec3(mag), 1.0);
	return x;
}

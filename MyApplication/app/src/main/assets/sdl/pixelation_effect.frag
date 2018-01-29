// generate a gray frame


vec4 FUNCNAME(vec2 tc) {
    float dx = PREFIX(widthStep)*PREFIX(pixel);
    float dy = PREFIX(heightStep)*PREFIX(pixel);
    vec2 tcNew = vec2(dx * floor(tc.x / dx), dy * floor(tc.y / dy));
    vec4 x = INPUT(tcNew);

	return x;
}

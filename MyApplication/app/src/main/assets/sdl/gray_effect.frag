// generate a gray frame

vec4 FUNCNAME(vec2 tc) {
	vec4 x = INPUT(tc);
#if 0
    float gray = 0.0;
    if (0 == PREFIX(type)) {
        gray = x.r*0.299 + x.g*0.587 + x.b*0.114;
    } else if (1 == PREFIX(type)) {
        gray = (x.r + x.g + x.b)/3.0;
    } else if (2 == PREFIX(type)) {
        gray = x.r;
    } else if (3 == PREFIX(type)) {
        gray = x.g;
    } else if (4 == PREFIX(type)) {
        gray = x.b;
    } else if (5 == PREFIX(type)) {
        gray = 0.5;
    }
    x.rgb = vec3(gray, gray, gray);
    return x;
#else
    float gray = x.r*0.299 + x.g*0.587 + x.b*0.114;
	x.rgb = vec3(gray, gray, gray);
	return x;
#endif
}

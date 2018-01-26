// generate a gray frame

vec4 FUNCNAME(vec2 tc) {
	vec4 x = INPUT(tc);

    float gray = x.r*0.299 + x.g*0.587 + x.b*0.114;
	x.rgb = vec3(gray, gray, gray);

	return x;
}

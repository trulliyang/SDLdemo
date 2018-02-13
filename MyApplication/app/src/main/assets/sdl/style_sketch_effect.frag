// generate a sketch style frame

vec4 FUNCNAME(vec2 tc) {
    float dx = PREFIX(widthStepSK);
    float dy = PREFIX(heightStepSK);

    vec3 bli = INPUT(tc + vec2(-dx, -dy)).rgb;
    vec3 tri = INPUT(tc + vec2( dx,  dy)).rgb;
    vec3 tli = INPUT(tc + vec2(-dx,  dy)).rgb;
    vec3 bri = INPUT(tc + vec2( dx, -dy)).rgb;
    vec3 li  = INPUT(tc + vec2(-dx, 0.0)).rgb;
    vec3 ri  = INPUT(tc + vec2( dx, 0.0)).rgb;
    vec3 bi  = INPUT(tc + vec2(0.0, -dy)).rgb;
    vec3 ti  = INPUT(tc + vec2(0.0,  dy)).rgb;
    vec3 ci  = INPUT(tc + vec2(0.0, 0.0)).rgb;

    vec3 h = -tli - 1.0 * ti - tri + bli + 1.0 * bi + bri;
    vec3 v = -bli - 1.0 * li - tli + bri + 1.0 * ri + tri;

    vec3 mag;
    mag.r = 1.0 - (length(vec2(h.r, v.r)) * PREFIX(edgeStrengthSK));
    mag.g = 1.0 - (length(vec2(h.g, v.g)) * PREFIX(edgeStrengthSK));
    mag.b = 1.0 - (length(vec2(h.b, v.b)) * PREFIX(edgeStrengthSK));

//    mag = mix(mag, ci, 0.0);
//    vec4 x = vec4(mag, 1.0);

    float edge = (mag.r + mag.g + mag.b)/3.0;
//    float edge = min(min(mag.r, mag.g), mag.b);
    vec4 x = vec4(vec3(edge), 1.0);

    return x;
}

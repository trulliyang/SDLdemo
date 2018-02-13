// use newColor to replace targetColor in originColor

vec4 FUNCNAME(vec2 tc) {
	vec4 originColor = INPUT(tc);
//	vec4 newColor = INPUT2(tc);

    vec4 newColor = vec4(0.0, 0.0, 0.0, 1.0);

    vec3 rgb2y = vec3(0.2989, 0.5866, 0.1145);
    vec3 targetColor = vec3(PREFIX(targetColor_r), PREFIX(targetColor_g), PREFIX(targetColor_b));
    targetColor = vec3(0.0, 0.7, 0.0);

    float targetY = dot(rgb2y, targetColor);
    float targetU = 0.7132 * (targetColor.r - targetY);
    float targetV = 0.5647 * (targetColor.b - targetY);

    float originY = dot(rgb2y, originColor.rgb);
    float originU = 0.7132 * (originColor.r - originY);
    float originV = 0.5647 * (originColor.b - originY);

//    float blendValue = 1.0 - smoothstep(PREFIX(thresholdSensitivity), PREFIX(thresholdSensitivity) + PREFIX(smoothing), distance(vec2(originU, originV), vec2(targetU, targetV)));
//    blendValue = 1.0;
    float blendValue = 1.0 - smoothstep(0.3, 0.5, distance(vec2(originU, originV), vec2(targetU, targetV)));

	return mix(originColor, newColor, blendValue);
}

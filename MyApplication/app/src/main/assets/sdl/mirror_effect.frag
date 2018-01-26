// Mirrors the image horizontally.
vec4 FUNCNAME(vec2 tc)
{
	tc.x = 1.0 - tc.x;
	tc.y = 1.0 - tc.y;
	return INPUT(tc);
}

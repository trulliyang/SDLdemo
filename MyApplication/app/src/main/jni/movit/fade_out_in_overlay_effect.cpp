#include "fade_out_in_overlay_effect.h"
#include "util.h"

using namespace std;

namespace movit {
	
	FadeOutInOverlayEffect::FadeOutInOverlayEffect()
	: swap_inputs(false)
{
	__android_log_print(ANDROID_LOG_ERROR, "shiyang", "shiyang FadeOutInOverlayEffect Constructor");
	register_int("swap_inputs", (int *)&swap_inputs);
    
    fade_progress = 0.0;
    register_float("fade_progress", &fade_progress);
}

string FadeOutInOverlayEffect::output_fragment_shader()
{
	__android_log_print(ANDROID_LOG_ERROR, "shiyang", "shiyang FadeOutInOverlayEffect output_fragment_shader");
	char buf[256];
	snprintf(buf, sizeof(buf), "#define SWAP_INPUTS %d\n", swap_inputs);
    return buf + read_file("fade_out_in_overlay_effect.frag");
}
	
void FadeOutInOverlayEffect::set_gl_state(GLuint glsl_program_num,
										  const string &prefix,
										  unsigned *sampler_num)
{
	Effect::set_gl_state(glsl_program_num, prefix, sampler_num);
	fade_progress += 0.01;
	if (fade_progress > 1.0f) {
		fade_progress = 1.0f;
	}
	
		
//		widthStep = 1.0f/1920.0f;
//		heightStep = 1.0f/1072.0f;
}
	
}  // namespace movit

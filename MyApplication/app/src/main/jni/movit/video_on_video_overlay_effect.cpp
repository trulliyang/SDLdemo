#include "video_on_video_overlay_effect.h"
#include "util.h"

using namespace std;

namespace movit {
	
	VideoOnVideoOverlayEffect::VideoOnVideoOverlayEffect()
	: swap_inputs(false)
{
	__android_log_print(ANDROID_LOG_ERROR, "shiyang", "shiyang VideoOnVideoOverlayEffect Constructor");
	register_int("swap_inputs", (int *)&swap_inputs);
    
    x_offset_percentage = 0.5;
    register_float("x_offset_percentage", &x_offset_percentage);
    
    y_offset_percentage = 0.25;
    register_float("y_offset_percentage", &y_offset_percentage);
    
    w_top_percentage = 0.3;
    register_float("w_top_percentage", &w_top_percentage);
    
    h_top_percentage = 0.2;
    register_float("h_top_percentage", &h_top_percentage);
}

string VideoOnVideoOverlayEffect::output_fragment_shader()
{
	__android_log_print(ANDROID_LOG_ERROR, "shiyang", "shiyang VideoOnVideoOverlayEffect output_fragment_shader");
	char buf[256];
	snprintf(buf, sizeof(buf), "#define SWAP_INPUTS %d\n", swap_inputs);
    return buf + read_file("video_on_video_overlay_effect.frag");
}
	
void VideoOnVideoOverlayEffect::set_gl_state(GLuint glsl_program_num,
										  const string &prefix,
										  unsigned *sampler_num)
{
	Effect::set_gl_state(glsl_program_num, prefix, sampler_num);
//	fade_progress += 0.01;
//	if (fade_progress > 1.0f) {
//		fade_progress = 1.0f;
//	}
//
		
//		widthStep = 1.0f/1920.0f;
//		heightStep = 1.0f/1072.0f;
}
	
}  // namespace movit

#include "green_replacing_overlay_effect.h"
#include "util.h"

using namespace std;

namespace movit {
	
GreenReplacingOverlayEffect::GreenReplacingOverlayEffect()
	: swap_inputs(false)
{
	__android_log_print(ANDROID_LOG_ERROR, "shiyang", "shiyang GreenReplacingOverlayEffect Constructor");
	register_int("swap_inputs", (int *)&swap_inputs);
	
	targetColorRed = 0.0f;
	targetColorGreen = 1.0f;
	targetColorBlue = 0.0f;
	targetColorAlpha = 1.0f;
	
	diff = 0.1f;
	diffMin = 0.1f;
	diffMax = 0.9f;
	
	register_float("targetColorRed", &targetColorRed);
	register_float("targetColorGreen", &targetColorGreen);
	register_float("targetColorBlue", &targetColorBlue);
	register_float("targetColorAlpha", &targetColorAlpha);
	
	register_float("diff", &diff);
	register_float("diffMin", &diffMin);
	register_float("diffMax", &diffMax);
}

string GreenReplacingOverlayEffect::output_fragment_shader()
{
	__android_log_print(ANDROID_LOG_ERROR, "shiyang", "shiyang GreenReplacingOverlayEffect output_fragment_shader");
	char buf[256];
	snprintf(buf, sizeof(buf), "#define SWAP_INPUTS %d\n", swap_inputs);
	return buf + read_file("green_replacing_overlay_effect.frag");
}

}  // namespace movit

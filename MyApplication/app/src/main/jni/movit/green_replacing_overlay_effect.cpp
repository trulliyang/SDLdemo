#include "green_replacing_overlay_effect.h"
#include "util.h"

using namespace std;

namespace movit {
	
GreenReplacingOverlayEffect::GreenReplacingOverlayEffect()
	: swap_inputs(false)
{
	__android_log_print(ANDROID_LOG_ERROR, "shiyang", "shiyang GreenReplacingOverlayEffect Constructor");
	register_int("swap_inputs", (int *)&swap_inputs);
	
//	  targetColorRed = 0.0f;
//	  targetColorGreen = 1.0f;
//	  targetColorBlue = 0.0f;
//	  targetColorAlpha = 1.0f;
    
//    targetColorRed = 16.0f/255.0f;
//    targetColorGreen = 240.0f/255.0f;
//    targetColorBlue = 16.0f/255.0f;
//    targetColorAlpha = 1.0f;
    
//    targetColorRed = -5.0f/255.0f;
//    targetColorGreen = 183.0f/255.0f;
//    targetColorBlue = -2.0f/255.0f;
//    targetColorAlpha = 1.0f;
	
//	  targetColorRed = 0.0f/255.0f;
//	  targetColorGreen = 255.0f/255.0f;
//	  targetColorBlue = 0.0f/255.0f;
//	  targetColorAlpha = 1.0f;
    
//    targetColorRed = 52.0f/255.0f;
//    targetColorGreen = 224.0f/255.0f;
//    targetColorBlue = 4.0f/255.0f;
//    targetColorAlpha = 1.0f;
    
	perTopWidth = 1.0f/1920.0f;
	perTopHeight = 1.0f/1070.0f;
	
//	  targetColorRed = 49.0f/255.0f;
//	  targetColorGreen = 251.0f/255.0f;
//	  targetColorBlue = 44.0f/255.0f;
//	  targetColorAlpha = 1.0f;
	
	// waterfall sccj01
	targetColorRed = 27.0f/255.0f;
	targetColorGreen = 157.0f/255.0f;
	targetColorBlue = 14.0f/255.0f;
	targetColorAlpha = 1.0f;
	
	// zombia sccj02
//    targetColorRed = 22.0f/255.0f;
//    targetColorGreen = 168.0f/255.0f;
//    targetColorBlue = 2.0f/255.0f;
//    targetColorAlpha = 1.0f;
	
	// box sccj03
//	  targetColorRed = 77.0f/255.0f;
//	  targetColorGreen = 198.0f/255.0f;
//	  targetColorBlue = 74.0f/255.0f;
//	  targetColorAlpha = 1.0f;
 
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
	
	register_float("perTopWidth", &perTopWidth);
	register_float("perTopHeight", &perTopHeight);
}

string GreenReplacingOverlayEffect::output_fragment_shader()
{
	__android_log_print(ANDROID_LOG_ERROR, "shiyang", "shiyang 1 GreenReplacingOverlayEffect output_fragment_shader");
	char buf[256];
	snprintf(buf, sizeof(buf), "#define SWAP_INPUTS %d\n", swap_inputs);
	return buf + read_file("green_replacing_overlay_effect.frag");
}

}  // namespace movit

#ifndef _MOVIT_WATERMARK_OVERLAY_EFFECT_H
#define _MOVIT_WATERMARK_OVERLAY_EFFECT_H 1

// Put one image on top of another, using alpha where appropriate.
// (If both images are the same aspect and the top image has alpha=1.0
// for all pixels, you will not see anything of the one on the bottom.)
//
// This is the “over” operation from Porter-Duff blending, also used
// when merging layers in e.g. GIMP or Photoshop.
//
// The first input is the bottom, and the second is the top.

#include <string>

#include "effect.h"

namespace movit {

class WatermarkOverlayEffect : public Effect {
public:
	WatermarkOverlayEffect();
	virtual std::string effect_type_id() const { return "WatermarkOverlayEffect"; }
	std::string output_fragment_shader();

	virtual bool needs_srgb_primaries() const { return false; }
	virtual unsigned num_inputs() const { return 2; }
	virtual bool one_to_one_sampling() const { return true; }
	void set_gl_state(GLuint glsl_program_num, const std::string &prefix, unsigned *sampler_num);
	// Actually, if _either_ image has blank alpha, our output will have
	// blank alpha, too (this only tells the framework that having _both_
	// images with blank alpha would result in blank alpha).
	// However, understanding that would require changes
	// to EffectChain, so postpone that optimization for later.
	virtual AlphaHandling alpha_handling() const { return INPUT_PREMULTIPLIED_ALPHA_KEEP_BLANK; }

private:
	// If true, overlays input1 on top of input2 instead of vice versa.
	// Must be set before finalize.
	bool swap_inputs;
    float x_offset_percentage;
    float y_offset_percentage;
    float w_top_percentage;
    float h_top_percentage;
	float alpha;
};

}  // namespace movit

#endif // !defined(_MOVIT_WATERMARK_OVERLAY_EFFECT_H)

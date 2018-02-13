//
// Created by shiyang1 on 2018/2/11.
//

#include "chroma_key_effect.h"
#include <epoxy/gl.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "effect_util.h"
#include "util.h"

using namespace std;

namespace movit {
    
    ChromaKeyEffect::ChromaKeyEffect()
    {
        register_float("targetColor_r", &targetColor_r);
        register_float("targetColor_g", &targetColor_g);
        register_float("targetColor_b", &targetColor_b);
        register_float("thresholdSensitivity", &thresholdSensitivity);
        register_float("smoothing", &smoothing);
    }

    string ChromaKeyEffect::output_fragment_shader()
    {
        return read_file("chroma_key_effect.frag");
    }

    void ChromaKeyEffect::inform_input_size(unsigned input_num, unsigned width, unsigned height) {
        assert(input_num == 0);
    }

    void ChromaKeyEffect::set_gl_state(GLuint glsl_program_num,
                                       const string &prefix,
                                       unsigned *sampler_num)
    {
        Effect::set_gl_state(glsl_program_num, prefix, sampler_num);
        targetColor_r = 0.0f;
        targetColor_g= 1.0f;
        targetColor_b = 0.0f;
        thresholdSensitivity = 0.3f;
        smoothing = 0.3f;
    }

}  // namespace movit
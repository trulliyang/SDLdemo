//
// Created by shiyang1 on 2018/2/26.
//

#include "green_replacing_effect.h"
#include <epoxy/gl.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "effect_util.h"
#include "util.h"

using namespace std;

namespace movit {
    
    GreenReplacingEffect::GreenReplacingEffect()
    {
        
        threshold = 0.0f;
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
        
        register_float("threshold", &threshold);
    }

    string GreenReplacingEffect::output_fragment_shader()
    {
        return read_file("green_replacing_effect.frag");
    }

    void GreenReplacingEffect::inform_input_size(unsigned input_num,
                                                 unsigned width,
                                                 unsigned height) {
        assert(input_num == 0);
    }

    void GreenReplacingEffect::set_gl_state(GLuint glsl_program_num,
                                            const string &prefix,
                                            unsigned *sampler_num)
    {
        Effect::set_gl_state(glsl_program_num, prefix, sampler_num);
    }

}  // namespace movit
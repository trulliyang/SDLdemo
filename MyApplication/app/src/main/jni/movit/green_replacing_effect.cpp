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
        greenSaturationMin0 = 0.0f;
        greenSaturationMax0 = 0.3f;
        greenValueMin0 = 0.25f;
        greenValueMax0 = 0.8f;
        greenHueMin0 = 90.0f;
        greenHueMax0 = 180.5f;
    
        greenSaturationMin1 = 0.0f;
        greenSaturationMax1 = 0.3f;
        greenValueMin1 = 0.25f;
        greenValueMax1 = 0.8f;
        greenHueMin1 = 90.0f;
        greenHueMax1 = 180.5f;
        
        threshold = 0.0f;
        
        register_float("greenSaturationMin0", &greenSaturationMin0);
        register_float("greenSaturationMax0", &greenSaturationMax0);
        register_float("greenValueMin0", &greenValueMin0);
        register_float("greenValueMax0", &greenValueMax0);
        register_float("greenHueMin0", &greenHueMin0);
        register_float("greenHueMax0", &greenHueMax0);
    
        register_float("greenSaturationMin1", &greenSaturationMin1);
        register_float("greenSaturationMax1", &greenSaturationMax1);
        register_float("greenValueMin1", &greenValueMin1);
        register_float("greenValueMax1", &greenValueMax1);
        register_float("greenHueMin1", &greenHueMin1);
        register_float("greenHueMax1", &greenHueMax1);
        
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
    
        greenSaturationMin0 = 0.0f;
        greenSaturationMax0 = 0.3f;
        greenValueMin0 = 0.25f;
        greenValueMax0 = 0.8f;
        greenHueMin0 = 90.0f;
        greenHueMax0 = 180.5f;
    
        greenSaturationMin1 = 0.0f;
        greenSaturationMax1 = 0.3f;
        greenValueMin1 = 0.25f;
        greenValueMax1 = 0.8f;
        greenHueMin1 = 90.0f;
        greenHueMax1 = 180.5f;
        
        threshold += 0.01f;
        if (threshold > 1.0f) {
            threshold = 0.0f;
        }
    }

}  // namespace movit
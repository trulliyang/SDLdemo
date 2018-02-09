//
// Created by shiyang1 on 2018/2/7.
//

#include "style_sketch_effect.h"
#include <epoxy/gl.h>
#include <assert.h>
#include <math.h>

#include "effect_util.h"
#include "util.h"

using namespace std;

namespace movit {

    StyleSketchEffect::StyleSketchEffect()
            : edgeStrength(2.0f), widthStep(1.0f/1920.0f), heightStep(1.0f/1072.0f)
    {
        register_float("edgeStrengthSK", &edgeStrength);
        register_float("widthStepSK", &widthStep);
        register_float("heightStepSK", &heightStep);
    }

    string StyleSketchEffect::output_fragment_shader()
    {
        return read_file("style_sketch_effect.frag");
    }

    void StyleSketchEffect::inform_input_size(unsigned input_num,
                                             unsigned width,
                                             unsigned height) {
        assert(input_num == 0);
        widthStep = 1.0f/width;
        heightStep = 1.0f/height;
    }

    void StyleSketchEffect::set_gl_state(GLuint glsl_program_num,
                                        const string &prefix,
                                        unsigned *sampler_num)
    {
        Effect::set_gl_state(glsl_program_num, prefix, sampler_num);
    
        edgeStrength = 2.0f;
//        widthStep = 1.0f/1920.0f;
//        heightStep = 1.0f/1072.0f;
    }

}  // namespace movit
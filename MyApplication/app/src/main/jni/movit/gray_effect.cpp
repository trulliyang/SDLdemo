//
// Created by shiyang1 on 2018/1/26.
//

#include "gray_effect.h"
#include <epoxy/gl.h>
#include <assert.h>
#include <math.h>

#include "effect_util.h"
#include "util.h"

using namespace std;

namespace movit {

    GrayEffect::GrayEffect()
            : type(0)
    {
        register_int("type", (int *)&type);
    }

    string GrayEffect::output_fragment_shader()
    {
        return read_file("gray_effect.frag");
    }

    void GrayEffect::inform_input_size(unsigned input_num, unsigned width, unsigned height) {
        assert(input_num == 0);
    }

    void GrayEffect::set_gl_state(GLuint glsl_program_num, const string &prefix, unsigned *sampler_num)
    {
        Effect::set_gl_state(glsl_program_num, prefix, sampler_num);

        type = 0;
    }

}  // namespace movit
//
// Created by shiyang1 on 2018/1/29.
//

#include "pixelation_effect.h"
#include <epoxy/gl.h>
#include <assert.h>
#include <math.h>

#include "effect_util.h"
#include "util.h"

using namespace std;

namespace movit {

    PixelationEffect::PixelationEffect()
            : pixel(8.0f), widthStep(1.0f/1920.0f), heightStep(1.0f/1072.0f)
    {
        register_float("pixel", (float *)&pixel);
        register_float("widthStep", (float *)&widthStep);
        register_float("heightStep", (float *)&heightStep);
    }

    string PixelationEffect::output_fragment_shader()
    {
        return read_file("pixelation_effect.frag");
    }

    void PixelationEffect::inform_input_size(unsigned input_num,
                                             unsigned width,
                                             unsigned height) {
        assert(input_num == 0);
        widthStep = 1.0f/width;
        heightStep = 1.0f/height;
    }

    void PixelationEffect::set_gl_state(GLuint glsl_program_num,
                                        const string &prefix,
                                        unsigned *sampler_num)
    {
        Effect::set_gl_state(glsl_program_num, prefix, sampler_num);

        pixel = 8.0f*5.0f;
        widthStep = 1.0f/1920.0f;
        heightStep = 1.0f/1072.0f;
    }

}  // namespace movit
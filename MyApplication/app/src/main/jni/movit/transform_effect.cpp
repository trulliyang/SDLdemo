//
// Created by shiyang1 on 2018/3/28.
//

#include "transform_effect.h"
#include <epoxy/gl.h>
#include <assert.h>
#include <math.h>

#include "effect_util.h"
#include "util.h"

using namespace std;

namespace movit {
    
    TransformEffect::TransformEffect()
            : scale_x(1.0f), scale_y(1.0f), rotate_degree(0.0f), translate_x(0.0f), translate_y(0.0f),
              out_color_red(1.0f), out_color_green(0.0f), out_color_blue(0.0f), out_color_alpha(1.0f),
              width(1920.0f), height(1070.0f)
    {
        
        register_float("scale_x", &scale_x);
        register_float("scale_y", &scale_y);
        register_float("rotate_degree", &rotate_degree);
        register_float("translate_x", &translate_x);
        register_float("translate_y", &translate_y);
        register_float("out_color_red", &out_color_red);
        register_float("out_color_green", &out_color_green);
        register_float("out_color_blue", &out_color_blue);
        register_float("out_color_alpha", &out_color_alpha);
        register_float("width", &width);
        register_float("height", &height);
        
        
        test_scale_xy = 0.0;
        test_rotate_radius = 0.0;
        test_translate_xy = 0.0;
        test_all_value = 0.0;
    }

    string TransformEffect::output_fragment_shader()
    {
        return read_file("transform_effect.frag");
    }

    void TransformEffect::inform_input_size(unsigned input_num,
                                             unsigned width,
                                             unsigned height) {
        assert(input_num == 0);
    }

    void TransformEffect::set_gl_state(GLuint glsl_program_num,
                                        const string &prefix,
                                        unsigned *sampler_num)
    {
        Effect::set_gl_state(glsl_program_num, prefix, sampler_num);
        
        // start dancing haha
        {
            float test_value_raw = cos(test_all_value);
            float test_value = abs(test_value_raw);
            scale_x = 2.0f*test_value;
            scale_y = 3.0f*test_value;
        
            width = 1920/scale_x;
            height = 1070/scale_y;
            rotate_degree = 360.0f*test_value;
            translate_x = (0.5f-scale_x/2.0f)*test_value_raw;
            translate_y = (0.5f-scale_y/2.0f)*test_value_raw;
            test_all_value += 0.01;
        }
    }

}  // namespace movit
//
// Created by shiyang1 on 2018/3/28.
//

#include "scale_effect.h"
#include <epoxy/gl.h>
#include <assert.h>
#include <math.h>

#include "effect_util.h"
#include "util.h"

using namespace std;

namespace movit {
    
    ScaleEffect::ScaleEffect()
            : scale_x(1.0f), scale_y(1.0f),
              out_color_red(0.0), out_color_green(0.0), out_color_blue(1.0), out_color_alpha(0.8)
    {
        register_float("scale_x", &scale_x);
        register_float("scale_y", &scale_y);
        register_float("out_color_red", &out_color_red);
        register_float("out_color_green", &out_color_green);
        register_float("out_color_blue", &out_color_blue);
        register_float("out_color_alpha", &out_color_alpha);
     
        ra = 0.0;
    }

    string ScaleEffect::output_fragment_shader()
    {
        return read_file("scale_effect.frag");
    }

    void ScaleEffect::inform_input_size(unsigned input_num,
                                             unsigned width,
                                             unsigned height) {
        assert(input_num == 0);
//        widthStep = 1.0f/width;
//        heightStep = 1.0f/height;
    }

    void ScaleEffect::set_gl_state(GLuint glsl_program_num,
                                        const string &prefix,
                                        unsigned *sampler_num)
    {
        Effect::set_gl_state(glsl_program_num, prefix, sampler_num);
    
//        if (scale_x > 0.1 && scale_y > 0.1) {
//            scale_x = 2.0f*abs(cos(ra));
//            scale_y = 2.0f*abs(cos(ra));
//            ra += 0.01;
//        }

        
//        pixel = 8.0f*5.0f;
//        widthStep = 1.0f/1920.0f;
//        heightStep = 1.0f/1072.0f;
    }

}  // namespace movit
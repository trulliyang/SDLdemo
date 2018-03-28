//
// Created by shiyang1 on 2018/3/28.
//

#ifndef MYAPPLICATION_TRANSFORM_EFFECT_H
#define MYAPPLICATION_TRANSFORM_EFFECT_H



// generate a gray frame in several ways

#include <epoxy/gl.h>
#include <string>

#include "effect.h"

namespace movit {

    class TransformEffect : public Effect {
    public:
        TransformEffect();
        virtual std::string effect_type_id() const { return "TransformEffect"; }
        std::string output_fragment_shader();

        virtual bool needs_srgb_primaries() const { return false; }
        virtual AlphaHandling alpha_handling() const { return DONT_CARE_ALPHA_TYPE; }
        virtual bool one_to_one_sampling() const { return true; }

        virtual void inform_input_size(unsigned input_num, unsigned width, unsigned height);
        void set_gl_state(GLuint glsl_program_num, const std::string &prefix, unsigned *sampler_num);

    private:
//        float center_x;
//        float center_y;
        float scale_x;
        float scale_y;
        float rotate_degree;
        float translate_x;
        float translate_y;
        float out_color_red;
        float out_color_green;
        float out_color_blue;
        float out_color_alpha;
        float width;
        float height;
        
        float test_scale_xy;
        float test_rotate_radius;
        float test_translate_xy;
        float test_all_value;
    };

}  // namespace movit


#endif //MYAPPLICATION_TRANSFORM_EFFECT_H

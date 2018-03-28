//
// Created by shiyang1 on 2018/3/28.
//

#ifndef MYAPPLICATION_SCALE_EFFECT_H
#define MYAPPLICATION_SCALE_EFFECT_H



// generate a gray frame in several ways

#include <epoxy/gl.h>
#include <string>

#include "effect.h"

namespace movit {

    class ScaleEffect : public Effect {
    public:
        ScaleEffect();
        virtual std::string effect_type_id() const { return "ScaleEffect"; }
        std::string output_fragment_shader();

        virtual bool needs_srgb_primaries() const { return false; }
        virtual AlphaHandling alpha_handling() const { return DONT_CARE_ALPHA_TYPE; }
        virtual bool one_to_one_sampling() const { return true; }

        virtual void inform_input_size(unsigned input_num, unsigned width, unsigned height);
        void set_gl_state(GLuint glsl_program_num, const std::string &prefix, unsigned *sampler_num);

    private:
        float scale_x;
        float scale_y;
        float out_color_red;
        float out_color_green;
        float out_color_blue;
        float out_color_alpha;
        
        float ra;
    };

}  // namespace movit


#endif //MYAPPLICATION_SCALE_EFFECT_H

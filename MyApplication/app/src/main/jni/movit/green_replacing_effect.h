//
// Created by shiyang1 on 2018/2/26.
//

#ifndef MYAPPLICATION_GREEN_REPLACING_EFFECT_H
#define MYAPPLICATION_GREEN_REPLACING_EFFECT_H



// use new color to replace the target color in origin color

#include <epoxy/gl.h>
#include <string>

#include "effect.h"

namespace movit {

    class GreenReplacingEffect : public Effect {
    public:
        GreenReplacingEffect();
        virtual std::string effect_type_id() const { return "GreenReplacingEffect"; }
        std::string output_fragment_shader();

        virtual bool needs_srgb_primaries() const { return false; }
        virtual AlphaHandling alpha_handling() const { return DONT_CARE_ALPHA_TYPE; }
        virtual bool one_to_one_sampling() const { return true; }

        virtual void inform_input_size(unsigned input_num, unsigned width, unsigned height);
        void set_gl_state(GLuint glsl_program_num, const std::string &prefix, unsigned *sampler_num);

    private:
        float greenSaturationMin0;
        float greenSaturationMax0;
        float greenValueMin0;
        float greenValueMax0;
        float greenHueMin0;
        float greenHueMax0;
    
        float greenSaturationMin1;
        float greenSaturationMax1;
        float greenValueMin1;
        float greenValueMax1;
        float greenHueMin1;
        float greenHueMax1;
        
        float threshold;
    };

}  // namespace movit


#endif //MYAPPLICATION_GREEN_REPLACING_EFFECT_H

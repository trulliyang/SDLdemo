//
// Created by shiyang1 on 2018/2/11.
//

#ifndef MYAPPLICATION_CHROMA_KEY_EFFECT_H
#define MYAPPLICATION_CHROMA_KEY_EFFECT_H



// use new color to replace the target color in origin color

#include <epoxy/gl.h>
#include <string>

#include "effect.h"

namespace movit {

    class ChromaKeyEffect : public Effect {
    public:
        ChromaKeyEffect();
        virtual std::string effect_type_id() const { return "ChromaKeyEffect"; }
        std::string output_fragment_shader();

        virtual bool needs_srgb_primaries() const { return false; }
        virtual AlphaHandling alpha_handling() const { return DONT_CARE_ALPHA_TYPE; }
        virtual bool one_to_one_sampling() const { return true; }

        virtual void inform_input_size(unsigned input_num, unsigned width, unsigned height);
        void set_gl_state(GLuint glsl_program_num, const std::string &prefix, unsigned *sampler_num);

    private:
        float targetColor_r;
        float targetColor_g;
        float targetColor_b;
        float thresholdSensitivity;
        float smoothing;
    };

}  // namespace movit


#endif //MYAPPLICATION_CHROMA_KEY_EFFECT_H

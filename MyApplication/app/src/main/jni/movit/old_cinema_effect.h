//
// Created by shiyang1 on 2018/2/6.
//

#ifndef MYAPPLICATION_OLD_CINEMA_EFFECT_H
#define MYAPPLICATION_OLD_CINEMA_EFFECT_H



// generate a old cinema style frame

#include <epoxy/gl.h>
#include <string>

#include "effect.h"

namespace movit {

    class OldCinemaEffect : public Effect {
    public:
        OldCinemaEffect();
        virtual std::string effect_type_id() const { return "OldCinemaEffect"; }
        std::string output_fragment_shader();

        virtual bool needs_srgb_primaries() const { return false; }
        virtual AlphaHandling alpha_handling() const { return DONT_CARE_ALPHA_TYPE; }
        virtual bool one_to_one_sampling() const { return true; }

        virtual void inform_input_size(unsigned input_num, unsigned width, unsigned height);
        void set_gl_state(GLuint glsl_program_num, const std::string &prefix, unsigned *sampler_num);

    private:
        float SepiaValue;
        float NoiseValue;
        float ScratchValue;
        float InnerVignetting;
        float OuterVignetting;
        float RandomValue;
        float TimeLapse;
        float VigCount;
        bool needIncrease;
    };

}  // namespace movit


#endif //MYAPPLICATION_OLD_CINEMA_EFFECT_H

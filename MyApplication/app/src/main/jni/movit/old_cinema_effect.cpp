//
// Created by shiyang1 on 2018/1/29.
//

#include "old_cinema_effect.h"
#include <epoxy/gl.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "effect_util.h"
#include "util.h"

using namespace std;

namespace movit {

    OldCinemaEffect::OldCinemaEffect()
            : SepiaValue(0.5f), NoiseValue(0.5f), ScratchValue(0.5f)
            , InnerVignetting(0.5f), OuterVignetting(0.9f), RandomValue(1.0f), TimeLapse(2.0f)
    {
        register_float("SepiaValue", &SepiaValue);
        register_float("NoiseValue", &NoiseValue);
        register_float("ScratchValue", &ScratchValue);
        register_float("InnerVignetting", &InnerVignetting);
        register_float("OuterVignetting", &OuterVignetting);
        register_float("RandomValue", &RandomValue);
        register_float("TimeLapse", &TimeLapse);
    
        VigCount = 0;
        needIncrease = true;
    }

    string OldCinemaEffect::output_fragment_shader()
    {
        return read_file("old_cinema_effect.frag");
    }

    void OldCinemaEffect::inform_input_size(unsigned input_num, unsigned width, unsigned height) {
        assert(input_num == 0);
    }

    void OldCinemaEffect::set_gl_state(GLuint glsl_program_num,
                                       const string &prefix,
                                       unsigned *sampler_num)
    {
        Effect::set_gl_state(glsl_program_num, prefix, sampler_num);
        SepiaValue = 0.12f;
        NoiseValue = 0.2f;
        ScratchValue = 0.5f;
        
        int up = 40;
        if (needIncrease) {
            if (VigCount < up) {
                VigCount++;
            } else {
                needIncrease = false;
            }
        }
        if (!needIncrease) {
            if (VigCount > 0) {
                VigCount--;
            } else {
                needIncrease = true;
            }
        }
        
        float vig = VigCount*0.05f/(float)up;
        
//        float vig = rand()%up*0.05f/(float)up;
        InnerVignetting = 0.775f + vig;
        OuterVignetting = 0.95f + vig;
        
        RandomValue = rand()%100*0.01f;
        TimeLapse = 1000.0f*(rand()%200*0.01f);
    }

}  // namespace movit
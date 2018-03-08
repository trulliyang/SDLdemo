//
// Created by shiyang1 on 2018/2/13.
//

#ifndef MYAPPLICATION_ALPHA_MATTING_EFFECT_H
#define MYAPPLICATION_ALPHA_MATTING_EFFECT_H



// use new color to replace the target color in origin color

#include <epoxy/gl.h>
#include <string>

#include "effect.h"

namespace movit {

    class AlphaMattingEffect : public Effect {
    public:
        AlphaMattingEffect();
        virtual std::string effect_type_id() const { return "AlphaMattingEffect"; }
        std::string output_fragment_shader();

        virtual bool needs_srgb_primaries() const { return false; }
        virtual AlphaHandling alpha_handling() const { return DONT_CARE_ALPHA_TYPE; }
        virtual bool one_to_one_sampling() const { return true; }

        virtual void inform_input_size(unsigned input_num, unsigned width, unsigned height);
        void set_gl_state(GLuint glsl_program_num, const std::string &prefix, unsigned *sampler_num);

    private:
        float blackValueMin;
        float blackValueMax;
    
        float graySaturationMin;
        float graySaturationMax;
        float grayValueMin;
        float grayValueMax;
    
        float whiteSaturationMin;
        float whiteSaturationMax;
        float whiteValueMin;
        float whiteValueMax;
    
        float redHueMin;
        float redHueMax;
    
        float orangeHueMin;
        float orangeHueMax;
    
        float yellowHueMin;
        float yellowHueMax;
    
        float greenHueMin;
        float greenHueMax;
    
        float cyanHueMin;
        float cyanHueMax;
    
        float blueHueMin;
        float blueHueMax;
    
        float purpleHueMin;
        float purpleHueMax;
    
        float pinkHueMin;
        float pinkHueMax;
    
        float targetColorRed;
        float targetColorGreen;
        float targetColorBlue;
        float targetColorAlpha;
    
        float diff;
        float diffMin;
        float diffMax;
    };

}  // namespace movit


#endif //MYAPPLICATION_ALPHA_MATTING_EFFECT_H

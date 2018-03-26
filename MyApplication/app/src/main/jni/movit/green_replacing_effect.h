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
        void read_color(int x=0, int y=0, int w=1, int h=1, GLenum format=GL_RGBA, GLenum type=GL_UNSIGNED_BYTE, void *pixels= NULL);
        void set_bar(float value0, float value1);
    private:
        float targetColorRed;
        float targetColorGreen;
        float targetColorBlue;
        float targetColorAlpha;
    
        float perTopWidth;
        float perTopHeight;
    
        float perBottomWidth;
        float perBottomHeight;
    
        float diff;
        float diffMin;
        float diffMax;
        float threshold;
    
        float threshold0;
        float threshold1;
    };

}  // namespace movit


#endif //MYAPPLICATION_GREEN_REPLACING_EFFECT_H

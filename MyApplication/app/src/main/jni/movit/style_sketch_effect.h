//
// Created by shiyang1 on 2018/2/7.
//

#ifndef MYAPPLICATION_STYLE_SKETCH_EFFECT_H
#define MYAPPLICATION_STYLE_SKETCH_EFFECT_H



// generate a sketch style frame in several ways

#include <epoxy/gl.h>
#include <string>

#include "effect.h"

namespace movit {

    class StyleSketchEffect : public Effect {
    public:
        StyleSketchEffect();
        virtual std::string effect_type_id() const { return "StyleSketchEffect"; }
        std::string output_fragment_shader();

        virtual bool needs_srgb_primaries() const { return false; }
        virtual AlphaHandling alpha_handling() const { return DONT_CARE_ALPHA_TYPE; }
        virtual bool one_to_one_sampling() const { return true; }

        virtual void inform_input_size(unsigned input_num, unsigned width, unsigned height);
        void set_gl_state(GLuint glsl_program_num, const std::string &prefix, unsigned *sampler_num);

    private:
        float edgeStrength;
        float widthStep;
        float heightStep;
    };

}  // namespace movit


#endif //MYAPPLICATION_STYLE_SKETCH_EFFECT_H

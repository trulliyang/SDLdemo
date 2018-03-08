//
// Created by shiyang1 on 2018/2/12.
//

#include "alpha_matting_effect.h"
#include <epoxy/gl.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "effect_util.h"
#include "util.h"

using namespace std;

namespace movit {
    
    AlphaMattingEffect::AlphaMattingEffect()
    {
        blackValueMin = 0.0f;
        blackValueMax = 0.25f;
    
        graySaturationMin = 0.0f;
        graySaturationMax = 0.3f;
        grayValueMin = 0.25f;
        grayValueMax = 0.8f;
    
        whiteSaturationMin = 0.0f;
        whiteSaturationMax = 0.3f;
        whiteValueMin = 0.8f;
        whiteValueMax = 1.0f;
    
        redHueMin = 337.5f;
        redHueMax = 22.5f;
    
        orangeHueMin = 22.5f;
        orangeHueMax = 45.0f;
    
        yellowHueMin = 45.0f;
        yellowHueMax = 90.0f;
    
        greenHueMin = 90.0f;
        greenHueMax = 205.5f;
    
        cyanHueMin = 205.5f;
        cyanHueMax = 228.0f;
    
        blueHueMin = 228.0f;
        blueHueMax = 292.5f;
    
        purpleHueMin = 292.5f;
        purpleHueMax = 337.5f;
    
        pinkHueMin = 306.0f;
        pinkHueMax = 324.0f;
    
        targetColorRed = 0.0f;
        targetColorGreen = 1.0f;
        targetColorBlue = 0.0f;
        targetColorAlpha = 1.0f;
    
        diff = 0.1f;
        diffMin = 0.1f;
        diffMax = 0.9f;
        
        register_float("blackValueMin", &blackValueMin);
        register_float("blackValueMax", &blackValueMax);
    
        register_float("graySaturationMin", &graySaturationMin);
        register_float("graySaturationMax", &graySaturationMax);
        register_float("grayValueMin", &grayValueMin);
        register_float("grayValueMax", &grayValueMax);
    
        register_float("whiteSaturationMin", &whiteSaturationMin);
        register_float("whiteSaturationMax", &whiteSaturationMax);
        register_float("whiteValueMin", &whiteValueMin);
        register_float("whiteValueMax", &whiteValueMax);
    
        register_float("redHueMin", &redHueMin);
        register_float("redHueMax", &redHueMax);
    
        register_float("orangeHueMin", &orangeHueMin);
        register_float("orangeHueMax", &orangeHueMax);
    
        register_float("yellowHueMin", &yellowHueMin);
        register_float("yellowHueMax", &yellowHueMax);
    
        register_float("greenHueMin", &greenHueMin);
        register_float("greenHueMax", &greenHueMax);
    
        register_float("cyanHueMin", &cyanHueMin);
        register_float("cyanHueMax", &cyanHueMax);
    
        register_float("blueHueMin", &blueHueMin);
        register_float("blueHueMax", &blueHueMax);
    
        register_float("purpleHueMin", &purpleHueMin);
        register_float("purpleHueMax", &purpleHueMax);
    
        register_float("pinkHueMin", &pinkHueMin);
        register_float("pinkHueMax", &pinkHueMax);
    
        register_float("targetColorRed", &targetColorRed);
        register_float("targetColorGreen", &targetColorGreen);
        register_float("targetColorBlue", &targetColorBlue);
        register_float("targetColorAlpha", &targetColorAlpha);
    
        register_float("diff", &diff);
        register_float("diffMin", &diffMin);
        register_float("diffMax", &diffMax);
    }

    string AlphaMattingEffect::output_fragment_shader()
    {
//        return read_file("alpha_matting_effect.frag");
        return read_file("alpha_matting_b_effect.frag");
    }

    void AlphaMattingEffect::inform_input_size(unsigned input_num,
                                               unsigned width,
                                               unsigned height) {
        assert(input_num == 0);
    }

    void AlphaMattingEffect::set_gl_state(GLuint glsl_program_num,
                                          const string &prefix,
                                          unsigned *sampler_num)
    {
        Effect::set_gl_state(glsl_program_num, prefix, sampler_num);
        
        blackValueMin = 0.0f;
        blackValueMax = 0.25f;
    
        graySaturationMin = 0.0f;
        graySaturationMax = 0.3f;
        grayValueMin = 0.25f;
        grayValueMax = 0.8f;
    
        whiteSaturationMin = 0.0f;
        whiteSaturationMax = 0.3f;
        whiteValueMin = 0.8f;
        whiteValueMax = 1.0f;
    
        redHueMin = 337.5f;
        redHueMax = 22.5f;
    
        orangeHueMin = 22.5f;
        orangeHueMax = 45.0f;
    
        yellowHueMin = 45.0f;
        yellowHueMax = 90.0f;
    
        greenHueMin = 90.0f;
        greenHueMax = 180.5f;
    
        cyanHueMin = 180.5f;
        cyanHueMax = 228.0f;
    
        blueHueMin = 228.0f;
        blueHueMax = 292.5f;
    
        purpleHueMin = 292.5f;
        purpleHueMax = 337.5f;
    
        pinkHueMin = 306.0f;
        pinkHueMax = 324.0f;
    
//        targetColorRed = 0.0f;
//        targetColorGreen = 1.0f;
//        targetColorBlue = 0.0f;
//        targetColorAlpha = 1.0f;
    
        // origin color
//        targetColorRed = 79.0f/255.0f;
//        targetColorGreen = 249.0f/255.0f;
//        targetColorBlue = 42.0f/255.0f;
//        targetColorAlpha = 1.0f;
    
        // normal rgb
        targetColorRed = 60.0f/255.0f;
        targetColorGreen = 255.0f/255.0f;
        targetColorBlue = 0.0f/255.0f;
        targetColorAlpha = 1.0f;
        
        diff = 0.1f;
        diffMin = 0.1f;
        diffMax = 0.9f;
    }

}  // namespace movit
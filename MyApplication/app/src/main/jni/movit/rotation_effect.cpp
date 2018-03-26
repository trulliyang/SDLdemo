#include <epoxy/gl.h>
#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <Eigen/Core>
#include <Eigen/LU>

#include "rotation_effect.h"
#include "effect_util.h"
#include "util.h"

using namespace std;
using namespace Eigen;

namespace movit {

RotationEffect::RotationEffect()
{
    rotate_degree = 0.0;
    register_uniform_float("rotate_degree", &rotate_degree);
    
    width = 1920.0;
    register_uniform_float("width", &width);
    
    height = 1070.0;
    register_uniform_float("height", &height);
    
    outColorRed = 0.0;
    register_uniform_float("outColorRed", &outColorRed);
    
    outColorGreen = 0.0;
    register_uniform_float("outColorGreen", &outColorGreen);
    
    outColorBlue = 0.0;
    register_uniform_float("outColorBlue", &outColorBlue);
    
    outColorAlpha = 1.0;
    register_uniform_float("outColorAlpha", &outColorAlpha);
}

string RotationEffect::output_fragment_shader()
{
	return read_file("rotation_effect.frag");
}

void RotationEffect::set_gl_state(GLuint glsl_program_num,
								  const string &prefix,
								  unsigned *sampler_num)
{
	Effect::set_gl_state(glsl_program_num, prefix, sampler_num);
//    rotate_degree += 10;
}

void RotationEffect::set_size(int _width, int _height) {
    width = _width;
    height = _height;
}

void RotationEffect::set_rotate_degree(float _degree) {
    rotate_degree = _degree;
}

void RotationEffect::set_out_color(float _r, float _g, float _b, float _a) {
    outColorRed = _r;
    outColorGreen = _g;
    outColorBlue = _b;
    outColorAlpha = _a;
}

}  // namespace movit

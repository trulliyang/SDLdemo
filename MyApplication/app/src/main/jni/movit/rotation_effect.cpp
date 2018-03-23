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
	register_uniform_mat3("rotation_matrix", &rotation_matrix);
    register_uniform_int("test", &test);
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
 
	Matrix3d rotation;
    test = 3;
    rotate_degree = 0.0;
    rotate_x = 0.0;
    rotate_y = 0.0;
    
    float rotate_radius = (float) (rotate_degree * 3.1415926535897932384626 / 180.0);
    
    float c = cos(rotate_radius);
    float s = sin(rotate_radius);
    
//    rotation_matrix(0,s.0;
    
    
    rotation(0,0) = 0.0;
    rotation(0,1) = 0.0;
    rotation(0,2) = 0.0;

    rotation(1,0) = 0.0;
    rotation(1,1) = 0.0;
    rotation(1,2) = 0.0;

    rotation(2,0) = 0.0;
    rotation(2,1) = 0.0;
    rotation(2,2) = 1.0;
    
	rotation_matrix = rotation.inverse();
}

}  // namespace movit

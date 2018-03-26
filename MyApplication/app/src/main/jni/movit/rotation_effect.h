#ifndef _MOVIT_ROTATION_EFFECT_H
#define _MOVIT_ROTATION_EFFECT_H 1

#include <epoxy/gl.h>
#include <Eigen/Core>
#include <string>

#include "effect.h"

namespace movit {

class RotationEffect : public Effect {
public:
	RotationEffect();
	virtual std::string effect_type_id() const { return "RotationEffect"; }
	std::string output_fragment_shader();
	void set_gl_state(GLuint glsl_program_num, const std::string &prefix, unsigned *sampler_num);
	void set_size(int _width, int _height);
	void set_rotate_degree(float _degree);
	void set_out_color(float _r, float _g, float _b, float _a);
private:

	Eigen::Matrix3d rotation_matrix;
//    float rotate_x;
//    float rotate_y;
    float rotate_degree;
	float width;
	float height;
	
	float outColorRed;
	float outColorGreen;
	float outColorBlue;
	float outColorAlpha;
	
	int test;
};

}  // namespace movit

#endif // !defined(_MOVIT_ROTATION_EFFECT_H)

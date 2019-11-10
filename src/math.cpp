#include "math.hpp"


using namespace glm;

vec2 operator*(const mat3& m, const vec2& v) {
	assert(m[2][2] == 1);
	vec3 temp(v, 1);
	return xy(temp*m);
}

vec2 operator*(const vec2& v, const Transform& transform) {
	mat3 trnsfrm = mat3(1);
	trnsfrm = translate(trnsfrm, transform.position);
	trnsfrm = rotate(trnsfrm, transform.rotation);
	trnsfrm = scale(trnsfrm, transform.scale);
	return trnsfrm * v;
}

float cross(vec2 a, vec2 b) {
	return  a.x*b.y - a.y*b.x;
}

// 2 times the signed area of the triangle abc, + if triangle is CCW and - if triangle is CW
// 0 if points collinear
float signed2DTriangleArea(vec2 a, vec2 b, vec2 c) {
	return cross(a-c, b-c);
}



#include "math.hpp"


using namespace glm;

vec2 operator*(const mat3& m, const vec2& v) {
	assert(m[2][2] == 1);
	vec3 temp(v, 1);
	return xy(temp*m);
}

vec2 operator*(const vec2& v, const Transform& transform) {
	auto m = transformTransformation(transform);
	return m * v;
}

glm::mat3 transformTranslation(const Transform& t) {
	return {
		1, 0, t.position.x,
		0, 1, t.position.y,
		0, 0, 1
	};
}

glm::mat3 transformRotation(const Transform& t) {
	return {
		std::cosf(t.rotation),	-std::sinf(t.rotation),	0,
		std::sinf(t.rotation),	std::cosf(t.rotation),	0,
							0,						0,	1
	};
}

glm::mat3 transformScale(const Transform& t) {
	return {
		t.scale.x, 			0,	0,
				0,	t.scale.y,	1,
				0,			0, 	1
	};
}

glm::mat3 transformTransformation(const Transform& t) {
	return transformTranslation(t) * transformRotation(t) * transformScale(t);
}

float cross(vec2 a, vec2 b) {
	return  a.x*b.y - a.y*b.x;
}

// 2 times the signed area of the triangle abc, + if triangle is CCW and - if triangle is CW
// 0 if points collinear
float signed2DTriangleArea(vec2 a, vec2 b, vec2 c) {
	return cross(a-c, b-c);
}



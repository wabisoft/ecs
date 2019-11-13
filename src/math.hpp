#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include <glm/ext/matrix_common.hpp>
#include <glm/geometric.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtx/vec_swizzle.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/ext/matrix_common.hpp>
#include <glm/ext/matrix_relational.hpp>

#include "settings.hpp"

struct Transform {
	glm::vec2 position = glm::vec2(0);
	f32 rotation = 0.f;
	glm::vec2 scale = glm::vec2(1);
};

glm::vec2 operator*(const glm::mat3& m, const glm::vec2& v);
glm::vec2 operator*(const glm::vec2&, const Transform&);


float cross(glm::vec2 a, glm::vec2 b);
// 2 times the signed area of the triangle abc, + if triangle is CCW and - if triangle is CW
// 0 if points collinear
float signed2DTriangleArea(glm::vec2 a, glm::vec2 b, glm::vec2 c);

constexpr f32 radToDeg = 180/PI;
constexpr f32 degToRad = PI/180;

inline f32 radiansToDegrees(f32 rad) { return rad * radToDeg; }
inline f32 degreesToRadians(f32 deg) { return deg * degToRad; }



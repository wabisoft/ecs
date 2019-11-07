#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include <glm/ext/matrix_common.hpp>
#include <glm/geometric.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/swizzle.hpp>

struct Transform {
	glm::vec2 position = glm::vec2(0);
	f32 rotation = 0.f;
	glm::vec2 scale = glm::vec2(0);
};

glm::vec2 operator*(const glm::mat3& m, const glm::vec2& v);
glm::vec2 operator*(const glm::vec2&, const Transform&);


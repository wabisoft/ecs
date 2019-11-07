#include "math.hpp"


glm::vec2 operator*(const glm::mat3& m, const glm::vec2& v) {
	assert(m[2][2] == 1);
	glm::vec3 temp(v, 1);
	return (temp*m).xy;
}

glm::vec2 operator*(const glm::vec2& v, const Transform& transform) {
	glm::mat3 trnsfrm = glm::mat3(1);
	trnsfrm = glm::translate(trnsfrm, transform.position);
	trnsfrm = glm::rotate(trnsfrm, transform.rotation);
	trnsfrm = glm::scale(trnsfrm, transform.scale);
	return trnsfrm * v;
}

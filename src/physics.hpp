#pragma once

#include "component.hpp"
#include "math.hpp"

struct Body : Component {
	glm::vec2 velocity(0);
	glm::vec2 acceleration(0);
};


# pragma once

#include "component.hpp"
#include "geometry.hpp"

struct AABB {
	glm::vec2 lower = glm::vec2(0);
	glm::vec2 upper = glm::vec2(0);
};

struct Scene;
struct Entity;

struct Collider : Component {
	wabi::Polygon shape;
	AABB aabb;
	u8 entity_id = -1;
	Scene* scene = nullptr;
};


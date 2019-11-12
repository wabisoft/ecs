#pragma once

#include "component.hpp"
#include "datastructures.hpp"
#include "math.hpp"

struct Scene;
struct Entity;

struct Body : Component {
	friend struct Scene;
	friend struct slot_set<Body, MAX_ENTITIES>;

	void update(Transform&) { }

	glm::vec2 velocity = glm::vec2(0);
	glm::vec2 acceleration = glm::vec2(0);
	static const Component::Kind kind = Component::Body;
private:
	Body() {}
	Body(u8 id) : Component(id) {}
};


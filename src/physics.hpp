#pragma once

#include "component.hpp"
#include "datastructures.hpp"
#include "math.hpp"

struct Scene;
struct Entity;

struct RigidbodyDef {
};

struct Rigidbody : Component {
	friend struct Scene;
	friend struct slot_set<Rigidbody, MAX_ENTITIES>;

	void update(Transform&) { }

	glm::vec2 velocity = glm::vec2(0);
	glm::vec2 acceleration = glm::vec2(0);
	Scene* scene_ptr;

	static const Component::Kind kind = Component::e_Rigidbody;
private:
	Rigidbody() {}
	Rigidbody(u8 id) : Component(id) {}
	Rigidbody(u8 id, Scene* s, RigidbodyDef definition) : Component(id), scene_ptr(s) {
		(void)definition;
	}
};


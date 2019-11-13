# pragma once

#include "component.hpp"
#include "geometry.hpp"
#include "settings.hpp"

struct AABB {
	glm::vec2 lower = glm::vec2(0);
	glm::vec2 upper = glm::vec2(0);
};

struct Scene;
struct Entity;

struct ColliderDef {
};

struct Collider : Component {
	friend struct Scene;
	friend struct slot_set<Collider, MAX_ENTITIES>;

	void update(Transform&) { }
	wabi::Polygon shape;
	AABB aabb;
	Scene* scene_ptr = nullptr;

	static const Component::Kind kind = Component::Collider;

private:
	Collider() { }
	Collider(u8 id) : Component(id) {}
	Collider(u8 id, Scene* s, ColliderDef definition) : Component(id), scene_ptr(s) {
		(void)definition;
	}
};


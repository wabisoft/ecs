#pragma once

#include "math.hpp"
#include "component.hpp"
#include "settings.hpp"

struct Scene;

struct Entity {
	friend struct slot_set<Entity, MAX_ENTITIES>;
	Entity() { }
	Entity(Scene* s) : scene(s) { }

	Component* getComponent(Component::Kind componentKind);

	bool components[Component::Count];
	Transform transform;
	u8 entity_id;
	Scene * scene = nullptr;
};



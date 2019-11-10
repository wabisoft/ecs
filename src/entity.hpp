#pragma once

#include "math.hpp"
#include "component.hpp"
#include "settings.hpp"

struct Scene;

struct Entity {
	friend struct Scene;
	friend struct slot_set<Entity, MAX_ENTITIES>;


	template <typename T>
	T& addComponent(); // defined in scene.hpp
	template <typename T>
	T& getComponent(); // definied in scene.hpp

	bool components[Component::Count];
	Transform transform;
	Scene * scene_ptr = nullptr;
	u8 entity_id;

private:
	Entity() { }
	Entity(u8 id, Scene* p_scene);

};


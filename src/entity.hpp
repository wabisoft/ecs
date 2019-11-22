#pragma once

#include "math.hpp"
#include "datastructures.hpp"
#include "component.hpp"
#include "settings.hpp"

struct Scene;

struct ComponentTable{
	void set(Component::Kind k) {
		mask |= 1u << (unsigned)k;
	}
	void unset(Component::Kind k) {
		mask &= ~(1u << (unsigned)k);
	}

	bool isset(Component::Kind k) const {
		return (mask >> (unsigned)k) & 1u;
	}

	std::vector<Component::Kind> allset() const {
		std::vector<Component::Kind> res;
		res.reserve(Component::e_Count);
		for (unsigned i = 0; i < Component::e_Count; ++i) {
			if(isset((Component::Kind)i)) {
				res.push_back((Component::Kind)i);
			}
		}
		return res;
	}

	u16 getMask() const { return mask; }

private:
	u16 mask = 0;
};

struct Entity {
	friend struct Scene;
	friend struct slot_set<Entity, MAX_ENTITIES>;

	template <typename T, typename Def>
	T& addComponent(Def definition); // defined in scene.hpp
	template <typename T>
	T& addComponent(); // defined in scene.hpp
	template <typename T>
	void removeComponent(); // definied in scene.hpp
	template <typename T>
	bool hasComponent(); // definied in scene.hpp
	template <typename T>
	T& getComponent(); // definied in scene.hpp
	template <typename T>
	T* getComponentOrNull(); // definied in scene.hpp

	Transform transform;
	ComponentTable componentTable;
	Scene * scene_ptr = nullptr;
	u8 entity_id;

private:
	Entity() { }
	Entity(u8 id, Scene* p_scene) : scene_ptr(p_scene), entity_id(id) { }

};


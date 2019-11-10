#pragma once

#include "collision.hpp"
#include "component.hpp"
#include "datastructures.hpp"
#include "entity.hpp"
#include "render.hpp"
#include "settings.hpp"


struct Scene {
	Scene() { }

	Component* getComponent(Component::Kind componentKind, u8 entity_id) {
		switch(componentKind) {
			case Component::Render:
				return &renderables[entity_id]; break;
			case Component::Collider:
				return &colliders[entity_id]; break;
			default: return nullptr; break;
		}
	}

	template <typename T>
	static size_t componentIdx(const T& t) { return (size_t)t.entity_id; }  // used to get entity_id out of both entities and components

	slot_set<Entity, MAX_ENTITIES> entities = slot_set<Entity, MAX_ENTITIES>(componentIdx);
	slot_set<Renderable, MAX_ENTITIES> renderables = slot_set<Renderable, MAX_ENTITIES>(componentIdx);
	slot_set<Collider, MAX_ENTITIES> colliders = slot_set<Collider, MAX_ENTITIES>(componentIdx);

	void update();
};

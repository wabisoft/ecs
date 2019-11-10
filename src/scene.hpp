#pragma once

#include "collision.hpp"
#include "component.hpp"
#include "datastructures.hpp"
#include "entity.hpp"
#include "render.hpp"
#include "settings.hpp"


struct Scene {
	Scene() { }

	Component* addCompnent(Component::Kind componentKind, u8 entity_id);
	Component* getComponent(Component::Kind componentKind, u8 entity_id);
	Entity& createEntity();

	template <typename T>
	static size_t componentIdx(const T& t) { return (size_t)t.entity_id; }  // used to get entity_id out of both entities and components
	void update();

private:
	slot_set<Entity, MAX_ENTITIES> entities_ = slot_set<Entity, MAX_ENTITIES>(componentIdx);
	slot_set<Renderable, MAX_ENTITIES> renderables_ = slot_set<Renderable, MAX_ENTITIES>(componentIdx);
	slot_set<Collider, MAX_ENTITIES> colliders_ = slot_set<Collider, MAX_ENTITIES>(componentIdx);
	u8 entity_id_seed_ = 0;
};

// Some of Entity's templates need Scene to be defined
// so we define those templates here after the definition of Scene
template <typename T>
T& Entity::addComponent() {
	Component* component = scene_ptr->addCompnent(T::kind, entity_id);
	// T* t = dynamic_cast<T*>(component);  // TODO: replace with c-style (T*) cast for performance
	// if(t) { return *t; }
	// assert(false)
	T* t = (T*)component;
	return *t;
}

template <typename T>
T& Entity::getComponent() {
	Component * component = scene_ptr->getComponent(T::kind, entity_id);
	T* t = dynamic_cast<T*>(component);  // TODO: replace with c-style (T*) cast for performance
	if(t) { return *t; }
	assert(false);
}

#pragma once

#include "collision.hpp"
#include "component.hpp"
#include "physics.hpp"
#include "render.hpp"
#include "datastructures.hpp"
#include "entity.hpp"
#include "settings.hpp"


struct Scene {
	friend struct Entity;

	Scene();
	~Scene();

	template <typename T>
	slot_set<T, MAX_ENTITIES>& getComponentSet();

	Entity& createEntity();
	Entity& getEntity(u8 entity_id);

	void init(sf::RenderWindow*); // init MUST becalled before first frame or undefined behavior
	void frame(f32 deltaTime);
	void step(f32 deltaTime=FIXED_TIMESTEP);

	CollisionSystem collisionSystem;
	RenderSystem renderSystem;
private:
	slot_set<Entity, MAX_ENTITIES>* entities_;
	void * components_[Component::e_Count];
	u8 entity_id_seed_ = 0;

	template <typename T>
	static size_t componentIdx(const T& t) { return (size_t)t.entity_id; }  // used to get entity_id out of both entities and components
};

template <typename T>
slot_set<T, MAX_ENTITIES>& Scene::getComponentSet() {
	return *(slot_set<T, MAX_ENTITIES>*)components_[T::kind];
}

// Some of Entity's templates need Scene to be defined
// so we define those templates here after the definition of Scene
template <typename T, typename Def>
T& Entity::addComponent(Def definition) {
	assert(!componentTable.isset(T::kind));
	slot_set<T, MAX_ENTITIES>& set = scene_ptr->getComponentSet<T>();
	T& component = set.add(T(entity_id, scene_ptr, definition));
	component.init();
	componentTable.set(T::kind);
	return component;
}

template <typename T>
T& Entity::addComponent() {
	assert(!componentTable.isset(T::kind));
	slot_set<T, MAX_ENTITIES>& set = scene_ptr->getComponentSet<T>();
	T& component = set.add(T(entity_id, scene_ptr));
	component.init();
	componentTable.set(T::kind);
	return component;
}

template <typename T>
void Entity::removeComponent() {
	assert(componentTable.isset(T::kind));
	auto res = scene_ptr->getComponentSet<T>().rem(entity_id);
	componentTable.unset(T::kind);
	return res;
}

template <typename T>
bool Entity::hasComponent() {
	return scene_ptr->getComponentSet<T>().has(entity_id) && componentTable.isset(T::Kind); // reverse me for a more efficient short circuit
}

template <typename T>
T& Entity::getComponent() {
	return scene_ptr->getComponentSet<T>()[entity_id];
}

template <typename T>
T* Entity::getComponentOrNull() {
	if(!componentTable.isset(T::kind)) { return nullptr;}
	else { return getComponent<T>(); }
}


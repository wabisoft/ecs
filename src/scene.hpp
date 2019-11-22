#pragma once

#include "collision.hpp"
#include "component.hpp"
#include "physics.hpp"
#include "render.hpp"
#include "datastructures.hpp"
#include "entity.hpp"
#include "settings.hpp"


struct Scene {
	Scene();
	~Scene();

	template <typename T>
	slot_set<T, MAX_ENTITIES>& getComponentSet();

	Entity& createEntity();
	Entity& getEntity(u8 entity_id);

	void init(sf::RenderWindow*); // init MUST becalled before first frame or undefined behavior
	void frame(f32 deltaTime);
	void step(f32 deltaTime=FIXED_TIMESTEP);

// private:
	// these guys below are pointers because we want to be able to touch the
	// scene through a pointer without blowing the cache (maybe)
	slot_set<Entity, MAX_ENTITIES>* entities_;
	RenderSystem renderSystem;
	CollisionSystem collisionSystem;
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
	slot_set<T, MAX_ENTITIES>& set = scene_ptr->getComponentSet<T>();
	T& component = set.add(T(entity_id, scene_ptr, definition));
	component.init();
	return component;
	// return scene_ptr->addComponent<T, Def>(entity_id, definition);
}

template <typename T>
T& Entity::addComponent() {
	slot_set<T, MAX_ENTITIES>& set = scene_ptr->getComponentSet<T>();
	T& component = set.add(T(entity_id, scene_ptr));
	component.init();
	return component;
	// return scene_ptr->addComponent<T>(entity_id);
}

template <typename T>
void Entity::removeComponent() {
	return scene_ptr->getComponentSet<T>().rem(entity_id);
	// return scene_ptr->removeComponent<T>(entity_id);
}

template <typename T>
bool Entity::hasComponent() {
	return scene_ptr->getComponentSet<T>().has(entity_id);
}

template <typename T>
T& Entity::getComponent() {
	return scene_ptr->getComponentSet<T>()[entity_id];
}


#pragma once

#include "collision.hpp"
#include "component.hpp"
#include "datastructures.hpp"
#include "entity.hpp"
#include "physics.hpp"
#include "render.hpp"
#include "settings.hpp"


struct Scene {
	Scene();
	~Scene();
	template <typename T>
	slot_set<T, MAX_ENTITIES>& getComponentSet();
	template <typename T>
	T& addComponent(u8 entity_id);
	template <typename T>
	void removeComponent(u8 entity_id);
	template <typename T>
	T& getComponent(u8 entity_id);
	Entity& createEntity();
	void update(f32 deltaTime);

private:
	// these guys below are pointers because we want to be able to touch the
	// scene through a pointer without blowing the cache (maybe)
	slot_set<Entity, MAX_ENTITIES>* entities_;
	void * components_[Component::Count];
	u8 entity_id_seed_ = 0;

	template <typename T>
	static size_t componentIdx(const T& t) { return (size_t)t.entity_id; }  // used to get entity_id out of both entities and components

};

template <typename T>
slot_set<T, MAX_ENTITIES>& Scene::getComponentSet() {
	return *(slot_set<T, MAX_ENTITIES>*)components_[T::kind];
}

template <typename T>
T& Scene::addComponent(u8 entity_id) {
	// auto set = (slot_set<T, MAX_ENTITIES>*)components_[T::kind];
	//return set->add(T(entity_id));
	return getComponentSet<T>().add(T(entity_id));
}

template <typename T>
void Scene::removeComponent(u8 entity_id) {
	// auto set = (slot_set<T, MAX_ENTITIES>*)components_[T::kind];
	// return set->rem(entity_id);
	return getComponentSet<T>().rem(entity_id);
}

template <typename T>
T& Scene::getComponent(u8 entity_id) {
	// auto set = (slot_set<T, MAX_ENTITIES>*)components_[T::kind];
	// return (*set)[entity_id];
	return getComponentSet<T>()[entity_id];
}

// Some of Entity's templates need Scene to be defined
// so we define those templates here after the definition of Scene
template <typename T>
T& Entity::addComponent() {
	return scene_ptr->addComponent<T>(entity_id);
}

template <typename T>
void Entity::removeComponent() {
	return scene_ptr->removeComponent<T>(entity_id);
}

template <typename T>
T& Entity::getComponent() {
	return scene_ptr->getComponent<T>(entity_id);
}

#include "scene.hpp"

Scene::Scene() {
	// TODO: OPTMZ: Alloc a block the size of all component sets so they are more tightly compact
	// after thinking about it for a while, this might be really hard to do because of the varying sizes.
	// I mean the compiler can do it so we should be able to, but it might not be worth the effort.
	entities_ = new slot_set<Entity, MAX_ENTITIES>(componentIdx);
	components_[Component::Collider] = (void*) new slot_set<Collider, MAX_ENTITIES>(componentIdx);
	components_[Component::Body] = (void*) new slot_set<Body, MAX_ENTITIES>(componentIdx);
	components_[Component::Render] = (void*) new slot_set<Render, MAX_ENTITIES>(componentIdx);
}

Scene::~Scene() {
	delete (slot_set<Collider, MAX_ENTITIES>*)	components_[Component::Collider];
	delete (slot_set<Body, MAX_ENTITIES>*)		components_[Component::Body];
	delete (slot_set<Render, MAX_ENTITIES>*)	components_[Component::Render];
}

Entity& Scene::createEntity() {
	return entities_->add(Entity(++entity_id_seed_, this));
}


void Scene::update(f32 deltaTime) {
	// this will be 1 frame
	slot_set<Entity, MAX_ENTITIES>& entities = *entities_;
	// start by stepping X physics steps, resolving collisions and then updating bodies and colliders
	// here X is deltaTime / FIXED_TIMESTEP (step a fraction of a step for then remainder)
	auto colliders = getComponentSet<Collider>();
	for(auto it = colliders.begin(); it != colliders.end(); ++it) {
		it->update(entities[it->entity_id].transform);
	}
}

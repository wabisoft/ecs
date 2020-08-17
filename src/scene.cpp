#include "scene.hpp"

Scene::Scene() : renderSystem(this), collisionSystem(this) {
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

Entity& Scene::getEntity(u8 entity_id) {
	return (*entities_)[entity_id];
}

void Scene::init(sf::RenderWindow* window_ptr) {
	renderSystem.window_ptr = window_ptr;
}

void Scene::tick(f32 deltaTime) {
	// Calculate how many steps to take for this frame
	f32 totalSteps = deltaTime / FIXED_TIMESTEP;
	i32 wholeSteps = (i32)totalSteps;
	f32 fractionalStep = totalSteps - wholeSteps;
	// take all the whole step we need to take
	for(int i =0; i < wholeSteps; ++i) {
		step();
	}
	// take 1 fractional step of the given size
	step(fractionalStep*FIXED_TIMESTEP);
	// update render components
	renderSystem.update();
}

void Scene::step(f32 deltaTime) {
 	// deltaTime can be less than 1 step for fractional steps but absolutely cannot be more than 1 step
	assert(deltaTime<=FIXED_TIMESTEP);
	collisionSystem.update();
	// TODO: physicsSystem.update();
	// TODO: anything else?
}

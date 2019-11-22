#include "scene.hpp"

Scene::Scene() : renderSystem(this), collisionSystem(this) {
	// TODO: OPTMZ: Alloc a block the size of all component sets so they are more tightly compact
	// after thinking about it for a while, this might be really hard to do because of the varying sizes.
	// I mean the compiler can do it so we should be able to, but it might not be worth the effort.
	entities_ = new slot_set<Entity, MAX_ENTITIES>(componentIdx);
	components_[Component::e_Collider] = (void*) new slot_set<Collider, MAX_ENTITIES>(componentIdx);
	components_[Component::e_Rigidbody] = (void*) new slot_set<Rigidbody, MAX_ENTITIES>(componentIdx);
	components_[Component::e_Render] = (void*) new slot_set<Render, MAX_ENTITIES>(componentIdx);
}

Scene::~Scene() {
	delete (slot_set<Collider, MAX_ENTITIES>*)	components_[Component::e_Collider];
	delete (slot_set<Rigidbody, MAX_ENTITIES>*)		components_[Component::e_Rigidbody];
	delete (slot_set<Render, MAX_ENTITIES>*)	components_[Component::e_Render];
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

void Scene::frame(f32 deltaTime) {
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
#ifdef _DEBUG
	// NOTE: This is about as hacky as it gets
	// don't be like me kids... stay in school
	// fuck you compiler I do what I want! (until someone rearranges my data structure, then I'm fucked!)
	for(int i = 0; i < Component::e_Count; ++i) { // for every kind of component
		void * v = components_[i]; // there should be a pointer to a slot_set of components stored as a void pointer in the "components_" array
		void* front = (void*)*(size_t*)v; // the first member of the slot_set should be the front of it's linked list
		if(front == nullptr) {continue;} // if this pointer is null then the linked list is empty (and so we skip this slot_set)
		do {
			void* next = (void*)*(size_t*)front; // the first member of each node in the linked should be a pointer to the next item in the linked list
			Component* c = (Component*)((size_t)front + sizeof(size_t)); // the second member of the node should be a type that inherits from component.
			c->debugRender(&renderSystem); // this component should either use the base "debugRender" virtual function or override it
			// NOTE: if there are other virtual methods you want to call here you can.
			front = next; // now we set front to whatever next is and walk the linked list, if the next is nullptr then our do-while will stop now.
		} while (front != nullptr);
	}
	// The reason there are SO many comments right here is because this code is stupid and sort of hard to understand.
	// I'm deliberately ignoring the type system because I know better in this limited situation.
	// There is almost certainly a better way to do this, but as Mr. Sinatra said: "I did it my way".
#endif
}

void Scene::step(f32 deltaTime) {
 	// deltaTime can be less than 1 step for fractional steps but absolutely cannot be more than 1 step
	assert(deltaTime<=FIXED_TIMESTEP);
	collisionSystem.update();
	// TODO: physicsSystem.update();
	// TODO: anything else?
}

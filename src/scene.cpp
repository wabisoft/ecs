#include "scene.hpp"


Component* Scene::addCompnent(Component::Kind componentKind, u8 entity_id) {
	switch(componentKind) {
		case Component::Collider:
			return &colliders_.add(Collider(entity_id));
			break;
		case Component::Renderable:
			return &renderables_.add(entity_id);
			break;
		default: return nullptr; break;
	}
}

Component* Scene::getComponent(Component::Kind componentKind, u8 entity_id) {
	switch(componentKind) {
		case Component::Collider:
			return &colliders_[entity_id]; break;
		case Component::Renderable:
			return &renderables_[entity_id]; break;
		default: return nullptr; break;
	}
}

Entity& Scene::createEntity() {
	u8 id = ++entity_id_seed_;
	entities_.add(Entity(id, this));
	return entities_[id];
}

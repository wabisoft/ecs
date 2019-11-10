#include "entity.hpp"
#include "scene.hpp"


Component* Entity::getComponent(Component::Kind componentKind) {
	return scene->getComponent(componentKind, entity_id);
}


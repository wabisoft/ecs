#include "entity.hpp"
#include "scene.hpp"


Entity::Entity(u8 id, Scene* p_scene) : scene_ptr(p_scene), entity_id(id) {
	for(int i = 0; i < Component::Count; ++i) {
		components[i] = false;
	}
}


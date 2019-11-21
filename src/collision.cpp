#include "collision.hpp"
#include "render.hpp"
#include "scene.hpp"


AABB::AABB(const wabi::Polygon& p) {
	wabi::boundingPoints(p.vertices, lower, upper);
}

bool AABB::overlaps(const AABB& them) const {
	return (
		(lower.x < them.lower.x && them.lower.x < upper.x) || // this.lower-----them.lower-----this.upper----them.upper
		(them.lower.x < lower.x && lower.x < them.upper.x) || // them.lower-----this.lower-----them.upper----this.upper
		(lower.y < them.lower.y && them.lower.y < upper.y) || // this.lower-----them.lower-----this.upper----them.upper
		(them.lower.y < lower.y && lower.y < them.upper.y)    // them.lower-----this.lower-----them.upper----this.upper
	);
}


void Collider::update() {
	// TODO (owen)
}

void Collider::debugRender(RenderSystem* p_renderSystem) {
	static sf::Vertex sfVertices [2] = {sf::Vertex(), sf::Vertex()};
	static sf::Color c = sf::Color::Black;
	for(int i = 0; i < shape.size; ++i) {
		auto v1 = shape.vertices[i];
		auto v2 = shape.vertices[(i+1)%shape.size];
		sfVertices[0] = sf::Vertex(p_renderSystem->worldToScreen(v1), c);
		sfVertices[1] = sf::Vertex(p_renderSystem->worldToScreen(v2), c);
		p_renderSystem->window_ptr->draw(sfVertices, 2, sf::LinesStrip);
	}
}


CollisionSystem::CollisionSystem(Scene* scene) : p_scene(scene) { }

void CollisionSystem::update() {
	// O(n^2) collision detection, fuck it man, figure it out later
#ifdef _DEBUG
	auto renderSystem = p_scene->renderSystem;
#endif
	slot_set<Collider, MAX_ENTITIES>& colliders = p_scene->getComponentSet<Collider>();
	for(auto it = colliders.begin(); it != colliders.end(); ++it) {
		auto e1 = p_scene->getEntity(it->entity_id);
		it->shape.update(e1.transform);
		for(auto jt = std::next(it); jt != colliders.end(); ++jt) {
			if(!it->aabb.overlaps(jt->aabb)) {
				continue;
			}
			u32 colId = getCollisionId(*it, *jt);
			collisions[colId] = getCollision(*it, *jt);
		}

#ifdef _DEBUG
		it->debugRender(&renderSystem);
#endif
	}
}

u32 CollisionSystem::getCollisionId(const Collider& c1, const Collider& c2) {
	u8 minId = std::min(c1.entity_id, c2.entity_id);
	u8 maxId = std::max(c1.entity_id, c2.entity_id);
	return NaryHash<MAX_ENTITIES>::hash(minId, maxId);
}

Collision CollisionSystem::getCollision(const Collider& c1, const Collider& c2) {
	// TODO: first check AABB then do better check,
	// the goal is to update the collision. if we change collision state from colliding to not
	// colliding  or vice versa we should call our behaviors (rigidbody and bahavior components)
	std::vector<glm::vec2> intersectionPoints = pointsOfIntersection(c1.shape, c2.shape);
	assert(intersectionPoints.size() <= 3);
	if(intersectionPoints.size() == 0) { return Collision(); }
	// Ok I can get my contact manifold but now how do I get my normal and my penetration etc
	return Collision();
}


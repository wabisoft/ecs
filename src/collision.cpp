#include "collision.hpp"
#include "scene.hpp"

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


CollisionSystem::CollisionSystem(Scene* scene) : scene_ptr(scene) { }

void CollisionSystem::update() {
	// O(n^2) collision detection, fuck it man, figure it out later
	slot_set<Collider, MAX_ENTITIES>& colliders = scene_ptr->getComponentSet<Collider>();
	for(auto it = colliders.begin(); it != colliders.end(); ++it) {
		for(auto jt = std::next(it); jt != colliders.end(); ++jt) {
			if(!it->aabb.overlaps(jt->aabb)) {
				continue;
			}
			u32 colId = getCollisionId(*it, *jt);
			collisions[colId] = getCollision(*it, *jt);
		}
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


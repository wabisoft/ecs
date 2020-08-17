#pragma once

#include "component.hpp"
#include "settings.hpp"
#include "geometry.hpp"

struct AABB {
	glm::vec2 lower = glm::vec2(0);
	glm::vec2 upper = glm::vec2(0);
	bool overlaps(const AABB& them) const;
};

struct Scene;
struct Entity;
struct ColliderDef {
};

struct Collider : Component {
	friend struct Scene;
	friend struct slot_set<Collider, MAX_ENTITIES>;

	void update();
	wabi::Polygon shape;
	AABB aabb;
	Scene* scene_ptr = nullptr;
	std::vector<u32> activeCollisions;

	static const Component::Kind kind = Component::Collider;

private:
	Collider() { }
	Collider(u8 id) : Component(id) {}
	Collider(u8 id, Scene* s, ColliderDef definition) : Component(id), scene_ptr(s) {
		(void)definition;
	}
};

typedef std::vector<glm::vec2> ContactManifold;

struct Collision {
	float penetration;
	glm::vec2 normal;
	glm::vec2 tangent;
	ContactManifold contacts = { { 0, 0}, { 0, 0} };
	u8 numContacts = 0;
};

struct CollisionSystem {
	CollisionSystem(Scene* scene);

	void update();

	u32 getCollisionId(const Collider& c1, const Collider& c2);
	Collision getCollision(const Collider& c1, const Collider& c2);

	Scene* scene_ptr = nullptr;
	Hash<u32, Collision> collisions;

};


# pragma once

#include "algorithm.hpp"
#include "component.hpp"
#include "settings.hpp"
#include "geometry.hpp"

struct AABB {
	AABB() {}
	explicit AABB(const wabi::Polygon& p);
	bool overlaps(const AABB& them) const;
	glm::vec2 lower = glm::vec2(0);
	glm::vec2 upper = glm::vec2(0);
};

struct Scene;
struct Entity;
struct ColliderDef {
	ColliderDef(bool is_chain, std::vector<glm::vec2> model) : is_chain(is_chain), is_model(true), model(model) {}
	ColliderDef(bool is_chain, wabi::Polygon shape) : is_chain(is_chain), is_model(false), shape(shape) {}
	bool is_chain = false;
	bool is_model = true; // data in model
	std::vector<glm::vec2> model; // unions create constructor hell, so I'm not using one
	wabi::Polygon shape;
};

struct Collider : Component {
	friend struct Scene;
	friend struct slot_set<Collider, MAX_ENTITIES>;

	void update();
	void debugRender(RenderSystem*) override;

	wabi::Polygon shape;
	AABB aabb;
	Scene* scene_ptr = nullptr;
	std::vector<u32> activeCollisions; // ? do I need this (the idea is this is a list of ids we are in contact with)

	static const Component::Kind kind = Component::Collider;

private:
	bool overlaps(const AABB& them) const;
	Collider() { }
	Collider(u8 id) : Component(id) {}
	Collider(u8 id, Scene* s, const ColliderDef& definition) : Component(id), scene_ptr(s) {
		shape = (definition.is_model) ? wabi::Polygon(definition.model, definition.is_chain) : definition.shape;
		aabb = AABB(shape);
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

	Scene* p_scene = nullptr;
	Hash<u32, Collision> collisions;

};


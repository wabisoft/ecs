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
	static sf::Color color = sf::Color::Green;
	for(int i = 0; i < shape.size; ++i) {
		auto v1 = shape.vertices[i];
		auto v2 = shape.vertices[(i+1)%shape.size];
		sfVertices[0] = sf::Vertex(p_renderSystem->worldToScreen(v1), color);
		sfVertices[1] = sf::Vertex(p_renderSystem->worldToScreen(v2), color);
		p_renderSystem->window_ptr->draw(sfVertices, 2, sf::LinesStrip);
	}
	static sf::CircleShape circle;
	circle.setRadius(2);
	circle.setOrigin(1, 1);
	circle.setFillColor(sf::Color::Red);
	for(auto& colId : activeCollisions) {
		auto& col = scene_ptr->collisionSystem.collisions[colId];
		for(int i = 0; i < col.numContacts; ++i) {
			circle.setPosition(p_renderSystem->worldToScreen(col.contacts[i]));
			p_renderSystem->window_ptr->draw(circle);
		}
	}
}


CollisionSystem::CollisionSystem(Scene* scene) : p_scene(scene) { }

void CollisionSystem::update() {
	// O(n^2) collision detection, fuck it man, figure it out later
	slot_set<Collider, MAX_ENTITIES>& colliders = p_scene->getComponentSet<Collider>();
	for(auto it = colliders.begin(); it != colliders.end(); ++it) {
		auto e1 = p_scene->getEntity(it->entity_id);
		it->shape.update(e1.transform);
		it->aabb = AABB(it->shape);
		for(auto jt = std::next(it); jt != colliders.end(); ++jt) {
			if(!it->aabb.overlaps(jt->aabb)) {
				continue;
			}
			getCollision(*it, *jt);
		}
	}
}

u32 CollisionSystem::getCollisionId(const Collider& c1, const Collider& c2) {
	u8 minId = std::min(c1.entity_id, c2.entity_id);
	u8 maxId = std::max(c1.entity_id, c2.entity_id);
	return NaryHash<MAX_ENTITIES>::hash(minId, maxId);
}

Collision CollisionSystem::getCollision(Collider& c1, Collider& c2) {
	// TODO: first check AABB then do better check,
	// the goal is to update the collision. if we change collision state from colliding to not
	// colliding  or vice versa we should call our behaviors (rigidbody and bahavior components)
	u32 colId = getCollisionId(c1, c2);
	std::vector<glm::vec2> intersectionPoints;
	for(int a = 0; a < 2; ++a) {
		const auto* p1 = &c1.shape;
		const auto* p2 = &c2.shape;
		if(a > 0) {
			p1 = &c2.shape;
			p2 = &c1.shape;
		}

		for(int i = 0; i < c1.shape.size; ++i) {
			auto a = p1->vertices[i];
			auto b = p1->vertices[(i+1)%p1->size];
			for(int j = 0; j < p2->size; ++j) {
				auto c = p2->vertices[j];
				auto d = p2->vertices[(j+1)%p2->size];
				glm::vec2 intersection(0);
				if(wabi::lineSegmentIntersection(a, b, c, d, intersection)) {
					// auto search = std::find(intersectionPoints.begin(), intersectionPoints.end(), intersection);
					// if(search == intersectionPoints.end()) {
						intersectionPoints.push_back(intersection);
					// }
				}
				if(wabi::lineSegmentIntersection(p1->centroid, a, c, d, intersection)){
					intersectionPoints.push_back(intersection);
				}
			}
		}
	}
	// assert(intersectionPoints.size() <= 3);
	if(intersectionPoints.size() == 0) { return Collision(); } // don't inser collision into hash
	// Ok I can get my contact manifold but now how do I get my normal and my penetration etc
	Collision col;
	col.numContacts = intersectionPoints.size();
	col.contacts = std::move(intersectionPoints);
	collisions[colId] = col;
	c1.activeCollisions.push_back(colId);
	c2.activeCollisions.push_back(colId);
	return col;
}


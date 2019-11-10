#pragma once

#include <SFML/Graphics.hpp>

#include "component.hpp"
#include "datastructures.hpp"

struct Scene;
struct Entity;

struct Renderable : Component {
	friend struct Scene;
	friend struct slot_set<Renderable, MAX_ENTITIES>;

	virtual void noop() override {}

	sf::VertexBuffer glVertices;
	Scene* scene = nullptr;

	static const Component::Kind kind = Component::Renderable;

private:
	Renderable() { }
	Renderable(u8 id) : Component(id) {}
};


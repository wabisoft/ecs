#pragma once

#include <SFML/Graphics.hpp>

#include "component.hpp"
#include "datastructures.hpp"

struct Scene;
struct Entity;

struct Renderable : Component {
	sf::VertexBuffer glVertices;
	Scene* scene = nullptr;
};


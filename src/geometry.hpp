#pragma once

#include "datastructures.hpp"
#include "math.hpp"

namespace wabi {
/**********
 * Shapes *
 **********/
// NOTE: !!!! Everything about our polygons assumes 2 things
// 1) a Polygon is convex (Concave polygons won't work for collision detection, and probably to a lesser degree our method of inclusion testing (pointInside checks)
// 2) a Polygon's model is arranged from top left first, clock wise to the last vertex
// i.e. Polygon vertices in the model/vertices vector in order from top left to topleft clockwise
// example:
//      index 0					index 1
//      	*---------------------*
//      	|                     |
//      	*---------------------*
//      index 3					 index 2
//
//


struct Polygon {
	void _reserve() {
		model.reserve(size);
		vertices.reserve(size);
		for(int i = 0; i < size; ++i) {
			model.push_back(glm::vec2(0));
			vertices.push_back(glm::vec2(0));
		}
	}

	Polygon() { _reserve(); }
	Polygon(int size, bool isChain=false): size(size), rotation(0), isChain(isChain) { _reserve(); }
	Polygon(int size, float rotation, bool isChain=false) : size(size), rotation(rotation), isChain(isChain) { _reserve(); }
	Polygon(std::vector<glm::vec2> model, float rotation, bool isChain=false) : vertices(model), model(model), rotation(rotation), size((int)model.size()), isChain(isChain) { }

	void update(const Transform& transform);

	void update(Polygon& polygon, glm::vec2 position, glm::mat3 transform = glm::mat3(1));
	float minDistFromEdge(const glm::vec2 point, const Polygon& polygon, int& edgeStartIndex, bool& onVertex, float onVertexTolerance = 1.f);
	bool pointInside(const glm::vec2 point, const Polygon& polygon);
	float area(const Polygon& polygon);

	std::vector<glm::vec2> model;
	std::vector<glm::vec2> vertices;
	int size = 0;
	bool isChain = false
}

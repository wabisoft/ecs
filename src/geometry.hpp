#pragma once

#include <vector>

#include "datastructures.hpp"
#include "math.hpp"
#include <iostream>

inline std::ostream& operator<<(std::ostream& os, const glm::vec2& v) {
	return os<<"("<<v.x<< ", "<<v.y<<")";
}

namespace wabi {

template <typename T>
void print_vector(const std::vector<T>& v) {
	bool did_something = false;
	for(auto &i: v) {
		std::cout << i << " ";
		did_something = true;
	}
	if(did_something)
		std::cout << std::endl;
}

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
	Polygon(int size, bool isChain=false): size(size), isChain(isChain) { _reserve(); }
	Polygon(std::vector<glm::vec2> model, bool isChain=false) : model(model), vertices(model), size((int)model.size()), isChain(isChain) { }
	Polygon(const Polygon& other) : model(other.model), vertices(other.vertices), size(other.size), isChain(other.isChain) {}
	// Polygon(Polygon&& other) : model(other.model), vertices(other.vertices), size(other.size), isChain(other.isChain) {}

	void update(const Transform& transform);

	float minDistFromEdge(const glm::vec2 point, int& edgeStartIndex, bool& onVertex, float onVertexTolerance=1);
	float minDistFromEdge(const glm::vec2 point, float onVertexTolerance) { int i; bool b; return minDistFromEdge(point, i, b, onVertexTolerance); }
	bool pointInside(const glm::vec2 point);
	float area();


	std::vector<glm::vec2> model;
	std::vector<glm::vec2> vertices;
	int size = 0;
	bool isChain = false;
};

Polygon Rectangle(f32 width, f32 height);
Polygon Circle(f32 radius);

bool lineSegmentIntersection(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d, glm::vec2 & intersection);
std::vector<glm::vec2> pointsOfIntersection(const Polygon& poly1, const Polygon& poly2);
void boundingPoints(const std::vector<glm::vec2>& points, glm::vec2 & lower, glm::vec2 & upper);

} // namespace wabi

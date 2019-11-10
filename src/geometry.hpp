#pragma once

#include <vector>

#include "datastructures.hpp"
#include "math.hpp"

namespace wabi {


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

inline void Polygon::update(const Transform& transform) {
	for(int i = 0; i < size; ++i) {
		vertices[i] = model[i] * transform;
	}
}

inline float Polygon::minDistFromEdge(const glm::vec2 point, int& edgeStartIndex, bool& onVertex, float onVertexTolerance) {
	float min = INF;
	for (int i = 0; i < size; ++i) {
		glm::vec2 a = vertices[i];
		glm::vec2 b = vertices[((size_t)i+1) % vertices.size()];
		glm::vec2 ba = b - a;
		glm::vec2 pa = point - a;
		float pa_d = length(pa);
		bool _onVert = false;
		if(pa_d <= onVertexTolerance) {
			_onVert = true;
		}
		float len_ba = length(ba);
		glm::vec2 pp = (dot(pa, ba) / len_ba) * (ba/len_ba) ;
		pp = a + pp;
		float dist = std::fabs(length(point - pp));
		if(min > dist) {
			min = dist;
			edgeStartIndex = i;
			onVertex = _onVert;
		}
		min = min > dist ? dist : min;
	}
	return min;
}

inline bool Polygon::pointInside(const glm::vec2 point) {
	if(isChain) { return false; } // you can't have a point "inside" a chain" you can only intersect a chain
	for (int i = 0; i < size; ++i) {
		glm::vec2 a = vertices[i];
		glm::vec2 b = vertices[(i+1) % size];
		float sign = signed2DTriangleArea(a, b, point);
		// If the point is on the right (remeber we are going clock wise) then return false
		if (sign > 0) { // the > 0 here means that points on an edge are considered inside the polygon
			return false;
		}
	}
	return true;
}

inline float Polygon::area() {
	float a = 0;
	for (int i = 0; i < size; ++i) {
		glm::vec2 cur = model[i];
		glm::vec2 next= model[(i+1)%size];
		a += (cur.y + next.y)/2 * (cur.x - next.x);
	}
	return std::fabs(a);
}

} // namespace wabi

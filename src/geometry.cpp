#include "geometry.hpp"
namespace wabi {

 void Polygon::update(const Transform& transform) {
	auto rot = transformRotation(transform);
	// auto scale = transformScale(transform); // scale seems to skew shit slightly, TODO: figure this out
	auto trans = transformTranslation(transform);
	centroid = glm::vec2(0);
	for(int i = 0; i < size; ++i) {
		// vertices[i] = trans * (rot * (scale * model[i])) ;
		vertices[i] = trans * (rot *  model[i]); // TODO: scale (see above)
		centroid += vertices[i];
	}
	centroid /= size;
}

 float Polygon::minDistFromEdge(const glm::vec2 point, int& edgeStartIndex, bool& onVertex, float onVertexTolerance) {
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

 bool Polygon::pointInside(const glm::vec2 point) {
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

 float Polygon::area() {
	float a = 0;
	for (int i = 0; i < size; ++i) {
		glm::vec2 cur = model[i];
		glm::vec2 next= model[(i+1)%size];
		a += (cur.y + next.y)/2 * (cur.x - next.x);
	}
	return std::fabs(a);
}

Polygon Rectangle(f32 width, f32 height) {
	Polygon p(4, 0);
	float halfWidth = width/2.f;
	float halfHeight = height/2.f;
	p.model[0] = {-halfWidth, halfHeight}; // top left
	p.model[1] = {halfWidth, halfHeight}; // top right
	p.model[2] = {halfWidth, -halfHeight}; // bottom right
	p.model[3] = {-halfWidth, -halfHeight}; // bottom left
	return p;
}

Polygon Circle(f32 radius) {
	const int CIRCLE_SIZE = 20;
	Polygon p(CIRCLE_SIZE, 0);
	constexpr float step = (2.f * PI) / CIRCLE_SIZE;
	for(int i = 0 ; i < p.size; ++i) {
		float theta = step * i;
		// NOTE: negative cos makes the circle parameterization clockwise, which is important for our collision checking routine
		p.model[i] = glm::vec2(-std::cos(theta), std::sin(theta)) * radius;
	}
	return p;
}

bool lineSegmentIntersection(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d, glm::vec2 & intersection) {
	// Return the point of intersection of 2 lineSegments
	// if return 0.f then no intersection
	// otherwise return depth of intersection and update intersection param
	// See line segment intersection in owen's notebook or online
	// http://www.cs.swan.ac.uk/~cssimon/line_intersection.html
	float h = (d.x - c.x) * (a.y - b.y) - (a.x - b.x) * (d.y - c.y); // this is dc x ab
	if (h == 0){
		return false; // h is zero when the lines are colinear
	}
	float t = ((a.x - c.x) * (a.y - b.y) + (a.y - c.y) * (b.x - a.x)) / h;
	float s = ((a.x - c.x) * (c.y - d.y) + (a.y - c.y) * (d.x - c.x)) / h;
	if (s >= 0.f && s < 1.f && t >=0.f && t < 1.f) {
		// assert(a + s * (b-a) == c + t * (d-c)); // assertion not safe because rounding error
		intersection = a + s * (b-a);
		return true;
	} else {
		return false;
	}
}

std::vector<glm::vec2> pointsOfIntersection(const Polygon& poly1, const Polygon& poly2) {
	std::vector<glm::vec2> intersectionPoints;
	for(int i = 0; i < poly1.size; ++i) {
		auto a = poly1.vertices[i];
		auto b = poly1.vertices[(i+1)%poly1.size];
		for(int j = 0; j < poly2.size; ++j) {
			auto c = poly2.vertices[j];
			auto d = poly2.vertices[(j+1)%poly2.size];
			glm::vec2 intersection(0);
			if(lineSegmentIntersection(a, b, c, d, intersection)) {
				// auto search = std::find(intersectionPoints.begin(), intersectionPoints.end(), intersection);
				// if(search == intersectionPoints.end()) {
					intersectionPoints.push_back(intersection);
				// }
			}
		}
	}
	return intersectionPoints;
}


void boundingPoints(const std::vector<glm::vec2>& points, glm::vec2 & lower, glm::vec2 & upper) {
	float minX = INF;
	float minY = INF;
	float maxX = -INF;
	float maxY = -INF;
	for (int i = 0; i < points.size(); ++i) {
		minX = std::fmin(points[i].x, minX);
		minY = std::fmin(points[i].y, minY);
		maxX = std::fmax(points[i].x, maxX);
		maxY = std::fmax(points[i].y, maxY);
	}
	lower = {minX, minY};
	upper = {maxX, maxY};
}


} // namespace wabi

# pragma once

#include "component.hpp"
#include "geometry.hpp"

struct Collider {
	wabi::Polygon shape;
	AABB aabb;
	u8 entity_id;
};


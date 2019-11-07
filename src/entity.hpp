#pragma once

#include "math.hpp"
#include "component.hpp"
#include "settings.hpp"

struct Entity {
	u8 componet_ids[Component::Count];
	Transform transform;
	u8 id;
};



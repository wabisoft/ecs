#pragma once

#include "math.hpp"
#include "geometry.hpp"

struct Entity;

// See https://github.com/tobias-stein/EntityComponentSystem/ for a very
// clever static template based approach to dynamically assigning
// type ids to new derived types
struct Component {
	enum Kind: u8 { // I'm going to use the enum array index pattern
		// and assert that we can list every kind of component here
		// this makes holding component refs in the Entity struct easy
		// if this becomes cumbersome then we should rethink it
		Collider = 0,
		Body,
		Render,
		Count// the number of kinds
	};

	u8 entity_id = -1;

protected:
	Component() {}
	Component(u8 id) : entity_id(id) {}
};


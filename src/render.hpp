#pragma once

#include <SFML/Graphics.hpp>

#include "component.hpp"
#include "datastructures.hpp"

struct Scene;
struct Entity;

struct Render : Component {
	friend struct Scene;
	friend struct slot_set<Render, MAX_ENTITIES>;

	void update(Transform&);
	sf::Vector2f worldToScreen(const glm::vec2);
	void render();

	sf::Sprite sprite;
	sf::RenderWindow* window_ptr;
	Scene* scene_ptr = nullptr;

	static const Component::Kind kind = Component::Render;

private:
	Render() { }
	Render(u8 id) : Component(id) {}
};

inline void Render::update(Transform& transform) {
	sf::Vector2f screenPos = worldToScreen(transform.position);
	sf::Vector2f screenScale = worldToScreen(transform.scale);
	sprite.setPosition(screenPos);
	sprite.setRotation(transform.rotation);
	sprite.setScale(screenScale);
}


inline sf::Vector2f Render::worldToScreen(const glm::vec2 v) {
	auto windowSize = window_ptr->getSize();
	auto yRatio = windowSize.y / METERS_PER_WINDOW_HEIGHT;
	auto xRatio = windowSize.x / METERS_PER_WINDOW_WIDTH;
	return sf::Vector2f(v.x * xRatio, windowSize.y - (v.y * yRatio));
}

inline void Render::render() {
	window_ptr->draw(sprite);
}

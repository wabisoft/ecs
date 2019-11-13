#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "component.hpp"
#include "datastructures.hpp"

struct Scene;
struct Entity;

struct RenderDef {
	RenderDef(std::string s, sf::RenderWindow* w) : texturePath(s), window_ptr(w) {}

	std::string texturePath;
	sf::RenderWindow* window_ptr;
};

struct Render : Component {
	friend struct Scene;
	friend struct slot_set<Render, MAX_ENTITIES>;

	void init(std::string path, sf::RenderWindow* win);
	void update(Transform&);
	void render();
	void loadTexture(std::string path="");

	sf::Texture texture;
	sf::Sprite sprite;
	std::string texturePath = "";
	sf::RenderWindow* window_ptr = nullptr;
	Scene* scene_ptr = nullptr;


	static sf::Vector2f worldToScreen(const glm::vec2, sf::RenderWindow&);
	static glm::vec2 screenToWorld(const sf::Vector2i, sf::RenderWindow&);

	static const Component::Kind kind = Component::Render;

private:
	Render() { }
	Render(u8 id) : Component(id) {}
	Render(u8 id, Scene*, const RenderDef&);
};

inline Render::Render(u8 id, Scene* s, const RenderDef& definition) :
	Component(id),
	texturePath(definition.texturePath),
	window_ptr(definition.window_ptr),
	scene_ptr(s)
{
	loadTexture();
	auto b = sprite.getLocalBounds();
	sprite.setOrigin(b.width/2, b.height/2);
}

inline void Render::update(Transform& transform) {
	sf::Vector2f screenPos = worldToScreen(transform.position, *window_ptr);
	sprite.setPosition(screenPos);
	sprite.setRotation(radiansToDegrees(transform.rotation));
	sprite.setScale(transform.scale.x, transform.scale.y);
}

inline void Render::render() {
	window_ptr->draw(sprite);
}

inline void Render::loadTexture(std::string path) {
	if(!path.empty()) {
		texturePath = path;
	}
	texture.loadFromFile(texturePath);
	sprite.setTexture(texture);
};

inline glm::mat3 screenToWorldTransform(sf::RenderWindow& window) {
	auto viewSize = window.getView().getSize();
	auto xScale = viewSize.x / METERS_PER_WINDOW_WIDTH;
	auto yScale = viewSize.y / METERS_PER_WINDOW_HEIGHT;
	return {
		xScale,	0.f,	0.f,
		0.f,	-yScale,viewSize.y,
		0.f,	0.f,	1.f
	};
}

inline sf::Vector2f Render::worldToScreen(const glm::vec2 v, sf::RenderWindow& window){
	auto vprime = screenToWorldTransform(window) * v;
	return {vprime.x, vprime.y};
}

inline glm::vec2 Render::screenToWorld(const sf::Vector2i v, sf::RenderWindow& window) {
	auto cp = window.mapPixelToCoords(v);
	auto vprime = glm::inverse(screenToWorldTransform(window)) * glm::vec2(cp.x, cp.y);
	return vprime;
}


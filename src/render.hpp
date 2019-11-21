#pragma once

#include <string>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "component.hpp"
#include "datastructures.hpp"

/*
 * This is the render system and component
 * *Nearly* all the code and logic for rendering is contained herein
 * there are some artifacts of this system located in scene (where scene holds an
 * instance of renderSystem, and must initialize this system with some information about the
 * window we're rendering.
 * NOTE: Ian, this system currently uses sf::RenderWindow for all the opengl rendering
 * Your mission should you choose to accept it is to adapt this system to use raw opengl and
 * sfml only as a platform layer (if we wanna be hardcore we can write our own platform layer
 * but the required interface for that is pretty universal so for now sfml is sufficient)
 * GLHF
 * See Scene::init in scene.hpp and scene.cpp so see where the render system gets initialized
 */

struct Scene;
struct Entity;

struct RenderDef {
	RenderDef(std::string s) : texturePath(s) {}
	std::string texturePath;
};

struct RenderSystem;

struct Render : Component {
	friend struct Scene;
	friend struct slot_set<Render, MAX_ENTITIES>;

	void init();
	void loadTexture(std::string path="");

	sf::Texture texture;
	sf::Sprite sprite;
	std::string texturePath = "";

	Scene* scene_ptr = nullptr;

	static const Component::Kind kind = Component::Render;

private:
	Render() { }
	~Render() { }
	Render(u8 id) : Component(id) { }
	Render(u8 id, Scene* scene_ptr, const RenderDef& definition): Component(id),
		texturePath(definition.texturePath), scene_ptr(scene_ptr) { }
};

struct RenderSystem {
	RenderSystem(Scene* scene_ptr);

	void update();
	glm::mat3 worldToScreenTransform() { return worldToScreenTransform(*window_ptr); }
	sf::Vector2f worldToScreen(const glm::vec2 v) { return worldToScreen(v, *window_ptr); }
	glm::vec2 screenToWorld(const sf::Vector2i v) { return screenToWorld(v, *window_ptr); }

	Scene* scene_ptr;
	sf::RenderWindow* window_ptr = nullptr;

	static glm::mat3 worldToScreenTransform(sf::RenderWindow& window);
	static sf::Vector2f worldToScreen(const glm::vec2, sf::RenderWindow&);
	static glm::vec2 screenToWorld(const sf::Vector2i, sf::RenderWindow&);
};

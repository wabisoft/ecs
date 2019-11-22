#include "render.hpp"
#include "scene.hpp"


void Render::init() {
	// The reason we don't load the texture in the ctor and instead do it here is because,
	// if any intermediate copies of this component get destroyed then the texture will be dealloced
	// off of vram and the texture will wind up empty. So we need to load the texture after we are
	// pretty sure that it won't get destructed again. Some move semantics for textures would be helpful
	// here, but I'm not going gonna get involved with SFML. Also move semantics create ctor hell and we hate that
	loadTexture();
}

void Render::loadTexture(std::string path) {
	if(!path.empty()) {
		texturePath = path;
	}
	texture.loadFromFile(texturePath);
	sprite.setTexture(texture);
	auto b = sprite.getLocalBounds();
	sprite.setOrigin((f32)b.width/2.f, (f32)b.height/2.f);
};


RenderSystem::RenderSystem(Scene* scene_ptr) : scene_ptr(scene_ptr) { }

void RenderSystem::update() {
	slot_set<Render, MAX_ENTITIES>& render_components = scene_ptr->getComponentSet<Render>();
	for(auto it = render_components.begin(); it != render_components.end(); ++it) {
		// update the render component with entity transform and draw
		auto entity = scene_ptr->getEntity(it->entity_id);
		sf::Vector2f screenPos = worldToScreen(entity.transform.position);
		it->sprite.setPosition(screenPos);
		it->sprite.setRotation(-radiansToDegrees(entity.transform.rotation));
		it->sprite.setScale(entity.transform.scale.x, entity.transform.scale.y);
		window_ptr->draw(it->sprite);
	}
}

glm::vec2 RenderSystem::getSpriteWorldDimensions(const sf::Sprite& sprite) {
	auto bounds = sprite.getLocalBounds(); // size in pixels
	auto viewSize = window_ptr->getView().getSize();
	auto xScale = viewSize.x / METERS_PER_WINDOW_WIDTH;
	auto yScale = viewSize.y / METERS_PER_WINDOW_HEIGHT;
	return glm::inverse(glm::mat3{ // inverse screen scale
		xScale, 0, 0,
		0, -yScale, 0,
		0, 0, 1}) * glm::vec2{bounds.width, bounds.height};
	// return {((f32)bounds.width * METERS_PER_WINDOW_WIDTH)/(f32)viewSize.x, ((f32)bounds.height * METERS_PER_WINDOW_HEIGHT)/(f32)viewSize.y};
}

glm::mat3 RenderSystem::worldToScreenTransform(sf::RenderWindow& window) {
	auto viewSize = window.getView().getSize();
	auto xScale = viewSize.x / METERS_PER_WINDOW_WIDTH;
	auto yScale = viewSize.y / METERS_PER_WINDOW_HEIGHT;
	return {
		xScale,	0.f,	0.f,
		0.f,	-yScale,viewSize.y,
		0.f,	0.f,	1.f
	};
}

sf::Vector2f RenderSystem::worldToScreen(const glm::vec2 v, sf::RenderWindow& window){
	auto vprime = worldToScreenTransform(window) * v;
	return {vprime.x, vprime.y};
}

glm::vec2 RenderSystem::screenToWorld(const sf::Vector2i v, sf::RenderWindow& window) {
	auto cp = window.mapPixelToCoords(v);
	auto vprime = glm::inverse(worldToScreenTransform(window)) * glm::vec2(cp.x, cp.y);
	return vprime;
}



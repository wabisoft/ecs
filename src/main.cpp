#include <iostream>
#include <string>
#include <chrono>
#include <cmath>
#include <ctime>

#ifdef _DEBUG
#include <typeinfo>
#endif

#include "math.hpp"
#include "datastructures.hpp"
#include "scene.hpp"
#include "settings.hpp"

using namespace std;

class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    double elapsed() const {
        return std::chrono::duration_cast<second_>
            (clock_::now() - beg_).count();
	}

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
};

constexpr f32 FRAME_RATE = 60.f;
constexpr f32 INV_FRAME_RATE = 1.f/60.f;

// int main() {
// 	std::vector<u32> hashes;
// 	hashes.reserve(MAX_ENTITIES*MAX_ENTITIES);
// 	std::vector<u32> duplicates;
// 	for(u8 i = 0; i < MAX_ENTITIES; ++i) {
// 		for(u8 j = 0; j < MAX_ENTITIES; ++j) {
// 			u32 h = NaryHash<MAX_ENTITIES>::hash(i, j);
// 			unsigned p = 0;
// 			unsigned q = 0;
// 			NaryHash<MAX_ENTITIES>::unhash(h, p, q);
// 			// std::cout << "(i, j)=(" << (int)i << "," << (int)j << "); h=" << (int)h << ";(p,q)=(" << (int)p <<"," << (int)q <<")" <<std::endl;
// 			if(p != i) {
// 				std::cout << (int)p << " != " << (int)i << std::endl;
// 			}else if(q != j) {
// 				std::cout << (int)q << " != " << (int)j << std::endl;
// 			}
// 			auto search = std::find(hashes.begin(), hashes.end(), h);
// 			if(search == hashes.end()) {
// 				hashes.push_back(h);
// 			} else {
// 				duplicates.push_back(h);
// 			}
// 		}
// 	}
// 	std::cout  << hashes.size()/MAX_ENTITIES << std::endl;
// 	std::cout  << duplicates.size() << std::endl;
// 	return 0;
//
// }

int main() {
	const int W = 1080;
	const int H = 608;
	sf::RenderWindow window(sf::VideoMode(W, H), "Waves");
	Scene scene;
	scene.init(&window);
	Entity& e1 = scene.createEntity();
	auto renderDef = RenderDef("assets/sprites/boat.png");
	Render& e1RC= e1.addComponent<Render>(renderDef);
	auto spriteDimsM = scene.renderSystem.getSpriteWorldDimensions(e1RC.sprite);
	auto colliderDef = ColliderDef(false, wabi::Rectangle(spriteDimsM.x, spriteDimsM.y)); // here we make the collider shape tight on the sprite rect
	e1.addComponent<Collider>(colliderDef);
	e1.transform.position = {64, 36};


	Entity& e2 = scene.createEntity();
	e2.addComponent<Render>(renderDef);
	e2.addComponent<Collider>(colliderDef);
	e2.transform.position = {60, 32};

	Timer timer;
	while(window.isOpen()) {
		sf::Event event;
		while(window.pollEvent(event)) {
			switch(event.type) {
				case sf::Event::Closed: window.close(); break;
				case sf::Event::MouseButtonPressed:
					{
						auto v = RenderSystem::screenToWorld({event.mouseButton.x, event.mouseButton.y}, window);
						std::cout << v.x << ", " << v.y << std::endl;
					}
					break;
				default: break;
			}
		}

		// window.clear(sf::Color(201,233,246));
		window.clear(sf::Color::Black);
		scene.frame(timer.elapsed()); // todo have a draw delta
		timer.reset();
		window.display();
	}
	return 0;
}

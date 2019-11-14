#include <iostream>
#include <string>
#include <chrono>
#include <cmath>

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

int main() {
	const int W = 1080;
	const int H = 608;
	sf::RenderWindow window(sf::VideoMode(W, H), "Waves");
	Scene scene;
	Entity& e = scene.createEntity();
	auto def = RenderDef("assets/sprites/boat.png", &window);
	e.addComponent<Render>(def);
	e.transform.position.x = 64;
	e.transform.position.y = 36;
	Timer timer;
	while(window.isOpen()) {
		sf::Event event;
		while(window.pollEvent(event)) {
			switch(event.type) {
				case sf::Event::Closed: window.close(); break;
				case sf::Event::MouseButtonPressed:
					{
						auto v = Render::screenToWorld({event.mouseButton.x, event.mouseButton.y}, window);
						std::cout << v.x << ", " << v.y << std::endl;
					}
					break;
				default: break;
			}
		}

		window.clear(sf::Color::White);
		scene.frame(0); // todo have a draw delta
		window.display();
	}
	return 0;
}

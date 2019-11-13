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

int main() {
	const int W = 1080;
	const int H = 608;
	sf::RenderWindow window(sf::VideoMode(W, H), "Waves");
	sf::Texture boatTex;
	// boatTex.setSmooth(true);
	boatTex.loadFromFile("assets/sprites/boat.png");
	sf::Sprite boatSprite(boatTex);
	auto bounds = boatSprite.getLocalBounds();
	boatSprite.setOrigin(bounds.width/2, bounds.height/2);
	boatSprite.setPosition(W/2, H/2);
	Timer timer;
	while(window.isOpen()) {
		sf::Event e;
		while(window.pollEvent(e)) {
			switch(e.type) {
				case sf::Event::Closed: window.close(); break;
				default: break;
			}
		}
		if(timer.elapsed() > 1.) {
			boatSprite.move(timer.elapsed(), 0);
			timer.reset();
		}
		window.clear(sf::Color::White);
		window.draw(boatSprite);
		window.display();
	}
	// Scene scene;
	// Entity& e = scene.createEntity();
	// e.addComponent<Collider>();
	// e.addComponent<Render>();
	// e.addComponent<Body>();
	// scene.frame(0);
	return 0;
}

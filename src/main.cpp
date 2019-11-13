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
	Scene scene;
	Entity& e = scene.createEntity();
	e.addComponent<Collider>();
	e.addComponent<Render>();
	e.addComponent<Body>();
	scene.frame(0);
	return 0;
}

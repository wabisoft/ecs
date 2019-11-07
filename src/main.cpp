#include <iostream>
#include <string>
#include <chrono>
#include <cmath>

#include "math.hpp"
#include "datastructures.hpp"
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
	glm::vec2 a[20];
	constexpr float step = (2.f * PI) / 20;
	for(int i = 0 ; i < 20; ++i) {
		float theta = step * i;
		// NOTE: negative cos makes the circle parameterization clockwise, which is important for our collision checking routine
		a[i] = glm::vec2(-std::cos(theta), std::sin(theta));
		std::cout << "{ " << a[i].x << ", " << a[i].y << " }" << std::endl;
	}
	return 0;
}

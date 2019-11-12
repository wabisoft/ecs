#pragma once

// types
typedef signed char	i8;
typedef signed short i16;
typedef signed int i32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;
typedef double f64;


// constants
#ifndef _WIN32
#define FLT_MAX __FLT_MAX__
#define FLT_EPSILON __FLT_EPSILON__
#endif

const f32 wabi_maxFloat	= FLT_MAX;
const f32 wabi_epsilon = FLT_EPSILON;
const f32 E =	2.71828182845904523536f; // exponential constant
const f32 PI = 3.14159265358979323846f; // mmmmm pie
#include <limits>
const f32 INF = std::numeric_limits<float>::infinity();


const size_t MAX_ENTITIES = (u8)-1;
const float METERS_PER_WINDOW_WIDTH = 128.f;
const float METERS_PER_WINDOW_HEIGHT = 72.f;

#pragma once

// types
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using f32 = float;
using f64 = double;

// constants
#ifndef _WIN32
#define FLT_MAX __FLT_MAX__
#define FLT_EPSILON __FLT_EPSILON__
#endif

const f32 wabi_maxFloat	= FLT_MAX;
const f32 wabi_epsilon = FLT_EPSILON;
const f32 E =	2.71828182845904523536f; // exponential constant
constexpr f32 PI = 3.14159265358979323846f; // mmmmm pie
#include <limits>
const f32 INF = std::numeric_limits<float>::infinity();


const size_t MAX_ENTITIES = (u8)-1;
const f32 METERS_PER_WINDOW_WIDTH = 128.f;
const f32 METERS_PER_WINDOW_HEIGHT = 72.f;
const f32 FIXED_TIMESTEP = 1.f/480.f;

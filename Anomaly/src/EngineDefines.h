#pragma once

#include <memory>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef float f32;
typedef double f64;

typedef int b32;
typedef char b8;

#if defined(__clang__) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif

STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 byte.");
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 byte.");
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 byte.");

STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 byte.");
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 byte.");
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 byte.");

STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 byte.");
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 byte.");

#define TRUE 1
#define FALSE 0

#if ANOM_BUILD_DLL
#define ANOM_API __declspec(dllexport)
#else
#define ANOM_API __declspec(dllimport)
#endif

#define BIT(x) (1 << x)

#ifdef ANOM_DEBUG
#define ANOM_ENABLE_ASSERTS 1
#endif

#if ANOM_ENABLE_ASSERTS == 1
#define ANOM_CORE_ASSERT(x, ...) {if(!(x)) {ANOM_ERROR("Assertion Failed: %s", __VA_ARGS__); __debugbreak();}}
#define ANOM_CORE_ASSERT(x, ...) {if(!(x)) {ANOM_CORE_ERROR("Assertion Failed: %s", __VA_ARGS__); __debugbreak();}}
#else
#define ANOM_CORE_ASSERT(x, ...)
#define ANOM_CORE_ASSERT(x, ...)
#endif

namespace Anomaly
{

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

cmake_minimum_required(VERSION 3.0.0)
project(AnomalyEngine VERSION 0.1.0)

file(GLOB_RECURSE sources "src/*.cpp")
file(GLOB_RECURSE headers "src/*.h")

add_compile_definitions(ANOM_BUILD_DLL) 
add_compile_definitions(PLATFORM_WINDOWS)
add_compile_definitions(DEBUG)

add_library(AnomalyEngine SHARED ${sources} ${headers})

target_include_directories(AnomalyEngine PUBLIC ${AnomalyEngine_SOURCE_DIR}/src)


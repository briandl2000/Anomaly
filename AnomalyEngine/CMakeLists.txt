cmake_minimum_required(VERSION 3.0.0)
project(AnomalyEngine VERSION 0.1.0)

file(GLOB_RECURSE sources "src/*.cpp")
file(GLOB_RECURSE headers "src/*.h")

add_library(AnomalyEngine ${sources} ${headers})

target_link_libraries(AnomalyEngine PUBLIC ImGui)

target_include_directories(AnomalyEngine PUBLIC ${AnomalyEngine_SOURCE_DIR}/src)
target_include_directories(AnomalyEngine PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty/imgui)


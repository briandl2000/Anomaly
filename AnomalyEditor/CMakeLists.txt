cmake_minimum_required(VERSION 3.0.0)
project(AnomalyEditor VERSION 0.1.0)

file(GLOB_RECURSE sources "src/*.cpp")
file(GLOB_RECURSE headers "src/*.h")

add_executable(AnomalyEditor ${sources} ${headers})

target_link_libraries(AnomalyEditor PUBLIC AnomalyEngine)

target_include_directories(AnomalyEditor PRIVATE ${AnomalyEditor_SOURCE_DIR}/src)
target_include_directories(AnomalyEditor PRIVATE ${AnomalyEngine_SOURCE_DIR}/src)



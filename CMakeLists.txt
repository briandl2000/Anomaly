cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)

set(OUTPUT_DIRECOTRY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECOTRY}/debug")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECOTRY}/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECOTRY}/debug")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECOTRY}/release")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECOTRY}/release")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECOTRY}/release")

if(WIN32)
    add_compile_definitions(PLATFORM_WINDOWS)
endif()

add_compile_definitions(
    PRIVATE
        _CRT_SECURE_NO_WARNINGS
    PUBLIC
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:RelWithDebInfo>:DEBUG>
        $<$<CONFIG:Release>:RELEASE>
        $<$<CONFIG:MinSizeRel>:RELEASE>
    )

add_subdirectory(ThirdParty)

project(Anomaly VERSION 0.1.0)

add_subdirectory(AnomalyEngine)
add_subdirectory(AnomalyEditor)

set(CPACK_INSTALL_CMAKE_PROJECTS
    "debug;ALL;/"
    "release;ALL;/"
    )
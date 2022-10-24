cmake_minimum_required(VERSION 3.0.0)

project(ThirdParty)

add_library(ImGui 
            "ThirdParty/ImGui/imconfig.h" 
            "ThirdParty/ImGui/imgui_demo.cpp"
            "ThirdParty/ImGui/imgui_draw.cpp"
            "ThirdParty/ImGui/imgui_internal.h"
            "ThirdParty/ImGui/imgui_tables.cpp"
            "ThirdParty/ImGui/imgui_widgets.cpp"
            "ThirdParty/ImGui/imgui.cpp"
            "ThirdParty/ImGui/imgui.h"
            "ThirdParty/ImGui/imstb_rectpack.h"
            "ThirdParty/ImGui/imstb_textedit.h"
            "ThirdParty/ImGui/imstb_truetype.h"
            )

target_include_directories(ImGui PUBLIC "ThirdParty/ImGui")

project(Anomaly VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")


add_subdirectory(AnomalyEngine)
add_subdirectory(AnomalyEditor)
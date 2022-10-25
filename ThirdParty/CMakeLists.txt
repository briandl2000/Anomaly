cmake_minimum_required(VERSION 3.0.0)

project(ThirdParty)

add_library(ImGui 
            "ImGui/imconfig.h" 
            "ImGui/imgui_demo.cpp"
            "ImGui/imgui_draw.cpp"
            "ImGui/imgui_internal.h"
            "ImGui/imgui_tables.cpp"
            "ImGui/imgui_widgets.cpp"
            "ImGui/imgui.cpp"
            "ImGui/imgui.h"
            "ImGui/imstb_rectpack.h"
            "ImGui/imstb_textedit.h"
            "ImGui/imstb_truetype.h"
            )

target_include_directories(ImGui PUBLIC "ThirdParty/ImGui")

#!/bin/bash

url="https://github.com/ocornut/imgui.git"

git clone $url ./working/imgui &&

mkdir ./deps/imgui &&
mkdir ./deps/imgui/include &&
mkdir ./deps/imgui/include/imgui &&

cd ./working/imgui &&

mv ./*.h ../../deps/imgui/include/imgui &&
mv ./*.cpp ../../deps/imgui &&
mv ./backends/imgui_impl_sdl3.h   ./backends/imgui_impl_sdlgpu3.h   ./backends/imgui_impl_sdlrenderer3.h   ./backends/imgui_impl_sdlgpu3_shaders.h ../../deps/imgui/include/imgui &&
mv ./backends/imgui_impl_sdl3.cpp ./backends/imgui_impl_sdlgpu3.cpp ./backends/imgui_impl_sdlrenderer3.cpp ../../deps/imgui/ &&
touch ../../deps/imgui/CMakeLists.txt &&
echo "add_library( ImGui STATIC ./imgui_demo.cpp ./imgui_draw.cpp ./imgui_impl_sdl3.cpp ./imgui_impl_sdlgpu3.cpp ./imgui_impl_sdlrenderer3.cpp ./imgui_tables.cpp ./imgui_widgets.cpp ./imgui.cpp )
target_include_directories( ImGui PRIVATE ./include/imgui )" > ../../deps/imgui/CMakeLists.txt &&

cd ../../
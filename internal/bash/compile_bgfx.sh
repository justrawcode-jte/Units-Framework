#!/bin/bash

bgfx_cmake_url="https://github.com/bkaradzic/bgfx.cmake.git"

mkdir ./working/bgfx.cmake &&
git clone $bgfx_cmake_url ./working/bgfx.cmake &&

cd ./working/bgfx.cmake &&

git submodule init &&
git submodule update &&
cmake -S ./ -B ./build &&
cmake --build ./build &&
cmake --install ./build --prefix ../../deps/bgfx-release &&
cmake -S ./ -B ./build -DBGFX_CONFIG_DEBUG=true &&
cmake --build ./build &&
cmake --install ./build --prefix ../../deps/bgfx-debug &&

cd ../../
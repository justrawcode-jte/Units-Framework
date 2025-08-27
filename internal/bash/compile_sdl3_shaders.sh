#!/bin/bash

shader_src=./assets/shaders/src
shadercross=../../../internal/deps/sdl_shadercross/bin/shadercross
compiled_export=../compiled

cd ${shader_src}

for filename in *.hlsl; do
    [ -f "$filename" ] &&
    ${shadercross} "$filename" -o "${compiled_export}/${filename/.hlsl/.spv}" &&
    ${shadercross} "$filename" -o "${compiled_export}/${filename/.hlsl/.msl}" &&
    ${shadercross} "$filename" -o "${compiled_export}/${filename/.hlsl/.dxil}"
done

cd ../../../
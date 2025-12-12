#!/bin/sh

# install nlohmann json

git clone https://github.com/nlohmann/json.git

mkdir build

cmake -S json -B build -D JSON_BuildTests=OFF

cmake --build build

cmake --install build --prefix install

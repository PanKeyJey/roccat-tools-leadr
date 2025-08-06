#!/bin/sh
set -e
export PATH=/usr/bin:/bin:$PATH

SCRIPT_DIR=$(cd "${0%/*}" && pwd)
DEPS_DIR="$SCRIPT_DIR/../deps"
/bin/mkdir -p "$DEPS_DIR"
cd "$DEPS_DIR"

if [ ! -d libgaminggear ]; then
  GIT_TERMINAL_PROMPT=0 git clone https://github.com/neo091977/libgaminggear.git
fi
cd libgaminggear
# Update ancient CMake requirement so modern versions can configure the project
sed -i '1s/.*/cmake_minimum_required(VERSION 3.5)/' CMakeLists.txt
# Enforce modern link policy required by recent CMake releases
sed -i '/CMAKE_POLICY(SET CMP0022/s/OLD/NEW/' CMakeLists.txt
/bin/mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
make -j$(nproc)
sudo make install
cd ../../

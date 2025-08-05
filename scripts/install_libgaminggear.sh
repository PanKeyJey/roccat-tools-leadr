#!/bin/sh
set -e
export PATH=/usr/bin:/bin:$PATH

SCRIPT_DIR=$(cd "${0%/*}" && pwd)
DEPS_DIR="$SCRIPT_DIR/../deps"
/bin/mkdir -p "$DEPS_DIR"
cd "$DEPS_DIR"

if [ ! -d libgaminggear ]; then
  git clone https://github.com/Roccat-Open-Source/libgaminggear.git
fi
cd libgaminggear
/bin/mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
make -j$(nproc)
sudo make install
cd ../../

if [ ! -d libgaminggear-gtk ]; then
  git clone https://github.com/Roccat-Open-Source/libgaminggear-gtk.git
fi
cd libgaminggear-gtk
/bin/mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
make -j$(nproc)
sudo make install
cd ../../..

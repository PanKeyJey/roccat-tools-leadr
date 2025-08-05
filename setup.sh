#!/bin/sh
set -e
export PATH=/usr/bin:/bin:$PATH

# Ensure libgaminggear dependency is available
if command -v pkg-config >/dev/null 2>&1; then
  if ! pkg-config --exists gaminggear; then
    echo "Installing libgaminggear..."
    /bin/sh scripts/install_libgaminggear.sh
  fi
else
  echo "pkg-config not found, installing libgaminggear..."
  /bin/sh scripts/install_libgaminggear.sh
fi

sudo rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_MODULE_PATH=/usr/share/libgaminggear/cmake/Modules
make -j$(nproc)
sudo make install
cd ..

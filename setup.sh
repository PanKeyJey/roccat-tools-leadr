#!/bin/sh
set -e
export PATH=/usr/bin:/bin:$PATH

# Ensure libgaminggear dependency is available
if command -v pkg-config >/dev/null 2>&1; then
  if ! pkg-config --exists libcanberra; then
    echo "Installing libcanberra..."
    if command -v apt-get >/dev/null 2>&1; then
      sudo apt-get install -y libcanberra-dev
    elif command -v yum >/dev/null 2>&1; then
      sudo yum install -y libcanberra-devel
    elif command -v yast >/dev/null 2>&1; then
      sudo yast -i libcanberra-devel
    elif command -v zypper >/dev/null 2>&1; then
      sudo zypper install -y libcanberra-devel
    elif command -v pacman >/dev/null 2>&1; then
      sudo pacman -S --needed libcanberra
    else
      echo "Please install libcanberra development package manually."
    fi
  fi
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

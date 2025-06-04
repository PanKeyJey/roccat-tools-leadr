sudo rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_MODULE_PATH=/usr/share/libgaminggear/cmake/Modules
make -j$(nproc)
sudo make install
cd ..
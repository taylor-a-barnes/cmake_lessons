#!/bin/sh

rm -rf build
cmake -S . -B build -DCmpWrapper_COMPRESSION_LIBRARY=ZSTD
cmake --build build

cd build
cpack -G DEB

# Let's look inside
dpkg-deb -R ./cmpwrapper_1.0.0_amd64.deb pack
ls pack

dpkg -i cmpwrapper_1.0.0_amd64.deb

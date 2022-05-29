#!/bin/bash

mkdir build
cd build

cmake .. -GNinja -DINSTALL_SWITCH=ON

ninja
sudo ninja install

cd ..

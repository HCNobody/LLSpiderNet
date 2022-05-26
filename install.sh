#!/bin/bash

mkdir build
cd build

cmake .. -GNinja -DINSTALL_SWITCH=ON

sudo ninja install

cd ..

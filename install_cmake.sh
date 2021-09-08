#!/usr/bin/env bash

wget https://github.com/Kitware/CMake/releases/download/v3.20.5/cmake-3.20.5-linux-x86_64.sh
bash cmake-3.20.5-linux-x86_64.sh --skip-license --prefix=cmake
rm cmake-3.20.5-linux-x86_64.sh

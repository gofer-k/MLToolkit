#! /bin/bash

conan install .. --profile clang-debug

cmake .. -G Ninja -DCMAKE_INSTALL_PREFIX=${ARMADILLO_LIBRARY}

cmake --build .
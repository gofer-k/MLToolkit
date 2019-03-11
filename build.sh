#! /bin/bash

conan install .. --profile clang-debug

BUILDMODE="Debug"

if [ "$1" = "Release" ]; then
   BUILDMODE=$1
fi

cmake .. -G Ninja -DCMAKE_INSTALL_PREFIX=${ARMADILLO_LIBRARY} -DCMAKE_BUILD_TYPE=$BUILDMODE

cmake --build .
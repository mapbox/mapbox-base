#!/usr/bin/env bash

set -eo pipefail

mkdir build && pushd build
export CC=${CCOMPILER} CXX=${CXXCOMPILER}
cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DBUILD_TESTING=ON
echo "travis_fold:start:MAKE"
make --jobs=${JOBS}
echo "travis_fold:end:MAKE"
ccache -s
popd

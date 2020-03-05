#!/usr/bin/env bash

set -eo pipefail

# All tests assume to be run from the build directory
pushd build/test
echo "travis_fold:start:TEST"
ctest --verbose
echo "travis_fold:end:TEST"
popd


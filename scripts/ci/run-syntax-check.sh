#!/usr/bin/env bash

set -eo pipefail

clang-format -i $(git ls-files *.hpp *.cpp |xargs -L1 -i find {} -type f -maxdepth 0) && git diff --exit-code
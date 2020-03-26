#!/usr/bin/env bash

set -eo pipefail

ROOT_PATH=$PWD
LICENSE_CHECKER_PATH=$PWD/scripts/license-checker

cd $LICENSE_CHECKER_PATH
npm ci
./license-checker generate $ROOT_PATH > $ROOT_PATH/license-lock && git diff --exit-code
./license-checker license "Mapbox Base" $ROOT_PATH > $ROOT_PATH/LICENSE.thirdparty && git diff --exit-code

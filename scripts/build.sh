#!/usr/bin/env bash
set -euo pipefail

# default values
BUILD_TYPE=Release
BUILD_TESTS=ON
BUILD_EXAMPLES=ON
LINK_SHARED=OFF
GENERATOR=""

usage() {
  cat <<EOF
Usage: $0 [options]

Options:
    -c, --config         Debug | Release         (default: Release)
                         Build type passed to CMake (CMAKE_BUILD_TYPE)
  
    -t, --tests          ON | OFF               (default: ON)
                         Enable or disable building of tests

    -e, --examples       ON | OFF               (default: ON)
                         Enable or disable building of examples
  
    -l, --link-shared    ON | OFF               (default: OFF)
                         Link tests and examples with shared (ON) or static (OFF) library

    -g, --generator      <name>                 (default: system default)
                         CMake generator to use (e.g. "Ninja", "Unix Makefiles", "Visual Studio 17 2022")
EOF
  exit 1
}

# parse arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    -c|--config)            BUILD_TYPE=$2; shift 2 ;;
    -t|--tests)             BUILD_TESTS=$2; shift 2 ;;
    -e|--examples)          BUILD_EXAMPLES=$2; shift 2 ;;
    -l|--link-shared)       LINK_SHARED=$2; shift 2 ;;
    -g|--generator)         GENERATOR="-G \"$2\""; shift 2 ;;
    -h|--help)              usage ;;
    *) echo "Unknown arg: $1"; usage ;;
  esac
done

# go to project root
cd "$(dirname "$0")/.."

# prepare build dir
mkdir -p build
cd build

# configure
eval cmake $GENERATOR \
  -DEXLIB_BUILD_TESTS="$BUILD_TESTS" \
  -DEXLIB_BUILD_EXAMPLES="$BUILD_EXAMPLES" \
  -DEXLIB_LINK_SHARED="$LINK_SHARED" \
  ..

# build
cmake --build . --config "$BUILD_TYPE"

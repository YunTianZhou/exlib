#!/usr/bin/env bash
set -euo pipefail

# default values
BUILD_TYPE=Release
BUILD_TESTS=ON
TESTS_LINK_SHARED=OFF
GENERATOR=""
ARCH=""

usage() {
  cat <<EOF
Usage: $0 [options]

  -c|--config            Release|Debug       (default: $BUILD_TYPE)
  -t|--tests             ON|OFF              (default: $BUILD_TESTS)
  -l|--link-shared-tests ON|OFF              (default: $TESTS_LINK_SHARED)
  -g|--generator         <name>              e.g. "Ninja", "Unix Makefiles"
  -a|--arch              <arch>              e.g. x64 (for multi-config generators)
  -h|--help                                  show this help
EOF
  exit 1
}

# parse arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    -c|--config)            BUILD_TYPE=$2; shift 2 ;;
    -t|--tests)             BUILD_TESTS=$2; shift 2 ;;
    -l|--link-shared-tests) TESTS_LINK_SHARED=$2; shift 2 ;;
    -g|--generator)         GENERATOR="-G \"$2\""; shift 2 ;;
    -a|--arch)              ARCH="-A $2"; shift 2 ;;
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
eval cmake $GENERATOR $ARCH \
  -DBUILD_TESTS="$BUILD_TESTS" \
  -DTESTS_LINK_SHARED="$TESTS_LINK_SHARED" \
  ..

# build
cmake --build . --config "$BUILD_TYPE"

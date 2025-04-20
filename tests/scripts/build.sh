set -e
cd "$(dirname "$0")"

cd ..
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
cd ../scripts

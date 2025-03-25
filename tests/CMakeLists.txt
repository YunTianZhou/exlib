cmake_minimum_required(VERSION 3.15)
project(exlib_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC_FILES src/*.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

set(EXLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
add_subdirectory(${EXLIB_DIR} ${CMAKE_BINARY_DIR}/exlib)
include_directories(${EXLIB_DIR}/include)

foreach(TEST_SOURCE ${SRC_FILES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    
    target_link_libraries(${TEST_NAME} exlib_static)
endforeach()

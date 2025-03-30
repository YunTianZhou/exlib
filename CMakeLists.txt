cmake_minimum_required(VERSION 3.5)
project(exlib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(external/glfw)
add_subdirectory(external/glew)
add_subdirectory(external/glm)

include_directories(include)
include_directories(external/glfw/include)
include_directories(external/glew/include)

add_library(exlib_static STATIC ${SRC_FILES})
set_target_properties(exlib_static PROPERTIES OUTPUT_NAME "exlib_s")
target_compile_definitions(exlib_static PUBLIC EXLIB_STATIC)

add_library(exlib_shared SHARED ${SRC_FILES})
set_target_properties(exlib_shared PROPERTIES OUTPUT_NAME "exlib")
target_compile_definitions(exlib_shared PRIVATE EXLIB_EXPORTS)

target_include_directories(exlib_static PUBLIC external/glm)
target_include_directories(exlib_shared PUBLIC external/glm)

if(WIN32)
    target_link_libraries(exlib_static glfw libglew_static opengl32)
    target_link_libraries(exlib_shared glfw libglew_static opengl32)
else()
    target_link_libraries(exlib_static glfw GLEW GL X11 Xxf86vm Xrandr pthread Xi dl)
    target_link_libraries(exlib_shared glfw GLEW GL X11 Xxf86vm Xrandr pthread Xi dl)
endif()

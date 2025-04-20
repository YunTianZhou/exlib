cmake_minimum_required(VERSION 3.5)
project(exlib)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_subdirectory(external/glfw)
add_subdirectory(external/glew)
add_subdirectory(external/glm)

add_library(exlib_static STATIC ${SRC_FILES})
set_target_properties(exlib_static PROPERTIES OUTPUT_NAME "exlib_s")
target_compile_definitions(exlib_static PUBLIC EXLIB_STATIC GLEW_STATIC)

target_include_directories(exlib_static PUBLIC
    include
    external/stb
    external/glm
    external/glfw/include
    external/glew/include
)

add_library(exlib_shared SHARED ${SRC_FILES})
set_target_properties(exlib_shared PROPERTIES OUTPUT_NAME "exlib")
target_compile_definitions(exlib_shared PRIVATE EXLIB_EXPORTS GLEW_STATIC)

target_include_directories(exlib_shared PUBLIC
    include
    external/stb
    external/glm
    external/glfw/include
    external/glew/include
)

if(WIN32)
    target_link_libraries(exlib_static PRIVATE glfw libglew_static opengl32)
    target_link_libraries(exlib_shared PRIVATE glfw libglew_static opengl32)
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(exlib_static PRIVATE OpenGL::GL glfw GLEW)
    target_link_libraries(exlib_shared PRIVATE OpenGL::GL glfw GLEW)
endif()

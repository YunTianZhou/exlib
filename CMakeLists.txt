cmake_minimum_required(VERSION 3.5)
project(exlib)

cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(BUILD_STATIC "Build static library" ON)
option(BUILD_SHARED "Build shared library" ON)
option(BUILD_TESTS "Build tests" OFF)
option(TESTS_LINK_SHARED "Link tests against the shared library" OFF)

# Dependencies
set(BUILD_SHARED_LIBS        OFF CACHE BOOL "Disable all shared libraries"    FORCE)
set(GLFW_BUILD_EXAMPLES      OFF CACHE BOOL "Disable GLFW examples"            FORCE)
set(GLFW_BUILD_TESTS         OFF CACHE BOOL "Disable GLFW tests"               FORCE)
set(GLFW_BUILD_DOCS          OFF CACHE BOOL "Disable GLFW docs"                FORCE)
set(GLFW_INSTALL             OFF CACHE BOOL "Disable GLFW installation target"   FORCE)

set(glew-cmake_BUILD_SHARED   OFF CACHE BOOL "Disable GLEW shared"  FORCE)
set(glew-cmake_BUILD_STATIC    ON  CACHE BOOL "Enable GLEW static"    FORCE)
set(ONLY_LIBS                  ON  CACHE BOOL "Skip GLEW executables" FORCE)

set(FT_WITH_ZLIB     OFF CACHE BOOL "Disable system zlib" FORCE)
set(FT_WITH_PNG      OFF CACHE BOOL "Disable PNG support" FORCE)
set(FT_WITH_BZIP2    OFF CACHE BOOL "Disable BZip2 support" FORCE)
set(FT_WITH_HARFBUZZ OFF CACHE BOOL "Disable HarfBuzz auto-hinting" FORCE)
set(FT_WITH_BROTLI   OFF CACHE BOOL "Disable Brotli WOFF2 support" FORCE)

set(CMAKE_DISABLE_FIND_PACKAGE_ZLIB      TRUE CACHE BOOL "" FORCE)
set(CMAKE_DISABLE_FIND_PACKAGE_PNG       TRUE CACHE BOOL "" FORCE)
set(CMAKE_DISABLE_FIND_PACKAGE_BZip2     TRUE CACHE BOOL "" FORCE)
set(CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz  TRUE CACHE BOOL "" FORCE)
set(CMAKE_DISABLE_FIND_PACKAGE_BrotliDec TRUE CACHE BOOL "" FORCE)

# Subdirectories
add_subdirectory(external/glfw)
add_subdirectory(external/glew)
add_subdirectory(external/glm)
add_subdirectory(external/freetype)

# Source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Build libraries based on options
if(BUILD_STATIC)
    add_library(exlib_static STATIC ${SRC_FILES})
    set_target_properties(exlib_static PROPERTIES OUTPUT_NAME "exlib_s")
    target_compile_definitions(exlib_static PUBLIC EXLIB_STATIC GLEW_STATIC)
    target_include_directories(exlib_static PUBLIC
        include
        external/stb
        external/glm
        external/glfw/include
        external/glew/include
        external/freetype/include
    )
    if(WIN32)
        target_link_libraries(exlib_static PRIVATE glfw libglew_static freetype  opengl32)
    else()
        target_link_libraries(exlib_static PRIVATE OpenGL::GL glfw GLEW freetype)
    endif()
endif()

if(BUILD_SHARED)
    add_library(exlib_shared SHARED ${SRC_FILES})
    set_target_properties(exlib_shared PROPERTIES OUTPUT_NAME "exlib")
    target_compile_definitions(exlib_shared PRIVATE EXLIB_EXPORTS GLEW_STATIC)
    target_include_directories(exlib_shared PUBLIC
        include
        external/stb
        external/glm
        external/glfw/include
        external/glew/include
        external/freetype/include
    )
    if(WIN32)
        target_link_libraries(exlib_shared PRIVATE glfw libglew_static freetype opengl32)
    else()
        target_link_libraries(exlib_shared PRIVATE OpenGL::GL glfw GLEW freetype)
    endif()
endif()

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
elseif(TESTS_LINK_SHARED)
    message(WARNING "Tests are disabled, but TESTS_LINK_SHARED option is set to ON")
endif()

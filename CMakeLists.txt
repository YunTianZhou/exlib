cmake_minimum_required(VERSION 3.5)
project(exlib)

cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(EXLIB_BUILD_STATIC    "Build the EXLIB static library"    ON)
option(EXLIB_BUILD_SHARED    "Build the EXLIB shared library"    ON)
option(EXLIB_BUILD_TESTS     "Build EXLIB tests"                 OFF)
option(EXLIB_BUILD_EXAMPLES  "Build EXLIB examples"              OFF)
option(EXLIB_LINK_SHARED     "Link EXLIB tests/examples against the shared library" OFF)

# Dependencies
set(BUILD_SHARED_LIBS        OFF CACHE BOOL "Disable all shared libraries"       FORCE)
set(GLFW_BUILD_EXAMPLES      OFF CACHE BOOL "Disable GLFW examples"              FORCE)
set(GLFW_BUILD_TESTS         OFF CACHE BOOL "Disable GLFW tests"                 FORCE)
set(GLFW_BUILD_DOCS          OFF CACHE BOOL "Disable GLFW docs"                  FORCE)
set(GLFW_INSTALL             OFF CACHE BOOL "Disable GLFW installation target"   FORCE)

set(glew-cmake_BUILD_SHARED   OFF CACHE BOOL "Disable GLEW shared"   FORCE)
set(glew-cmake_BUILD_STATIC    ON CACHE BOOL "Enable GLEW static"    FORCE)
set(ONLY_LIBS                  ON CACHE BOOL "Skip GLEW executables" FORCE)

set(FT_WITH_ZLIB     OFF CACHE BOOL "Disable system zlib"           FORCE)
set(FT_WITH_PNG      OFF CACHE BOOL "Disable PNG support"           FORCE)
set(FT_WITH_BZIP2    OFF CACHE BOOL "Disable BZip2 support"         FORCE)
set(FT_WITH_HARFBUZZ OFF CACHE BOOL "Disable HarfBuzz auto-hinting" FORCE)
set(FT_WITH_BROTLI   OFF CACHE BOOL "Disable Brotli WOFF2 support"  FORCE)

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
set(EXLIB_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glew/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/freetype/include
)

if(EXLIB_BUILD_STATIC)
    add_library(exlib_static STATIC ${SRC_FILES})
    set_target_properties(exlib_static PROPERTIES OUTPUT_NAME "exlib_s")
    target_compile_definitions(exlib_static PUBLIC EXLIB_STATIC GLEW_STATIC)
    target_include_directories(exlib_static PUBLIC ${EXLIB_INCLUDE_DIRS})
    if(WIN32)
        target_link_libraries(exlib_static PUBLIC glfw libglew_static freetype opengl32)
    else()
        target_link_libraries(exlib_static PUBLIC OpenGL::GL glfw GLEW freetype)
    endif()
endif()

if(EXLIB_BUILD_SHARED)
    add_library(exlib_shared SHARED ${SRC_FILES})
    set_target_properties(exlib_shared PROPERTIES OUTPUT_NAME "exlib")
    target_compile_definitions(exlib_shared PRIVATE EXLIB_EXPORTS PUBLIC GLEW_STATIC)
    target_include_directories(exlib_shared PUBLIC ${EXLIB_INCLUDE_DIRS})
    if(WIN32)
        target_link_libraries(exlib_shared PUBLIC glfw libglew_static freetype opengl32)
    else()
        target_link_libraries(exlib_shared PUBLIC OpenGL::GL glfw GLEW freetype)
    endif()
endif()

# Tests and Examples
if (EXLIB_BUILD_TESTS OR EXLIB_BUILD_EXAMPLES)

    # Link target for tests and examples
    if (EXLIB_LINK_SHARED)
        if (TARGET exlib_shared)
            set(EXLIB_LINK_TARGET exlib_shared)
        else()
            message(FATAL_ERROR "EXLIB_LINK_SHARED is ON but exlib_shared target does not exist")
        endif()
    else()
        if (TARGET exlib_static)
            set(EXLIB_LINK_TARGET exlib_static)
        else()
            message(FATAL_ERROR "EXLIB_LINK_SHARED is OFF but exlib_static target does not exist")
        endif()
    endif()

    # Tests
    if(EXLIB_BUILD_TESTS)
        add_subdirectory(tests)
    endif()

    # Examples
    if(EXLIB_BUILD_EXAMPLES)
        add_subdirectory(examples)
    endif()

endif()

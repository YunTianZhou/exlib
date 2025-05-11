file(GLOB TESTS_SRC_FILES "src/*.cpp")

set(RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res/")

add_definitions(-DRES_DIR=\"${RES_DIR}\")

if (TESTS_LINK_SHARED)
    if (TARGET exlib_shared)
        link_libraries(exlib_shared)
    else()
        message(FATAL_ERROR "TESTS_LINK_SHARED is ON but exlib_shared target does not exist")
    endif()
else()
    if (TARGET exlib_static)
        link_libraries(exlib_static)
    else()
        message(FATAL_ERROR "TESTS_LINK_SHARED is OFF but exlib_static target does not exist")
    endif()
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

foreach(TEST_SOURCE ${TESTS_SRC_FILES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
endforeach()

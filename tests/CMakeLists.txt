file(GLOB TESTS_SRC_FILES "src/*.cpp")

set(IMG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/img")

add_definitions(-DIMG_DIR=\"${IMG_DIR}/\")

link_libraries(exlib_static)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

foreach(TEST_SOURCE ${TESTS_SRC_FILES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
endforeach()

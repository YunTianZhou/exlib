file(GLOB TESTS_SRC_FILES "src/*.cpp")

set(RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res/")

add_definitions(-DRES_DIR=\"${RES_DIR}\")

link_libraries(${EXLIB_LINK_TARGET})

foreach(TEST_SOURCE ${TESTS_SRC_FILES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
endforeach()

cmake_minimum_required(VERSION 3.12)

option(ENABLE_CODE_COVERAGE
    "Add flags to compiler and linker for code coverage tracing"
    ON
)

project(catnap CXX)
# Test executable name
set(EXE_NAME kitten)

#===============================================================================

# Compiler features used by the project
# Not enforced on users of the shared lib
set(PROJ_FEAT
    cxx_std_20
    cxx_constexpr
)

set(PROJ_OPTS
    -Wall
    -Wextra
    $<$<CONFIG:Debug>:-D_DEBUG>
)
set(CODE_COV
    -fprofile-instr-generate
    -fcoverage-mapping
    -fsanitize=address
    -fsanitize-coverage=trace-pc-guard
)
set(CODE_COV_SRC_DIRS
    ${CMAKE_SOURCE_DIR}/lib${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/${EXE_NAME}
)

#===============================================================================

# Source: https://github.com/open-source-parsers/jsoncpp
# Docs: https://open-source-parsers.github.io/jsoncpp-docs/doxygen/index.html
# CMake Target: jsoncpp_lib
#find_package(jsoncpp REQUIRED)

# Source:
# Docs:
# CMake Target: nlohmann_json::nlohmann_json
find_package(nlohmann_json REQUIRED)

# Source:
# Docs:
# CMake Target: GTest::gtest GMock::gmock
#       (premade runners GTest::gtest_main GMock::gmock_main)
find_package(GTest REQUIRED)

find_package(Boost REQUIRED system)
set(BOOSTLIBS
    Boost::headers
    Boost::system
)

#===============================================================================

file(GLOB_RECURSE SRCS ${CMAKE_SOURCE_DIR}/lib${PROJECT_NAME}/*.cpp)
file(GLOB_RECURSE EXE_SRCS ${CMAKE_SOURCE_DIR}/${EXE_NAME}/*.cpp)

#===============================================================================

# Shareable library with the guts
add_library(${PROJECT_NAME} ${SRCS})
target_compile_features(${PROJECT_NAME}
    PUBLIC ${PROJ_FEAT}
)
target_compile_options(${PROJECT_NAME}
    PRIVATE ${PROJ_OPTS}
    $<$<BOOL:ENABLE_CODE_COVERAGE>:${CODE_COV}>
)
target_link_options(${PROJECT_NAME}
    PRIVATE
    $<$<BOOL:ENABLE_CODE_COVERAGE>:${CODE_COV}>
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${BOOSTLIBS}
    PUBLIC
    nlohmann_json::nlohmann_json
)
target_include_directories(${PROJECT_NAME}
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib${PROJECT_NAME}>
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

# Test executable using the library
add_executable(${EXE_NAME} ${EXE_SRCS})
target_compile_features(${EXE_NAME}
    PRIVATE ${PROJ_FEAT}
)
target_compile_options(${EXE_NAME}
    PRIVATE ${PROJ_OPTS}
    $<$<BOOL:ENABLE_CODE_COVERAGE>:${CODE_COV}>
)
target_link_options(${EXE_NAME}
    PRIVATE ${CODE_COV}
    $<$<BOOL:ENABLE_CODE_COVERAGE>:${CODE_COV}>
)
target_link_libraries(${EXE_NAME}
    PRIVATE
    ${PROJECT_NAME}
    PUBLIC
    pthread
)
target_include_directories(${EXE_NAME}
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/${EXE_NAME}>
)

#===============================================================================

enable_testing()

file(GLOB_RECURSE TESTS ${CMAKE_SOURCE_DIR}/test/*.cpp)
set(TESTS_DIR "${CMAKE_BINARY_DIR}/tests")
set(PROFRAW_DIR "${CMAKE_BINARY_DIR}/profraw")

foreach(test_src ${TESTS})
    # Get the name without extension or directory
    get_filename_component(test_name ${test_src} NAME_WLE)
    # Message for the message gods
    message(STATUS "Adding test ${test_name}")
    list(APPEND TEST_NAMES ${test_name})
    list(APPEND TEST_EXES "${TESTS_DIR}/${test_name}")
    # Ensure output directory exists
    file(MAKE_DIRECTORY ${TESTS_DIR})
    # Build the test into an exe
    add_executable(${test_name} ${test_src})
    target_compile_options(${test_name}
        PRIVATE
        $<$<BOOL:ENABLE_CODE_COVERAGE>:${CODE_COV}>
    )
    target_link_options(${test_name}
        PRIVATE
        $<$<BOOL:ENABLE_CODE_COVERAGE>:${CODE_COV}>
    )
    # Link it with GTests and main library
    target_link_libraries(${test_name}
        GTest::gtest
        GTest::gtest_main
        ${PROJECT_NAME}
    )
    target_include_directories(${test_name}
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib${PROJECT_NAME}>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/test>
    )
    # Change the output directory to group tests
    set_target_properties(${test_name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TESTS_DIR}
    )
    # Add it to the list for ctest to handle
    add_test(NAME ${test_name}
        COMMAND ${test_name}
        WORKING_DIRECTORY ${TESTS_DIR}
    )
    # Generate a profraw file target to be used along with the coverage target.
    if(${ENABLE_CODE_COVERAGE})
        message(STATUS "Creating target: ${test_name}.profraw")
        list(APPEND PROFRAW_FILES "${PROFRAW_DIR}/${test_name}.profraw")
        list(APPEND PROFRAW_TAR "${test_name}.profraw")
        file(MAKE_DIRECTORY "${PROFRAW_DIR}")
        add_custom_target(${test_name}.profraw
            COMMAND
                LLVM_PROFILE_FILE=${PROFRAW_DIR}/${test_name}.profraw
                ${TESTS_DIR}/${test_name} || echo ""
            DEPENDS ${test_name}
            BYPRODUCTS ${PROFRAW_DIR}/${test_name}.profraw
            #WORKING_DIRECTORY ${TESTS_DIR}/profraw
        )
    endif(${ENABLE_CODE_COVERAGE})
endforeach(test_src)

#
# Code Coverage
#

if(${ENABLE_CODE_COVERAGE})
    message(STATUS "Creating target: coverage")
    add_custom_target(coverage
        COMMAND
            ${CMAKE_SOURCE_DIR}/tools/coverage/generate_coverage_report.py
            --tests ${TEST_EXES}
            --profraw ${PROFRAW_FILES}
            --style ${CMAKE_SOURCE_DIR}/save/style/style.css
        BYPRODUCTS html
        DEPENDS "${PROFRAW_TAR}"
        DEPENDS "${TEST_EXES}"
    )
endif(${ENABLE_CODE_COVERAGE})

# Keep at bottom
# vim: set expandtab sw=4 ts=4:

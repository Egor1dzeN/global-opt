cmake_minimum_required(VERSION 3.14)
project(find_global_opt)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_NAME_TEST ${PROJECT_NAME}_test)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_Declare(
        pybind11
        URL https://github.com/pybind/pybind11/archive/refs/tags/v2.13.6.tar.gz
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest  pybind11)
enable_testing()

file(GLOB_RECURSE TESTS
        tests/differential_evolution/*.cpp
        tests/shgo/*.cpp
        src/differential_evolution/*.cpp
        src/shgo/*.cpp
        src/measuring_time.cpp
)
file(GLOB GCGEN_SRC_FILES
#                ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/sample_src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/sample_src/GKLS/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/sample_src/Grishagin/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/sample_src/Hill/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/sample_src/Shekel/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/src/*.cpp
)
add_executable(
        ${EXECUTABLE_NAME_TEST}
        ${TESTS}
        ${GCGEN_SRC_FILES}
        include/create_bounds.h
        include/optimize_result.h
        include/shgo/measuring_time.h

)
target_link_libraries(
        ${EXECUTABLE_NAME_TEST}
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(${EXECUTABLE_NAME_TEST})

add_subdirectory(3rd_party/GCGen)

add_executable(${PROJECT_NAME}
        ${GCGEN_SRC_FILES}
        include/differential_evolution/differential_evolution.h
        src/differential_evolution/differential_evolution.cpp
        example/main.cpp
        include/measuring_time.h
        src/measuring_time.cpp
        include/create_bounds.h
        src/shgo/shgo.cpp
        include/shgo/measuring_time.h
)
#add_library(${PROJECT_NAME}_lib
#        ${GCGEN_SRC_FILES}
#        include/DE_finding_global_min.h
#        src/DE_finding_global_min.cpp
#        include/measuring_time.h
#        src/measuring_time.cpp
#)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/src
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/sample_src
        include
)
target_include_directories(${EXECUTABLE_NAME_TEST} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/src
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/sample_src
        include
)
#target_include_directories(${PROJECT_NAME}_lib PRIVATE
#        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/src
#        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/sample_src
#        include
#)

# Python bindings
pybind11_add_module(global_opt_py
        src/python_bindings.cpp
        ${GCGEN_SRC_FILES}
        src/differential_evolution/differential_evolution.cpp
        src/shgo/shgo.cpp
)

target_include_directories(global_opt_py PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/src
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/sample_src
        include
)
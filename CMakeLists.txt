cmake_minimum_required(VERSION 3.14)
project(find_global_opt)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(
        hello_test
        tests/test_main.cpp
)
target_link_libraries(
        hello_test
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)

# Добавляем сабмодуль, но отключаем его исполняемый файл
add_subdirectory(3rd_party/GCGen)

# Вместо использования библиотеки, добавляем исходники напрямую
file(GLOB GCGEN_SRC_FILES
        #        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/sample_src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/sample_src/GKLS/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/sample_src/Grishagin/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/sample_src/Hill/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/sample_src/Shekel/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/src/*.cpp
)
add_executable(${PROJECT_NAME}
        ${GCGEN_SRC_FILES}
        include/DE_finding_global_min.h
        src/DE_finding_global_min.cpp
        example/main.cpp
        include/measuring_time.h
        src/measuring_time.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/src
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/sample_src
        include
)
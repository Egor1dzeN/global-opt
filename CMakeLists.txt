cmake_minimum_required(VERSION 3.15)
project(untitled5)

set(CMAKE_CXX_STANDARD 17)

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
        include/GlobalMinimumShekel.h
        src/GlobalMinimumShekel.cpp
        example/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/src
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GCGen/sample_src
)
include_directories(include)
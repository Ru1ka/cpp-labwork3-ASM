cmake_minimum_required(VERSION 3.5.0)
set(CMAKE_CXX_STANDARD 20)

project(
    ASM
    VERSION 1.0
    DESCRIPTION "C++ Year 2024 Labwork3"
    LANGUAGES CXX
)

add_subdirectory(lib)

add_executable(${PROJECT_NAME} main.cpp)

include_directories(lib)
target_link_libraries(${PROJECT_NAME} PUBLIC args_parsing read_tsv structures iteration scripts)

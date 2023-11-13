cmake_minimum_required(VERSION 3.18)

project(containers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library files
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Examples
add_executable(stack_vector ${CMAKE_CURRENT_SOURCE_DIR}/examples/stack_vector.cpp)
target_link_libraries(stack_vector PRIVATE containers)
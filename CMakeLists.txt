cmake_minimum_required(VERSION 3.20)

project(Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(fmt CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

# target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
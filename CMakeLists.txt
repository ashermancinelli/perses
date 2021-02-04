cmake_minimum_required (VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(perses CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(PersesBoost REQUIRED)
find_package(NVML REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(src)
add_subdirectory(test)

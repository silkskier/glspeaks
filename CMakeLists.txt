cmake_minimum_required(VERSION 3.12)
project(glspeaks)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math -fopenmp")

# Uncomment the following two lines to use icpx as the compiler
# set(CMAKE_CXX_COMPILER icpx)
# set(CMAKE_LINKER icpx)

find_package(OpenMP REQUIRED)

set(SOURCES source/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/local/bin/)

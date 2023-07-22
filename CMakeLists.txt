cmake_minimum_required(VERSION 3.12)

project(glspeaks VERSION 0.0.4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_C_COMPILER "icx")
#set(CMAKE_CXX_COMPILER "icpx")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenMP REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math -fopenmp")

add_executable(glspeaks source/main.cpp source/qt.h)
target_include_directories(glspeaks PRIVATE source)
target_link_libraries(glspeaks PRIVATE OpenMP::OpenMP_CXX Qt5::Widgets)

install(TARGETS glspeaks DESTINATION /usr/local/bin/)

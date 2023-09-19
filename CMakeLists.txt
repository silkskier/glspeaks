cmake_minimum_required(VERSION 3.12)

project(glspeaks VERSION 0.1.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_C_COMPILER "icx")
#set(CMAKE_CXX_COMPILER "icpx")
# set(CUDA_FLAGS) ""
# set(CUDA_FLAGS "-fsycl -fsycl-targets=nvptx64-nvidia-cuda -Xsycl-target-backend=nvptx64-nvidia-cuda --cuda-gpu-arch=sm_75")

set(CMAKE_AUTOMOC ON)
# Set the path to the Qt5 installation
# set(CMAKE_PREFIX_PATH "/home/kacper/Qt5static/5.15" CACHE PATH "Qt5 installation path")

# Enforce the use of static Qt5 libraries
# set(Qt5_USE_STATIC_LIBS ON)

# Find the required Qt5 components
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenMP REQUIRED)
find_package(fmt REQUIRED)

include(cmake/CPM.cmake)

#[[
CPMAddPackage(
  NAME             Finufft
  GIT_REPOSITORY   https://github.com/flatironinstitute/finufft.git
  GIT_TAG          master
  GIT_SHALLOW      Yes
  GIT_PROGRESS     Yes
  EXCLUDE_FROM_ALL Yes
  SYSTEM
  CMAKE_ARGS       -DCXXFLAGS=-DFFTW_PLAN_SAFE
)
]]


# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CUDA_FLAGS} -Ofast -fopenmp -mavx2 -fno-sanitize=all")# -march=native -fno-sanitize=all -static -Ofast -pg -fomit-frame-pointer // -fomit-frame-pointer

# Add the source files for the executable
add_executable(glspeaks source/main.cpp source/qt.h)

target_include_directories(glspeaks
    PUBLIC include #/yas
)

# Set the AUTOMOC flag for the target
set_property(TARGET glspeaks PROPERTY AUTOMOC ON)

# Link against the required libraries
target_link_libraries(glspeaks PRIVATE
    Qt5::Widgets
    OpenMP::OpenMP_CXX
    fmt::fmt
    #fftw3
    #finufft_static
)

# Install the executable to /usr/local/bin
install(TARGETS glspeaks DESTINATION /usr/local/bin/)

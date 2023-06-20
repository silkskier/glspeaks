# Makefile requires libgomp and boost libraries installed to work.

CXX := icpx                # C++ compiler
LINK := icpx               # C++ linker
CXXFLAGS := -std=c++17     # C++ flags
CXXFLAGS += -O3            # Optimization level 3
CXXFLAGS += -ffast-math    # Enable fast math optimizations
CXXFLAGS += -fopenmp       # Enable OpenMP parallelization

TARGET := glspeaks         # Name of the output executable
SRC := source/main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(TARGET)

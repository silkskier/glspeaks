TEMPLATE = app
TARGET = glspeaks

QMAKE_CXX = icpx    #uncomment to compile with icpx (recommended compiler)
QMAKE_LINK = icpx   #uncomment to compile with icpx (recommended compiler)

#QMAKE_CXX = x86_64-w64-mingw32-g++-win32
#QMAKE_LINK = x86_64-w64-mingw32-g++-win32

CONFIG += c++17 optimize_full

QMAKE_CXXFLAGS += -ffast-math -fopenmp
QMAKE_LFLAGS += -fopenmp

LIBS += /usr/lib/llvm-16/lib/libomp.so.5

QT += widgets core gui

SOURCES += source/main.cpp
HEADERS += source/qt.h

target.path = /usr/local/bin/
INSTALLS += target

TEMPLATE = app
TARGET = glspeaks

QMAKE_CXX = icpx    #comment to compile with g++
QMAKE_LINK = icpx   #comment to compile with g++

CONFIG += optimize_full

QMAKE_CXXFLAGS += -ffast-math

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp

QT += core gui widgets

SOURCES += source/main.cpp

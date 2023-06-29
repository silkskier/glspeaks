TEMPLATE = app
TARGET = glspeaks

#QMAKE_CXX = icpx    #uncomment to compile with icpx (recommended compiler)
#QMAKE_LINK = icpx   #uncomment to compile with icpx (recommended compiler)

CONFIG += optimize_full

QMAKE_CXXFLAGS += -ffast-math  -fopenmp
QMAKE_LFLAGS += -fopenmp
#QT += widgets #core gui

SOURCES += source/main.cpp

target.path = /usr/bin/
INSTALLS += target

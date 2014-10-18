TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    filter.cpp \
    lowpassfilter.cpp

INCLUDEPATH += '/opt/AMDAPP/include'
LIBS += -lOpenCL

LIBS += `pkg-config opencv --libs`

OTHER_FILES += \
    cl/gaussian_blur.cl

HEADERS += \
    filter.h \
    lowpassfilter.h

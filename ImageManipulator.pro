TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    filter.cpp

INCLUDEPATH += '/usr/local/cuda-6.5/include'
LIBS += -lOpenCL

LIBS += `pkg-config opencv --libs`

OTHER_FILES += \
    cl/copy_image.cl
OTHER_FILES += \
    cl/low_pass.cl

HEADERS += \
    filter.h

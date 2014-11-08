TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    filter.cpp

INCLUDEPATH += '/opt/AMDAPP/include'
LIBS += -lOpenCL

LIBS += `pkg-config opencv --libs`

OTHER_FILES += \
    cl/copy_image.cl
    cl/low_pass.cl

HEADERS += \
    filter.h

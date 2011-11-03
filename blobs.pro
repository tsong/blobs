#-------------------------------------------------
#
# Project created by QtCreator 2011-09-26T22:45:42
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = blobs
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    displaywidget.cpp \
    utils/glutils.cpp \
    utils/pointutils.cpp \
    polygonizer.cpp \
    implicitsphere.cpp

HEADERS  += mainwindow.h \
    displaywidget.h \
    utils/matrix.h \
    utils/vector.h \
    utils/glutils.h \
    utils/pointutils.h \
    polygonizer.h \
    implicitsurface.h \
    implicitsphere.h

FORMS    +=

OTHER_FILES += \
    README

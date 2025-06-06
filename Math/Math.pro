QT += core
QT -= gui

CONFIG += c++11

TARGET = VectorMatchConsole
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Vector2f.cpp \
    Vector3f.cpp \
    Matrix2f.cpp \
    Matrix3f.cpp \
    Point2f.cpp \
    Point3f.cpp \
    Utilities.cpp \
    Point4f.cpp \
    Vector4f.cpp \
    Matrix4f.cpp \
    Transform2f.cpp \
    Transform3f.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    Vector2f.h \
    Vector3f.h \
    Matrix2f.h \
    Matrix3f.h \
    Point2f.h \
    Point3f.h \
    Utilities.h \
    Point4f.h \
    Vector4f.h \
    Matrix4f.h \
    Transform2f.h \
    Transform3f.h

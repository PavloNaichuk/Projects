#-------------------------------------------------
#
# Project created by QtCreator 2016-10-05T20:32:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FindFilesWidget
TEMPLATE = app


SOURCES += main.cpp \
    FindFilesWidget.cpp \
    FindWordInFilesThread.cpp \
    ProgressWidget.cpp

    FindFilesWidget.cpp
    FindWordInFilesThread.cpp

HEADERS  += \
    FindFilesWidget.h \
    FindWordInFilesThread.h \
    ProgressWidget.h

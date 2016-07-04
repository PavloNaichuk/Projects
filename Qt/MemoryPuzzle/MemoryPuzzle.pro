#-------------------------------------------------
#
# Project created by QtCreator 2016-06-09T11:24:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MemoryPuzzle
TEMPLATE = app

CONFIG += c++11


SOURCES += main.cpp\
        MemoryPuzzle.cpp \
    Card.cpp \
    CardBoard.cpp \
    CardBoardWidget.cpp \
    LevelGenerator.cpp

HEADERS  += MemoryPuzzle.h \
    Card.h \
    CardBoard.h \
    CardBoardWidget.h \
    LevelGenerator.h

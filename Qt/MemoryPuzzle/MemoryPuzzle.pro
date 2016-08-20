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
    LevelGenerator.cpp \
    SelectFirstCardState.cpp \
    SelectSecondCardState.cpp \
    GameState.cpp \
    GameLogic.cpp \
    WaitBeforeClosingState.cpp \
    CardRenderer.cpp \
    ClosedCardRenderer.cpp \
    OpenedCardRenderer.cpp \
    CardRendererDecorator.cpp \
    SelectedCardRenderer.cpp \
    MainWidget.cpp \
    LevelCompletedWidget.cpp \
    GameMenuWidget.cpp \
    GameTimer.cpp

HEADERS  += MemoryPuzzle.h \
    Card.h \
    CardBoard.h \
    CardBoardWidget.h \
    LevelGenerator.h \
    SelectFirstCardState.h \
    SelectSecondCardState.h \
    GameState.h \
    GameLogic.h \
    WaitBeforeClosingState.h \
    CardRenderer.h \
    ClosedCardRenderer.h \
    OpenedCardRenderer.h \
    CardRendererDecorator.h \
    SelectedCardRenderer.h \
    MainWidget.h \
    LevelCompletedWidget.h \
    GameMenuWidget.h \
    GameTimer.h

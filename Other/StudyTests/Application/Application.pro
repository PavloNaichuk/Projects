#-------------------------------------------------
#
# Project created by QtCreator 2018-06-06T13:50:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Application
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        MainWidget.cpp \
    StartProgramMenuWidget.cpp \
    StudentEnterMenuWidget.cpp \
    StudentWidget.cpp \
    StudyTests.cpp \
    TeacherEnterMenuWidget.cpp \
    TeacherWidget.cpp

HEADERS  += MainWidget.h \
    StartProgramMenuWidget.h \
    StudentEnterMenuWidget.h \
    StudentWidget.h \
    StudyTests.h \
    TeacherEnterMenuWidget.h \
    TeacherWidget.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DataModel/release/ -lDataModel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DataModel/debug/ -lDataModel
else:unix: LIBS += -L$$OUT_PWD/../DataModel/ -lDataModel

INCLUDEPATH += $$PWD/../DataModel
DEPENDPATH += $$PWD/../DataModel

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DataModel/release/libDataModel.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DataModel/debug/libDataModel.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DataModel/release/DataModel.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DataModel/debug/DataModel.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../DataModel/libDataModel.a

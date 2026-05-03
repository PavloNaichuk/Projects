QT += quick qml widgets quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BattleCity
TEMPLATE = app

HEADERS += \
	Core/Config.h \
	Core/GameUnit.h \
	Core/LevelMap.h \
	Core/MobileUnit.h \
	Core/StationaryUnit.h \
	Core/Utilities.h \
	MobileUnits/Missile.h \
	StationaryUnits/PlayerBase.h \
    Core/GameUnitModel.h \
    BattleCityGame.h \
    MobileUnitStates/MobileUnitState.h \
    MobileUnitStates/MoveState.h \
    MobileUnitStates/MobileUnitStateTypes.h \
    MobileUnits/Tank.h \
    AI/SmartTankAI.h \
    AI/TankAI.h \
    AI/DummyTankAI.h \
    Core/GameUnitTypes.h \
    StationaryUnits/Wall.h \
    MobileUnitStates/FollowPathState.h \
    Navigation/Path.h \
    Navigation/PathFinder.h \
    MobileUnitStates/StopState.h \
    AI/Goal.h \
    AI/DestroyStationaryTarget.h \
    AI/DestroyMobileTarget.h \
    CollisionDetection/CollisionDetector.h \
    Core/PlayGameLevelLogic.h \
    MobileUnitStates/MobileUnitStates.h

SOURCES += \
    Core/LevelMap.cpp \
    Core/Utilities.cpp \
    MobileUnits/Missile.cpp \
    StationaryUnits/PlayerBase.cpp \
    BattleCity.cpp \
    Core/GameUnitModel.cpp \
    BattleCityGame.cpp \
    MobileUnitStates/MoveState.cpp \
    MobileUnits/Tank.cpp \
    AI/SmartTankAI.cpp \
    StationaryUnits/Wall.cpp \
    MobileUnitStates/FollowPathState.cpp \
    Navigation/Path.cpp \
    Navigation/PathFinder.cpp \
    AI/DestroyMobileTarget.cpp \
    AI/DestroyStationaryTarget.cpp \
    CollisionDetection/CollisionDetector.cpp \
    Core/PlayGameLevelLogic.cpp \
    MobileUnitStates/MobileUnitStates.cpp

RESOURCES += \
    Qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

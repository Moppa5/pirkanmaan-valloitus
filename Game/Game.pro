TARGET = Game
TEMPLATE = app

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main/main.cpp \
    main/mapwindow.cc \
    dialog/dialog.cpp \
    interfaces/gameeventhandler.cpp \
    graphics/gamescene.cpp \
    interfaces/objectmanager.cpp \
    graphics/mapitem.cpp \
    core/gamemanager.cpp \
    tiles/mountain.cpp \
    core/worldgeneratorperlin.cpp \
    core/perlinnoise.cpp \
    tiles/lake.cpp \
    tiles/ocean.cpp \
    buildings/mine.cpp \
    core/player.cpp \
    workers/farmer.cpp \
    buildings/cottage.cpp \
    buildings/fishingboat.cpp \
    workers/miner.cpp \
    dialog/scoredialog.cpp \
	buildings/buildingbase.cpp \
    buildings/headquarters.cpp \
    buildings/outpost.cpp \
    buildings/farm.cpp \
    tiles/grassland.cpp \
    tiles/forest.cpp \
    tiles/tilebase.cpp \
    workers/basicworker.cpp \
    workers/workerbase.cpp \
    core/placeablegameobject.cpp \
    core/worldgenerator.cpp \
    core/coordinate.cpp \
    core/playerbase.cpp \
    core/gameobject.cpp \
    core/basicresources.cpp

HEADERS += \
    main/mapwindow.hh \
    dialog/dialog.h \
    constants/constants.hh \
    interfaces/gameeventhandler.hh \
    graphics/gamescene.hh \
    interfaces/objectmanager.hh \
    graphics/mapitem.hh \
    core/gamemanager.hh \
    tiles/mountain.h \
    core/worldgeneratorperlin.hh \
    core/perlinnoise.hh \
    tiles/lake.h \
    tiles/ocean.hh \
    buildings/mine.h \
    core/player.hh \
    constants/resourcemaps2.h \
    workers/farmer.hh \
    buildings/cottage.h \
    buildings/fishingboat.hh \
    workers/miner.hh \
    dialog/scoredialog.h \
	buildings/buildingbase.h \
    buildings/farm.h \
    buildings/headquarters.h \
    buildings/outpost.h \
    exceptions/baseexception.h \
    exceptions/keyerror.h \
    exceptions/ownerconflict.h \
    exceptions/invalidpointer.h \
    exceptions/illegalaction.h \
    exceptions/notenoughspace.h \
    interfaces/iobjectmanager.h \
    interfaces/igameeventhandler.h \
    tiles/grassland.h \
    tiles/forest.h \
    tiles/tilebase.h \
    workers/basicworker.h \
    workers/workerbase.h \
    core/basicresources.h \
    core/placeablegameobject.h \
    core/worldgenerator.h \
    core/coordinate.h \
    core/playerbase.h \
    core/gameobject.h \
    core/resourcemaps.h

FORMS += \
    main/mapwindow.ui \
    dialog/dialog.ui \
    dialog/scoredialog.ui

RESOURCES += \
    resources.qrc

DISTFILES +=

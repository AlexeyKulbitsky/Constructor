#-------------------------------------------------
#
# Project created by QtCreator 2015-06-07T15:46:41
#
#-------------------------------------------------

QT       += core gui opengl webkit webkitwidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Constructor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
#    road.cpp \
    roundingroad.cpp \
    objectslist.cpp \
    linesimple.cpp \
    ruler.cpp \
    roadsimple.cpp \
    linebroken.cpp \
    statemanager.cpp \
    model.cpp \
    defaultstate.cpp \
    selectedstate.cpp \
    rulerstate.cpp \
    linebuilderstate.cpp \
    roadbuilderstate.cpp \
    roadbroken.cpp \
    jsonfilemanager.cpp \
    scene2d.cpp \
    scene3d.cpp \
    compositeroad.cpp \
#    multiselectedstate.cpp \
#    cube.cpp \
#    objectstoolbox.cpp \
    objfilemanager.cpp \
    roadelement3d.cpp \
    _3dsfilemanager.cpp \
    stepdialog.cpp \
    roadelementobj.cpp \
    intersection.cpp \
    texturemanager.cpp \
    curve.cpp \
    splitzone.cpp \
#    voltageline.cpp \
#    doublevoltageline.cpp \
    railway.cpp \
    linelinked.cpp \
    logger.cpp \
    roadelement.cpp \
    state.cpp \
    commands.cpp \
    overlayedmapswidget.cpp \
    yandexmapsview.cpp \
    googlemapsview.cpp \
    settingsdialog.cpp \
    filemanager.cpp




HEADERS  += mainwindow.h\
#    road.h \
    roadelement.h \
    roundingroad.h \
    objectslist.h \
    linesimple.h \
    ruler.h \
    roadsimple.h \
    linebroken.h \
    statemanager.h \
    state.h \
    model.h \
    defaultstate.h \
    selectedstate.h \
    rulerstate.h \
    linebuilderstate.h \
    roadbuilderstate.h \
    roadbroken.h \
    filemanager.h \
    jsonfilemanager.h \
    scene2d.h \
    scene3d.h \
    compositeroad.h \
#    multiselectedstate.h \
#    cube.h \
#    objectstoolbox.h \
    objfilemanager.h \
    roadelement3d.h \
    _3dsfilemanager.h \
    stepdialog.h \
    roadelementobj.h \
    intersection.h \
    texturemanager.h \
    curve.h \
    splitzone.h \
#    voltageline.h \
#    doublevoltageline.h \
    railway.h \
    linelinked.h \
    logger.h \
#    mptr.h
    commands.h \
    overlayedmapswidget.h \
    yandexmapsview.h \
    googlemapsview.h \
    settingsdialog.h

##LIBS += -LC:/OpenSSL-Win64 -llibeay32 -llibssl32


FORMS    += mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    textures.qrc \
    icons.qrc

#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg

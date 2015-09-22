#-------------------------------------------------
#
# Project created by QtCreator 2015-06-07T15:46:41
#
#-------------------------------------------------

QT       += core gui opengl xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Constructor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
    road.cpp \
    roundingcrossroad.cpp \
    roundingroad.cpp \
    objectslist.cpp \
	roadmodel.cpp \
	sliderdelegate.cpp \
    mapper.cpp \
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
    linebuilder.cpp \
    roadbuilderstate.cpp \
    roadbroken.cpp \
    jsonfilemanager.cpp \
    scene2d.cpp \
    scene3d.cpp \
    propertiesmanager.cpp \
    compositeroad.cpp \
    multiselectedstate.cpp \
    cube.cpp \
    objectstoolbox.cpp \
    objfilemanager.cpp \
    roadelement3d.cpp \
    _3dsfilemanager.cpp \
    stepdialog.cpp \
    roadelementobj.cpp \
    intersection.cpp \
    texturemanager.cpp \
    curve.cpp \
    splitzone.cpp




HEADERS  += mainwindow.h\
    glext.h \
    glu.h \
    road.h \
    roadelement.h \
    roundingcrossroad.h \
    roundingroad.h \
    objectslist.h \
	roadmodel.h \
	sliderdelegate.h \
    mapper.h \
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
    linebuilder.h \
    roadbuilderstate.h \
    roadbroken.h \
    filemanager.h \
    jsonfilemanager.h \
    scene2d.h \
    scene3d.h \
    propertiesmanager.h \
    compositeroad.h \
    multiselectedstate.h \
    cube.h \
    objectstoolbox.h \
    objfilemanager.h \
    roadelement3d.h \
    3d.h \
    _3dsfilemanager.h \
    stepdialog.h \
    roadelementobj.h \
    intersection.h \
    texturemanager.h \
    curve.h \
    splitzone.h




FORMS    += mainwindow.ui

RESOURCES += \
    textures.qrc \
    icons.qrc

#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg

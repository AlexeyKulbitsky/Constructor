#ifndef SCENE2D_H
#define SCENE2D_H

#include <QGLWidget>
#include <QListWidgetItem>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glaux.h>
#include <vector>
#include <iostream>
#include <QMenu>
#include <QAction>
#include <QWebView>
class StateManager;
#include "statemanager.h"
#include "model.h"

class Scene2D : public QGLWidget
{
    Q_OBJECT
public:
    StateManager* stateManager;
    QSettings* settings;
    QFont shrift;
    bool rulerIsActive;
    bool rightButtonIsPressed;
    bool leftButtonIsPressed;
    GLfloat ratio; // отношение высоты окна виджета к его ширине
    GLfloat xRot;
    GLfloat yRot;
    GLfloat zRot;
    GLfloat zTra;
    GLdouble nSca;
    GLdouble scaleStep;
    GLdouble gridStep;
    GLfloat xDelta;
    GLfloat yDelta;
    GLfloat dX;
    GLfloat dY;
    QPoint ptrMousePosition;

    bool figureIsSelected;
    bool controlIsSelected;
    bool drawRectStatus;
    bool showMaps;
    bool showScene;
    bool sceneActive;
    bool fixedScale;
    QPoint rectPoint1, rectPoint2;
    void scalePlus();
    void scaleMinus();
    void rotateUp();
    void rotateDown();
    void rotateLeft();
    void rotateRight();
    void translateDown();
    void translateUp();
    void defaultScene();

   // bool tryToSelectControlsInSelectedFigure(QPoint mp, RoadElement* element, int& index);
   // bool tryToSelectFigures(QPoint mp, RoadElement*& element);
    void getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz);
    void getWorldCoord(double x, double y, double z, double &wx, double &wy, double &wz);



protected:
    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();
    void mousePressEvent(QMouseEvent* pe);
    void mouseMoveEvent(QMouseEvent* pe);
    void mouseReleaseEvent(QMouseEvent* pe);
    void wheelEvent(QWheelEvent* pe);
    void keyPressEvent(QKeyEvent* pe);
    void keyReleaseEvent(QKeyEvent *pe);

    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent * event);

public:
    explicit Scene2D(QWidget* parent = 0);
    explicit Scene2D(QSettings* settings, QWidget* parent = 0);
    ~Scene2D();
    void drawGrid();
    void drawAxis();
    void drawRect(QPoint p1, QPoint p2);
    bool isRulerActive();
    void setModel(Model* model);
    void setProperties(QFormLayout* properties);
    void drawBackground(QPainter *painter);
    void setDrawRectStatus(bool status);
    void setSettings(QSettings* settings);
    void setOverlayWidget(QWebView *widget);
    void drawModel();
    void loadSettings();
    void saveSettings();
    static bool getLogging();
    void drawSubstrate();
    float getScaleStep() { return scaleStep; }

    void getXDelta(float value) { xDelta = value; }
    float getXDelta() { return xDelta; }

    void getYDelta(float value) { yDelta = value; }
    float getYDelta() { return yDelta; }

signals:
    //void selectedFigureChanged(RoadElement* roadElement);
    void rulerStatusChanged(bool status);
    void scaleChanged(double value);
    void scaleStepChanged(double value);
    void gridStepChanged(double value);
    void showMapsStatusChanged(bool status);
    void currentIndexChanged(int index);
    void widgetWidthChanged(int w);
    void widgetHeightChanged(int h);

public slots:
    void activateRuler();
    void deActivateRuler();
    void setRulerActive(bool status);
    void setShowGrid(bool);
    void setScale(double scale);
    void setScaleStep(double step);
    void setGridStep(double step);
    void getProperties(QFormLayout* layout);
    void setShowMapsStatus(bool status);
    void setShowSceneStatus(bool status);
    void setSceneActive(bool status);
    void setFixedScaleStatus(bool status);
    static void setLogging(bool status);
    void copy();
    void paste();
    void cut();
    void del();
    void saveToPresets();
    void setDrawSubstrateStatus(bool status);
    void setSubstrateLength(double length);
    void setSubstrateWidth(double length);
    void setSubstrateColor();
    void setMapType(int index);
    void setWidgetWidth(int w);
    void setWidgetHeight(int h);
    void setWidgetWidthScaleFactor(double factor);
    void setWidgetHeightScaleFactor(double factor);

    void saveImage();

private:
    QImage widgetImage;
    bool scaleImage;
    Model* model;
    QFormLayout* properties;
    bool showGrid;
    QWebView* widget;
    int widgetWidth;
    int widgetHeight;
    float widgetWidthScaleFactor;
    float widgetHeightScaleFactor;

    QColor substrateColor;
    float substrateWidth, substrateLength;
    bool drawSubstrateStatus;
    static bool log;
    // QWidget interface
protected:
    virtual void contextMenuEvent(QContextMenuEvent *pe);

};
#endif

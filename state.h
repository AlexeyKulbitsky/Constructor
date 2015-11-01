#ifndef STATE_H
#define STATE_H

#include <QWidget>

class Model;
#include "model.h"
#include <QMenu>
#include <QAction>
#include <QString>
#include <QObject>

class State: public QObject
{
    Q_OBJECT
public:
    virtual void mousePressEvent(QMouseEvent* pe) = 0;
    virtual void mouseMoveEvent(QMouseEvent* pe) = 0;
    virtual void mouseReleaseEvent(QMouseEvent* pe) = 0;
    virtual void wheelEvent(QWheelEvent* pe) = 0;
    virtual void keyPressEvent(QKeyEvent* pe) = 0;
    virtual void keyReleaseEvent(QKeyEvent *) = 0;
    virtual void dragEnterEvent(QDragEnterEvent* event) = 0;
    virtual void dropEvent(QDropEvent * event);
    virtual void contextMenuEvent(QContextMenuEvent *pe) = 0;
    virtual QString getName() = 0;
    virtual ~State(){}
    virtual void getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz);
    virtual void getWorldCoord(double x, double y, double z, double &wx, double &wy, double &wz);
};

#endif // STATE_H




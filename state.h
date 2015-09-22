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
    virtual void dropEvent(QDropEvent * event) = 0;
    virtual void contextMenuEvent(QContextMenuEvent *pe) = 0;
    virtual QString getName() = 0;
    virtual ~State(){}
};

#endif // STATE_H

#ifndef OBJECTSLIST_H
#define OBJECTSLIST_H

#include <QListWidget>
#include <QDrag>
#include <QWidget>
#include <QMainWindow>
#include <QDir>

class ObjectsList : public QListWidget
{


    public:

    ObjectsList(QWidget *parent = 0);
    ObjectsList(QDir directory, QString fileType, QWidget *parent = 0);
    protected:
    void dropEvent(QDropEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QPoint dragStartPosition;
    QString filePath;
    // QWidget interface
protected:
    virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif // OBJECTSLIST_H

#ifndef OSMVIEW_H
#define OSMVIEW_H

#include <QWebView>
#include "logger.h"

class OSMview : public QWebView
{
    Q_OBJECT
public:
    OSMview(QWidget *parent=0);
    float getDiagonal();
    void setActive(bool status);
    bool isActive() { return active; }

private:
    bool active;
};

#endif // OSMVIEW_H

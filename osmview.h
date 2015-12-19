#ifndef OSMVIEW_H
#define OSMVIEW_H

#include <QWebView>
#include "logger.h"

class OSMview : public QWebView
{
    Q_OBJECT
public:
    OSMview(QWidget *parent=0);
};

#endif // OSMVIEW_H

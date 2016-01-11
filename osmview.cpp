#include "osmview.h"

#include <QWebFrame>
#include <QWebPage>

OSMview::OSMview(QWidget *parent)
    : QWebView(parent)
{
    setObjectName("OSM");
    load(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/osm.html"));
    active = true;
    this->page()->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
}

float OSMview::getDiagonal()
{
    float res = this->page()->mainFrame()->evaluateJavaScript(QString("var bounds = map.getBounds(); var p1 = bounds.getSouthWest(); var p2 = bounds.getNorthEast(); p1.distanceTo(p2);")).toFloat();
    return res;
}

void OSMview::setActive(bool status)
{
    active = status;
}


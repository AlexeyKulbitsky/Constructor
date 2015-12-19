#include "osmview.h"

OSMview::OSMview(QWidget *parent)
    : QWebView(parent)
{
    setObjectName("OSM");
    load(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/osm.html"));
    //load(QUrl("http://www.openstreetmap.org/?minlon=22.3418234&minlat=57.5129102&maxlon=22.5739625&maxlat=57.6287332&box=yes"));
    this->page()->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
}


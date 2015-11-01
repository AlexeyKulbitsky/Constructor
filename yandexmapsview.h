#ifndef YANDEXMAPSVIEW_H
#define YANDEXMAPSVIEW_H

#include <QWebView>
#include "logger.h"

class YandexMapsView: public QWebView
{
    Q_OBJECT
public:
    YandexMapsView(QWidget* parent = 0);
    void wheelEv(QWheelEvent* pe);
    void mouseMo(QMouseEvent* pe);
    static void setLogging(bool status);
    static bool getLogging() { return log; }
private:
    int zoom;
    static bool log;
public:
    void scalePlus();
    void scaleMinus();
};

#endif // MAPSVIEW_H

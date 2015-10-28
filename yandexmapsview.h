#ifndef YANDEXMAPSVIEW_H
#define YANDEXMAPSVIEW_H

#include <QWebView>

class YandexMapsView: public QWebView
{
    Q_OBJECT
public:
    YandexMapsView(QWidget* parent = 0);
    void wheelEv(QWheelEvent* pe);
    void mouseMo(QMouseEvent* pe);

private:
    int zoom;

public:
    void scalePlus();
    void scaleMinus();
};

#endif // MAPSVIEW_H

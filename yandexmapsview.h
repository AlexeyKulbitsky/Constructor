#ifndef YANDEXMAPSVIEW_H
#define YANDEXMAPSVIEW_H

#include <QWebView>
#include "logger.h"

class YandexMapsView: public QWebView
{
    Q_OBJECT
public:
    YandexMapsView(QWidget* parent = 0);
//    void wheelEv(QWheelEvent* pe);
//    void mouseMo(QMouseEvent* pe);
//    void mousePre(QMouseEvent* pe);
//    void mouseRel(QMouseEvent* pe);
    static void setLogging(bool status);
    static bool getLogging() { return log; }
private:
    int zoom;
    static bool log;
    bool active;
    float scaleStep;
    float scaleFactor;
    int scaleCounter;

public:
    void scalePlus();
    void scaleMinus();
    void setActive(bool status);
    bool isActive() { return active; }
    float getDiagonal();

    void setScaleStep(float step) { scaleStep = step; }
    float getScaleStep() { return scaleStep; }

    void setScaleFactor(float factor) { scaleFactor = factor; }
    float getScaleFactor() { return scaleFactor; }

    void setScaleCounter(int counter) { scaleCounter = counter; }
    int getScaleCounter() { return scaleCounter; }

    QString getZoom();
    QString getCenterX();
    QString getCenterY();

    void setCenter(QString& centerX, QString& centerY, QString& zoom);
};

#endif // MAPSVIEW_H

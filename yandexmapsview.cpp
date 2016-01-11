#include "yandexmapsview.h"
#include <QApplication>
#include <QWebFrame>
#include <QVariant>
#include <QWheelEvent>

bool YandexMapsView::log = true;

YandexMapsView::YandexMapsView(QWidget *parent)
    :QWebView(parent)
{
    setObjectName("YandexMaps");
    load(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/yandex.html"));
    this->page()->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    scaleStep = 1.0f;
    scaleFactor = 1.0f;
    scaleCounter = 0;
    active = true;
}

void YandexMapsView::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "YandexMapsView::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}

void YandexMapsView::setActive(bool status)
{
    active = status;
    if (active)
    {
        this->page()->mainFrame()->evaluateJavaScript(QString("myMap.controls.add('searchControl');") +
                                                      QString("myMap.controls.add('typeSelector');") +
                                                      QString("myMap.controls.add('zoomControl');") +
                                                      QString("myMap.controls.add('ruler');"));

    }
    else
    {
        this->page()->mainFrame()->evaluateJavaScript(QString("myMap.controls.remove('searchControl');") +
                                                      QString("myMap.controls.remove('typeSelector');") +
                                                      QString("myMap.controls.remove('zoomControl');") +
                                                      QString("myMap.controls.remove('ruler');"));
    }
}

float YandexMapsView::getDiagonal()
{
    return this->page()->mainFrame()->evaluateJavaScript(QString("var b = myMap.getBounds(); ymaps.coordSystem.geo.getDistance(b[0], b[1]);")).toFloat();
}

QString YandexMapsView::getZoom()
{
    return this->page()->mainFrame()->evaluateJavaScript(QString("myMap.getZoom();")).toString();
}

QString YandexMapsView::getCenterX()
{
    return this->page()->mainFrame()->evaluateJavaScript(QString("myMap.getCenter()[0];")).toString();
}

QString YandexMapsView::getCenterY()
{
    return this->page()->mainFrame()->evaluateJavaScript(QString("myMap.getCenter()[1];")).toString();
}

void YandexMapsView::setCenter(QString &centerX, QString &centerY, QString &zoom)
{
    this->page()->mainFrame()->evaluateJavaScript(QString("myMap.setCenter([") + centerX + QString(", ") +
                                                         centerY + QString("], ") + zoom + QString(");"));
}


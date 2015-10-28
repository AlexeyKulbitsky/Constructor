#include "yandexmapsview.h"
#include <QApplication>
#include <QWebFrame>
#include <QVariant>

YandexMapsView::YandexMapsView(QWidget *parent)
    :QWebView(parent)
{
    load(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/index.html"));
    this->page()->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    this->page()->mainFrame()->evaluateJavaScript("myMap.controls.add(new ymaps.control.ScaleLine());");
}

void YandexMapsView::wheelEv(QWheelEvent *pe)
{
    this->wheelEvent(pe);
}

void YandexMapsView::mouseMo(QMouseEvent *pe)
{
    this->mouseMoveEvent(pe);
}


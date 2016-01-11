#include "googlemapsview.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWebFrame>
#include <QWebPage>
#include <QResizeEvent>
#include <QApplication>
#include <math.h>

bool GoogleMapsView::log = true;

GoogleMapsView::GoogleMapsView(QWidget *parent)
    : QWebView(parent)//, pendingRequests(0)
{
    setObjectName("GoogleMaps");
//    manager = new QNetworkAccessManager(this);
//    connect(manager, SIGNAL(finished(QNetworkReply*)),
//            this, SLOT(replyFinished(QNetworkReply*)));
//    connect(this,SIGNAL(reloadMap()),
//            this,SLOT(loadCoordinates()));
    load(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/google.html"));
    this->page()->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    active = true;
}

void GoogleMapsView::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "GoogleMapsView::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}

void GoogleMapsView::setActive(bool status)
{
    active = status;
    if (active)
    {
        this->page()->mainFrame()->evaluateJavaScript(QString("addSearchBar();"));
    }
    else
    {
        this->page()->mainFrame()->evaluateJavaScript("map.MapOptions.disableDefaultUI = false;");
    }
}

float GoogleMapsView::getDiagonal()
{
    float res = this->page()->mainFrame()->evaluateJavaScript(QString("var bounds = map.getBounds(); var p1 = bounds.getSouthWest(); var p2 = bounds.getNorthEast(); google.maps.geometry.spherical.computeDistanceBetween(p1, p2);")).toFloat();
    return res;
}

/*
 * Запрашивает координаты в формате CSV по указанному адресу, мспользуя
 * Google Maps API
 */
//void GoogleMapsView::geoCode(const QString& address)
//{
//    if (log)
//        Logger::getLogger()->infoLog() << "GoogleMapsView::geoCode(const QString& address)"
//                                       << " address = " << address << "\n";
//    clearCoordinates();
//    QString requestStr( tr("http://maps.google.com/maps/geo?q=%1&output=%2&key=%3")
//            .arg(address)
//            .arg("csv")
//            .arg("ABQIAAAAO49nNEVnwxGsuIBDsUs1sBRMlhDm1om-sbK3hx0z3hRQyGwd6hSC3qtdFB_5tj9DICmkSlnZqX5hrw") );

//    manager->get( QNetworkRequest(requestStr) );
//    ++pendingRequests;
//}

//void GoogleMapsView::replyFinished(QNetworkReply *reply)
//{
//    if (log)
//        Logger::getLogger()->infoLog() << "GoogleMapsView::replyFinished(QNetworkReply *reply)\n";
//    QString replyStr( reply->readAll() );
//    QStringList coordinateStrList = replyStr.split(",");

//    if( coordinateStrList.size() == 4) {
//        QPointF coordinate( coordinateStrList[2].toFloat(),coordinateStrList[3].toFloat() );
//        coordinates << coordinate;
//        qDebug() << coordinates;
//    }

//    --pendingRequests;
//    if( pendingRequests<1 )
//        emit( reloadMap() );
//}

//void GoogleMapsView::loadCoordinates()
//{
//    if (log)
//        Logger::getLogger()->infoLog() << "GoogleMapsView::loadCoordinates()\n";
//    QStringList scriptStr;
//    scriptStr << QString("map.panTo(new GLatLng(%1, %2));")
//                         .arg(coordinates.last().x())
//                         .arg(coordinates.last().y());
//    page()->mainFrame()->evaluateJavaScript( scriptStr.join("\n") );
//}

//void GoogleMapsView::clearCoordinates()
//{
//    if (log)
//        Logger::getLogger()->infoLog() << "GoogleMapsView::clearCoordinates()\n";
//    coordinates.clear();
//}

//void GoogleMapsView::wheelEv(QWheelEvent *pe)
//{
//    this->wheelEvent(pe);
//}

//void GoogleMapsView::mouseMo(QMouseEvent *pe)
//{
//    this->mouseMoveEvent(pe);
//}

//void GoogleMapsView::mousePre(QMouseEvent *pe)
//{
//    this->mousePressEvent(pe);
//}

//void GoogleMapsView::mouseRel(QMouseEvent *pe)
//{
//    this->mouseReleaseEvent(pe);
//}

//void GoogleMapsView::resizeEvent(QResizeEvent *event)
//{
//    if (log)
//        Logger::getLogger()->infoLog() << "GoogleMapsView::resizeEvent(QResizeEvent *event)\n";
//    if(page())
//        page()->setViewportSize(event->size());

//    int x = event->size().width();
//    int y = event->size().height();

//    QStringList scriptStr;
//    scriptStr
//        << "var elem = document.getElementById(\"map\");"
//        << QString("elem.style.width = '%1px';").arg(x-6)
//        << QString("elem.style.height = '%2px';").arg(y-6);

//    page()->mainFrame()->evaluateJavaScript( scriptStr.join("\n") );
//}



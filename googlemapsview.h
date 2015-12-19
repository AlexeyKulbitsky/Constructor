#ifndef GOOGLEMAPSVIEW_H
#define GOOGLEMAPSVIEW_H

#include <QWebView>
//class QNetworkAccessManager;
#include "logger.h"

class GoogleMapsView: public QWebView
{
    Q_OBJECT

public:
    GoogleMapsView(QWidget *parent=0);
    static void setLogging(bool status);
    static bool getLogging() { return log; }
    void setActive(bool status);
    bool isActive() { return active; }
    float getDiagonal();

public slots:
//    void replyFinished(QNetworkReply*);
//    void loadCoordinates();
//    void geoCode(const QString &address);
//    void clearCoordinates();

//    void wheelEv(QWheelEvent* pe);
//    void mouseMo(QMouseEvent* pe);
//    void mousePre(QMouseEvent* pe);
//    void mouseRel(QMouseEvent* pe);

protected:
    //void resizeEvent(QResizeEvent *event);

signals:
    //void reloadMap();

private:
    //QNetworkAccessManager *manager;
    //QList<QPointF> coordinates;
    //int pendingRequests;
    static bool log;
    bool active;
};

#endif // GOOGLEMAPSVIEW_H

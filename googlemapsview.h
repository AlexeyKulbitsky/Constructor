#ifndef GOOGLEMAPSVIEW_H
#define GOOGLEMAPSVIEW_H

#include <QWebView>
class QNetworkAccessManager;

class GoogleMapsView: public QWebView
{
    Q_OBJECT

public:
    GoogleMapsView(QWidget *parent=0);

public slots:
    void replyFinished(QNetworkReply*);
    void loadCoordinates();
    void geoCode(const QString &address);
    void clearCoordinates();

protected:
    void resizeEvent(QResizeEvent *event);

signals:
    void reloadMap();

private:
    QNetworkAccessManager *manager;
    QList<QPointF> coordinates;
    int pendingRequests;
};

#endif // GOOGLEMAPSVIEW_H

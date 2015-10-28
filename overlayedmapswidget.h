#ifndef OVERLAYEDMAPSWIDGET_H
#define OVERLAYEDMAPSWIDGET_H

#include "scene2d.h"

class OverlayedMapsWidget: public Scene2D
{
public:
    OverlayedMapsWidget(QWidget* parent = 0):
        Scene2D(parent),
        m_overlayWidget(0)
    {
    }
    void setOverlayWidget(QWidget *widget)
    {
        m_overlayWidget = widget;
    }
protected:
    void moveEvent(QMoveEvent *event)
    {
        if (m_overlayWidget)
            m_overlayWidget->move(event->pos());
    }
    void resizeEvent(QResizeEvent *event)
    {
        if (m_overlayWidget)
            m_overlayWidget->resize(event->size());
    }
    void hideEvent(QHideEvent *)
    {
        if (m_overlayWidget)
            m_overlayWidget->hide();
    }
    void showEvent(QShowEvent *)
    {
        if (m_overlayWidget)
            m_overlayWidget->show();
    }
    void closeEvent(QCloseEvent *)
    {
        if (m_overlayWidget)
            m_overlayWidget->close();
    }
private:
    QWidget *m_overlayWidget;
};
#endif // OVERLAYEDMAPSWIDGET_H

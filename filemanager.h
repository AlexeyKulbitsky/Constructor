#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include "model.h"
#include <QWebView>
class FileManager : public QObject
{
    Q_OBJECT
public:

    virtual ~FileManager(){ model = NULL; }

    virtual bool openFile(QString source) = 0;
    virtual bool saveFile(QString source) = 0;
    virtual void addMap(QWebView* map);
    static void setAutoSaveFileName(QString fileName);
    static void setAutoSaveTime(int time);
    static void setAutoSaveOn(bool status);

protected:
    Model* model;
    QVector<QWebView*> maps;
    static QString autoSaveFileName;
    static int autoSaveTime;
    static bool autoSaveOn;
    static QTimer timer;
};

#endif // FILEMANAGER_H

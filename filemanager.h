#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include "model.h"

class FileManager : public QObject
{
    Q_OBJECT
public:

    virtual ~FileManager(){ model = NULL; }

    virtual bool openFile(QString source) = 0;
    virtual bool saveFile(QString source) = 0;
    static void setAutoSaveFileName(QString fileName);
    static void setAutoSaveTime(int time);
    static void setAutoSaveOn(bool status);

protected:
    Model* model;

    static QString autoSaveFileName;
    static int autoSaveTime;
    static bool autoSaveOn;
    static QTimer timer;
};

#endif // FILEMANAGER_H

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include "model.h"

class FileManager
{

public:

    virtual ~FileManager(){ model = NULL; }

    virtual bool openFile(QString source) = 0;
    virtual bool saveFile(QString source) = 0;

protected:
    Model* model;
};

#endif // FILEMANAGER_H

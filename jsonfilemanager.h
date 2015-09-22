#ifndef JSONFILEMANAGER_H
#define JSONFILEMANAGER_H

#include "filemanager.h"

class JSONFileManager: public FileManager
{
public:
    JSONFileManager(Model* model);

    // FileManager interface
public:
    virtual bool openFile(QString source);
    virtual bool saveFile(QString source);
};

#endif // JSONFILEMANAGER_H

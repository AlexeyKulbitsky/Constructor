#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include "logger.h"

class TextureManager
{

protected:
    static TextureManager* manager;
    TextureManager(){}
    virtual ~TextureManager(){}
protected:
    QMap<QString, unsigned int> textureMap;
public:
    static TextureManager* getInstance()
    {
        if (!manager)
        {
            manager = new TextureManager();
        }
        return manager;
    }

    static bool deleteInstance()
    {
        if (manager)
        {
            delete manager;
            manager = NULL;
            return true;
        }
        return false;
    }

    int getTextureID(QString textureSource);
    int getID(QString texture);
    void addTexture(QString textureSource);

};

#endif // TEXTUREMANAGER_H

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>

class TextureManager: public QObject
{
    Q_OBJECT
protected:
    static TextureManager* manager;
    TextureManager(){}
    TextureManager(const TextureManager &source){}
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


};

#endif // TEXTUREMANAGER_H

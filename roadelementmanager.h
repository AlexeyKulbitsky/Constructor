#ifndef ROADELEMENTMANAGER_H
#define ROADELEMENTMANAGER_H

#include <QMap>
#include <QString>

struct Id
{
    unsigned int id;


    int count = 0;
};

class RoadElementManager
{
protected:
    static RoadElementManager* manager;
    RoadElementManager(){}
    virtual ~RoadElementManager(){}

    QMap<QString, Id> elements;

public:
    static RoadElementManager* getInstance()
    {
        if (!manager)
        {
            manager = new RoadElementManager();
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

    int getElementID(const QString &name);
    void setID(const QString &name, unsigned int ID);
    void addReference(const QString &name);
    void deleteReference(const QString &name);
};

#endif // ROADELEMENTMANAGER_H

#ifndef JSONFILEMANAGER_H
#define JSONFILEMANAGER_H

#include "filemanager.h"
#include "ruler.h"

class JSONFileManager: public FileManager
{
    Q_OBJECT
public:
    JSONFileManager(Model* model);
    virtual ~JSONFileManager();
    // FileManager interface
public:
    virtual bool openFile(QString source);
    virtual bool saveFile(QString source);
    static bool saveFile(QString source, QList<RoadElement*> &elements);
    static bool saveFile(QString source, RoadElement* element);
    RoadElement* readElementFromFile(QString source);

private:
    RoadSimple* readRoadSimple(QJsonObject& obj);
    RoadBroken *readRoadBroken(QJsonObject& obj);
    RoundingRoad *readRoundingRoad(QJsonObject& obj);
    Curve *readCurve(QJsonObject& obj);
    CompositeRoad *readCompositeRoad(QJsonObject& obj);
    LineBroken* readLineBroken(QJsonObject& obj);
    LineSimple *readLineSimple(QJsonObject& obj);
    SplitZone *readSplitZone(QJsonObject& obj);
    RailWay *readRailWay(QJsonObject& obj);
    Ruler* readRuler(QJsonObject& obj);
    Intersection* readIntersection(QJsonObject& obj);

public slots:
    void autoSaveFile();
};

#endif // JSONFILEMANAGER_H

#ifndef JSONFILEMANAGER_H
#define JSONFILEMANAGER_H

#include "filemanager.h"
#include "ruler.h"
#include "objfilemanager.h"


class JSONFileManager: public FileManager
{
    Q_OBJECT
public:
    JSONFileManager(Model* model);
    virtual ~JSONFileManager();
    void setCameraView(CameraView* camera);
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
    RoadElementOBJ* readRoadElementOBJ(QJsonObject& obj);
    VoltageLine* readVoltageLine(QJsonObject& obj);
    DoubleVoltageLine* readDoubleVoltageLine(QJsonObject& obj);
    Pole* readPole(QJsonObject& obj);
    Cube* readCube(QJsonObject& obj);
    Billboard* readBillboard(QJsonObject& obj);
    Arch* readArch(QJsonObject& obj);
    MapPlane* readMapPlane(QJsonObject& obj);
    void readYandexMaps(QJsonObject& obj);
    Camera* readCamera(QJsonObject& obj);
    
public slots:
    void autoSaveFile();
    
private:
    CameraView* cameraView;
};

#endif // JSONFILEMANAGER_H

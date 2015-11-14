#include "jsonfilemanager.h"

JSONFileManager::JSONFileManager(Model *model)
{
    this->model = model;
    timer.setInterval(1000 * 60 * 5);
    connect(&timer, SIGNAL(timeout()), &timer, SLOT(start()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(autoSaveFile()));
}

JSONFileManager::~JSONFileManager()
{

}



bool JSONFileManager::openFile(QString source)
{
    QFile file(source);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    else
    {
        QByteArray saveData = file.readAll();

        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

        QJsonArray ar = loadDoc.array();


        for (int i = 0; i < ar.size(); ++i)
        {
            RoadElement* element = NULL;
            QJsonObject obj = ar[i].toObject();
            QString name = obj["Name"].toString();
            if (name == "RoadSimple" ||
                name == "Crosswalk")
                element = readRoadSimple(obj);
            else
            if (name == "RoundingRoad")
                element = readRoundingRoad(obj);
            else
            if (name == "Curve")
                element = readCurve(obj);
            else
            if (name == "LineBroken" ||
                    name == "Tramways")
                element = readLineBroken(obj);
            else
            if (name == "LineSimple")
                element = readLineSimple(obj);
            else
            if (name == "RoadBroken")
                element = readRoadBroken(obj);
            else
            if (name == "SplitZone")
                element = readSplitZone(obj);
            else
            if (name == "RailWay")
                element = readRailWay(obj);
            else
            if (name == "Ruler")
                element = readRuler(obj);
            else
            if (name == "Intersection")
                element = readIntersection(obj);
            else
            if (name == "RoadElementOBJ")
                element = readRoadElementOBJ(obj);
            model->getGroup(element->getLayer()).push_back(element);
        }
        file.close();
    }
    return true;
}

bool JSONFileManager::saveFile(QString source)
{
    QJsonArray elements;

    for (int i =  0; i < model->getNumberOfGroups(); ++i)
    {
        for(QList<RoadElement*>::iterator it = model->getGroup(i).begin();
            it != model->getGroup(i).end(); ++it)
            elements.append((*it)->getJSONInfo());
    }

    QFile file(source);
    if (!file.open(QIODevice::WriteOnly))
    {
        return false;
    }
    else
    {
        QJsonDocument saveDoc(elements);
        file.write(saveDoc.toJson());
        file.close();
    }
    return true;
}

bool JSONFileManager::saveFile(QString source, QList<RoadElement*> &elements)
{
    QJsonArray elementsArray;

    for (int i = 0; i < elements.size(); ++i)
    {
        elementsArray.append(elements[i]->getJSONInfo());
    }
    QFile file(source);
    if (!file.open(QIODevice::WriteOnly))
    {
        return false;
    }
    else
    {
        QJsonDocument saveDoc(elementsArray);
        file.write(saveDoc.toJson());
        file.close();
    }
    return true;
}

bool JSONFileManager::saveFile(QString source, RoadElement *element)
{
    QJsonArray elementsArray;

    elementsArray.append(element->getJSONInfo());

    QFile file(source);
    if (!file.open(QIODevice::WriteOnly))
    {
        return false;
    }
    else
    {
        QJsonDocument saveDoc(elementsArray);
        file.write(saveDoc.toJson());
        file.close();
    }
    return true;
}

RoadElement *JSONFileManager::readElementFromFile(QString source)
{
    QFile file(source);
    if (!file.open(QIODevice::ReadOnly))
    {
        return NULL;
    }
    else
    {
        QByteArray saveData = file.readAll();

        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

        QJsonArray ar = loadDoc.array();

        QList<RoadElement*> elements;

        for (int i = 0; i < ar.size(); ++i)
        {
            RoadElement* element = NULL;
            QJsonObject obj = ar[i].toObject();
            QString name = obj["Name"].toString();
            if (name == "RoadSimple" ||
                name == "Crosswalk")
                element = readRoadSimple(obj);
            else
            if (name == "RoundingRoad")
                element = readRoundingRoad(obj);
            else
            if (name == "Curve")
                element = readCurve(obj);
            else
            if (name == "LineBroken" ||
                    name == "Tramways")
                element = readLineBroken(obj);
            else
            if (name == "LineSimple")
                element = readLineSimple(obj);
            else
            if (name == "RoadBroken")
                element = readRoadBroken(obj);
            else
            if (name == "SplitZone")
                element = readSplitZone(obj);
            else
            if (name == "RailWay")
                element = readRailWay(obj);
            else
            if (name == "Ruler")
                element = readRuler(obj);
            else
            if (name == "Intersection")
                element = readIntersection(obj);
            else
            if (name == "RoadElementOBJ")
                element = readRoadElementOBJ(obj);
            elements.push_back(element);
        }
        file.close();
        RoadElement* result = NULL;
        if (elements.size() > 1)
        {
            CompositeRoad* road = new CompositeRoad();
            for (int i = 0; i < elements.size(); ++i)
            {
                road->addElement(elements[i]);
            }
            result = road;
        }
        else
        {
            result = elements[0];
        }
        return result;
    }
}

RoadSimple *JSONFileManager::readRoadSimple(QJsonObject &obj)
{
    QJsonArray coordAr = obj["Vertices"].toArray();
    float x1 = coordAr[0].toDouble();
    float y1 = coordAr[1].toDouble();
    float x2 = coordAr[2].toDouble();
    float y2 = coordAr[3].toDouble();

    float leftWidth = obj["LeftWidth"].toDouble();
    float rightWidth = obj["LeftWidth"].toDouble();
    float leftBoardWidth = obj["LeftBoardWidth"].toDouble();
    float rightBoardWidth = obj["RightBoardWidth"].toDouble();

    bool fixed = obj["Fixed"].toBool();
    bool showRightBoard = obj["ShowRightBoard"].toBool();
    bool showLeftBoard = obj["ShowLeftBoard"].toBool();

    float width = obj["Width"].toDouble();
    int layer = obj["Layer"].toInt();
    QString name =  obj["Name"].toString();
    RoadSimple* road = NULL;

    if (obj["UseColor"].toBool() == true)
    {
        QJsonArray colorAr = obj["Color"].toArray();
        float r = colorAr[0].toDouble();
        float g = colorAr[1].toDouble();
        float b = colorAr[2].toDouble();
        float a = colorAr[3].toDouble();

        road = new RoadSimple(x1, y1, x2, y2, width, r, g, b, a, obj["Name"].toString(), layer);
    }
    else
    {
        QString texture1Source = obj["Texture1Source"].toString();
        float texture1USize = obj["Texture1USize"].toDouble();
        float texture1VSize = obj["Texture1VSize"].toDouble();
        QString texture2Source = obj["Texture2Source"].toString();
        float texture2USize = obj["Texture2USize"].toDouble();
        float texture2VSize = obj["Texture2VSize"].toDouble();

        road = new RoadSimple(x1, y1, x2, y2,
                                          leftWidth, rightWidth, leftBoardWidth, rightBoardWidth,
                                          showRightBoard, showLeftBoard,
                                          texture1Source, texture1USize, texture1VSize,
                                          texture2Source, texture2USize, texture2VSize,
                                          name, layer, fixed);

        QJsonArray lines = obj["Lines"].toArray();
        for (int i = 0; i < lines.size(); ++i)
        {
            QJsonObject line = lines[i].toObject();
            QJsonObject lineElement = line["Line"].toObject();
            QString name = lineElement["Name"].toString();
            LineLinked lineLinked;

            if (name == "LineBroken" ||
                name == "Tramways")
            {
                lineLinked.line = readLineBroken(lineElement);
            } else
            if (name == "SplitZone")
                lineLinked.line = readSplitZone(lineElement);
            else
            if (name == "LineSimple")
                lineLinked.line = readLineSimple(lineElement);

            lineLinked.lineType = line["LineType"].toInt();
            lineLinked.lineWidth = line["LineWidth"].toDouble();
            lineLinked.rightSide = line["RightSide"].toBool();
            lineLinked.step = line["Step"].toDouble();
            lineLinked.beginSide = line["BeginSide"].toBool();
            lineLinked.beginStep = line["BeginStep"].toDouble();
            lineLinked.endStep = line["EndStep"].toDouble();
            lineLinked.differentDirections = line["DifferentDirections"].toBool();
            lineLinked.splitZoneWidth = line["SplitZoneWidth"].toDouble();
            road->addLine(lineLinked);
        }


    }
    int Id = obj["Id"].toInt();
    road->setId(Id);
    return road;
}

RoadBroken* JSONFileManager::readRoadBroken(QJsonObject &obj)
{
    int layer = obj["Layer"].toInt();
    QString name =  obj["Name"].toString();

    QJsonArray tempVertexArray = obj["VertexArray"].toArray();
    QVector<float> vertexArray;
    vertexArray.resize(tempVertexArray.size());
    for (int i = 0; i < tempVertexArray.size(); ++i)
        vertexArray[i] = tempVertexArray[i].toDouble();


    QJsonArray tempVertexArrayRight = obj["VertexArrayRight"].toArray();
    QVector<float> vertexArrayRight;
    vertexArrayRight.resize(tempVertexArrayRight.size());
    for (int i = 0; i < tempVertexArrayRight.size(); ++i)
        vertexArrayRight[i] = tempVertexArrayRight[i].toDouble();

    QJsonArray tempVertexArrayLeft = obj["VertexArrayLeft"].toArray();
    QVector<float> vertexArrayLeft;
    vertexArrayLeft.resize(tempVertexArrayLeft.size());
    for (int i = 0; i < tempVertexArrayLeft.size(); ++i)
        vertexArrayLeft[i] = tempVertexArrayLeft[i].toDouble();

    bool showRightBoard = obj["ShowRightBoard"].toBool();
    bool showLeftBoard = obj["ShowLeftBoard"].toBool();

    QString texture1 = obj["Texture1Source"].toString();
    float texture_1Usize = obj["Texture1USize"].toDouble();
    float texture_1Vsize = obj["Texture1VSize"].toDouble();

    QString texture2 = obj["Texture2Source"].toString();
    float texture_2Usize = obj["Texture2USize"].toDouble();
    float texture_2Vsize = obj["Texture2VSize"].toDouble();

    bool fixed = obj["Fixed"].toBool();

    RoadBroken* road = new RoadBroken(vertexArray,
                                      vertexArrayRight,
                                      vertexArrayLeft,
                                      texture1, texture_1Usize, texture_1Vsize,
                                      texture2, texture_2Usize, texture_2Vsize,
                                      showRightBoard, showLeftBoard, fixed,
                                      name, layer);



    QJsonArray lines = obj["Lines"].toArray();
    for (int i = 0; i < lines.size(); ++i)
    {
        QJsonObject line = lines[i].toObject();
        QJsonObject lineElement = line["Line"].toObject();
        QString name = lineElement["Name"].toString();
        LineBrokenLinkedToRoadBroken lineLinked;
        if (name == "LineBroken" ||
            name == "Tramways")
        {
            lineLinked.line = readLineBroken(lineElement);
        } else
        if (name == "SplitZone")
            lineLinked.line = readSplitZone(lineElement);
        lineLinked.lineType = line["LineType"].toInt();
        lineLinked.lineWidth = line["LineWidth"].toDouble();
        lineLinked.rightSide = line["RightSide"].toBool();
        lineLinked.step = line["Step"].toDouble();
        lineLinked.beginSide = line["BeginSide"].toBool();
        lineLinked.beginStep = line["BeginStep"].toDouble();
        lineLinked.endStep = line["EndStep"].toDouble();
        lineLinked.differentDirections = line["DifferentDirections"].toBool();
        lineLinked.splitZoneWidth = line["SplitZoneWidth"].toDouble();
        lineLinked.beginRounding = line["BeginRounding"].toBool();
        lineLinked.endRounding = line["EndRounding"].toBool();
        lineLinked.textureSource = line["TextureSource"].toString();
        road->addLine(lineLinked);
    }

    int Id = obj["Id"].toInt();
    road->setId(Id);

    return road;
}

RoundingRoad* JSONFileManager::readRoundingRoad(QJsonObject &obj)
{

    int layer = obj["Layer"].toInt();
    QString name =  obj["Name"].toString();

    float xCenterNearRadius = obj["xCenterNearRadius"].toDouble();
    float yCenterNearRadius = obj["yCenterNearRadius"].toDouble();
    float angel1NearRadius = obj["Angel1NearRadius"].toDouble();
    float angel2NearRadius = obj["Angel2NearRadius"].toDouble();
    float nearRadius = obj["NearRadius"].toDouble();

    float xCenterFarRadius = obj["xCenterFarRadius"].toDouble();
    float yCenterFarRadius = obj["yCenterFarRadius"].toDouble();
    float angel1FarRadius = obj["Angel1FarRadius"].toDouble();
    float angel2FarRadius = obj["Angel2FarRadius"].toDouble();
    float farRadius = obj["FarRadius"].toDouble();

    bool showNearBoard = obj["ShowNearBoard"].toBool();
    bool showFarBoard = obj["ShowFarBoard"].toBool();

    QString texture1 = obj["Texture1Source"].toString();
    float texture_1Usize = obj["Texture1USize"].toDouble();
    float texture_1Vsize = obj["Texture1VSize"].toDouble();

    QString texture2 = obj["Texture2Source"].toString();
    float texture_2Usize = obj["Texture2USize"].toDouble();
    float texture_2Vsize = obj["Texture2VSize"].toDouble();

    bool fixed = obj["Fixed"].toBool();

    float numberOfSides = obj["NumberOfSides"].toDouble();

    float nearBoardWidth = obj["NearBoardWidth"].toDouble();
    float farBoardWidth = obj["FarBoardWidth"].toDouble();

    RoundingRoad* road = new RoundingRoad(xCenterNearRadius, yCenterNearRadius, nearRadius, angel1NearRadius, angel2NearRadius,
                                          xCenterFarRadius, yCenterFarRadius, farRadius, angel1FarRadius, angel2FarRadius,
                                          numberOfSides, name, layer,
                                          nearBoardWidth, farBoardWidth,
                                          showNearBoard, showFarBoard, fixed,
                                          texture1, texture_1Usize, texture_1Vsize,
                                          texture2, texture_2Usize, texture_2Vsize);

    QJsonArray lines = obj["Lines"].toArray();
    for (int i = 0; i < lines.size(); ++i)
    {
        QJsonObject line = lines[i].toObject();
        QJsonObject lineElement = line["Line"].toObject();
        QString name = lineElement["Name"].toString();
        LineBrokenLinked lineLinked;

        if (name == "LineBroken" ||
            name == "Tramways")
        {
            lineLinked.line = readLineBroken(lineElement);
        } else
        if (name == "SplitZone")
            lineLinked.line = readSplitZone(lineElement);
        else
        if (name == "LineSimple")
            lineLinked.line = readLineSimple(lineElement);

        lineLinked.lineType = line["LineType"].toInt();
        lineLinked.lineWidth = line["LineWidth"].toDouble();
        lineLinked.nearSide = line["NearSide"].toBool();
        lineLinked.step = line["Step"].toDouble();
        lineLinked.beginSide = line["BeginSide"].toBool();
        lineLinked.beginStep = line["BeginStep"].toDouble();
        lineLinked.endStep = line["EndStep"].toDouble();
        lineLinked.differentDirections = line["DifferentDirections"].toBool();
        lineLinked.splitZoneWidth = line["SplitZoneWidth"].toDouble();
        road->addLine(lineLinked);
    }

    int Id = obj["Id"].toInt();
    road->setId(Id);

    return road;
}

Curve* JSONFileManager::readCurve(QJsonObject &obj)
{
    int layer = obj["Layer"].toInt();
    QString name =  obj["Name"].toString();

    QString texture1 = obj["Texture1Source"].toString();
    float texture_1Usize = obj["Texture1USize"].toDouble();
    float texture_1Vsize = obj["Texture1VSize"].toDouble();

    QString texture2 = obj["Texture2Source"].toString();
    float texture_2Usize = obj["Texture2USize"].toDouble();
    float texture_2Vsize = obj["Texture2VSize"].toDouble();

    bool fixed = obj["Fixed"].toBool();
    bool showBoard = obj["ShowBoard"].toBool();

    float numberOfSides = obj["NumberOfSides"].toInt();

    float boardWidth = obj["BoardWidth"].toDouble();
    float angleRounding = obj["AngleRounding"].toDouble();

    QJsonArray tempControlPoints = obj["ControlPoints"].toArray();
    float controls[9];
    for (int i = 0; i < tempControlPoints.size(); ++i)
        controls[i] = tempControlPoints[i].toDouble();

    Curve* curve = new Curve(controls, tempControlPoints.size(),
                             texture1, texture_1Usize, texture_1Vsize,
                             texture2, texture_2Usize, texture_2Vsize,
                             numberOfSides, angleRounding, boardWidth,
                             fixed, showBoard, layer, name);
    int Id = obj["Id"].toInt();
    curve->setId(Id);

    return curve;
}

CompositeRoad* JSONFileManager::readCompositeRoad(QJsonObject &obj)
{
    bool fixed = obj["Fixed"].toBool();

    CompositeRoad* road = new CompositeRoad();

    QJsonArray tempElements = obj["Elements"].toArray();;
    for (int i = 0; i < tempElements.size(); ++i)
    {
        RoadElement* element = NULL;
        QJsonObject obj = tempElements[i].toObject();
        QString name = obj["Name"].toString();
        if (name == "RoadSimple" ||
            name == "Crosswalk")
            element = readRoadSimple(obj);
        else
        if (name == "RoundingRoad")
            element = readRoundingRoad(obj);
        else
        if (name == "Curve")
            element = readCurve(obj);
        else
        if (name == "LineBroken" ||
                name == "Tramways")
            element = readLineBroken(obj);
        else
        if (name == "LineSimple")
            element = readLineSimple(obj);
        else
        if (name == "RoadBroken")
            element = readRoadBroken(obj);
        else
        if (name == "SplitZone")
            element = readSplitZone(obj);
        else
        if (name == "RailWay")
            element = readRailWay(obj);
        else
        if (name == "RoadElementOBJ")
            element = readRoadElementOBJ(obj);
        road->addElement(element);
    }

    road->setFixed(fixed);

    int Id = obj["Id"].toInt();
    road->setId(Id);

    return road;
}

LineBroken *JSONFileManager::readLineBroken(QJsonObject &obj)
{
    int layer = obj["Layer"].toInt();
    QString name =  obj["Name"].toString();
    bool fixed = obj["Fixed"].toBool();
    float width = obj["Width"].toDouble();


    QString textureSource = obj["TextureSource"].toString();
    float textureSize = obj["TextureSize"].toDouble();

    QJsonArray temp = obj["VertexArrayForAxis"].toArray();
    int size = temp.size();
    float axisVertices[size];
    for (int i = 0; i < size; ++i)
    {
        axisVertices[i] = temp[i].toDouble();
    }
    LineBroken* line = new LineBroken(width, axisVertices, size, textureSource, textureSize, name, layer);
    line->setFixed(fixed);

    int Id = obj["Id"].toInt();
    line->setId(Id);

    return line;
}

LineSimple* JSONFileManager::readLineSimple(QJsonObject &obj)
{
    QJsonArray coordAr = obj["Vertices"].toArray();
    float x1 = coordAr[0].toDouble();
    float y1 = coordAr[1].toDouble();
    float x2 = coordAr[2].toDouble();
    float y2 = coordAr[3].toDouble();

    int layer = obj["Layer"].toInt();
    QString name =  obj["Name"].toString();
    bool fixed = obj["Fixed"].toBool();
    float width = obj["Width"].toDouble();
    float textureSize = obj["TextureSize"].toDouble();
    QString textureSource = obj["TextureSource"].toString();
    LineSimple* line = new LineSimple(x1, y1, x2, y2, width, textureSource, textureSize, name, layer);
    line->setFixed(fixed);

    int Id = obj["Id"].toInt();
    line->setId(Id);

    return line;

}

SplitZone* JSONFileManager::readSplitZone(QJsonObject &obj)
{
    bool fixed = obj["Fixed"].toBool();
    float width = obj["Width"].toDouble();
    int type = obj["Type"].toInt();
    QString description =  obj["Description"].toString();
    bool beginRounding = obj["BeginRounding"].toBool();
    bool endRounding = obj["EndRounding"].toBool();

    QJsonArray temp = obj["AxisArray"].toArray();
    int size = temp.size();
    float pointsArray[size];
    for (int i = 0; i < size; ++i)
    {
        pointsArray[i] = temp[i].toDouble();
    }

    SplitZone* splitZone = NULL;
    switch (type)
    {
    case 0:
        splitZone = new SplitZone(pointsArray, size,
                                  width,
                                  beginRounding,
                                  endRounding,
                                  description);
        break;
    case 1:
    case 2:
    {
        float height = obj["Height"].toDouble();

        QString texture1 = obj["Texture1Source"].toString();
        float texture_1Usize = obj["Texture1USize"].toDouble();
        float texture_1Vsize = obj["Texture1VSize"].toDouble();

        QString texture2 = obj["Texture2Source"].toString();
        float texture_2Usize = obj["Texture2USize"].toDouble();
        float texture_2Vsize = obj["Texture2VSize"].toDouble();

        splitZone = new SplitZone(pointsArray, size,
                                  width,
                                  beginRounding,
                                  endRounding,
                                  type,
                                  height,
                                  texture1,
                                  texture_1Usize, texture_1Vsize,
                                  texture2,
                                  texture_2Usize, texture_2Vsize,
                                  description);
    }
        break;
    default:
        break;
    }
    splitZone->setFixed(fixed);

    int Id = obj["Id"].toInt();
    splitZone->setId(Id);

    return splitZone;

}

RailWay* JSONFileManager::readRailWay(QJsonObject &obj)
{
    bool fixed = obj["Fixed"].toBool();

    QString texture =  obj["TextureSource"].toString();
    float textureUsize = obj["TextureUSize"].toDouble();
    float textureVsize = obj["TextureVSize"].toDouble();

    QJsonArray temp = obj["AxisVertexArray"].toArray();
    QVector<float> axisArray;
    axisArray.resize(temp.size());
    for (int i = 0; i < temp.size(); ++i)
    {
        axisArray[i] = temp[i].toDouble();
    }

    RailWay* railWay = new RailWay(axisArray, texture, textureUsize, textureVsize);
    railWay->setFixed(fixed);

    int Id = obj["Id"].toInt();
    railWay->setId(Id);

    return railWay;

}

Ruler *JSONFileManager::readRuler(QJsonObject &obj)
{
    Ruler* ruler = new Ruler();

    float x1 = obj["X1"].toDouble();
    float y1 = obj["Y1"].toDouble();
    float z1 = obj["Z1"].toDouble();

    float x2 = obj["X2"].toDouble();
    float y2 = obj["Y2"].toDouble();
    float z2 = obj["Z2"].toDouble();

    int elementId1 = obj["ElementId1"].toInt();
    bool ok = false;
    if (elementId1 >= 0)
    {
        for (int i = 0; i < model->getNumberOfGroups(); ++i)
        {
            for (int j = 0; j < model->getGroup(i).size(); ++j)
            {
                if (model->getGroup(i)[j]->getId() == elementId1)
                {
                    int index = obj["Index1"].toInt();
                    ruler->setStartLinkPoint(model->getGroup(i)[j], index, x1, y1, z1);
                    ok = true;
                    break;
                }
            }
            if (ok) break;
        }
    }
    ruler->setStartPoint(x1, y1, z1);
    ruler->activateStartPoint();

    int elementId2 = obj["ElementId2"].toInt();
    ok = false;
    if (elementId2 >= 0)
    {
        for (int i = 0; i < model->getNumberOfGroups(); ++i)
        {
            for (int j = 0; j < model->getGroup(i).size(); ++j)
            {
                if (model->getGroup(i)[j]->getId() == elementId2)
                {
                    int index = obj["Index2"].toInt();
                    ruler->setEndLinkPoint(model->getGroup(i)[j], index, x2, y2, z2);
                    ok = true;
                    break;
                }
            }
            if (ok) break;
        }
    }
    ruler->setEndPoint(x2, y2, z2);
    ruler->activateEndPoint();

    int Id = obj["Id"].toInt();
    ruler->setId(Id);

    return ruler;
}

Intersection *JSONFileManager::readIntersection(QJsonObject &obj)
{
    QJsonArray tempRoads = obj["Roads"].toArray();
    QVector<RoadSimple*> roads;
    roads.resize(tempRoads.size());
    for (int i = 0; i < tempRoads.size(); ++i)
    {
        QJsonObject temp = tempRoads[i].toObject();
        roads[i] = readRoadSimple(temp);
    }

    QJsonArray tempCurves = obj["Curves"].toArray();
    QVector<Curve*> curves;
    curves.resize(tempCurves.size());
    for (int i = 0; i < tempCurves.size(); ++i)
    {
        QJsonObject temp = tempCurves[i].toObject();
        curves[i] = readCurve(temp);
    }

    Intersection* intersection = new Intersection(roads, curves);
    int Id = obj["Id"].toInt();
    intersection->setId(Id);
    bool fixed = obj["Fixed"].toBool();
    intersection->setFixed(fixed);
    return intersection;
}

RoadElementOBJ *JSONFileManager::readRoadElementOBJ(QJsonObject &obj)
{
    bool fixed = obj["Fixed"].toBool();
    int Id = obj["Id"].toInt();
    QString folder = obj["Folder"].toString();
    QString filename = obj["Filemane"].toString();

    float x = obj["DeltaX"].toDouble();
    float y = obj["DeltaY"].toDouble();
    float zRot = obj["ZRot"].toDouble();
    float xScale = obj["XScale"].toDouble();
    float yScale = obj["XScale"].toDouble();
    float zScale = obj["YScale"].toDouble();

    RoadElementOBJ* element = new RoadElementOBJ(x,y,folder,filename);

    OBJFileManager* manager = new OBJFileManager(model);
    manager->loadOBJ(folder,filename, element->meshes, 2.374f, element->scaleFactor);

    float scaleFactor = obj["ScaleFactor"].toDouble();
    element->setId(Id);
    element->setFixed(fixed);
    element->setXScale(xScale);
    element->setYScale(yScale);
    element->setZScale(zScale);
    element->setZRotation(zRot);
    element->setScale(scaleFactor);
    return element;
}


void JSONFileManager::autoSaveFile()
{
    //QMessageBox::about(0, "df", QApplication::applicationDirPath() + "/" + autoSaveFileName + ".json");
    QString fileName = QApplication::applicationDirPath() + "/" + autoSaveFileName + ".json";
    saveFile(fileName);
}

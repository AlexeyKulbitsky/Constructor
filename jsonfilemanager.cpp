#include "jsonfilemanager.h"

JSONFileManager::JSONFileManager(Model *model)
{
    this->model = model;
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
            if (ar[i].toObject()["Name"].toString() == "RoadSimple" ||
                ar[i].toObject()["Name"].toString() == "Crosswalk")
            {
                QJsonObject obj = ar[i].toObject();
                QJsonArray coordAr = obj["Vertices"].toArray();
                float x1 = coordAr[0].toDouble();
                float y1 = coordAr[1].toDouble();
                float x2 = coordAr[2].toDouble();
                float y2 = coordAr[3].toDouble();

                float width = obj["Width"].toDouble();
                int layer = obj["Layer"].toInt();
                if (obj["UseColor"].toBool() == true)
                {
                    QJsonArray colorAr = obj["Color"].toArray();
                    float r = colorAr[0].toDouble();
                    float g = colorAr[1].toDouble();
                    float b = colorAr[2].toDouble();
                    float a = colorAr[3].toDouble();

                    model->getGroup(layer).push_back(new RoadSimple(x1, y1, x2, y2, width, r, g, b, a, ar[i].toObject()["Name"].toString(), layer));
                    model->setModified(true);
                }
                else
                {
                    float textureSize = obj["TextureSize"].toDouble();
                    QString textureSource = obj["TextureSource"].toString();
                    //model->getGroup(layer).push_back(new RoadSimple(x1, y1, x2, y2, width, textureSource, textureSize, ar[i].toObject()["Name"].toString(), layer));
                    model->setModified(true);
                }

            }


            if (ar[i].toObject()["Name"].toString() == "LineSolidBroken" ||
                    ar[i].toObject()["Name"].toString() == "LineIntermittentBroken" ||
                    ar[i].toObject()["Name"].toString() == "LineDoubleSolidBroken" ||
                    ar[i].toObject()["Name"].toString() == "LineDoubleSolidIntermittentBroken" ||
                    ar[i].toObject()["Name"].toString() == "TramwaysBroken" ||
                    ar[i].toObject()["Name"].toString() == "RailroadBroken")
            {
                QJsonObject obj = ar[i].toObject();
                QJsonArray coordAr = obj["Vertices"].toArray();
                int size = coordAr.size();
                GLfloat* array = new GLfloat[size];
                for (int i = 0; i < size; ++i)
                {
                    array[i] = coordAr[i].toDouble();
                }
                int layer = obj["Layer"].toInt();
                float width = obj["Width"].toDouble();

                if (obj["UseColor"].toBool() == true)
                {
                    QJsonArray colorAr = obj["Color"].toArray();
                    float r = colorAr[0].toDouble();
                    float g = colorAr[1].toDouble();
                    float b = colorAr[2].toDouble();
                    float a = colorAr[3].toDouble();
                    model->getGroup(layer).push_back(new LineBroken(width, array, size, r, g, b, a, ar[i].toObject()["Name"].toString(), layer));
                    model->setModified(true);
                }
                else
                {
                    float textureSize = obj["TextureSize"].toDouble();
                    QString textureSource = obj["TextureSource"].toString();
                    model->getGroup(layer).push_back(new LineBroken(width, array, size, textureSource, textureSize, ar[i].toObject()["Name"].toString(), layer));
                    model->setModified(true);
                }
            }
            if (ar[i].toObject()["Name"].toString() == "LineSolid" ||
                    ar[i].toObject()["Name"].toString() == "LineIntermittent" ||
                    ar[i].toObject()["Name"].toString() == "LineDoubleSolid" ||
                    ar[i].toObject()["Name"].toString() == "LineDoubleSolidIntermittent" ||
                    ar[i].toObject()["Name"].toString() == "Tramways" ||
                    ar[i].toObject()["Name"].toString() == "Railroad")
            {
                QJsonObject obj = ar[i].toObject();
                QJsonArray coordAr = obj["Vertices"].toArray();
                float x1 = coordAr[0].toDouble();
                float y1 = coordAr[1].toDouble();
                float x2 = coordAr[2].toDouble();
                float y2 = coordAr[3].toDouble();

                int layer = obj["Layer"].toInt();

                float width = obj["Width"].toDouble();

                if (obj["UseColor"].toBool() == true)
                {
                    QJsonArray colorAr = obj["Color"].toArray();
                    float r = colorAr[0].toDouble();
                    float g = colorAr[1].toDouble();
                    float b = colorAr[2].toDouble();
                    float a = colorAr[3].toDouble();
                    model->getGroup(layer).push_back(new LineSimple(x1, y1, x2, y2, width, r, g, b, a, ar[i].toObject()["Name"].toString(), layer));
                    model->setModified(true);
                }
                else
                {
                    float textureSize = obj["TextureSize"].toDouble();
                    QString textureSource = obj["TextureSource"].toString();
                    model->getGroup(layer).push_back(new LineSimple(x1, y1, x2, y2, width, textureSource, textureSize, ar[i].toObject()["Name"].toString(), layer));
                    model->setModified(true);
                }
            }
            if (ar[i].toObject()["Name"].toString() == "RoadBroken")
            {
                QJsonObject obj = ar[i].toObject();
                QJsonArray coordAr = obj["Vertices"].toArray();
                int size = coordAr.size();

                QVector<GLfloat> array;
                for (int i = 0; i < size; ++i)
                {
                    array.push_back(coordAr[i].toDouble());
                }
                int layer = obj["Layer"].toInt();
                    QJsonArray colorAr = obj["Color"].toArray();
                    float r = colorAr[0].toDouble();
                    float g = colorAr[1].toDouble();
                    float b = colorAr[2].toDouble();
                    float a = colorAr[3].toDouble();
                    model->getGroup(layer).push_back(new RoadBroken(array, r, g, b, a, ar[i].toObject()["Name"].toString(), layer));
                    model->setModified(true);

            }
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
        for(std::list<RoadElement*>::iterator it = model->getGroup(i).begin();
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

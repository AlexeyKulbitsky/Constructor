#include "roadelement.h"

bool RoadElement::Ids[SIZE] = {false};

QUndoStack* RoadElement::undoStack = NULL;

std::vector<vec3> RoadElement::getCoordOfControl(int)
{
    std::vector<vec3> p; return p;
}


void RoadElement::setCoordForControl(int, std::vector<vec3>&)
{

}


QString RoadElement::getName()
{
    return name;
}


QJsonObject RoadElement::getJSONInfo()
{
    QJsonObject a; return a;
}


void RoadElement::clearProperties(QLayout *)

{
    /*
        QLayoutItem *item;
        while((layout->count() > 0)) {
            item = layout->itemAt(0);
            if (item->layout()) {
                clearProperties(item->layout());
                delete item->layout();
            }
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
      */
   // QLayoutItem *child;
  //  while ((child = layout->takeAt(0)) != 0) {
  //      delete child;
  //  }
}


void RoadElement::setModel(Model *model)
{
    this->model = model;
}


void RoadElement::addElement(RoadElement *)
{

}


void RoadElement::deleteElement(int)
{

}


int RoadElement::getNumberOfElements()
{
    return 0;
}


RoadElement *RoadElement::getElement(int)
{
    return NULL;
}


RoadElement::RoadElement()
{
    for (unsigned i = 0; i < SIZE; ++i)
    {
        if (Ids[i] == false)
        {
            Id = i;
            Ids[i] = true;
            break;
        }
    }
    showMeasurements = true;
}

RoadElement::~RoadElement()
{
    Ids[Id] = false;
}


void RoadElement::rotate(float, float, float, float)
{

}


void RoadElement::setStepDialog(StepDialog *dialog)
{
    if (dialog)
        stepDialog = dialog;
}


void RoadElement::setStepDialogs(StepDialog **dialogs, int size)
{
    if (!dialogs)
        return;
    for (int i = 0; i < size; ++i)
        stepDialogs[i] = dialogs[i];
}


void RoadElement::deleteLine(RoadElement *)
{

}

void RoadElement::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    gluProject(x, y, z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
    wy=viewport[3]-wy;
}

void RoadElement::getWorldCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    if (log)
    Logger::getLogger()->infoLog() << "RoadElement::getWorldCoord(double x, double y, double z, double &wx, double &wy, double &wz)\n";
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
    y = viewport[3]-y;
    gluUnProject(x, y, z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
}

void RoadElement::setId(int Id)
{
    Ids[this->Id] = false;
    this->Id = Id;
    Ids[Id] = true;
}

bool RoadElement::calculateLinesIntersection(float a1, float b1, float c1, float a2, float b2, float c2, float &x, float &y)
{
    float eps = 1e-9;
    float den = a1 * b2 - a2 * b1;
    float numX = c1 * b2 - c2 * b1;
    float numY = a1 * c2 - a2 * c1;
    if (fabs(den) < eps)
        return false;
    if (fabs(numX) < eps && fabs(numY) < eps)
        return false;
    x = (-1.0f) * numX / den;
    y = (-1.0f) * numY / den;

    return true;
}

float RoadElement::calculateAngle(vec2 p1, vec2 p2, vec2 p3, vec2 p4)
{
    float dx1 = p2.x - p1.x;
    float dy1 = p2.y - p1.y;
    float r1 = sqrt(dx1*dx1 + dy1*dy1);
    float dx2 = p4.x - p3.x;
    float dy2 = p4.y - p3.y;
    float r2 = sqrt(dx2*dx2 + dy2*dy2);
    float t = (dx1*dx2 + dy1*dy2) / (r1 * r2);
    if (t > 1)
        t = 1.0f;
    if (t < -1)
        t = -1.0f;
    float angle = acos(t);
    float res = dx1*dy2 - dx2*dy1;
    float pi = 3.14159265f;
    if (res < 0)
        angle = 2.0f * pi - angle;
    return angle;
}

float RoadElement::calculateAngle(vec3 v1, vec3 v2)
{
    float r1 = sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
    float r2 = sqrt(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);
    float num = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    float den = r1 * r2;
    float t = num / den;
    //qDebug() << t;
    if (t > 1)
        t = 1.0f;
    if (t < -1)
        t = -1.0f;
    float angle = acosf(t);

    float res = v1.x * v2.y - v2.x * v1.y;
    if (res < 0)
    {
        float pi = 3.14159265f;
        angle = 2.0f * pi - angle;
    }
    return angle;
}

void RoadElement::setShowMeasurements(bool status)
{
    showMeasurements = status;
}


RoadElementMimeData::RoadElementMimeData(RoadElement *element)
{
    this->elements << element->getCopy();
    myFormats << "constructor/roadelement";
}

RoadElementMimeData::RoadElementMimeData(QList<RoadElement *> &elements)
{
    for (int i = 0; i < elements.size(); ++i)
        this->elements << elements[i]->getCopy();
    myFormats << "constructor/roadelement";
}

RoadElementMimeData::RoadElementMimeData()
{
    myFormats << "constructor/roadelement";
}

RoadElementMimeData::~RoadElementMimeData()
{
    //QMessageBox::about(0, "JHF","RoadElementMimeData::~RoadElementMimeData()");
    for (int i = 0; i < elements.size(); ++i)
    {
        if (elements[i])
            delete elements[i];
        elements[i] = NULL;
    }
}

QList<RoadElement *> RoadElementMimeData::getElements() const
{
    return elements;
}


QVariant RoadElementMimeData::retrieveData(const QString &mimetype, QVariant::Type preferredType) const
{
    if (mimetype == "constructor/roadelement")
        return QVariant::fromValue(elements);
    else
        return QMimeData::retrieveData(mimetype, preferredType);
}

QStringList RoadElementMimeData::formats() const
{
    return myFormats;
}


bool RoadElementMimeData::hasFormat(const QString &mimetype) const
{
    if (mimetype == "constructor/roadelement")
        return true;
    else
        return false;
}

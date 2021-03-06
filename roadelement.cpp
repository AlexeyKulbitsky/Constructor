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


void RoadElement::clearProperties(QLayout *layout)

{
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        if (item->layout() != NULL)
        {
            clearProperties(item->layout());
            delete item->layout();
        }
        if (item->widget() != NULL)
        {
            delete item->widget();
        }
    }
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
    drawRoad = drawLines = true;
}

RoadElement::~RoadElement()
{
    Ids[Id] = false;
}


void RoadElement::rotate(float, float, float, float)
{

}

void RoadElement::rotate(float angle, float x0, float y0, float &x, float &y)
{
    float tx = 0.0f, ty = 0.0f;
    x -= x0;
    y -= y0;
    tx = x;
    ty = y;
    x = tx * cos(angle) - ty * sin(angle);
    y = tx * sin(angle) + ty * cos(angle);
    x += x0;
    y += y0;
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

bool RoadElement::calculateLinesIntersection(vec2 p1, vec2 p2, vec2 s1, vec2 s2, float& x, float& y)
{
    float a1, a2, b1, b2, c1, c2;

    a1 = p2.y - p1.y;
    b1 = p1.x - p2.x;
    c1 = p1.y * (p2.x - p1.x) - p1.x * (p2.y - p1.y);

    a2 = s2.y - s1.y;
    b2 = s1.x - s2.x;
    c2 = s1.y * (s2.x - s1.x) - s1.x * (s2.y - s1.y);

    return calculateLinesIntersection(a1, b1, c1, a2, b2, c2, x, y);
}

int RoadElement::calculateLineCircleIntersection(float x0, float y0, float r,
                                                 float x1, float y1,
                                                 float x2, float y2,
                                                 float &xa, float &ya,
                                                 float &xb, float &yb)
{
    float a1, a2, b1, b2, c1, c2;
    a1 = y2 - y1;
    b1 = x1 - x2;
    c1 = y1 * (x2 - x1) - x1 * (y2 - y1);

    a2 = b1;
    b2 = -a1;
    c2 = a1 * y0 - b1 * x0;
    float xTemp, yTemp;
    if (!calculateLinesIntersection(a1, b1, c1, a2, b2, c2, xTemp, yTemp))
        return 0;

    float d = sqrt((xTemp - x0) * (xTemp - x0) + (yTemp - y0) * (yTemp - y0));
    if (d > r)
    {
        return 0;
    }
    else
    {
        if (fabs(d - r) < 1e-5)
        {
            xa = xTemp;
            ya = yTemp;
            return 1;
        }
        else
        {
            if (d < r)
            {
                float l = sqrt(r * r - d * d);
                float dr = sqrt(a1 * a1 + b1 * b1);
                xa = xTemp - b1 * l / dr;
                ya = yTemp + a1 * l / dr;
                xb = xTemp + b1 * l / dr;
                yb = yTemp - a1 * l / dr;
                return 2;
            }
        }
    }
}

float RoadElement::calculateAngle(vec2 p1, vec2 p2, vec2 p3, vec2 p4)
{
//    qDebug() << "----------------";
    float dx1 = p2.x - p1.x;
    float dy1 = p2.y - p1.y;
    float r1 = sqrt(dx1*dx1 + dy1*dy1);
    float dx2 = p4.x - p3.x;
    float dy2 = p4.y - p3.y;
    float r2 = sqrt(dx2*dx2 + dy2*dy2);
    float t = (dx1*dx2 + dy1*dy2) / (r1 * r2);
//    qDebug() << "T =" << t;
    if (t > 1.0f)
        t = 1.0f;
    if (t < -1.0f)
        t = -1.0f;
    float angle = acos(t);
//    qDebug() << "Ang1 =" << angle;
//    float res = dx1*dy2 - dx2*dy1;
//    float pi = 3.14159265f;
//    if (res < 0)
//        angle = 2.0f * pi - angle;
//    qDebug() << "Ang2 =" << angle;
//    qDebug() << "----------------";
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

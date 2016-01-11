#include "ruler.h"

bool Ruler::log = true;

Ruler::Ruler()
{
    name = "Ruler";
    x1 = y1 = z1 = x2 = y2 = z2 = 0.0f;
    x1_ptr = y1_ptr = z1_ptr = x2_ptr = y2_ptr = z2_ptr = NULL;
    startPointIsActivated = false;
    endPointIsActivated = false;
    fixed = false;
    selected = true;
    layer = 3;
    element1 = element2 = NULL;
    elementId1 = elementId2 = -1;
    index1 = index2 = -1;
}

Ruler::Ruler(const Ruler &source)
{
    x1 = source.x2;
    y1 = source.y2;
    z1 = source.z2;
    x2 = source.x2;
    y2 = source.y2;
    z2 = source.z2;
    x1_ptr = source.x1_ptr;
    y1_ptr = source.y1_ptr;
    z1_ptr = source.z1_ptr;
    x2_ptr = source.x2_ptr;
    y2_ptr = source.y2_ptr;
    z2_ptr = source.z2_ptr;
    startPointIsActivated = source.startPointIsActivated;
    endPointIsActivated = source.endPointIsActivated;
    selected = source.selected;
    fixed = source.fixed;
    layer = source.layer;
    indexOfSelectedControl = source.indexOfSelectedControl;

    element1 = source.element1;
    element2 = source.element2;
    index1 = source.index1;
    index2 = source.index2;
    partOfStartLine = source.partOfStartLine;
    partOfEndLine = source.partOfEndLine;
}

Ruler::~Ruler()
{
    element1 = NULL;
    element2 = NULL;
}

void Ruler::activateStartPoint()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::activateStartPoint()\n";
    startPointIsActivated = true;
}

void Ruler::deActivateStartPoint()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::deActivateStartPoint()\n";
    startPointIsActivated = false;
}

void Ruler::setStartPoint(float x, float y, float z)
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::setStartPoint(float x, float y, float z)"
                                       << " x = " << x
                                       << " y = " << y
                                       << " z = " << z << "\n";
    this->x1 = x;
    this->y1 = y;
    this->z1 = z;
    this->x2 = x;
    this->y2 = y;
    this->z2 = z;

    x1_ptr = &x1;
    y1_ptr = &y1;
    z1_ptr = &z1;
    x2_ptr = &x2;
    y2_ptr = &y2;
    z2_ptr = &z2;
}

void Ruler::activateEndPoint()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::activateEndPoint()\n";
    endPointIsActivated = true;
}

void Ruler::deActivateEndPoint()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::deActivateEndPoint()\n";
    endPointIsActivated = false;
}

void Ruler::setEndPoint(float x, float y, float z)
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::setEndPoint(float x, float y, float z)"
                                       << " x = " << x
                                       << " y = " << y
                                       << " z = " << z << "\n";
    this->x2 = x;
    this->y2 = y;
    this->z2 = z;

    x2_ptr = &x2;
    y2_ptr = &y2;
    z2_ptr = &z2;
}

bool Ruler::isEndPointActivated()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::isEndPointActivated()\n";
    return endPointIsActivated;
}

bool Ruler::isStartPointActivated()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::isStartPointActivated()\n";
    return startPointIsActivated;
}

QPoint Ruler::getStartPoint()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::getStartPoint()\n";
    QPoint point(x1, y1);
    return point;
}

QPoint Ruler::getEndPoint()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::getEndPoint()\n";
    QPoint point(x2, y2);
    return point;
}

void Ruler::setStartLinkPoint(RoadElement *element, int indexOfControl, float x, float y, float z)
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::setStartLinkPoint(RoadElement *element, int indexOfControl, float x, float y, float z)"
                                       << " indexIfControl = " << indexOfControl
                                       << " x = " << x
                                       << " y = " << y
                                       << " z = " << z << "\n";
    if (element != NULL &&
            (indexOfControl >=0 && indexOfControl < element->getNumberOfControls()))
    {
        element1 = element;
        index1 = indexOfControl;
        elementId1 = element->getId();
        std::vector<vec3> p = element1->getCoordOfControl(index1);
        switch (p.size())
        {
        case 1:
            //vec3 p1 = element1->getCoordOfControl(index1);
            break;
        case 2:
        {
            vec3 p1 = p[0];
            vec3 p2 = p[1];
            partOfStartLine = ((p2.x - p1.x)*(x - p1.x) + (p2.y - p1.y)*(y - p1.y))/
                    ((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
        }
            break;
        default:
            break;
        }

    }
}

void Ruler::setEndLinkPoint(RoadElement *element, int indexOfControl, float x, float y, float z)
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::setEndLinkPoint(RoadElement *element, int indexOfControl, float x, float y, float z)"
                                       << " indexIfControl = " << indexOfControl
                                       << " x = " << x
                                       << " y = " << y
                                       << " z = " << z << "\n";
    if (element != NULL &&
            (indexOfControl >=0 && indexOfControl < element->getNumberOfControls()))
    {
        element2 = element;
        index2 = indexOfControl;
        elementId2 = element->getId();
        std::vector<vec3> p = element2->getCoordOfControl(index2);
        switch (p.size())
        {
        case 1:
            //vec3 p1 = element1->getCoordOfControl(index1);

            break;
        case 2:
        {
            vec3 p1 = p[0];
            vec3 p2 = p[1];
            partOfEndLine = ((p2.x - p1.x)*(x - p1.x) + (p2.y - p1.y)*(y - p1.y))/
                    ((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
        }
            break;
        default:
            break;
        }
    }
}

void Ruler::clearStartLinkPoint()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::clearStartLinkPoint()\n";
    element1 = NULL;
    elementId1 = -1;
    index1 = -1;
}

void Ruler::clearEndLinkPoint()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::clearEndLinkPoint()\n";
    element2 = NULL;
    elementId2 = -1;
    index2 = -1;
}

bool Ruler::getLogging()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::getLogging()\n";
    return log;
}

void Ruler::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "Ruler::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}



bool Ruler::isSelected()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::isSelected()\n";
    return selected;
}

void Ruler::setSelectedStatus(bool status)
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::setSelectedStatus(bool status)"
                                       << " status = " << status << "\n";
    selected = status;
}

void Ruler::drawFigure(QGLWidget *)
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::drawFigure(QGLWidget *render)\n";

    if (element1)
    {
        std::vector<vec3> p = element1->getCoordOfControl(index1);

        switch (p.size())
        {
        case 1:
        {

            this->x1 = p[0].x;
            this->y1 = p[0].y;
            this->z1 = p[0].z;
        }
            break;
        case 2:
        {
            vec3 p1 = p[0];
            vec3 p2 = p[1];
            this->x1 = p1.x + (p2.x - p1.x) * partOfStartLine;
            this->y1 = p1.y + (p2.y - p1.y) * partOfStartLine;
            this->z1 = p1.z + (p2.z - p1.z) * partOfStartLine;
        }
            break;
        default:
            break;
        }

    }
    glPointSize(5.0);
    glBegin(GL_POINTS);
    glColor4f(0.3f,0.3f,0.3f, 1.0f);
    glVertex3f(x1, y1, z1);
    glEnd();

    if (startPointIsActivated)
    {
        glLineWidth(2.0);
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,0.0);
        glVertex3f(x1, y1, z1);
        glColor3f(0.0,0.0,0.0);
        glVertex3f(x2, y2, z2);
        glEnd();
        //drawMeasurements(render);
    }
    if (endPointIsActivated)
    {
        if (element2)
        {
            std::vector<vec3> p = element2->getCoordOfControl(index2);
            switch (p.size())
            {
            case 1:
            {

                this->x2 = p[0].x;
                this->y2 = p[0].y;
                this->z2 = p[0].z;
            }
                break;
            case 2:
            {
                vec3 p1 = p[0];
                vec3 p2 = p[1];
                this->x2 = p1.x + (p2.x - p1.x) * partOfStartLine;
                this->y2 = p1.y + (p2.y - p1.y) * partOfStartLine;
                this->z2 = p1.z + (p2.z - p1.z) * partOfStartLine;
            }
                break;
            default:
                break;
            }


        }
        glPointSize(5.0);
        glBegin(GL_POINTS);
        glColor4f(0.3f,0.3f,0.3f, 1.0f);
        glVertex3f(x2, y2, z2);
        glEnd();
    }
    if (indexOfSelectedControl == 0 && indexOfSelectedControl == 1)
        drawControlElement(indexOfSelectedControl, 5.0f, 5.0f);
}

void Ruler::drawSelectionFrame()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::drawSelectionFrame()\n";
    for (int i = 0; i < getNumberOfControls(); ++i)
        drawControlElement(i, 5.0f, 10.0f);
}

void Ruler::drawMeasurements(QGLWidget *render)
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::drawMeasurements(QGLWidget *render)\n";
    float r = sqrt((x1-x2)*(x1-x2) + (y1 - y2)*(y1 - y2));
    GLdouble wx, wy, wz;
    getWindowCoord(x2, y2, 0.0f, wx, wy, wz);
    glColor3f(0.0f, 0.0f, 0.0f);
    render->renderText (wx + 5, wy - 5, QString("%1").arg(r, 0, 'f', 2));
}

void Ruler::move(float , float , float )
{
    //    if (log)
    //        Logger::getLogger()->infoLog() << "Ruler::move(float dx, float dy, float dz)"
    //                                       << " ";
}

void Ruler::drawControlElement(int index, float lineWidth, float pointSize)
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::drawControlElement(int index, float lineWidth, float pointSize)"
                                       << " index = " << index
                                       << " lineWidth = " << lineWidth
                                       << " pointSize = " << pointSize << "\n";
    float X, Y;
    switch (index)
    {
    case 0:
        if (startPointIsActivated)
        {
            X = x1;
            Y = y1;
            glPointSize(pointSize);
            glBegin(GL_POINTS);
            glColor3f(0.0f,0.0f,0.0f);
            glVertex3f(X, Y, 0.0);
            glEnd();
        }
        break;
    case 1:
        if (endPointIsActivated)
        {
            X = x2;
            Y = y2;
            glPointSize(pointSize);
            glBegin(GL_POINTS);
            glColor3f(0.0f,0.0f,0.0f);
            glVertex3f(X, Y, 0.0);
            glEnd();
        }
        break;
    default:
        break;
    }

}

QCursor Ruler::getCursorForControlElement(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::getCursorForControlElement(int index)"
                                       << " index = " << index << "\n";
    return Qt::CrossCursor;
}

void Ruler::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::resizeByControl(int index, float dx, float dy, float x, float y)"
                                       << " index = " << index
                                       << " dx = " << dx
                                       << " dy = " << dy
                                       << " x = " << x
                                       << " y = " << y << "\n";
    switch (index)
    {
    case 0:
        x1 += dx;
        y1 += dy;
        break;
    case 1:
        x2 += dx;
        y2 += dy;
        break;
    default:
        break;
    }
}

int Ruler::getNumberOfControls()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::getNumberOfControls()\n";
    return 2;
}

int Ruler::controlsForPoint()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::controlsForPoint()\n";
    return 1;
}

void Ruler::changeColorOfSelectedControl(int index)
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::changeColorOfSelectedControl(int index)"
                                       << " index = " << index << "\n";
    indexOfSelectedControl = index;
}

void Ruler::getProperties(QFormLayout *, QGLWidget *)
{
}

bool Ruler::isFixed()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::isFixed()\n";
    return fixed;
}

int Ruler::getLayer()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::getLayer()\n";
    return layer;
}

void Ruler::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)"
                                       << " x = " << x
                                       << " y = " << y
                                       << " z = " << z
                                       << " wx = " << wx
                                       << " wy = " << wy
                                       << " wz = " << wz << "\n";
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    gluProject(x, y, z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
    wy=viewport[3]-wy;
}

bool Ruler::setFixed(bool fixed)
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::setFixed(bool fixed)"
                                       << " fixed = " << fixed << "\n";
    this->fixed = fixed;
    return true;
}


void Ruler::clear()
{
}


RoadElement *Ruler::getCopy()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::getCopy()\n";
    Ruler* copyElement = new Ruler(*this);
    return copyElement;
}


QJsonObject Ruler::getJSONInfo()
{
    if (log)
        Logger::getLogger()->infoLog() << "Ruler::getJSONInfo()\n";
    QJsonObject element;
    element["Name"] = name;
    element["Layer"] = layer;
    element["ElementId1"] = elementId1;
    element["ElementId2"] = elementId2;
    element["Index1"] = index1;
    element["Index2"] = index2;
    element["Id"] = Id;


    if (element1)
    {
        std::vector<vec3> p = element1->getCoordOfControl(index1);

        switch (p.size())
        {
        case 1:
        {

            this->x1 = p[0].x;
            this->y1 = p[0].y;
            this->z1 = p[0].z;
        }
            break;
        case 2:
        {
            vec3 p1 = p[0];
            vec3 p2 = p[1];
            this->x1 = p1.x + (p2.x - p1.x) * partOfStartLine;
            this->y1 = p1.y + (p2.y - p1.y) * partOfStartLine;
            this->z1 = p1.z + (p2.z - p1.z) * partOfStartLine;
        }
            break;
        default:
            break;
        }

    }

    element["X1"] = x1;
    element["Y1"] = y1;
    element["Z1"] = z1;

    if (element2)
    {
        std::vector<vec3> p = element2->getCoordOfControl(index2);
        switch (p.size())
        {
        case 1:
        {

            this->x2 = p[0].x;
            this->y2 = p[0].y;
            this->z2 = p[0].z;
        }
            break;
        case 2:
        {
            vec3 p1 = p[0];
            vec3 p2 = p[1];
            this->x2 = p1.x + (p2.x - p1.x) * partOfStartLine;
            this->y2 = p1.y + (p2.y - p1.y) * partOfStartLine;
            this->z2 = p1.z + (p2.z - p1.z) * partOfStartLine;
        }
            break;
        default:
            break;
        }


    }
    element["X2"] = x2;
    element["Y2"] = y2;
    element["Z2"] = z2;

    return element;
}

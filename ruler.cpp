#include "ruler.h"

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
    index1 = index2 = -1;
}

void Ruler::activateStartPoint()
{
    startPointIsActivated = true;
}

void Ruler::deActivateStartPoint()
{
    startPointIsActivated = false;
}

void Ruler::setStartPoint(float x, float y, float z)
{
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
    endPointIsActivated = true;
}

void Ruler::deActivateEndPoint()
{
    endPointIsActivated = false;
}

void Ruler::setEndPoint(float x, float y, float z)
{
    this->x2 = x;
    this->y2 = y;
    this->z2 = z;

    x2_ptr = &x2;
    y2_ptr = &y2;
    z2_ptr = &z2;
}

bool Ruler::isEndPointActivated()
{
    return endPointIsActivated;
}

bool Ruler::isStartPointActivated()
{
    return startPointIsActivated;
}

QPoint Ruler::getStartPoint()
{
    QPoint point(x1, y1);
    return point;
}

QPoint Ruler::getEndPoint()
{
    QPoint point(x2, y2);
    return point;
}

void Ruler::setStartLinkPoint(RoadElement *element, int indexOfControl, float x, float y, float z)
{
    if (element != NULL &&
            (indexOfControl >=0 && indexOfControl < element->getNumberOfControls()))
    {
        element1 = element;
        index1 = indexOfControl;

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
    if (element != NULL &&
            (indexOfControl >=0 && indexOfControl < element->getNumberOfControls()))
    {
        element2 = element;
        index2 = indexOfControl;

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
    element1 = NULL;
    index1 = -1;
}

void Ruler::clearEndLinkPoint()
{
    element2 = NULL;
    index2 = -1;
}



bool Ruler::isSelected()
{
    return selected;
}

void Ruler::setSelectedStatus(bool status)
{
    selected = status;
}

void Ruler::drawFigure(QGLWidget *render)
{
    float z = 0.3f;
    if (element1)
    {
        std::vector<vec3> p = element1->getCoordOfControl(index1);

        switch (p.size())
        {
        case 1:
        {

        this->x1 = p[0].x;
        this->y1 = p[0].y;
        z = p[0].z;
        }
            break;
        case 2:
        {
            vec3 p1 = p[0];
            vec3 p2 = p[1];
            this->x1 = p1.x + (p2.x - p1.x) * partOfStartLine;
            this->y1 = p1.y + (p2.y - p1.y) * partOfStartLine;
        }
            break;
        default:
            break;
        }

    }
    glPointSize(5.0);
    glBegin(GL_POINTS);
    glColor4f(0.3f,0.3f,0.3f, 1.0f);
    glVertex3f(x1, y1, z);
    glEnd();

    if (startPointIsActivated)
    {
        glLineWidth(2.0);
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,0.0);
        glVertex3f(x1, y1, 0.3);
        glColor3f(0.0,0.0,0.0);
        glVertex3f(x2, y2, 0.3);
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
            z = p[0].z;
            }
                break;
            case 2:
            {
                vec3 p1 = p[0];
                vec3 p2 = p[1];
                this->x2 = p1.x + (p2.x - p1.x) * partOfStartLine;
                this->y2 = p1.y + (p2.y - p1.y) * partOfStartLine;
            }
                break;
            default:
                break;
            }


        }
        glPointSize(5.0);
        glBegin(GL_POINTS);
        glColor4f(0.3f,0.3f,0.3f, 1.0f);
        glVertex3f(x2, y2, z);
        glEnd();
    }
    if (indexOfSelectedControl == 0 && indexOfSelectedControl == 1)
        drawControlElement(indexOfSelectedControl, 5.0f, 5.0f);
}

void Ruler::drawSelectionFrame()
{
    for (int i = 0; i < getNumberOfControls(); ++i)
        drawControlElement(i, 5.0f, 10.0f);
}

void Ruler::drawMeasurements(QGLWidget *render)
{
    float r = sqrt((x1-x2)*(x1-x2) + (y1 - y2)*(y1 - y2));
    GLdouble wx, wy, wz;
    getWindowCoord(x2, y2, 0.0f, wx, wy, wz);
    glColor3f(0.0f, 0.0f, 0.0f);
    render->renderText (wx + 5, wy - 5, QString("%1").arg(r, 0, 'f', 2));
}

void Ruler::move(float dx, float dy, float dz)
{
}

void Ruler::drawControlElement(int index, float lineWidth, float pointSize)
{
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
    return Qt::CrossCursor;
}

void Ruler::resizeByControl(int index, float dx, float dy, float x, float y)
{
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
    return 2;
}

int Ruler::controlsForPoint()
{
    return 1;
}

void Ruler::changeColorOfSelectedControl(int index)
{
    indexOfSelectedControl = index;
}

void Ruler::getProperties(QFormLayout *layout, QGLWidget *render)
{
}

bool Ruler::isFixed()
{
    return fixed;
}

int Ruler::getLayer()
{
    return layer;
}

void Ruler::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
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
    this->fixed = fixed;
}


void Ruler::clear()
{
}

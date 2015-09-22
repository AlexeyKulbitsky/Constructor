#include "linebuilder.h"
#include "linebuilderstate.h"
LineBuilder::LineBuilder(LineBuilderState *lineBuilderState)
{
    xCurrent = yCurrent = 0.0f;
    this->lineBuilderState = lineBuilderState;
}



bool LineBuilder::isSelected()
{
    return false;
}

void LineBuilder::setSelectedStatus(bool status)
{
}

void LineBuilder::drawFigure(QGLWidget* render)
{
    // Отрисовка линий
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < vertexArray.size(); i += 3)
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(vertexArray[i], vertexArray[i + 1], vertexArray[i + 2]);
    }
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(xCurrent, yCurrent, 0.02f);
    glEnd();

    // Отрисовка узловых точек
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < vertexArray.size(); i += 3)
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(vertexArray[i], vertexArray[i + 1], vertexArray[i + 2]);

    }
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(xCurrent, yCurrent, 0.02f);

    glEnd();

}

void LineBuilder::drawSelectionFrame()
{
}

void LineBuilder::move(float dx, float dy, float dz)
{
    if (fixed)
    {
        return;
    }
    for (int i = 0; i < vertexArray.size(); i += 3)
    {
        vertexArray[i] += dx;
        vertexArray[i + 1] += dy;
    }
}

void LineBuilder::drawControlElement(int index, float lineWidth, float pointSize)
{
    glPointSize(5.0f);
    glBegin(pointSize);
    for (int i = 0; i < vertexArray.size(); i += 3)
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(vertexArray[i], vertexArray[i + 1], vertexArray[i + 2]);
    }
    glEnd();
}

QCursor LineBuilder::getCursorForControlElement(int index)
{
    return Qt::CrossCursor;
}

void LineBuilder::resizeByControl(int index, float dx, float dy, float x, float y)
{
    if (fixed)
    {
        return;
    }
    vertexArray[index * 3] += dx;
    vertexArray[index * 3 + 1] += dy;
}

int LineBuilder::getNumberOfControls()
{
    return vertexArray.size() / 3;
}

int LineBuilder::controlsForPoint()
{
    return 1;
}

void LineBuilder::changeColorOfSelectedControl(int index)
{
}

void LineBuilder::addPoint(float x, float y)
{
    vertexArray.push_back(x);
    vertexArray.push_back(y);
    vertexArray.push_back(0.02f);
}

void LineBuilder::setCurrentPoint(float x, float y)
{
    xCurrent = x;
    yCurrent = y;
}

std::vector<GLfloat> &LineBuilder::getVertexArray()
{
    return vertexArray;
}


void LineBuilder::getProperties(QFormLayout *layout, QGLWidget* render)
{
    while(QLayoutItem* child = layout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }
    QPushButton* linkButton = new QPushButton();
    linkButton->setCheckable(true);
    linkButton->setChecked(false);
    connect(linkButton, SIGNAL(toggled(bool)), lineBuilderState, SLOT(setLinking(bool)));
    layout->addRow("Привязать", linkButton);
}


bool LineBuilder::isFixed()
{
    return false;
}


void LineBuilder::drawMeasurements(QGLWidget *render)
{
}


bool LineBuilder::setFixed(bool fixed)
{
    this->fixed = fixed;
}


int LineBuilder::getLayer()
{
    return layer;
}


void LineBuilder::clear()
{
}
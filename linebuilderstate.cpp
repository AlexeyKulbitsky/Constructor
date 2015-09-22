#include "linebuilderstate.h"

LineBuilderState::LineBuilderState()
{
    this->stateManager = NULL;
    this->model = NULL;
    this->scene = NULL;
    this->properties = NULL;
    lineBuilder = NULL;
    scene->setMouseTracking(true);
    rightButtonIsPressed = false;
    leftButtonIsPressed = false;
    width = 1.0f;
    useColor = true;
    linking = false;
}

LineBuilderState::LineBuilderState(StateManager *manager, Model *model, Scene2D *scene, QFormLayout *properties)
{

    this->stateManager = manager;
    this->model = model;
    this->scene = scene;
    this->properties = properties;
    lineBuilder = NULL;
    scene->setMouseTracking(true);
    rightButtonIsPressed = false;
    leftButtonIsPressed = false;
    width = 1.0f;
    useColor = true;
    linking = false;
}



void LineBuilderState::mousePressEvent(QMouseEvent *pe)
{
    ptrMousePosition = pe->pos();
    switch (pe->button())
    {
    case Qt::LeftButton:
{
        leftButtonIsPressed = true;

        GLint viewport[4];
        GLdouble modelview[16];
        GLdouble projection[16];
        GLfloat winX, winY, winZ;
        GLdouble posX = 0.0, posY = 0.0, posZ = 0.0;

        glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
        glGetDoublev( GL_PROJECTION_MATRIX, projection );
        glGetIntegerv( GL_VIEWPORT, viewport );

        winX = (float)pe->pos().x();
        winY = (float)viewport[3] - (float)pe->pos().y();
        glReadPixels( int(winX), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

        gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

        float x = posX / scene->nSca + scene->xDelta;
        float y = posY / scene->nSca + scene->yDelta;



        if (model->getGroup(model->getNumberOfGroups() - 1).empty())
        {
            lineBuilder = new LineBuilder(this);
            lineBuilder->getProperties(properties, scene);
            model->getGroup(model->getNumberOfGroups() - 1).push_back(lineBuilder);
        }
        lineBuilder->addPoint(x, y);
        //lineBuilder->setCurrentPoint(x, y);

        //scene->updateGL();
}
        break;

    case Qt::RightButton:
        rightButtonIsPressed = true;
        break;
    default:
        break;
    }
scene->updateGL();
}

void LineBuilderState::mouseMoveEvent(QMouseEvent *pe)
{

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX = 0.0, posY = 0.0, posZ = 0.0;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)pe->pos().x();
    winY = (float)viewport[3] - (float)pe->pos().y();
    glReadPixels( int(winX), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    float x = posX / scene->nSca + scene->xDelta;
    float y = posY / scene->nSca + scene->yDelta;

    if (model->getGroup(model->getNumberOfGroups() - 1).empty())
    {
        lineBuilder = new LineBuilder(this);
        lineBuilder->getProperties(properties, scene);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(lineBuilder);
    }

    lineBuilder->setCurrentPoint(x, y);

    scene->updateGL();

    if (rightButtonIsPressed == true)
    {
        // двигать камеру
        scene->yDelta += (GLfloat)(pe->y()-ptrMousePosition.y())/
                scene->width()/(scene->nSca * scene->ratio) * 2.0;
        scene->xDelta += (GLfloat)(-1)*(pe->x()-ptrMousePosition.x())/
                scene->width()/(scene->nSca * scene->ratio) * 2.0;
        scene->updateGL();
    }
    ptrMousePosition = pe->pos();
}

void LineBuilderState::mouseReleaseEvent(QMouseEvent *pe)
{
    if(pe->button() == Qt::RightButton)
    {
        rightButtonIsPressed = false;
    }
    if(pe->button() == Qt::LeftButton)
    {
        leftButtonIsPressed = false;

    }
    ptrMousePosition = pe->pos();
}

void LineBuilderState::wheelEvent(QWheelEvent *pe)
{

    if ((pe->delta())>0) scene->scale_plus();
    else
        if ((pe->delta())<0) scene->scale_minus();

    scene->updateGL();
}

void LineBuilderState::keyPressEvent(QKeyEvent *pe)
{
    switch (pe->key())
    {
    case Qt::Key_Return:
    {
        int size = lineBuilder->getVertexArray().size();
        GLfloat* array = new GLfloat[size];
        for (int i = 0; i < size; ++i)
        {
            array[i] = lineBuilder->getVertexArray()[i];
        }
        if (useColor)
        {
            model->getGroup(layer).push_back(new LineBroken(width, array, size, 1.0f, 1.0f, 1.0f, 1.0f, name, layer));
            model->setModified(true);
        }
        else
        {
            model->getGroup(layer).push_back(new LineBroken(width, array, size, textureSource, textureSize, name, layer));
            model->setModified(true);
        }
        model->getGroup(model->getNumberOfGroups() - 1).pop_back();
        delete[] array;
        array = NULL;
        stateManager->setState(stateManager->defaultState);
        scene->updateGL();
    }
        break;
    default:
        break;
    }
}

void LineBuilderState::dragEnterEvent(QDragEnterEvent *event)
{
}

void LineBuilderState::dropEvent(QDropEvent *event)
{
}

void LineBuilderState::setTexture(QString source, float size)
{
    textureSource = source;
    textureSize = size;
    useColor = false;
}

void LineBuilderState::setWidth(float width)
{
    this->width = width;
}

void LineBuilderState::setName(QString name)
{
    this->name = name;
}

void LineBuilderState::setLayer(int layer)
{
    this->layer = layer;
}

LineBuilderState::~LineBuilderState()
{

}


void LineBuilderState::keyReleaseEvent(QKeyEvent *pe)
{
}




QString LineBuilderState::getName()
{
    return "LineBuilderState";
}


void LineBuilderState::contextMenuEvent(QContextMenuEvent *pe)
{
}

void LineBuilderState::setLinking(bool state)
{
    this->linking = state;
}

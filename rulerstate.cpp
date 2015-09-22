#include "rulerstate.h"

RulerState::RulerState()
{
    this->stateManager = NULL;
    this->model = NULL;
    this->scene = NULL;
    this->ruler = NULL;
    this->selectedFigure = NULL;
    this->indexOfControl = -1;
    this->rightButtonIsPressed = false;
    this->leftButtonIsPressed = false;
}

RulerState::RulerState(StateManager *manager, Model *model, Scene2D *scene)
{
    this->stateManager = manager;
    this->model = model;
    this->scene = scene;
    this->ruler = NULL;
    this->selectedFigure = NULL;
    this->indexOfControl = -1;
    this->rightButtonIsPressed = false;
    this->leftButtonIsPressed = false;
}



void RulerState::mousePressEvent(QMouseEvent *pe)
{
    ptrMousePosition = pe->pos();
    if (pe->button() == Qt::RightButton)
    {
        rightButtonIsPressed = true;
        //ptrMousePosition = pe->pos();
        scene->updateGL();
        return;
    }
    float x, y;
    if (ruler == NULL)
    {
        ruler = new Ruler();
        model->getGroup(3).push_back(ruler);
    }


    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX=0, posY=0, posZ=0;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)pe->pos().x();
    winY = (float)viewport[3] - (float)pe->pos().y();
    glReadPixels( int(winX), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);



    x = posX / scene->nSca + scene->xDelta;
    y = posY / scene->nSca + scene->yDelta;

    if (ruler->isStartPointActivated() == false)
    {

        if (selectedFigure && indexOfControl >= 0)
        {
            ruler->setStartLinkPoint(selectedFigure,indexOfControl, x, y, 0.3f);
            ruler->setStartPoint(x, y, 0.3f);
            selectedFigure->changeColorOfSelectedControl(-1);
            selectedFigure->setSelectedStatus(false);
            selectedFigure = NULL;
            indexOfControl = -1;

        }
        else
        {
            ruler->setStartPoint(x, y, 0.3f);
        }
        ruler->activateStartPoint();
        //scene->renderText (ptrMousePosition.x(), ptrMousePosition.y(), "HELLO");

        scene->updateGL();
    }
    else
    {
        if (ruler->isEndPointActivated() == false)
        {
            if (selectedFigure && indexOfControl >= 0)
            {
                ruler->setEndLinkPoint(selectedFigure,indexOfControl, x, y, 0.3f);
                selectedFigure->changeColorOfSelectedControl(-1);
                selectedFigure->setSelectedStatus(false);
                selectedFigure = NULL;
                indexOfControl = -1;
            }
            else
            {
                ruler->setEndPoint(x, y, 0.3f);
            }
            ruler->activateEndPoint();
            scene->updateGL();
        }
        else
        {
            ruler->deActivateStartPoint();
            ruler->deActivateEndPoint();
            ruler->clearStartLinkPoint();
            ruler->clearEndLinkPoint();
            ruler->setStartPoint(x, y, 0.3f);
            scene->updateGL();
        }
    }
}

void RulerState::mouseMoveEvent(QMouseEvent *pe)
{
    if (rightButtonIsPressed == true)
    {
        // двигать камеру
        scene->yDelta += (GLfloat)(pe->y()-ptrMousePosition.y())/
                scene->width()/(scene->nSca * scene->ratio) * 2.0;
        scene->xDelta += (GLfloat)(-1)*(pe->x()-ptrMousePosition.x())/
                scene->width()/(scene->nSca * scene->ratio) * 2.0;
        scene->updateGL();
    }
    if (ruler == NULL)
    {
        ruler = new Ruler();
        model->getGroup(3).push_back(ruler);
    }

    qDebug() << "RulerState";

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX=0, posY=0, posZ=0;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)pe->pos().x();
    winY = (float)viewport[3] - (float)pe->pos().y();
    glReadPixels( int(winX), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
    float x = posX / scene->nSca + scene->xDelta;
    float y = posY / scene->nSca + scene->yDelta;

    if (ruler->isStartPointActivated())
    {
        if (!ruler->isEndPointActivated())
        {
            if (scene->tryToSelectFigures(pe->pos(), selectedFigure) == true)
            {
                selectedFigure->setSelectedStatus(true);
                if (scene->tryToSelectControlsInSelectedFigure(pe->pos(),selectedFigure,indexOfControl) == false)
                {
                    indexOfControl = -1;
                }
                selectedFigure->changeColorOfSelectedControl(indexOfControl);
                ruler->setEndPoint(x, y, 0.3f);
                scene->updateGL();
            }
            else
            {
                if (selectedFigure)
                {
                    selectedFigure->setSelectedStatus(false);
                    selectedFigure = NULL;                   
                }
                ruler->setEndPoint(x, y, 0.3f);
                scene->updateGL();
            }
        }
    }
    else
    {
        if (scene->tryToSelectFigures(pe->pos(), selectedFigure) == true)
        {
            selectedFigure->setSelectedStatus(true);
            if (scene->tryToSelectControlsInSelectedFigure(pe->pos(),selectedFigure,indexOfControl) == false)
            {
                indexOfControl = -1;
            }
            selectedFigure->changeColorOfSelectedControl(indexOfControl);
            ruler->setStartPoint(x, y, 0.3f);
            scene->updateGL();
        }
        else
        {
            if (selectedFigure)
            {
                selectedFigure->setSelectedStatus(false);
                selectedFigure = NULL;
            }
            ruler->setStartPoint(x, y, 0.3f);
            scene->updateGL();
        }
    }
    ptrMousePosition = pe->pos();
}

void RulerState::mouseReleaseEvent(QMouseEvent *pe)
{
    if (rightButtonIsPressed)
        rightButtonIsPressed = false;
    ptrMousePosition = pe->pos();
    scene->updateGL();
}

void RulerState::wheelEvent(QWheelEvent *pe)
{
    if ((pe->delta())>0) scene->scale_plus();
    else
        if ((pe->delta())<0) scene->scale_minus();
    scene->updateGL();
}

void RulerState::keyPressEvent(QKeyEvent *pe)
{
    switch (pe->key())
    {
    case Qt::Key_Return:
        if (ruler->isEndPointActivated())
        {
            ruler = NULL;
        }
        else
        {
            model->getGroup(3).pop_back();
            delete ruler;
            ruler = NULL;
        }
        emit rulerStatusChanged(false);
        scene->updateGL();
        break;
    default:
        break;
    }
}

void RulerState::dragEnterEvent(QDragEnterEvent *event)
{
}

void RulerState::dropEvent(QDropEvent *event)
{
}

RulerState::~RulerState()
{

}


void RulerState::keyReleaseEvent(QKeyEvent *pe)
{
}


QString RulerState::getName()
{
    return "RulerState";
}

void RulerState::getGlobalCoord(double wx, double wy, double wz, double &x, double &y, double &z)
{
    /*
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX=0, posY=0, posZ=0;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)pe->pos().x();
    winY = (float)viewport[3] - (float)pe->pos().y();
    glReadPixels( int(winX), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    x = posX / scene->nSca + scene->xDelta;
    y = posY / scene->nSca + scene->yDelta;
    */
}

void RulerState::clear()
{
    if (ruler)
    {
        model->getGroup(3).pop_back();
        delete ruler;
        ruler = NULL;
    }
    selectedFigure = NULL;
    indexOfControl = -1;
}


void RulerState::contextMenuEvent(QContextMenuEvent *pe)
{
}

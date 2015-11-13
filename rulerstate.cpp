#include "rulerstate.h"
#include <QApplication>

bool RulerState::log = true;

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
    if (log)
        Logger::getLogger()->infoLog() << "RulerState::mousePressEvent(QMouseEvent *pe)\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "RulerState::mouseMoveEvent(QMouseEvent *pe)\n";
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

    //qDebug() << "RulerState";

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
            if (tryToSelectFigures(pe->pos(), selectedFigure) == true)
            {
                selectedFigure->setSelectedStatus(true);
                if (tryToSelectControlsInSelectedFigure(pe->pos(),selectedFigure,indexOfControl) == false)
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
        if (tryToSelectFigures(pe->pos(), selectedFigure) == true)
        {
            selectedFigure->setSelectedStatus(true);
            if (tryToSelectControlsInSelectedFigure(pe->pos(),selectedFigure,indexOfControl) == false)
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
    if (log)
        Logger::getLogger()->infoLog() << "RulerState::wheelEvent(QWheelEvent *pe)\n";
    if ((pe->delta())>0) scene->scalePlus();
    else
        if ((pe->delta())<0) scene->scaleMinus();
    scene->updateGL();
}

void RulerState::keyPressEvent(QKeyEvent *pe)
{
    if (log)
        Logger::getLogger()->infoLog() << "RulerState::keyPressEvent(QKeyEvent *pe)\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "RulerState::dragEnterEvent(QDragEnterEvent *event)\n";
    event->acceptProposedAction();
}

void RulerState::dropEvent(QDropEvent *event)
{
    if (log)
        Logger::getLogger()->infoLog() << "RulerState::dropEvent(QDropEvent *event)\n";
}

RulerState::~RulerState()
{
    stateManager = NULL;
    model = NULL;
    scene = NULL;
    ruler = NULL;
    selectedFigure = NULL;
}


void RulerState::keyReleaseEvent(QKeyEvent *pe)
{
}


QString RulerState::getName()
{
    if (log)
        Logger::getLogger()->infoLog() << "RulerState::getName()\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "RulerState::clear()\n";
    if (ruler)
    {
        model->getGroup(3).pop_back();
        delete ruler;
        ruler = NULL;
    }
    selectedFigure = NULL;
    indexOfControl = -1;
}

bool RulerState::getLogging()
{
    if (log)
        Logger::getLogger()->infoLog() << "RulerState::getLogging()\n";
    return log;
}

void RulerState::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "RulerState::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}


void RulerState::contextMenuEvent(QContextMenuEvent *pe)
{
}


void RulerState::copy()
{
}

void RulerState::paste()
{
}

void RulerState::cut()
{
}

void RulerState::del()
{
}

void RulerState::clearProperties(QFormLayout *layout)
{
}

bool RulerState::tryToSelectFigures(QPoint mp, RoadElement *&element)
{
    if (log)
        Logger::getLogger()->infoLog() << "RulerState::tryToSelectFigures(QPoint mp, RoadElement *&element)\n";
    GLfloat ratio = scene->ratio; // отношение высоты окна виджета к его ширине
    GLint viewport[4]; // декларируем матрицу поля просмотра
    glGetIntegerv(GL_VIEWPORT, viewport); // извлечь матрицу поля просмотра в viewport
    GLfloat nSca = scene->nSca;
    GLfloat xDelta = scene->xDelta;
    GLfloat yDelta = scene->yDelta;
    GLuint selectBuffer[40]; // буфер выбора (буфер совпадений)
    GLint hitsForFigure = 0;

    glSelectBuffer(40, selectBuffer); // использовать указанный буфер выбора
    glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
    glPushMatrix(); // поместить текущую матрицу в стек матриц
    glRenderMode(GL_SELECT); // переход в режим выбора
    glLoadIdentity(); // загрузить единичную матрицу

    // новый объём под указателем мыши
    gluPickMatrix((GLdouble)mp.x(), (GLdouble)(viewport[3]-mp.y()), 1.0, 1.0, viewport);
    // мировое окно
    if (scene->width() >= scene->height())
        glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -10.0, 1.0);
    else
        glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -10.0, 1.0);


    glMatrixMode(GL_MODELVIEW); // модельно-видовая матрица стала активной
    glLoadIdentity();           // загружается единичная матрица моделирования

    glInitNames(); // инициализируется и очищается стек имён
    glPushName(0); // в стек имён помещается значение 0 (обязательно должен храниться хотя бы один элемент)

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int i = 1;
    for (int j = model->getNumberOfGroups() - 1; j >= 0; --j)
    {
        int element = 0;
        for (QList<RoadElement*>::iterator it = model->getGroup(j).begin();
             it != model->getGroup(j).end(); ++it, ++element)
        {
            glPushMatrix();
            glScalef(nSca, nSca, nSca);
            gluLookAt(xDelta,yDelta,0.5,
                      xDelta,yDelta,-10,
                      0,1,0);
            glLoadName(i++); // загрузить имя на вершину стека имён
            (*it)->drawFigure();
            glPopMatrix();

        }
    }

    hitsForFigure = glRenderMode(GL_RENDER); // число совпадений и переход в режим рисования
    if (log)
        Logger::getLogger()->infoLog() << "RulerState::tryToSelectFigures(QPoint mp, RoadElement *&element), hits = "
                                       << hitsForFigure << "\n";

    if (hitsForFigure > 0) // есть совпадания и нет ошибок
    {
        i = selectBuffer[3] - 1; // имя фигуры верхняя фигура
        for (int j = model->getNumberOfGroups() - 1; j >= 0; --j)
        {
            if (i < model->getGroup(j).size())
            {
                QList<RoadElement*>::iterator it = model->getGroup(j).begin();
                for(int k = 0; k < i; ++k)
                    ++it;
                element = *it;
                glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
                glPopMatrix(); // извлечь матрицу из стека матриц
                //scene->updateGL(); // обновить изображение
                return true;
            }
            else
            {
                i -= model->getGroup(j).size();
            }
        }
    }

    glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
    glPopMatrix(); // извлечь матрицу из стека матриц
    //scene->updateGL(); // обновить изображение

    return false;
}

bool RulerState::tryToSelectControlsInSelectedFigure(QPoint mp, RoadElement *element, int &index)
{
    if (log)
        Logger::getLogger()->infoLog() << "RulerState::tryToSelectControlsInSelectedFigure(QPoint mp, RoadElement *element, int &index)\n";
    GLfloat ratio = scene->ratio; // отношение высоты окна виджета к его ширине
    GLint viewport[4]; // декларируем матрицу поля просмотра
    glGetIntegerv(GL_VIEWPORT, viewport); // извлечь матрицу поля просмотра в viewport
    GLfloat nSca = scene->nSca;
    GLfloat xDelta = scene->xDelta;
    GLfloat yDelta = scene->yDelta;
    GLuint selectBuffer[16]; // буфер выбора (буфер совпадений)
    GLint hitsForControl = 0;

    glSelectBuffer(16, selectBuffer); // использовать указанный буфер выбора
    glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
    glPushMatrix(); // поместить текущую матрицу в стек матриц
    glRenderMode(GL_SELECT); // переход в режим выбора
    glLoadIdentity(); // загрузить единичную матрицу

    // новый объём под указателем мыши
    gluPickMatrix((GLdouble)mp.x(), (GLdouble)(viewport[3]-mp.y()), 10.0, 10.0, viewport);
    // мировое окно
    if (scene->width() >= scene->height())
        glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -10.0, 1.0);
    else
        glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -10.0, 1.0);


    glMatrixMode(GL_MODELVIEW); // модельно-видовая матрица стала активной
    glLoadIdentity();           // загружается единичная матрица моделирования

    glInitNames(); // инициализируется и очищается стек имён
    glPushName(0); // в стек имён помещается значение 0 (обязательно должен храниться хотя бы один элемент)

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    for (int i = 1; i < element->getNumberOfControls() + 1; ++i)
    {
        glPushMatrix();
        glScalef(nSca, nSca, nSca);
        gluLookAt(xDelta,yDelta,0.5,
                  xDelta,yDelta,-10,
                  0,1,0);
        glLoadName(i); // загрузить имя на вершину стека имён
        element->drawControlElement(i - 1, 15.0f, 30.0f);

        glPopMatrix();
    }


    hitsForControl = glRenderMode(GL_RENDER); // число совпадений и переход в режим рисования
    if (log)
        Logger::getLogger()->infoLog() << "RulerState::tryToSelectControlsInSelectedFigure(QPoint mp, RoadElement *element, int &index), hits = "
                                       << hitsForControl << "\n";
    if (hitsForControl > 0) // есть совпадания и нет ошибок
    {
        index = selectBuffer[3] - 1;
        glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
        glPopMatrix(); // извлечь матрицу из стека матриц
        //scene->updateGL(); // обновить изображение
        return true;
    }

    glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
    glPopMatrix(); // извлечь матрицу из стека матриц
    //scene->updateGL(); // обновить изображение
    return false;
}


void RulerState::saveToPresets()
{
}
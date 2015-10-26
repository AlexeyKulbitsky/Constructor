#include "roadbuilderstate.h"

#include "objfilemanager.h"
#include "_3dsfilemanager.h"
#include <QApplication>

RoadBuilderState::RoadBuilderState()
{
    this->stateManager = NULL;
    this->model = NULL;
    this->scene = NULL;
    this->roadBroken = NULL;
    this->properties = NULL;
    rightButtonIsPressed = false;
    leftButtonIsPressed = false;

    controlIsSelected = false;
    controlIndex = -1;
    scene->setMouseTracking(true);
}

RoadBuilderState::RoadBuilderState(StateManager *manager, Model *model, Scene2D *scene, QFormLayout *properties)
{
    if (manager == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Cannot create RoadBuilderState: StateManager = NULL, program terminates");
        Logger::getLogger()->writeLog("Cannot create RoadBuilderState: StateManager = NULL, program terminates");
        QApplication::exit(0);
    }
    this->stateManager = manager;
    if (model == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Cannot create RoadBuilderState: Model = NULL, program terminates");
        Logger::getLogger()->writeLog("Cannot create RoadBuilderState: Model = NULL, program terminates");
        QApplication::exit(0);
    }
    this->model = model;
    if (scene == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Cannot create RoadBuilderState: Scene2D = NULL, program terminates");
        Logger::getLogger()->writeLog("Cannot create RoadBuilderState: Scene2D = NULL, program terminates");
        QApplication::exit(0);
    }
    this->scene = scene;
    if (properties == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Cannot create RoadBuilderState: QFormLayout(properties) = NULL, program terminates");
        Logger::getLogger()->writeLog("Cannot create RoadBuilderState: QFormLayout(properties) = NULL, program terminates");
        QApplication::exit(0);
    }
    this->properties = properties;
    this->roadBroken = NULL;
    rightButtonIsPressed = false;
    leftButtonIsPressed = false;
    controlIsSelected = false;
    controlIndex = -1;
    scene->setMouseTracking(true);
}

RoadBuilderState::~RoadBuilderState()
{
    stateManager = NULL;
    model = NULL;
    scene = NULL;
    properties = NULL;

    roadBroken = NULL;
}



void RoadBuilderState::mousePressEvent(QMouseEvent *pe)
{
    Logger::getLogger()->writeLog("RoadBuilderState::mousePressEvent(QMouseEvent *pe)");
    ptrMousePosition = pe->pos();
    switch (pe->button())
    {
    case Qt::LeftButton:
    {
        leftButtonIsPressed = true;


        //if (model->getGroup(model->getNumberOfGroups() - 1).empty())
        if (roadBroken == NULL)
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

            //roadBroken = new RoadBroken(x, y, x + 0.5f, y, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, name, layer);
            roadBroken = new RoadBroken(x, y, x + 2.75f, y, 6.0f,
                                        QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                        QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                        name, layer);
            roadBroken->setSelectedStatus(true);
            roadBroken->getProperties(properties,scene);
            //model->getGroup(model->getNumberOfGroups() - 1).push_back(roadBroken);
            model->getGroup(layer).push_back(roadBroken);
            groupIndex = layer;
            elementIndex = model->getGroup(layer).size() - 1;
            scene->updateGL();
        }
        else
        {

            if (this->tryToSelectControlsInSelectedFigure(ptrMousePosition) == true)
            {
                controlIsSelected = true;
                if (key == Qt::Key_Control)
                {
                    if (controlIndex == roadBroken->getNumberOfControls() - 1)
                    {
                        roadBroken->addBreak(false);
                        controlIndex = roadBroken->getNumberOfControls() - 1;
                    }
                    else
                        if (controlIndex == roadBroken->getNumberOfControls() - 2)
                        {
                            roadBroken->addBreak(true);
                            controlIndex = roadBroken->getNumberOfControls() - 2;
                        }
                }
                scene->setCursor(roadBroken->getCursorForControlElement(controlIndex));
                scene->updateGL();
            }
            else
            {
                if (this->tryToSelectFigures(ptrMousePosition) == true)
                {
                    scene->setCursor(Qt::SizeAllCursor);
                    scene->updateGL();
                }
                else
                {
                    // Переключить в состояние по умолчанию
                    roadBroken->setSelectedStatus(false);
                    //roadBroken->clearProperties(properties);
                    clear();
                    scene->setMouseTracking(false);
                    scene->setCursor(Qt::ArrowCursor);
                    clearProperties(properties);
                    scene->updateGL();
                    stateManager->setState(stateManager->defaultState);
                }

            }
        }

        scene->updateGL();
    }
        break;

    case Qt::RightButton:
        rightButtonIsPressed = true;
        break;
    default:
        break;
    }
}

void RoadBuilderState::mouseMoveEvent(QMouseEvent *pe)
{
    Logger::getLogger()->writeLog("RoadBuilderState::mouseMoveEvent(QMouseEvent *pe)");
    if (rightButtonIsPressed == true)
    {
        // двигать камеру
        scene->yDelta += (GLfloat)(pe->y()-ptrMousePosition.y())/
                scene->width()/(scene->nSca * scene->ratio) * 2.0;
        scene->xDelta += (GLfloat)(-1)*(pe->x()-ptrMousePosition.x())/
                scene->width()/(scene->nSca * scene->ratio) * 2.0;
        scene->updateGL();
    }
    else
    {
        //if (!model->getGroup(model->getNumberOfGroups() - 1).empty())
        if (roadBroken != NULL)
        {
            if (leftButtonIsPressed == true)
            {
                // двигать фигуру или контролы
                if (controlIsSelected == true)
                {
                    float dY = (GLdouble)(-1)*(pe->y()-ptrMousePosition.y())/
                            scene->width()/(scene->nSca * scene->ratio) * 2.0;
                    float dX = (GLdouble)(pe->x()-ptrMousePosition.x())/
                            scene->width()/(scene->nSca * scene->ratio) * 2.0;

                    float x = (GLdouble)ptrMousePosition.x()/
                            scene->width()/(scene->nSca * scene->ratio) * 2.0;
                    float y = (GLdouble)(scene->height() -  ptrMousePosition.y())/
                            scene->width()/(scene->nSca * scene->ratio) * 2.0;

                    switch (key)
                    {
                    case Qt::Key_Shift:
                    {
                        roadBroken->resizeByControl(controlIndex, dX, dY, x, y, key);
                    }
                        break;
                    default:
                        roadBroken->resizeByControl(controlIndex, dX, dY, x, y);
                        break;
                    }

                    scene->updateGL();
                }
                else
                {
                    //if (this->tryToSelectFigures(pe->pos()) == true)
                    //{
                        float dY = (GLfloat)(-1)*(pe->y()-ptrMousePosition.y())/
                                scene->width()/(scene->nSca * scene->ratio) * 2.0;
                        float dX = (GLfloat)(pe->x()-ptrMousePosition.x())/
                                scene->width()/(scene->nSca * scene->ratio) * 2.0;

                        roadBroken->move(dX, dY);
                        scene->updateGL();
                    //}
                }
            }
            else
            {
                // изменять курсор в зависимости от положения мыши
                if (tryToSelectControlsInSelectedFigure(pe->pos()) == true)
                {
                    scene->setCursor(roadBroken->getCursorForControlElement(controlIndex));
                    scene->updateGL();
                }
                else
                {
                    if (tryToSelectFigures(pe->pos()) == true)
                    {
                        scene->setCursor(Qt::SizeAllCursor);
                        scene->updateGL();
                    }
                    else
                    {
                        scene->setCursor(Qt::ArrowCursor);
                        scene->updateGL();
                    }
                }
            }
        }
    }
    ptrMousePosition = pe->pos();
}

void RoadBuilderState::mouseReleaseEvent(QMouseEvent *pe)
{
    Logger::getLogger()->writeLog("RoadBuilderState::mouseReleaseEvent(QMouseEvent *pe)");
    if(pe->button() == Qt::RightButton)
    {
        rightButtonIsPressed = false;
    }
    if(pe->button() == Qt::LeftButton)
    {
        leftButtonIsPressed = false;
        controlIsSelected = false;
        controlIndex = -1;

    }
    ptrMousePosition = pe->pos();
}

void RoadBuilderState::wheelEvent(QWheelEvent *pe)
{
    Logger::getLogger()->writeLog("RoadBuilderState::wheelEvent(QWheelEvent *pe)");
    if ((pe->delta())>0) scene->scalePlus();
    else
        if ((pe->delta())<0) scene->scaleMinus();

    scene->updateGL();
}

void RoadBuilderState::keyPressEvent(QKeyEvent *pe)
{
    key = pe->key();
    QString s;
    switch (pe->key())
    {
    case Qt::Key_Plus:
        s = "Qt::Key_Plus";
        scene->scalePlus();
        break;

    case Qt::Key_Equal:
        s = "Qt::Key_Equal";
        scene->scalePlus();
        break;

    case Qt::Key_Minus:
        s = "Qt::Key_Minus";
        scene->scaleMinus();
        break;

    case Qt::Key_Up:
        s = "Qt::Key_Up";
        scene->rotateUp();
        break;

    case Qt::Key_Down:
        s = "Qt::Key_Down";
        scene->rotateDown();
        break;

    case Qt::Key_Left:
        s = "Qt::Key_Left";
        scene->rotateLeft();
        break;

    case Qt::Key_Right:
        s = "Qt::Key_Right";
        scene->rotateRight();
        break;

    case Qt::Key_Z:
        s = "Qt::Key_Z";
        scene->translateDown();
        break;

    case Qt::Key_X:
        s = "Qt::Key_X";
        scene->translateUp();
        break;

    case Qt::Key_Space:
        s = "Qt::Key_Space";
        scene->defaultScene();
        break;
    case Qt::Key_Return:
        s = "Qt::Key_Return";
       // roadBroken->setSelectedStatus(false);
       // scene->setCursor(Qt::ArrowCursor);
       // model->getGroup(layer).push_back(roadBroken);
       // model->getGroup(model->getNumberOfGroups() - 1).pop_back();
       // roadBroken = NULL;
       // stateManager->setState(stateManager->defaultState);
        break;

    case Qt::Key_Delete:
        s = "Qt::Key_Delete";
        //model->getGroup(model->getNumberOfGroups() - 1).pop_back();
       // model->getGroup(layer).pop_back();
       // scene->setCursor(Qt::ArrowCursor);
       // delete roadBroken;
       // roadBroken = NULL;
       // clearProperties(properties);
       // stateManager->setState(stateManager->defaultState);
       // break;
/////////////////////////////

    {
        QList<RoadElement*>::iterator it = model->getGroup(groupIndex).begin();
        for (int j = 0; j < elementIndex; ++j)
            ++it;
        (*it)->clear();
        delete (*it);
        model->getGroup(groupIndex).erase(it);
        clearProperties(properties);
        scene->updateGL();
        groupIndex = -1;
        elementIndex = -1;

        scene->setMouseTracking(false);
        scene->setCursor(Qt::ArrowCursor);
        stateManager->setState(stateManager->defaultState);
    }
        break;
    default:
        break;
    }
    Logger::getLogger()->writeLog(QString("RoadBuilderState::keyPressEvent(QKeyEvent *pe), key = ") + s);
    scene->updateGL();
}

void RoadBuilderState::keyReleaseEvent(QKeyEvent *pe)
{
    Logger::getLogger()->writeLog("RoadBuilderState::keyReleaseEvent(QKeyEvent *pe)");
    key = 0;
}

void RoadBuilderState::dragEnterEvent(QDragEnterEvent *event)
{
    Logger::getLogger()->writeLog("RoadBuilderState::dragEnterEvent(QDragEnterEvent *event)");
    event->acceptProposedAction();
}

void RoadBuilderState::dropEvent(QDropEvent *event)
{
    Logger::getLogger()->writeLog("RoadBuilderState::dropEvent(QDropEvent *event)");
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX = 0.0, posY = 0.0, posZ = 0.0;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)event->pos().x();
    winY = (float)viewport[3] - (float)event->pos().y();
    glReadPixels( int(winX), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);


    QString s(event->mimeData()->text());
    float x = posX / scene->nSca + scene->xDelta;
    float y = posY / scene->nSca + scene->yDelta;


    scene->updateGL();
}

bool RoadBuilderState::tryToSelectControlsInSelectedFigure(QPoint mp)
{
    Logger::getLogger()->writeLog("RoadBuilderState::tryToSelectControlsInSelectedFigure(QPoint mp)");
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



    for (int i = 1; i < roadBroken->getNumberOfControls() + 1; ++i)
    {
        glPushMatrix();
        glScalef(nSca, nSca, nSca);
        gluLookAt(xDelta,yDelta,0.5,
                  xDelta,yDelta,-10,
                  0,1,0);
        glLoadName(i); // загрузить имя на вершину стека имён
        roadBroken->drawControlElement(i - 1, 15.0f, 30.0f);

        glPopMatrix();
    }


    hitsForControl = glRenderMode(GL_RENDER); // число совпадений и переход в режим рисования

    Logger::getLogger()->writeLog(QString("RoadBuilderState::tryToSelectControlsInSelectedFigure(QPoint mp), hits = ") +
                                  QString::number(hitsForControl));

    if (hitsForControl > 0) // есть совпадания и нет ошибок
    {
        //controlIndex = selectBuffer[hitsForControl * 4 - 1] - 1;
        controlIndex = selectBuffer[3] - 1;
        glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
        glPopMatrix(); // извлечь матрицу из стека матриц
        scene->updateGL(); // обновить изображение
        return true;
    }

    controlIndex = -1;
    glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
    glPopMatrix(); // извлечь матрицу из стека матриц
    scene->updateGL(); // обновить изображение
    return false;
}

bool RoadBuilderState::tryToSelectFigures(QPoint mp)
{
    Logger::getLogger()->writeLog("RoadBuilderState::tryToSelectFigures(QPoint mp)");
    GLfloat ratio = scene->ratio; // отношение высоты окна виджета к его ширине
    GLint viewport[4]; // декларируем матрицу поля просмотра
    glGetIntegerv(GL_VIEWPORT, viewport); // извлечь матрицу поля просмотра в viewport
    GLfloat nSca = scene->nSca;
    GLfloat xDelta = scene->xDelta;
    GLfloat yDelta = scene->yDelta;
    GLuint selectBuffer[4]; // буфер выбора (буфер совпадений)
    GLint hitsForFigure = 0;

    glSelectBuffer(4, selectBuffer); // использовать указанный буфер выбора
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
    glPushMatrix();
    glScalef(nSca, nSca, nSca);
    gluLookAt(xDelta,yDelta,0.5,
              xDelta,yDelta,-10,
              0,1,0);
    glLoadName(i); // загрузить имя на вершину стека имён

    roadBroken->setSelectedStatus(false);
    roadBroken->drawFigure();
    roadBroken->setSelectedStatus(true);
    glPopMatrix();

    hitsForFigure=glRenderMode(GL_RENDER); // число совпадений и переход в режим рисования

    Logger::getLogger()->writeLog(QString("RoadBuilderState::tryToSelectFigures(QPoint mp), hits = ") +
                                  QString::number(hitsForFigure));

    if (hitsForFigure > 0) // есть совпадания и нет ошибок
    {
        glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
        glPopMatrix(); // извлечь матрицу из стека матриц
        scene->updateGL(); // обновить изображение
        return true;
    }
    else
    {
        glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
        glPopMatrix(); // извлечь матрицу из стека матриц
        scene->updateGL(); // обновить изображение
        return false;
    }
}

void RoadBuilderState::setName(QString name)
{
    Logger::getLogger()->writeLog("RoadBuilderState::setName(QString name)");
    this->name = name;
}

void RoadBuilderState::setLayer(int layer)
{
    Logger::getLogger()->writeLog("RoadBuilderState::setLayer(int layer)");
    if (layer < 0 || layer >= model->getNumberOfGroups())
    {
        QMessageBox::critical(0, "Ошибка", "Layer is out of range", QMessageBox::Yes);
        Logger::getLogger()->writeLog("Layer is out of range");
        QApplication::exit(0);
    }
    this->layer = layer;
}

void RoadBuilderState::setGroupIndex(int index)
{
    Logger::getLogger()->writeLog("RoadBuilderState::setGroupIndex(int index)");
    if (index < 0 || index >= model->getNumberOfGroups())
    {
        QMessageBox::critical(0, "Ошибка", "Group index out of range, program terminates");
        Logger::getLogger()->writeLog("Group index out of range, program terminates");
        QApplication::exit(0);
    }
    groupIndex = index;
}

void RoadBuilderState::setElementIndex(int index)
{
    Logger::getLogger()->writeLog("RoadBuilderState::setElementIndex(int index)");
    if (groupIndex < 0 || groupIndex >= model->getNumberOfGroups())
    {
        QMessageBox::critical(0, "Ошибка", "Group index out of range, program terminates");
        Logger::getLogger()->writeLog("Group index out of range, program terminates");
        QApplication::exit(0);
    }
    if (index < 0 || index >= model->getGroup(groupIndex).size())
    {
        QMessageBox::critical(0, "Ошибка", "Element index out of range, program terminates");
        Logger::getLogger()->writeLog("Element index out of range, program terminates");
        QApplication::exit(0);
    }
    elementIndex = index;
}


QString RoadBuilderState::getName()
{
    Logger::getLogger()->writeLog("RoadBuilderState::getName()");
    return "RoadBuilderState";
}

void RoadBuilderState::clearProperties(QFormLayout *layout)
{
    Logger::getLogger()->writeLog("RoadBuilderState::clearProperties(QFormLayout *layout)");
    if (layout == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "QFormLayout* layout = NULL, cannot clearProperties, program terminates");
        Logger::getLogger()->writeLog("QFormLayout* layout = NULL, cannot clearProperties, program terminates");
        QApplication::exit(0);
    }
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }
}

void RoadBuilderState::clear()
{
    Logger::getLogger()->writeLog("RoadBuilderState::clear()");
    roadBroken = NULL;
    rightButtonIsPressed = false;
    leftButtonIsPressed = false;
    controlIsSelected = false;
    key = -1;
    controlIndex = -1;
    groupIndex = -1;
    elementIndex = -1;
}

void RoadBuilderState::setRoad(RoadBroken *roadBroken)
{
    Logger::getLogger()->writeLog("RoadBuilderState::setRoad(RoadBroken *roadBroken)");
    if (roadBroken)
        this->roadBroken = roadBroken;
    else
    {
        QMessageBox::critical(0, "Ошибка", "RoadBroken* roadBroken = NULL, program terminates");
        Logger::getLogger()->writeLog("RoadBroken* roadBroken = NULL, program terminates");
        QApplication::exit(0);
    }
}


void RoadBuilderState::contextMenuEvent(QContextMenuEvent *pe)
{
    Logger::getLogger()->writeLog("RoadBuilderState::contextMenuEvent(QContextMenuEvent *pe)");
}

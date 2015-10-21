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
    this->stateManager = manager;
    this->model = model;
    this->scene = scene;
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

}



void RoadBuilderState::mousePressEvent(QMouseEvent *pe)
{
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
                    roadBroken->clearProperties(properties);
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
    if ((pe->delta())>0) scene->scale_plus();
    else
        if ((pe->delta())<0) scene->scale_minus();

    scene->updateGL();
}

void RoadBuilderState::keyPressEvent(QKeyEvent *pe)
{
    key = pe->key();

    switch (pe->key())
    {
    case Qt::Key_Plus:
        scene->scale_plus();
        break;

    case Qt::Key_Equal:
        scene->scale_plus();
        break;

    case Qt::Key_Minus:
        scene->scale_minus();
        break;

    case Qt::Key_Up:
        scene->rotate_up();
        break;

    case Qt::Key_Down:
        scene->rotate_down();
        break;

    case Qt::Key_Left:
        scene->rotate_left();
        break;

    case Qt::Key_Right:
        scene->rotate_right();
        break;

    case Qt::Key_Z:
        scene->translate_down();
        break;

    case Qt::Key_X:
        scene->translate_up();
        break;

    case Qt::Key_Space:
        scene->defaultScene();
        break;
    case Qt::Key_Return:
       // roadBroken->setSelectedStatus(false);
       // scene->setCursor(Qt::ArrowCursor);
       // model->getGroup(layer).push_back(roadBroken);
       // model->getGroup(model->getNumberOfGroups() - 1).pop_back();
       // roadBroken = NULL;
       // stateManager->setState(stateManager->defaultState);
        break;

    case Qt::Key_Delete:
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
        std::list<RoadElement*>::iterator it = model->getGroup(groupIndex).begin();
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

    scene->updateGL();
}

void RoadBuilderState::keyReleaseEvent(QKeyEvent *pe)
{
    key = 0;
}

void RoadBuilderState::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void RoadBuilderState::dropEvent(QDropEvent *event)
{
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

    if (s == "Дорога простая")
    {
        RoadSimple* road =  new RoadSimple(x - 2.0f, y, x + 2.0f, y, 6.0f,
                                           QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                           QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                           "RoadSimple", 0);
        road->setModel(model);
        model->getGroup(0).push_back(road);
        model->setModified(true);
    } else
        if (s == "Закругление")
        {
            Curve* curve = new Curve(x, y, 0.0f,
                                     x - 5.0f, y, 0.0f,
                                     x, y + 5.0f, 0.0f,
                                     QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                     QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                     10);
            curve->setModel(model);
             model->getGroup(0).push_back(curve);
             model->setModified(true);
        } else
            if (s == "Разделительная зона (разметка)")
            {
                SplitZone* splitZone = new SplitZone(x,y,0.02f,
                                                     x + 10.0f, y, 0.02f,
                                                     2.0f,
                                                     true,
                                                     true);

                splitZone->setModel(model);
                model->getGroup(0).push_back(splitZone);
                model->setModified(true);
            }
            else
            if (s == "Разделительная зона (газон)")
                        {
                            SplitZone* splitZone = new SplitZone(x,y,0.0f,
                                                                 x + 10.0f, y, 0.0f,
                                                                 2.0f,
                                                                 true,
                                                                 true,
                                                                 1,
                                                                 0.1,
                                                                 QApplication::applicationDirPath() + "/models/city_roads/board.jpg",
                                                                 0.25f, 6.0f,
                                                                 QApplication::applicationDirPath() + "/models/city_roads/grass.jpg",
                                                                 3.0f, 3.0f);

                            splitZone->setModel(model);
                            model->getGroup(0).push_back(splitZone);
                            model->setModified(true);
                        }
            else
                        if (s == "Разделительная зона (тротуар)")
                        {
                            SplitZone* splitZone = new SplitZone(x,y,0.0f,
                                                                 x + 10.0f, y, 0.0f,
                                                                 2.0f,
                                                                 true,
                                                                 true,
                                                                 2,
                                                                 0.1,
                                                                 QApplication::applicationDirPath() + "/models/city_roads/board.jpg",
                                                                 0.25f, 6.0f,
                                                                 QApplication::applicationDirPath() + "/models/city_roads/nr_07S.jpg",
                                                                 6.0f, 6.0f);
                            splitZone->setModel(model);
                            model->getGroup(0).push_back(splitZone);
                            model->setModified(true);
                        }
    else
    if (s == "Дорога ломаная")
    {
        RoadBroken* road = new RoadBroken(x, y, x + 2.75f, y, 6.0f,
                                          QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                          QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                          "RoadBroken", 0);
        road->setModel(model);
        model->getGroup(0).push_back(road);
        model->setModified(true);
    } else
    if (s == "Круговой перекресток")
    {
        model->getGroup(0).push_back(new RoundingCrossRoad(x, y, 10, 16, 50, "RoundingCrossRoad", 0));
        model->setModified(true);
    } else
    if (s == "Поворот дороги")
    {
        RoundingRoad* road = new RoundingRoad(x, y, 10, 0, 90,
                                              x, y, 16, 0, 90,
                                              20, "RoundingRoad", 0,
                                              QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                              QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f);
        road->setModel(model);
        model->getGroup(0).push_back(road);
        model->setModified(true);
    } else
        if (s == "Перекресток")
        {
            Intersection* intersection = new Intersection(x, y);
            intersection->setModel(model);
            model->getGroup(0).push_back(intersection);
            model->setModified(true);
        } else
    if (s == "Сплошая")
    {
        float axis[6];
        axis[0] = x - 2.5f;
        axis[1] = y;
        axis[2] = 0.02f;
        axis[3] = x + 2.5f;
        axis[4] = y;
        axis[5] = 0.02f;
        LineBroken* line = new LineBroken(0.1f, axis, 6, QApplication::applicationDirPath() + "/models/city_roads/solid.png", 6.0f, QString("LineBroken"), 1);
        line->setModel(model);
        model->getGroup(1).push_back(line);
        model->setModified(true);
    } else
    if (s == "Прерывистая")
    {
        float axis[6];
        axis[0] = x - 2.5f;
        axis[1] = y;
        axis[2] = 0.02f;
        axis[3] = x + 2.5f;
        axis[4] = y;
        axis[5] = 0.02f;
        LineBroken* line = new LineBroken(0.1f, axis, 6, QApplication::applicationDirPath() + "/models/city_roads/inter.png", 6.0f, QString("LineBroken"), 1);
        line->setModel(model);
        model->getGroup(1).push_back(line);
        model->setModified(true);
    } else
    if (s == "Двойная сплошая")
    {
        float axis[6];
        axis[0] = x - 2.5f;
        axis[1] = y;
        axis[2] = 0.02f;
        axis[3] = x + 2.5f;
        axis[4] = y;
        axis[5] = 0.02f;
        LineBroken* line = new LineBroken(0.25f, axis, 6, QApplication::applicationDirPath() + "/models/city_roads/d_solid.png", 6.0f, QString("LineBroken"), 1);
        line->setModel(model);
        model->getGroup(1).push_back(line);
        model->setModified(true);
    } else
    if (s == "Двойная прерывистая")
    {
        float axis[6];
        axis[0] = x - 2.5f;
        axis[1] = y;
        axis[2] = 0.02f;
        axis[3] = x + 2.5f;
        axis[4] = y;
        axis[5] = 0.02f;
        LineBroken* line = new LineBroken(0.25f, axis, 6, QApplication::applicationDirPath() + "/models/city_roads/d_inter.png", 6.0f, QString("LineBroken"), 1);
        line->setModel(model);
        model->getGroup(1).push_back(line);
        model->setModified(true);
    } else
    if (s == "Пешеходный переход")
    {
        RoadSimple* crosswalk = new RoadSimple(x - 2.0f, y, x + 2.0f, y, 2.0f,
                                               QString(":/textures/crosswalk.png"), 1.0f, 1.0f,
                                               QString(":/textures/crosswalk.png"), 1.0f, 1.0f,
                                               "Crosswalk", 1);
        crosswalk->setModel(model);
        model->getGroup(1).push_back(crosswalk);
        model->setModified(true);
    } else
    if (s == "Трамвайные пути")
    {
        float axis[6];
        axis[0] = x - 2.5f;
        axis[1] = y;
        axis[2] = 0.02f;
        axis[3] = x + 2.5f;
        axis[4] = y;
        axis[5] = 0.02f;
        LineBroken* tramways = new LineBroken(1.5f, axis, 6, QString(":/textures/tramways.png"), 1.5f, "Tramways", 1);
        tramways->setModel(model);
        model->getGroup(1).push_back(tramways);
        model->setModified(true);
    } else
    if (s == "Железная дорога123")
    {
        LineSimple* railway = new LineSimple(x - 2.0, y, x + 2.0, y, 2.1, QString(":/textures/railroad.png"), 2.1f, "Railroad", 1);
        railway->setModel(model);
        model->getGroup(1).push_back(railway);
        model->setModified(true);
    } else
        if (s == "Железная дорога")
        {
            float axis[6];
            axis[0] = x - 2.5f;
            axis[1] = y;
            axis[2] = 0.02f;
            axis[3] = x + 2.5f;
            axis[4] = y;
            axis[5] = 0.02f;
            RailWay* railway = new RailWay(axis, 6,
                                           QApplication::applicationDirPath() + "/models/city_roads/railway.jpg",
                                           2.65f, 6.0f);
            railway->setModel(model);
            model->getGroup(1).push_back(railway);
            model->setModified(true);
        } else
        if (s == "Провод")
        {
            float axis[6];
            axis[0] = x - 2.5f;
            axis[1] = y - 0.5f;
            axis[2] = 1.0f;
            axis[3] = x + 2.5f;
            axis[4] = y + 0.5f;
            axis[5] = 1.0f;
            VoltageLine* line = new VoltageLine(axis, 6);
            line->setModel(model);
            model->getGroup(1).push_back(line);
            model->setModified(true);
        } else
            if (s == "Двойной провод")
            {
                float axis[6];
                axis[0] = x - 5.0f;
                axis[1] = y - 0.5f;
                axis[2] = 1.0f;
                axis[3] = x + 5.0f;
                axis[4] = y + 0.5f;
                axis[5] = 1.0f;
                DoubleVoltageLine* line = new DoubleVoltageLine(axis, 6);
                line->setModel(model);
                model->getGroup(1).push_back(line);
                model->setModified(true);
            } else
    if (s == "Ломаная")
    {
        float ar[15];
        ar[0] = x - 2.4f;
        ar[1] = y + 1.3f;
        ar[2] = 0.025f;
        ar[3] = x - 2.0f;
        ar[4] = y + 0.5f;
        ar[5] = 0.025f;
        ar[6] = x;
        ar[7] = y;
        ar[8] = 0.025f;
        ar[9] = x + 2.0f;
        ar[10] = y + 1.0f;
        ar[11] = 0.025f;
        ar[12] = x + 3.0f;
        ar[13] = y - 1.5f;
        ar[14] = 0.025f;
        LineBroken* line = new LineBroken(1.1f, ar, 15, 1.0f, 1.0f, 1.0f, 1.0f, "LineSolidBroken", 1);
        line->setModel(model);
        model->getGroup(1).push_back(line);
        model->setModified(true);
    } else
    if (s == "Куб")
    {
        model->getGroup(2).push_back(new Cube(x, y, 1.0f, 2.0f, "Cube", 2));
        model->setModified(true);
    } else
    if (s == "Audi Q7")
    {
        RoadElementOBJ* element = new RoadElementOBJ(x, y);

       stateManager->fileManagerOBJ->loadOBJ((QApplication::applicationDirPath() + "/models/cars/audi_q7/").toStdString().c_str(),"audi_q7.obj",
                             element->meshes,2.177f, element->scaleFactor, 2);
       element->setModel(model);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        model->setModified(true);

    } else
    if (s == "BMW M3")
    {
        RoadElementOBJ* element = new RoadElementOBJ(x, y);
        stateManager->fileManagerOBJ->loadOBJ(QApplication::applicationDirPath() + "/models/cars/bmw_m3/","bmw_m3.obj",
                             element->meshes,1.976f, element->scaleFactor, 2);
        //fileManager->loadOBJ("models/cars/","bmw_m3.obj",
        //                     element->meshes,2.177f, element->scaleFactor);
        element->setModel(model);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        element->setSelectedStatus(false);
        model->setModified(true);
    } else
            if (s == "ВАЗ-2104")
            {
                RoadElementOBJ* element = new RoadElementOBJ(x, y);
                stateManager->fileManagerOBJ->loadOBJ((QApplication::applicationDirPath() + "/models/cars/vaz_2104/").toStdString().c_str(),"vaz_2104.obj",
                                     element->meshes,1.74f, element->scaleFactor);
                //fileManager->loadOBJ("models/cars/","vaz_2104.obj",
                //                     element->meshes,1.74f, element->scaleFactor);
               element->setModel(model);
                model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
                model->setModified(true);
            } else
    if (s == "ВАЗ-2106")
    {
        RoadElementOBJ* element = new RoadElementOBJ(x, y);
        stateManager->fileManagerOBJ->loadOBJ((QApplication::applicationDirPath() + "/models/cars/VAZ_2106/").toStdString().c_str(),"VAZ_2106.obj",
                             element->meshes,1.74f, element->scaleFactor, 1);
        //fileManager->loadOBJ("models/cars/","VAZ_2106.obj",
        //                     element->meshes,1.74f, element->scaleFactor);
        element->setModel(model);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        model->setModified(true);
    } else
        if (s == "Dodge_Ram_2007")
        {
            RoadElementOBJ* element = new RoadElementOBJ(x, y);
            stateManager->fileManagerOBJ->loadOBJ((QApplication::applicationDirPath() + "/models/cars/Dodge_Ram_2007/").toStdString().c_str(),"Dodge_Ram_2007.obj",
                                 element->meshes,2.022f, element->scaleFactor);
            //fileManager->loadOBJ("models/cars/","Dodge_Ram_2007.obj",
            //                     element->meshes,1.74f, element->scaleFactor);
           element->setModel(model);
            model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
            model->setModified(true);
        } else
                if (s == "Автобус ПАЗ")
                {
                    RoadElementOBJ* element = new RoadElementOBJ(x, y);
                    stateManager->fileManagerOBJ->loadOBJ((QApplication::applicationDirPath() + "/models/cars/PAZ_1/").toStdString().c_str(),"PAZ_1.obj",
                                         element->meshes,2.5f, element->scaleFactor, 1);
                    //fileManager->loadOBJ("models/cars/","PAZ_1.obj",
                    //                     element->meshes,1.74f, element->scaleFactor);
                    element->setModel(model);
                    model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
                    model->setModified(true);
                } else
    if (s == "Микроавтобус Ford")
    {
        RoadElementOBJ* element = new RoadElementOBJ(x, y);
        stateManager->fileManagerOBJ->loadOBJ((QApplication::applicationDirPath() + "/models/cars/ford_transit_bus/").toStdString().c_str(),"ford_transit_bus.obj",
                             element->meshes,2.374f, element->scaleFactor);
        //fileManager->loadOBJ("models/cars/","ford_transit_bus.obj",
        //                     element->meshes,1.74f, element->scaleFactor);
        element->setModel(model);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        model->setModified(true);
    } else
    if (s == "Грузовик Ford")
    {
        RoadElementOBJ* element = new RoadElementOBJ(x, y);
        stateManager->fileManagerOBJ->loadOBJ((QApplication::applicationDirPath() + "/models/cars/ford_transit_1/").toStdString().c_str(),"ford_transit_1.obj",
                             element->meshes,2.374f, element->scaleFactor);
        //fileManager->loadOBJ("models/cars/","ford_transit_1.obj",
        //                     element->meshes,1.74f, element->scaleFactor);
        element->setModel(model);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        model->setModified(true);
    } else
        if (s == "DAF XF")
        {
            RoadElementOBJ* element = new RoadElementOBJ(x, y);
            stateManager->fileManagerOBJ->loadOBJ((QApplication::applicationDirPath() + "/models/cars/DAF_xf/").toStdString().c_str(),"DAF_XF.obj",
                                 element->meshes,3.374f, element->scaleFactor);
            //fileManager->loadOBJ("models/cars/","ford_transit_1.obj",
            //                     element->meshes,1.74f, element->scaleFactor);
            element->setModel(model);
            model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
            model->setModified(true);
        } else
    if (s == "Остановка")
    {
        RoadElementOBJ* element = new RoadElementOBJ(x, y);
        stateManager->fileManagerOBJ->loadOBJ((QApplication::applicationDirPath() + "/models/cars/bus_stop_1/").toStdString().c_str(),"bus_stop_1.obj",
                             element->meshes,2.374f, element->scaleFactor);
        element->setModel(model);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        model->setModified(true);
    } else
        if (s == "Человек")
        {
            RoadElementOBJ* element = new RoadElementOBJ(x, y);
            stateManager->fileManagerOBJ->loadOBJ("D:/QT/Projects/Constructor/build-Constructor-Desktop_Qt_5_4_1_MinGW_32bit-Debug/debug/","man.obj",
                                 element->meshes,2.374f, element->scaleFactor);
            element->setModel(model);
            model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
            model->setModified(true);
        } else
    /*
    if (s == "Знак")
    {
        OBJFileManager* fileManager = new OBJFileManager(model);
        RoadElement3D* element = new RoadElement3D();
        fileManager->loadOBJ("D:/QT/Projects/Constructor/build-Constructor-Desktop_Qt_5_4_1_MinGW_32bit-Debug/debug/","city_bump2.obj",
                             element->meshes,2.374f, element->scaleFactor);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        model->setModified(true);
    }
    */
            if (s == "Здание 1")
            {
                RoadElementOBJ* element = new RoadElementOBJ(x, y);
                stateManager->fileManagerOBJ->loadOBJ((QApplication::applicationDirPath() + "/models/buildings/build10/").toStdString().c_str(),"Build10_obj.obj",
                                     element->meshes,50.374f, element->scaleFactor);
               element->setModel(model);
                model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
                model->setModified(true);
            }
    else
                if (s == "Здание 2")
                {
                    RoadElementOBJ* element = new RoadElementOBJ(x, y);
                    stateManager->fileManagerOBJ->loadOBJ((QApplication::applicationDirPath() + "/models/buildings/build11/").toStdString().c_str(),"Build11_obj.obj",
                                         element->meshes,2.374f, element->scaleFactor);
                    element->setModel(model);
                    model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
                    model->setModified(true);
                }
        else
                    if (s == "Дерево 1")
                    {
                        RoadElementOBJ* element = new RoadElementOBJ(x, y);
                        stateManager->fileManagerOBJ->loadOBJ((QApplication::applicationDirPath() + "/models/humans/man/").toStdString().c_str(),"Man.obj",
                                             element->meshes,2.374f, element->scaleFactor);
                        element->setModel(model);
                        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
                        model->setModified(true);
                    }
            else
                        if (s == "Дерево 2")
                        {
                            RoadElementOBJ* element = new RoadElementOBJ(x, y);
                            stateManager->fileManagerOBJ->loadOBJ((QApplication::applicationDirPath() + "/models/plants/tree2/").toStdString().c_str(),"Tree2.obj",
                                                 element->meshes,2.374f, element->scaleFactor);
                            element->setModel(model);
                            model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
                            model->setModified(true);
                        }
                else
                            if (s == "Дерево 3")
                            {
                                RoadElement3D* element = new RoadElement3D(x, y);
                                stateManager->fileManager3DS->load3DS((QApplication::applicationDirPath() + "/models/plants/BlackLocust/").toStdString().c_str(),
                                                     "black_locust1.3ds",
                                                     element->meshes,element->materials);
                                element->setModel(model);
                                element->setSelectedStatus(true);
                                model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
                                model->setModified(true);
                            }
                    else
    if (s == "Знак")
    {
        RoadElement3D* element = new RoadElement3D(x, y);
        stateManager->fileManager3DS->load3DS("D:/QT/Projects/Constructor/build-Constructor-Desktop_Qt_5_4_1_MinGW_32bit-Debug/debug/","road60m_15left_barrier.3ds",
                             element->meshes,element->materials);
        element->setSelectedStatus(true);
        element->setModel(model);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        model->setModified(true);
    }else
        if (s == "Здание")
        {
            RoadElementOBJ* element = new RoadElementOBJ(x, y);
            stateManager->fileManagerOBJ->loadOBJ((QApplication::applicationDirPath() + "/models/buildings/").toStdString().c_str(),"Bld_02.obj",
                                 element->meshes,0.0f, element->scaleFactor);
            element->setModel(model);
            element->scaleFactor = 1.0f;
            model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
            model->setModified(true);
        }
    /*
    else
    {
        _3DsFileManager* fileManager = new _3DsFileManager();
        RoadElement3D* element = new RoadElement3D();
        fileManager->load3DS("D:/3ds/elements/",s.toStdString().c_str(), element->meshes,element->materials);
        element->setSelectedStatus(true);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        model->setModified(true);
    }
    */
    //QString(event->mimeData()->data());
    else
    {

        QStringList lst =  QString(event->mimeData()->data("text/plain")).split(' ');
        for (int i = 0; i < lst.size(); ++i)
        {
            //qDebug() << lst.at(i);
        }
        if (lst.at(1)[lst.at(1).size() - 1] == 's')
        {

            RoadElement3D* element = new RoadElement3D(x, y);
            stateManager->fileManager3DS->load3DS(lst.at(0).toStdString().c_str(),
                                 lst.at(1).toStdString().c_str(),
                                 element->meshes,
                                 element->materials);

            element->setModel(model);
            model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
            model->setModified(true);
            element->setSelectedStatus(false);

        }
        else
            if (lst.at(1)[lst.at(1).size() - 1] == 'j')
            {
                /*
                RoadElementOBJ* element = new RoadElementOBJ(x, y);
                stateManager->fileManagerOBJ->loadOBJ(lst.at(0).toStdString().c_str(),
                                     lst.at(1).toStdString().c_str(),
                                     element->meshes,2.374f, element->scaleFactor);
                element->setModel(model);
                element->scaleFactor = 1.0f;
                model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
                model->setModified(true);
                element->setSelectedStatus(false);
                */
                RoadElementOBJ* element = new RoadElementOBJ(x, y);
                stateManager->fileManagerOBJ->loadOBJ(lst.at(0),
                                     lst.at(1),
                                     element->meshes,2.374f, element->scaleFactor);
                element->setModel(model);
                element->scaleFactor = 1.0f;
                model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
                model->setModified(true);
                element->setSelectedStatus(false);
            }
        ////qDebug() << lst.at(1)[lst.at(1).size() - 1];
        ////qDebug() << s.toStdString().c_str();


    }


    scene->updateGL();
}

bool RoadBuilderState::tryToSelectControlsInSelectedFigure(QPoint mp)
{
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
    this->name = name;
}

void RoadBuilderState::setLayer(int layer)
{
    this->layer = layer;
}

void RoadBuilderState::setGroupIndex(int index)
{
    groupIndex = index;
}

void RoadBuilderState::setElementIndex(int index)
{
    elementIndex = index;
}


QString RoadBuilderState::getName()
{
    return "RoadBuilderState";
}

void RoadBuilderState::clearProperties(QFormLayout *layout)
{
    while(QLayoutItem* child = layout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }
}

void RoadBuilderState::clear()
{
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
    if (roadBroken)
        this->roadBroken = roadBroken;
}


void RoadBuilderState::contextMenuEvent(QContextMenuEvent *pe)
{
}

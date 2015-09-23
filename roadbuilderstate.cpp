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
                    roadBroken = NULL;
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

                    roadBroken->resizeByControl(controlIndex, dX, dY, x, y);
                    scene->updateGL();
                }
                else
                {
                    if (this->tryToSelectFigures(pe->pos()) == true)
                    {
                        float dY = (GLfloat)(-1)*(pe->y()-ptrMousePosition.y())/
                                scene->width()/(scene->nSca * scene->ratio) * 2.0;
                        float dX = (GLfloat)(pe->x()-ptrMousePosition.x())/
                                scene->width()/(scene->nSca * scene->ratio) * 2.0;

                        roadBroken->move(dX, dY);
                        scene->updateGL();
                    }
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
        model->getGroup(layer).pop_back();
        scene->setCursor(Qt::ArrowCursor);
        delete roadBroken;
        roadBroken = NULL;
        clearProperties(properties);
        stateManager->setState(stateManager->defaultState);
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
        //model->getGroup(0).push_back(new RoadSimple(x - 2.0f, y, x + 2.0f, y, 2.0f, 0.5f, 0.5f, 0.5f, 1.0f, "RoadSimple", 0));
        model->getGroup(0).push_back(new RoadSimple(x - 2.0f, y, x + 2.0f, y, 6.0f,
                                                    QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                                    QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                                    "RoadSimple", 0));
        model->setModified(true);
    } else
        if (s == "Закругление")
        {
             model->getGroup(0).push_back(new Curve(x, y, 0.0f,
                                                    x - 5.0f, y, 0.0f,
                                                    x, y + 5.0f, 0.0f,
                                                    QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                                    QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                                    10));
             model->setModified(true);
        } else
            if (s == "Разделительная зона")
            {
                model->getGroup(0).push_back(new SplitZone(x,y,0.3f,
                                                           x + 10.0f, y, 0.3f,
                                                           2.0f,
                                                           true,
                                                           true));
                model->setModified(true);
            }
    else
    if (s == "Дорога ломаная")
    {
        model->getGroup(0).push_back(new RoadBroken(x, y, x + 2.75f, y, 6.0f,
                                                    QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                                    QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                                    "RoadBroken", 0));
        model->setModified(true);
    } else
    if (s == "Круговой перекресток")
    {
        model->getGroup(0).push_back(new RoundingCrossRoad(x, y, 10, 16, 50, "RoundingCrossRoad", 0));
        model->setModified(true);
    } else
    if (s == "Поворот дороги")
    {
        //model->getGroup(0).push_back(new RoundingRoad(x, y, 10, 0, 90,
        //                                              x, y, 16, 0, 90,
        //                                              50, "RoundingRoad", 0));
        model->getGroup(0).push_back(new RoundingRoad(x, y, 50, 0, 90,
                                                      x, y, 56, 0, 90,
                                                      20, "RoundingRoad", 0,
                                                      QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                                      QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f));
        model->setModified(true);
    } else
        if (s == "Перекресток")
        {
            model->getGroup(0).push_back(new Intersection(x, y));
            model->setModified(true);
        } else
    if (s == "Сплошая")
    {
        model->getGroup(1).push_back(new LineSimple(x - 2.0, y, x + 2.0, y, 0.1, 1.0f, 1.0f, 1.0f, 1.0f, "LineSolid", 1));
        model->setModified(true);
    } else
    if (s == "Прерывистая")
    {
        model->getGroup(1).push_back(new LineSimple(x - 2.0, y, x + 2.0, y, 0.1, QString(":/textures/intermittent.png"), 0.8f, "LineIntermittent", 1));
        model->setModified(true);
    } else
    if (s == "Двойная сплошая")
    {
        model->getGroup(1).push_back(new LineSimple(x - 2.0, y, x + 2.0, y, 0.28, QString(":/textures/double_solid.png"), 1.0f, "LineDoubleSolid", 1));
        model->setModified(true);
    } else
    if (s == "Двойная прерывистая")
    {
        model->getGroup(1).push_back(new LineSimple(x - 2.0, y, x + 2.0, y, 0.28, QString(":/textures/double_solid_intermittent.png"), 0.8f, "LineDoubleSolidIntermittent", 1));
        model->setModified(true);
    } else
    if (s == "Пешеходный переход")
    {
        model->getGroup(1).push_back(new RoadSimple(x - 2.0f, y, x + 2.0f, y, 2.0f,
                                                    QString(":/textures/crosswalk.png"), 1.0f, 1.0f,
                                                    QString(":/textures/crosswalk.png"), 1.0f, 1.0f,
                                                    "Crosswalk", 1));
        model->setModified(true);
    } else
    if (s == "Трамвайные пути")
    {
        model->getGroup(1).push_back(new LineSimple(x - 2.0, y, x + 2.0, y, 1.5, QString(":/textures/tramways.png"), 1.5f, "Tramways", 1));
        model->setModified(true);
    } else
    if (s == "Железная дорога")
    {
        model->getGroup(1).push_back(new LineSimple(x - 2.0, y, x + 2.0, y, 2.1, QString(":/textures/railroad.png"), 2.1f, "Railroad", 1));
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

        model->getGroup(1).push_back(new LineBroken(1.1f, ar, 15, 1.0f, 1.0f, 1.0f, 1.0f, "LineSolidBroken", 1));
        model->setModified(true);
    } else
    if (s == "Куб")
    {
        model->getGroup(2).push_back(new Cube(x, y, 1.0f, 2.0f, "Cube", 2));
        model->setModified(true);
    } else
    if (s == "Audi Q7")
    {
        OBJFileManager* fileManager = new OBJFileManager(model);
        RoadElementOBJ* element = new RoadElementOBJ(x, y);
       fileManager->loadOBJ((QApplication::applicationDirPath() + "/models/cars/audi_q7/").toStdString().c_str(),"audi_q7.obj",
                             element->meshes,2.177f, element->scaleFactor);
        //fileManager->loadOBJ("models/cars/","audi_q7.obj",
        //                     element->meshes,2.177f, element->scaleFactor);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        model->setModified(true);

    } else
    if (s == "BMW M3")
    {
        OBJFileManager* fileManager = new OBJFileManager(model);
        RoadElementOBJ* element = new RoadElementOBJ(x, y);
        fileManager->loadOBJ((QApplication::applicationDirPath() + "/models/cars/bmw_m3/").toStdString().c_str(),"bmw_m3.obj",
                             element->meshes,1.976f, element->scaleFactor);
        //fileManager->loadOBJ("models/cars/","bmw_m3.obj",
        //                     element->meshes,2.177f, element->scaleFactor);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        element->setSelectedStatus(false);
        model->setModified(true);
    } else
            if (s == "ВАЗ-2104")
            {
                OBJFileManager* fileManager = new OBJFileManager(model);
                RoadElementOBJ* element = new RoadElementOBJ(x, y);
                fileManager->loadOBJ((QApplication::applicationDirPath() + "/models/cars/vaz_2104/").toStdString().c_str(),"vaz_2104.obj",
                                     element->meshes,1.74f, element->scaleFactor);
                //fileManager->loadOBJ("models/cars/","vaz_2104.obj",
                //                     element->meshes,1.74f, element->scaleFactor);
                model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
                model->setModified(true);
            } else
    if (s == "ВАЗ-2106")
    {
        OBJFileManager* fileManager = new OBJFileManager(model);
        RoadElementOBJ* element = new RoadElementOBJ(x, y);
        fileManager->loadOBJ((QApplication::applicationDirPath() + "/models/cars/VAZ_2106/").toStdString().c_str(),"VAZ_2106.obj",
                             element->meshes,1.74f, element->scaleFactor);
        //fileManager->loadOBJ("models/cars/","VAZ_2106.obj",
        //                     element->meshes,1.74f, element->scaleFactor);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        model->setModified(true);
    } else
        if (s == "Dodge_Ram_2007")
        {
            OBJFileManager* fileManager = new OBJFileManager(model);
            RoadElementOBJ* element = new RoadElementOBJ(x, y);
            fileManager->loadOBJ((QApplication::applicationDirPath() + "/models/cars/Dodge_Ram_2007/").toStdString().c_str(),"Dodge_Ram_2007.obj",
                                 element->meshes,2.022f, element->scaleFactor);
            //fileManager->loadOBJ("models/cars/","Dodge_Ram_2007.obj",
            //                     element->meshes,1.74f, element->scaleFactor);
            model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
            model->setModified(true);
        } else
                if (s == "Автобус ПАЗ")
                {
                    OBJFileManager* fileManager = new OBJFileManager(model);
                    RoadElementOBJ* element = new RoadElementOBJ(x, y);
                    fileManager->loadOBJ((QApplication::applicationDirPath() + "/models/cars/PAZ_1/").toStdString().c_str(),"PAZ_1.obj",
                                         element->meshes,2.5f, element->scaleFactor);
                    //fileManager->loadOBJ("models/cars/","PAZ_1.obj",
                    //                     element->meshes,1.74f, element->scaleFactor);
                    model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
                    model->setModified(true);
                } else
    if (s == "Микроавтобус Ford")
    {
        OBJFileManager* fileManager = new OBJFileManager(model);
        RoadElementOBJ* element = new RoadElementOBJ(x, y);
        fileManager->loadOBJ((QApplication::applicationDirPath() + "/models/cars/ford_transit_bus/").toStdString().c_str(),"ford_transit_bus.obj",
                             element->meshes,2.374f, element->scaleFactor);
        //fileManager->loadOBJ("models/cars/","ford_transit_bus.obj",
        //                     element->meshes,1.74f, element->scaleFactor);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        model->setModified(true);
    } else
    if (s == "Грузовик Ford")
    {
        OBJFileManager* fileManager = new OBJFileManager(model);
        RoadElementOBJ* element = new RoadElementOBJ(x, y);
        fileManager->loadOBJ((QApplication::applicationDirPath() + "/models/cars/ford_transit_1/").toStdString().c_str(),"ford_transit_1.obj",
                             element->meshes,2.374f, element->scaleFactor);
        //fileManager->loadOBJ("models/cars/","ford_transit_1.obj",
        //                     element->meshes,1.74f, element->scaleFactor);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        model->setModified(true);
    } else
    if (s == "Остановка")
    {
        OBJFileManager* fileManager = new OBJFileManager(model);
        RoadElementOBJ* element = new RoadElementOBJ();
        fileManager->loadOBJ((QApplication::applicationDirPath() + "/models/cars/bus_stop_1/").toStdString().c_str(),"bus_stop_1.obj",
                             element->meshes,2.374f, element->scaleFactor);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        model->setModified(true);
    } else
        if (s == "Человек")
        {
            OBJFileManager* fileManager = new OBJFileManager(model);
            RoadElementOBJ* element = new RoadElementOBJ(x, y);
            fileManager->loadOBJ("D:/QT/Projects/Constructor/build-Constructor-Desktop_Qt_5_4_1_MinGW_32bit-Debug/debug/","man.obj",
                                 element->meshes,2.374f, element->scaleFactor);
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
    if (s == "Знак")
    {
        //OBJFileManager* fileManager = new OBJFileManager(model);
        _3DsFileManager* fileManager = new _3DsFileManager();
        RoadElement3D* element = new RoadElement3D(x, y);
        fileManager->load3DS("D:/QT/Projects/Constructor/build-Constructor-Desktop_Qt_5_4_1_MinGW_32bit-Debug/debug/","road60m_15left_barrier.3ds",
                             element->meshes,element->materials);
        element->setSelectedStatus(true);
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
        _3DsFileManager* fileManager = new _3DsFileManager();
        RoadElement3D* element = new RoadElement3D(x, y);
        QStringList lst =  QString(event->mimeData()->data("text/plain")).split(' ');
        for (int i = 0; i < lst.size(); ++i)
        {
            qDebug() << lst.at(i);
        }
        //qDebug() << s.toStdString().c_str();
        fileManager->load3DS(lst.at(0).toStdString().c_str(),
                             lst.at(1).toStdString().c_str(),
                             element->meshes,
                             element->materials);
        element->setSelectedStatus(false);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        model->setModified(true);
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

void RoadBuilderState::setRoad(RoadBroken *roadBroken)
{
    if (roadBroken)
        this->roadBroken = roadBroken;
}


void RoadBuilderState::contextMenuEvent(QContextMenuEvent *pe)
{
}

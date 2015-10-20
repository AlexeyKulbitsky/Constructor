#include "defaultstate.h"
#include "objfilemanager.h"
#include "_3dsfilemanager.h"
#include <QApplication>
#include <GL/glu.h>

DefaultState::DefaultState()
{
    this->stateManager = NULL;
    this->model = NULL;
    this->scene = NULL;
    this->properties = NULL;
    rightButtonIsPressed = false;
    leftButtonIsPressed = false;
    rectSelection = false;
    selectedElementsCount = 0;
}

DefaultState::DefaultState(StateManager *manager, Model *model, Scene2D* scene, QFormLayout *properties)
{
    this->stateManager = manager;
    this->model = model;
    this->scene = scene;
    this->properties = properties;
    rightButtonIsPressed = false;
    leftButtonIsPressed = false;
    rectSelection = false;
    selectedElementsCount = 0;
}


void DefaultState::mousePressEvent(QMouseEvent *pe)
{
    //renderText (pe->pos().x(), pe->pos().y(), "HELLO");
    ptrMousePosition = pe->pos();
    switch (pe->button())
    {
    case Qt::LeftButton:
        leftButtonIsPressed = true;
        scene->rectPoint1 = pe->pos();
        rectSelection = true;
        break;

    case Qt::RightButton:
        rightButtonIsPressed = true;
        break;
    default:
        break;
    }
    scene->updateGL();
}

void DefaultState::mouseMoveEvent(QMouseEvent *pe)
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
    if (leftButtonIsPressed == true)
    {
        if (rectSelection == true)
        {
            scene->rectPoint2 = pe->pos();
            scene->setDrawRectStatus(true);
        }
    }
    ptrMousePosition = pe->pos();
    scene->updateGL();
}

void DefaultState::mouseReleaseEvent(QMouseEvent *pe)
{
    if(pe->button() == Qt::RightButton)
    {
        rightButtonIsPressed = false;
    }
    if(pe->button() == Qt::LeftButton)
    {
        leftButtonIsPressed = false;
        scene->setDrawRectStatus(false);
        if (rectSelection == true)
        {

            if (abs(scene->rectPoint1.x() - pe->pos().x()) <= 5 &&
                abs(scene->rectPoint1.y() - pe->pos().y()) <= 5)
            {
                if (this->tryToSelectFigures(pe->pos(), true) == true)
                {
                    scene->setMouseTracking(true);
                    scene->updateGL();
                    std::list<RoadElement*>::iterator it = model->getGroup(selectedGroup).begin();
                    for(int k = 0; k < selectedIndex; ++k)
                        ++it;
                    if ((*it)->getName() == "RoadBroken")
                    {
                        stateManager->roadBuilderState->setRoad(dynamic_cast<RoadBroken*>(*it));
                        stateManager->roadBuilderState->setGroupIndex(selectedGroup);
                        stateManager->roadBuilderState->setElementIndex(selectedIndex);
                        stateManager->setState(stateManager->roadBuilderState);
                    }
                    else
                    {
                        if ((*it)->getName() == "LineBroken" ||
                                (*it)->getName() == "Tramways" ||
                                (*it)->getName() == "VoltageLine" ||
                                (*it)->getName() == "DoubleVoltageLine" ||
                                (*it)->getName() == "RailWay")
                        {
                            stateManager->lineBuilderState->setLine(dynamic_cast<LineBroken*>(*it));
                            stateManager->lineBuilderState->setGroupIndex(selectedGroup);
                            stateManager->lineBuilderState->setElementIndex(selectedIndex);
                            stateManager->setState(stateManager->lineBuilderState);
                        }
                        else
                        {
                            stateManager->setState(stateManager->selectedState);
                        }
                    }
                    rectSelection = false;
                }
            }
            else

            if (this->tryToSelectFigures(scene->rectPoint1, pe->pos(), true) == true)
            {
                scene->setMouseTracking(true);
                scene->updateGL();
                if (selectedElementsCount == 1)
                {
                    std::list<RoadElement*>::iterator it = model->getGroup(selectedGroup).begin();
                    for(int k = 0; k < selectedIndex; ++k)
                        ++it;
                    if ((*it)->getName() == "RoadBroken")
                    {
                        stateManager->roadBuilderState->setRoad(dynamic_cast<RoadBroken*>(*it));
                        stateManager->roadBuilderState->setGroupIndex(selectedGroup);
                        stateManager->roadBuilderState->setElementIndex(selectedIndex);
                        stateManager->setState(stateManager->roadBuilderState);
                    }
                    else
                    {
                        if ((*it)->getName() == "LineBroken" ||
                                (*it)->getName() == "Tramways" ||
                                (*it)->getName() == "VoltageLine" ||
                                (*it)->getName() == "DoubleVoltageLine")
                        {
                            stateManager->lineBuilderState->setLine(dynamic_cast<LineBroken*>(*it));
                            stateManager->lineBuilderState->setGroupIndex(selectedGroup);
                            stateManager->lineBuilderState->setElementIndex(selectedIndex);
                            stateManager->setState(stateManager->lineBuilderState);
                        }
                        else
                        {
                            stateManager->setState(stateManager->selectedState);
                        }
                    }
                }
                else
                    if (selectedElementsCount > 1)
                    {
                        stateManager->setState(stateManager->selectedState);
                    }

                rectSelection = false;

            }
        }

    }
    ptrMousePosition = pe->pos();
    scene->updateGL();
}

void DefaultState::wheelEvent(QWheelEvent *pe)
{
    if ((pe->delta())>0) scene->scale_plus();
    else
        if ((pe->delta())<0) scene->scale_minus();
    qDebug() << "Wheel";
    scene->updateGL();
}

void DefaultState::keyPressEvent(QKeyEvent *pe)
{
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

    case Qt::Key_Delete:
        qDebug() << "DefaultState - Key_Delete";
        break;


    }

    scene->updateGL();
}

void DefaultState::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void DefaultState::dropEvent(QDropEvent *event)
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
            if (s == "Разделительная зона")
            {
                SplitZone* splitZone = new SplitZone(x,y,0.3f,
                                                     x + 10.0f, y, 0.3f,
                                                     2.0f,
                                                     true,
                                                     true);
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
    if (s == "Железная дорога")
    {
        LineSimple* railway = new LineSimple(x - 2.0, y, x + 2.0, y, 2.1, QString(":/textures/railroad.png"), 2.1f, "Railroad", 1);
        railway->setModel(model);
        model->getGroup(1).push_back(railway);
        model->setModified(true);
    } else
        if (s == "Железная дорога (Новая)")
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
                             element->meshes,2.177f, element->scaleFactor);
       element->setModel(model);
        model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
        model->setModified(true);

    } else
    if (s == "BMW M3")
    {
        RoadElementOBJ* element = new RoadElementOBJ(x, y);
        stateManager->fileManagerOBJ->loadOBJ(QApplication::applicationDirPath() + "/models/cars/bmw_m3/","bmw_m3.obj",
                             element->meshes,1.976f, element->scaleFactor);
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
                             element->meshes,1.74f, element->scaleFactor);
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
                                         element->meshes,2.5f, element->scaleFactor);
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
            qDebug() << lst.at(i);
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
                                     element->meshes,2.374f, element->scaleFactor, -1);
                element->setModel(model);
                element->scaleFactor = 1.0f;
                model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
                model->setModified(true);
                element->setSelectedStatus(false);
            }
        //qDebug() << lst.at(1)[lst.at(1).size() - 1];
        //qDebug() << s.toStdString().c_str();


    }


    scene->updateGL();
}

DefaultState::~DefaultState()
{

}

void DefaultState::clearProperties(QFormLayout *layout)
{
    while(QLayoutItem* child = layout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }

}

bool DefaultState::tryToSelectFigures(QPoint mp, bool withResult)
{
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
        for (std::list<RoadElement*>::iterator it = model->getGroup(j).begin();
             it != model->getGroup(j).end(); ++it)
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

    hitsForFigure=glRenderMode(GL_RENDER); // число совпадений и переход в режим рисования

    if (hitsForFigure > 0) // есть совпадания и нет ошибок
    {
        selectedElementsCount = hitsForFigure;
        i = selectBuffer[3] - 1; // имя фигуры верхняя фигура
        for (int j = model->getNumberOfGroups() - 1; j >= 0; --j)
        {
            if (i < model->getGroup(j).size())
            {

                selectedGroup = j;
                selectedIndex = i;
                std::list<RoadElement*>::iterator it = model->getGroup(selectedGroup).begin();
                for(int k = 0; k < selectedIndex; ++k)
                    ++it;
                if ((*it)->getName() != "RoadBroken")
                {
                    stateManager->selectedState->setSelectedElement(i, j);
                    stateManager->selectedState->setSelectedElement(*it);
                }

                (*it)->setSelectedStatus(true);
                (*it)->setStepDialog(stateManager->stepDialog);
                (*it)->getProperties(properties, scene);

                scene->setCursor(Qt::SizeAllCursor);
                glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
                glPopMatrix(); // извлечь матрицу из стека матриц
                scene->updateGL(); // обновить изображение
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
    scene->updateGL(); // обновить изображение
    return false;

}

bool DefaultState::tryToSelectFigures(QPoint mp1, QPoint mp2, bool withResult)
{
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
   // gluPickMatrix((GLdouble)((mp1.x() + mp2.x()) / 2), (GLdouble)(viewport[3] - (mp1.y() + mp2.y()) / 2),
   //         (GLdouble)(abs(mp2.x() - mp1.x())), (GLdouble)(abs(mp2.y() - mp1.y())),
   //         viewport);

    //qDebug () << "X1=" << mp1.x();
    //qDebug () << "Y1=" << mp1.y();
    //qDebug () << "X2=" << mp2.x();
    //qDebug () << "Y2=" << mp2.y();

    GLdouble width = abs(mp1.x() - mp2.x()) > 2 ? (GLdouble)(abs(mp1.x() - mp2.x())) : 2.0;
    GLdouble height = abs(mp1.y() - mp2.y()) > 2 ? (GLdouble)(abs(mp1.y() - mp2.y())) : 2.0;
    gluPickMatrix((GLdouble)((mp1.x() + mp2.x()) / 2), (GLdouble)(viewport[3] - (mp1.y() + mp2.y()) / 2),
                width, height,
                viewport);
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
        for (std::list<RoadElement*>::iterator it = model->getGroup(j).begin();
             it != model->getGroup(j).end(); ++it)
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

    hitsForFigure = glRenderMode(GL_RENDER);

    selectedElementsCount = hitsForFigure;

    qDebug() << "Hits: " << hitsForFigure;
    if (hitsForFigure > 0) // есть совпадания и нет ошибок
    {
        // Если выделена одна фигура
        if (hitsForFigure == 1)
        {
            i = selectBuffer[3] - 1; // имя фигуры верхняя фигура
            for (int j = model->getNumberOfGroups() - 1; j >= 0; --j)
            {
                if (i < model->getGroup(j).size())
                {

                    selectedGroup = j;
                    selectedIndex = i;
                    std::list<RoadElement*>::iterator it = model->getGroup(selectedGroup).begin();
                    for(int k = 0; k < selectedIndex; ++k)
                        ++it;
                    if ((*it)->getName() != "RoadBroken")
                    {
                        stateManager->selectedState->setSelectedElement(i, j);
                        stateManager->selectedState->setSelectedElement(*it);
                    }

                    (*it)->setSelectedStatus(true);
                    (*it)->setStepDialog(stateManager->stepDialog);
                    (*it)->getProperties(properties, scene);

                    scene->setCursor(Qt::SizeAllCursor);
                    glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
                    glPopMatrix(); // извлечь матрицу из стека матриц
                    scene->updateGL(); // обновить изображение
                    return true;
                }
                else
                {
                    i -= model->getGroup(j).size();
                }
            }
        }
        else
        {
            for (int hitNumber = 1; hitNumber <= hitsForFigure; ++hitNumber)
            {
                i = selectBuffer[hitNumber * 4 - 1] - 1; // имя фигуры верхняя фигура
                for (int j = model->getNumberOfGroups() - 1; j >= 0; --j)
                {
                    if (i < model->getGroup(j).size())
                    {
                        selectedGroup = j;
                        selectedIndex = i;
                        std::list<RoadElement*>::iterator it = model->getGroup(selectedGroup).begin();
                        for(int k = 0; k < selectedIndex; ++k)
                            ++it;
                        (*it)->setSelectedStatus(true);
                        stateManager->selectedState->selectedElements.push_back(*it);
                        break;
                    }
                    else
                    {
                        i -= model->getGroup(j).size();
                    }
                }
            }

            scene->setCursor(Qt::SizeAllCursor);
            glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
            glPopMatrix(); // извлечь матрицу из стека матриц
            scene->updateGL(); // обновить изображение
            return true;
        }

        glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
        glPopMatrix(); // извлечь матрицу из стека матриц
        scene->updateGL(); // обновить изображение
        return false;
    }


    glMatrixMode(GL_PROJECTION); // матрица проекции стала активной
    glPopMatrix(); // извлечь матрицу из стека матриц
    scene->updateGL(); // обновить изображение
    return false;

}


void DefaultState::keyReleaseEvent(QKeyEvent *pe)
{
}



QString DefaultState::getName()
{
    return "DefaultState";
}

void DefaultState::drawRect(QPoint p1, QPoint p2)
{
    GLdouble x1, y1, z1;
    GLdouble x2, y2, z2;
    getWorldCoord(p1.x(), p1.y(), 0, x1, y1, z1);
    getWorldCoord(p2.x(), p2.y(), 0, x2, y2, z2);
    glLineWidth(5.0f);
    glDisable(GL_DEPTH_TEST);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3d(x1, y1, z1);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3d(x1, y2, z1);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3d(x2, y2, z2);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3d(x2, y1, z2);
    glEnd();
    glEnable(GL_DEPTH_TEST);
    //qDebug() << "drawRect";
}

void DefaultState::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    gluProject(x, y, z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
    wy=viewport[3]-wy;
}

void DefaultState::getWorldCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
    y - viewport[3]-y;
    gluUnProject(x, y, z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
}


void DefaultState::contextMenuEvent(QContextMenuEvent *pe)
{
}

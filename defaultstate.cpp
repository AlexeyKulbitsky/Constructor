#include "defaultstate.h"
#include "objfilemanager.h"
#include "_3dsfilemanager.h"
#include <QApplication>
#include <GL/glu.h>

bool DefaultState::log = true;

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
    if (log)
        Logger::getLogger()->infoLog() << "Creating DefaultState\n";
    if (manager == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Cannot create DefaultState: StateManager = NULL, program terminates");
        if (log)
            Logger::getLogger()->errorLog() << "Cannot create DefaultState: StateManager = NULL, program terminates\n";
        QApplication::exit(0);
    }
    this->stateManager = manager;
    if (model == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Cannot create DefaultState: Model = NULL, program terminates");
        if (log)
            Logger::getLogger()->errorLog() << "Cannot create DefaultState: Model = NULL, program terminates\n";
        QApplication::exit(0);
    }
    this->model = model;
    if (scene == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Cannot create DefaultState: Scene2D = NULL, program terminates");
        if (log)
            Logger::getLogger()->errorLog() << "Cannot create DefaultState: Scene2D = NULL, program terminates\n";
        QApplication::exit(0);
    }
    this->scene = scene;
    if (properties == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Cannot create DefaultState: QFormLayout(properties) = NULL, program terminates");
        if (log)
            Logger::getLogger()->errorLog() << "Cannot create DefaultState: QFormLayout(properties) = NULL, program terminates\n";
        QApplication::exit(0);
    }
    this->properties = properties;
    rightButtonIsPressed = false;
    leftButtonIsPressed = false;
    rectSelection = false;
    selectedElementsCount = 0;
}


void DefaultState::mousePressEvent(QMouseEvent *pe)
{
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::mousePressEvent(QMouseEvent *pe)\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::mouseMoveEvent\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::mouseReleaseEvent(QMouseEvent *pe)\n";
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
                    QList<RoadElement*>::iterator it = model->getGroup(selectedGroup).begin();
                    for(int k = 0; k < selectedIndex; ++k)
                        ++it;
                    (*it)->setSelectedStatus(true);

                    (*it)->setStepDialog(stateManager->stepDialog);
                    (*it)->setStepDialogs(stateManager->stepDialogs, 10);
                    (*it)->getProperties(properties, scene);
                    scene->setCursor(Qt::SizeAllCursor);

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
                                (*it)->getName() == "RailWay" ||
                                (*it)->getName() == "SplitZone")
                        {
                            stateManager->lineBuilderState->setLine(dynamic_cast<LineBroken*>(*it));
                            stateManager->lineBuilderState->setGroupIndex(selectedGroup);
                            stateManager->lineBuilderState->setElementIndex(selectedIndex);
                            stateManager->setState(stateManager->lineBuilderState);
                        }
                        else
                        {
                            stateManager->selectedState->setSelectedElement(selectedIndex, selectedGroup);
                            stateManager->selectedState->setSelectedElement(*it);
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
                    scene->setCursor(Qt::SizeAllCursor);
                    if (selectedElementsCount == 1)
                    {
                        QList<RoadElement*>::iterator it = model->getGroup(selectedGroup).begin();
                        for(int k = 0; k < selectedIndex; ++k)
                            ++it;
                        (*it)->setSelectedStatus(true);

                        (*it)->setStepDialog(stateManager->stepDialog);
                        (*it)->setStepDialogs(stateManager->stepDialogs, 10);
                        (*it)->getProperties(properties, scene);


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
                                    (*it)->getName() == "RailWay" ||
                                    (*it)->getName() == "SplitZone")
                            {
                                stateManager->lineBuilderState->setLine(dynamic_cast<LineBroken*>(*it));
                                stateManager->lineBuilderState->setGroupIndex(selectedGroup);
                                stateManager->lineBuilderState->setElementIndex(selectedIndex);
                                stateManager->setState(stateManager->lineBuilderState);
                            }
                            else
                            {
                                stateManager->selectedState->setSelectedElement(selectedIndex, selectedGroup);
                                stateManager->selectedState->setSelectedElement(*it);
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
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::wheelEvent(QWheelEvent *pe)\n";
    if ((pe->delta())>0) scene->scalePlus();
    else
        if ((pe->delta())<0) scene->scaleMinus();
    scene->updateGL();
}

void DefaultState::keyPressEvent(QKeyEvent *pe)
{
    QString k;
    switch (pe->key())
    {
    case Qt::Key_Plus:
        k = "Qt::Key_Plus";
        scene->scalePlus();
        break;

    case Qt::Key_Equal:
        k = "Qt::Key_Equal";
        scene->scalePlus();
        break;

    case Qt::Key_Minus:
        k = "Qt::Key_Minus";
        scene->scaleMinus();
        break;

    case Qt::Key_Up:
        k = "Qt::Key_Up";
        scene->rotateUp();
        break;

    case Qt::Key_Down:
        k = "Qt::Key_Down";
        scene->rotateDown();
        break;

    case Qt::Key_Left:
        k = "Qt::Key_Left";
        scene->rotateLeft();
        break;

    case Qt::Key_Right:
        k = "Qt::Key_Right";
        scene->rotateRight();
        break;

    case Qt::Key_Z:
        k = "Qt::Key_Z";
        scene->translateDown();
        break;

    case Qt::Key_X:
        k = "Qt::Key_X";
        scene->translateUp();
        break;

    case Qt::Key_Space:
        k = "Qt::Key_Space";
        scene->defaultScene();
        break;

    case Qt::Key_Delete:
        k = "Qt::Key_Delete";
        break;

    }
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::keyPressEvent(QKeyEvent *pe), key = " << k << "\n";
    scene->updateGL();
}

void DefaultState::dragEnterEvent(QDragEnterEvent *event)
{
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::dragEnterEvent(QDragEnterEvent *event)\n";
    event->acceptProposedAction();
}

void DefaultState::dropEvent(QDropEvent *event)
{
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::dropEvent(QDropEvent *event)\n";
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
        RoadElement::undoStack->push(new InsertCommand(road, stateManager, properties, model, 0, scene));
        return;
    }
    if (s == "Дорога ломаная")
    {

        RoadBroken* road = new RoadBroken(x, y, x + 2.75f, y, 6.0f,
                                          QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                          QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f,
                                          "RoadBroken", 0);
        road->setModel(model);
        RoadElement::undoStack->push(new InsertCommand(road, stateManager, properties, model, 0, scene));
        return;
    }
    if (s == "Поворот дороги")
    {
        RoundingRoad* road = new RoundingRoad(x, y, 10, 0, 90,
                                              x, y, 16, 0, 90,
                                              20, "RoundingRoad", 0,
                                              QApplication::applicationDirPath() + "/models/city_roads/nr_07C.jpg", 6.0f, 6.0f,
                                              QApplication::applicationDirPath() + "/models/city_roads/bksid_11.jpg", 2.75f, 6.0f);
        road->setModel(model);
        RoadElement::undoStack->push(new InsertCommand(road, stateManager, properties, model, 0, scene));
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
            RoadElement::undoStack->push(new InsertCommand(curve, stateManager, properties, model, 0, scene));
            return;
        }
    if (s == "Перекресток")
    {
        bool ok;
        Intersection* intersection = NULL;
        int numberOfRoads = QInputDialog::getInt(0,"Введите колчество рукавов","Количество рукавов",4,3,20,1,&ok);
        if (ok)
            intersection = new Intersection(x, y, numberOfRoads);
        else
            intersection = new Intersection(x, y);
        intersection->setModel(model);
        RoadElement::undoStack->push(new InsertCommand(intersection, stateManager, properties, model, 0, scene));
        return;
    }
    if (s == "Разделительная зона (разметка)")
    {
        SplitZone* splitZone = new SplitZone(x,y,0.02f,
                                             x + 10.0f, y, 0.02f,
                                             2.0f,
                                             true,
                                             true);

        splitZone->setModel(model);
        RoadElement::undoStack->push(new InsertCommand(splitZone, stateManager, properties, model, 1, scene));
        return;
    }
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
        RoadElement::undoStack->push(new InsertCommand(splitZone, stateManager, properties, model, 1, scene));
        return;
    }
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
        RoadElement::undoStack->push(new InsertCommand(splitZone, stateManager, properties, model, 1, scene));
        return;
    }
    if (s == "Сплошная")
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
        RoadElement::undoStack->push(new InsertCommand(line, stateManager, properties, model, 1, scene));
        return;
    }
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
        RoadElement::undoStack->push(new InsertCommand(line, stateManager, properties, model, 1, scene));
        return;
    }
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
        RoadElement::undoStack->push(new InsertCommand(line, stateManager, properties, model, 1, scene));
        return;
    }
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
        RoadElement::undoStack->push(new InsertCommand(line, stateManager, properties, model, 1, scene));
        return;
    }
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
        RoadElement::undoStack->push(new InsertCommand(line, stateManager, properties, model, 1, scene));
        return;
    }
    if (s == "Пешеходный переход")
    {
        RoadSimple* crosswalk = new RoadSimple(x - 2.0f, y, x + 2.0f, y, 2.0f,
                                               QString(":/textures/crosswalk.png"), 1.0f, 1.0f,
                                               QString(":/textures/crosswalk.png"), 1.0f, 1.0f,
                                               "Crosswalk", 1);
        crosswalk->setModel(model);
        RoadElement::undoStack->push(new InsertCommand(crosswalk, stateManager, properties, model, 1, scene));
        return;
    }
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
        RoadElement::undoStack->push(new InsertCommand(tramways, stateManager, properties, model, 1, scene));
        return;
    }
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
        RoadElement::undoStack->push(new InsertCommand(railway, stateManager, properties, model, 1, scene));
        return;
    }


    else
    {

        QStringList lst =  QString(event->mimeData()->data("text/plain")).split(' ');
        if (lst.at(1)[lst.at(1).size() - 1] == 's')
        {

            RoadElement3D* element = new RoadElement3D(x, y);
            stateManager->fileManager3DS->load3DS(lst.at(0).toStdString().c_str(),
                                 lst.at(1).toStdString().c_str(),
                                 element->meshes,
                                 element->materials);

            element->setModel(model);
//            model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
//            model->setModified(true);
//            element->setSelectedStatus(false);
            RoadElement::undoStack->push(new InsertCommand(element, stateManager, properties, model, model->getNumberOfGroups() - 1, scene));

        }
        else
            if (lst.at(1)[lst.at(1).size() - 1] == 'j')
            {
                RoadElementOBJ* element = new RoadElementOBJ(x, y, lst.at(0),lst.at(1));
                stateManager->fileManagerOBJ->loadOBJ(lst.at(0),
                                     lst.at(1),
                                     element->meshes,2.374f, element->scaleFactor);

                element->setModel(model);
//                model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
//                model->setModified(true);
//                element->setSelectedStatus(false);
                RoadElement::undoStack->push(new InsertCommand(element, stateManager, properties, model, model->getNumberOfGroups() - 1, scene));
            }
        else
            if (lst.at(1)[lst.at(1).size() - 1] == 'n')
            {
                RoadElement* element = NULL;
                QString source = lst.at(0) + lst.at(1);
                element = stateManager->fileManagerJSON->readElementFromFile(source);
                if (element)
                {
                    float dx = x - element->getElementX();
                    float dy = y - element->getElementY();
                    element->move(dx, dy);
                    RoadElement::undoStack->push(new InsertCommand(element, stateManager, properties, model, element->getLayer(), scene));
                }
            }

    }













    /*

         else
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



    /*
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

    /*
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
    /*
                RoadElementOBJ* element = new RoadElementOBJ(x, y);
                stateManager->fileManagerOBJ->loadOBJ(lst.at(0),
                                     lst.at(1),
                                     element->meshes,2.374f, element->scaleFactor);
                element->setModel(model);
                model->getGroup(model->getNumberOfGroups() - 1).push_back(element);
                model->setModified(true);
                element->setSelectedStatus(false);
            }
        ////qDebug() << lst.at(1)[lst.at(1).size() - 1];
        ////qDebug() << s.toStdString().c_str();


    }

*/
    //scene->updateGL();
}

DefaultState::~DefaultState()
{
    stateManager = NULL;
    model = NULL;
    scene = NULL;
    properties = NULL;
}

void DefaultState::clearProperties(QFormLayout *layout)
{
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::clearProperties(QFormLayout *layout)\n";
    if (layout == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "QFormLayout* layout = NULL, cannot clearProperties, program terminates");
        if (log)
            Logger::getLogger()->errorLog() << "QFormLayout* layout = NULL, cannot clearProperties, program terminates\n";
        QApplication::exit(0);
    }
    while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }

}

bool DefaultState::tryToSelectFigures(QPoint mp, bool withResult)
{
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::tryToSelectFigures(QPoint mp, bool withResult)\n";
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
        for (QList<RoadElement*>::iterator it = model->getGroup(j).begin();
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
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::tryToSelectFigures(QPoint mp, bool withResult), hits = "
                                       << hitsForFigure << "\n";

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
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::tryToSelectFigures(QPoint mp1, QPoint mp2, bool withResult)\n";
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
        for (QList<RoadElement*>::iterator it = model->getGroup(j).begin();
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
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::tryToSelectFigures(QPoint mp1, QPoint mp2, bool withResult), hits = " << hitsForFigure << "\n";
    selectedElementsCount = hitsForFigure;


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
                        QList<RoadElement*>::iterator it = model->getGroup(selectedGroup).begin();
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
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::keyReleaseEvent(QKeyEvent *pe)\n";
}



QString DefaultState::getName()
{
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::getName()\n";
    return "DefaultState";
}

void DefaultState::drawRect(QPoint p1, QPoint p2)
{
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::drawRect(QPoint p1, QPoint p2)\n";
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
    ////qDebug() << "drawRect";
}

void DefaultState::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::getWorldCoord(double x, double y, double z, double &wx, double &wy, double &wz)\n";
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
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::contextMenuEvent(QContextMenuEvent *pe)\n";
}

void DefaultState::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "DefaultState::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}


void DefaultState::copy()
{
}

void DefaultState::paste()
{
    if (log)
        Logger::getLogger()->infoLog() << "DefaultState::paste()\n";
//    QClipboard *buffer = QApplication::clipboard();
//    const RoadElementMimeData* data = qobject_cast<const RoadElementMimeData*>(buffer->mimeData());
//    QList<RoadElement*> elements = data->getElements();
//    for (int i = 0; i < elements.size(); ++i)
//        model->getGroup(elements[i]->getLayer()).push_back(elements[i]->getCopy());
//    scene->updateGL();

    RoadElement::undoStack->push(new PasteCommand(stateManager,properties,scene,model));
}


void DefaultState::cut()
{
}

void DefaultState::del()
{
}


void DefaultState::clear()
{
}


void DefaultState::saveToPresets()
{
    if (log)
        Logger::getLogger()->infoLog() << "RoadBuilderState::saveToPresets()\n";
    bool ok;
    QString text = QInputDialog::getText(0, tr("Введите название шаблона"),
                                             tr("Название шаблона:"), QLineEdit::Normal,
                                              tr("template"), &ok);
    if (text.size() == 0)
    {
        QMessageBox::warning(0, "Ошибка ввода", "Имя отсутствует! Шаблон не сохранен!");
        return;
    }
    if (ok)
    {
        QString fileName = QApplication::applicationDirPath() + "/models/user/" + text + ".json";
        QList<RoadElement*> elements;
        for (int i = 0; i < model->getNumberOfGroups(); ++i)
        {
            for (int j = 0; j < model->getGroup(i).size(); ++j)
            {
                elements.push_back(model->getGroup(i)[j]);
            }
        }
        JSONFileManager::saveFile(fileName, elements);
        stateManager->processTemplate();
    }
}

#include "selectedstate.h"
//#include "objfilemanager.h"
//#include "_3dsfilemanager.h"
#include <QApplication>
#include <QInputDialog>

bool SelectedState::log = true;

SelectedState::SelectedState()
{
    this->stateManager = NULL;
    this->model = NULL;
    this->scene = NULL;
    resizeByControlCommand = NULL;
    selectedElement = NULL;
    rightButtonIsPressed = false;
    leftButtonIsPressed = false;
    controlIsSelected = false;
    figureIsSelected = false;

    elementIndex = -1;
    groupIndex = -1;
    controlIndex = -1;
}

SelectedState::SelectedState(StateManager *manager, Model *model, Scene2D* scene, QVBoxLayout *properties)
{
    if (log)
        Logger::getLogger()->infoLog() << "Creating SelectedState\n";
    if (manager == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Cannot create SelectedState: StateManager = NULL, program terminates");
        if (log)
            Logger::getLogger()->errorLog() << "Cannot create SelectedState: StateManager = NULL, program terminates\n";
        QApplication::exit(0);
    }
    this->stateManager = manager;
    if (model == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Cannot create SelectedState: Model = NULL, program terminates");
        if (log)
            Logger::getLogger()->errorLog() << "Cannot create SelectedState: Model = NULL, program terminates\n";
        QApplication::exit(0);
    }
    this->model = model;
    if (scene == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Cannot create SelectedState: Scene2D = NULL, program terminates");
        if (log)
            Logger::getLogger()->errorLog() << "Cannot create SelectedState: Scene2D = NULL, program terminates\n";
        QApplication::exit(0);
    }
    this->scene = scene;
    if (properties == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "Cannot create SelectedState: QFormLayout(properties) = NULL, program terminates");
        if (log)
            Logger::getLogger()->errorLog() << "Cannot create SelectedState: QFormLayout(properties) = NULL, program terminates\n";
        QApplication::exit(0);
    }
    this->properties = properties;

    rightButtonIsPressed = false;
    leftButtonIsPressed = false;
    controlIsSelected = false;
    figureIsSelected = false;
    resizeByControlCommand = NULL;
    selectedElement = NULL;
    elementIndex = -1;
    groupIndex = -1;
    controlIndex = -1;
    createActions();
    createMenu();
}



void SelectedState::mousePressEvent(QMouseEvent *pe)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::mousePressEvent(QMouseEvent *pe)\n";
    ptrMousePosition = pe->pos();
    if (selectedElements.size() == 0)
        selectedElement = model->getGroup(groupIndex)[elementIndex];
    switch (pe->button())
    {
    case Qt::LeftButton:
    {
        leftButtonIsPressed = true;
        RoadElement* element = NULL;
        int index = -1;
        if (selectedElements.size() > 0)
        {
            if (this->tryToSelectFigures(pe->pos(), selectedElements) == true)
            {
//                oldX = (GLdouble)pe->pos().x()/
//                        scene->width()/(scene->nSca * scene->ratio) * 2.0;
//                oldY = (GLdouble)(scene->height() -  pe->pos().y())/
//                        scene->width()/(scene->nSca * scene->ratio) * 2.0;
                float x, y;
                scene->getWorldCoord(pe->pos(), x, y);
                oldX = x;
                oldY = y;
                scene->setCursor(Qt::SizeAllCursor);
            }
            else
            {
                clear();
                scene->setMouseTracking(false);
                scene->setCursor(Qt::ArrowCursor);
                scene->updateGL();
                stateManager->setState(stateManager->defaultState);
            }
        }
        else
            if (this->tryToSelectControlsInSelectedFigure(pe->pos(), selectedElement, index) == true)
            {
//                qDebug() << "x" << (GLdouble)pe->x()/
//                        scene->width()/(scene->nSca * scene->ratio) * 2.0 - scene->dX;
//                qDebug() << "y" << (GLdouble)(scene->height() -  pe->y())/
//                        scene->width()/(scene->nSca * scene->ratio) * 2.0 - scene->dY;
//                qDebug() << "x" << (GLdouble)(pe->x() - scene->width() / 2.0)/
//                            scene->width()/(scene->nSca * scene->ratio) * 2.0 + scene->dX;
                controlIsSelected = true;
                controlIndex = index;
                if (resizeByControlCommand == NULL)
                    resizeByControlCommand = new ResizeByControlCommand(selectedElement,controlIndex, scene);
                scene->setCursor(selectedElement->getCursorForControlElement(index));
                scene->updateGL();
                scene->setFocus();
            }
            else
            {
                if (tryToSelectFigures(pe->pos(), element) == true)
                {
                    if (element != selectedElement)
                    {
                        if (keyboardKey == Qt::Key_Control)
                        {
                            if (selectedElements.size() == 0)
                                selectedElements.push_back(selectedElement);
                            element->setSelectedStatus(true);
                            scene->updateGL();
                            selectedElements.push_back(element);
                        }
                        else
                        {
                            for (QList<RoadElement*>::iterator it = selectedElements.begin();
                                 it != selectedElements.end();
                                 ++it)

                                (*it)->setSelectedStatus(false);

                            selectedElements.clear();
                            if (selectedElement)
                            {
                                selectedElement->setSelectedStatus(false);
                                selectedElement->clearProperties(properties);
                            }
                            selectedElement = NULL;
                            keyboardKey = 0;
                            clear();

                            clearProperties(properties);
                            scene->setMouseTracking(false);
                            scene->setCursor(Qt::ArrowCursor);
                            scene->updateGL();
                            stateManager->setState(stateManager->defaultState);
                        }
                    }
                    else
                    {
                        figureIsSelected = true;
//                        oldX = (GLdouble)pe->x()/
//                                scene->width()/(scene->nSca * scene->ratio) * 2.0;
//                        oldY = (GLdouble)(scene->height() -  pe->y())/
//                                scene->width()/(scene->nSca * scene->ratio) * 2.0;
//                        oldX = (GLdouble)pe->pos().x()/
//                                scene->width()/(scene->nSca * scene->ratio) * 2.0;
//                        oldY = (GLdouble)(scene->height() -  pe->pos().y())/
//                                scene->width()/(scene->nSca * scene->ratio) * 2.0;

                        float x, y;
                        scene->getWorldCoord(pe->pos(), x, y);
                        oldX = x;
                        oldY = y;
                    }
                }
                else
                {
                    clear();
                    scene->setMouseTracking(false);
                    scene->setCursor(Qt::ArrowCursor);
                    scene->updateGL();
                    stateManager->setState(stateManager->defaultState);
                }
            }

    }
        break;
    case Qt::RightButton:
        rightButtonIsPressed = true;
        break;
    default:
        break;
    }
}

void SelectedState::mouseMoveEvent(QMouseEvent *pe)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::mouseMoveEvent(QMouseEvent *pe)\n";
    if (selectedElements.size() == 0)
        selectedElement = model->getGroup(groupIndex)[elementIndex];
    if (rightButtonIsPressed == true)
    {
        // двигать камеру
        scene->yDelta += (GLfloat)(pe->y()-ptrMousePosition.y())/
                scene->width()/(scene->nSca * scene->ratio) * 2.0;
        scene->xDelta += (GLfloat)(-1)*(pe->x()-ptrMousePosition.x())/
                scene->width()/(scene->nSca * scene->ratio) * 2.0;
        scene->updateGL();
        ptrMousePosition = pe->pos();
    }
    else
    {
        if (leftButtonIsPressed == true)
        {
            if (selectedElements.size() > 0)
            {
//                float dY = (GLfloat)(-1)*(pe->y()-ptrMousePosition.y())/
//                        scene->width()/(scene->nSca * scene->ratio) * 2.0;
//                float dX = (GLfloat)(pe->x()-ptrMousePosition.x())/
//                        scene->width()/(scene->nSca * scene->ratio) * 2.0;

                float x1, y1, x2, y2, dX, dY;
                scene->getWorldCoord(pe->pos(), x1, y1);
                scene->getWorldCoord(ptrMousePosition, x2, y2);
                dX = x1 - x2;
                dY = y1 - y2;

                for (QList<RoadElement*>::iterator it = selectedElements.begin();
                     it != selectedElements.end(); ++it)
                    (*it)->move(dX, dY);
                scene->updateGL();
                model->setModified(true);
                ptrMousePosition = pe->pos();
            }
            else
                // двигать фигуру или контролы
                if (controlIsSelected == true)
                {
//                    float dY = (GLdouble)(-1)*(pe->pos().y()-ptrMousePosition.y())/
//                            scene->width()/(scene->nSca * scene->ratio) * 2.0;
//                    float dX = (GLdouble)(pe->pos().x()-ptrMousePosition.x())/
//                            scene->width()/(scene->nSca * scene->ratio) * 2.0;

//                    float x = (GLdouble)ptrMousePosition.x()/
//                            scene->width()/(scene->nSca * scene->ratio) * 2.0;
//                    float y = (GLdouble)(scene->height() -  ptrMousePosition.y())/
//                            scene->width()/(scene->nSca * scene->ratio) * 2.0;
                    float x1, y1, x2, y2, dX, dY, x, y;
                    scene->getWorldCoord(pe->pos(), x1, y1);
                    scene->getWorldCoord(ptrMousePosition, x2, y2);
                    dX = x1 - x2;
                    dY = y1 - y2;
                    scene->getWorldCoord(ptrMousePosition, x, y);
                    selectedElement->resizeByControl(controlIndex, dX, dY, x, y);
                    scene->updateGL();
                    model->setModified(true);
                    ptrMousePosition = pe->pos();
                }
                else
                {
                    if (figureIsSelected == true)
                    {
//                        float dY = (GLfloat)(-1)*(pe->y()-ptrMousePosition.y())/
//                                scene->width()/(scene->nSca * scene->ratio) * 2.0;
//                        float dX = (GLfloat)(pe->x()-ptrMousePosition.x())/
//                                scene->width()/(scene->nSca * scene->ratio) * 2.0;
                        float x1, y1, x2, y2, dX, dY;
                        scene->getWorldCoord(pe->pos(), x1, y1);
                        scene->getWorldCoord(ptrMousePosition, x2, y2);
                        dX = x1 - x2;
                        dY = y1 - y2;
                        selectedElement->move(dX, dY);
                        scene->updateGL();
                        model->setModified(true);
                        ptrMousePosition = pe->pos();
                    }
                }
        }
        else
        {

            if (selectedElements.size() > 0)
            {
                if (this->tryToSelectFigures(pe->pos(), selectedElements) == true)
                {
                    scene->setCursor(Qt::SizeAllCursor);
                }
                else
                {
                    scene->setCursor(Qt::ArrowCursor);
                }
            }
            else
            {
                // изменять курсор в зависимости от положения мыши
                int index = -1;
                if (tryToSelectControlsInSelectedFigure(pe->pos(), selectedElement, index) == true)
                {
                    scene->setCursor(selectedElement->getCursorForControlElement(index));
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

}

void SelectedState::mouseReleaseEvent(QMouseEvent *pe)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::mouseReleaseEvent(QMouseEvent *pe)\n";
    if(pe->button() == Qt::RightButton)
    {
        rightButtonIsPressed = false;
    }
    if(pe->button() == Qt::LeftButton)
    {

        if (selectedElements.size() > 0)
        {
//            newX = (GLdouble)pe->x()/
//                    scene->width()/(scene->nSca * scene->ratio) * 2.0;
//            newY = (GLdouble)(scene->height() -  pe->y())/
//                    scene->width()/(scene->nSca * scene->ratio) * 2.0;

            float x, y;
            scene->getWorldCoord(pe->pos(), x, y);
            newX = x;
            newY = y;
            RoadElement::undoStack->push(new MoveCommand(selectedElements, oldX, oldY, newX, newY, scene));
        }
        else
            if (controlIsSelected)
            {
                resizeByControlCommand->setNewPosition();
                RoadElement::undoStack->push(resizeByControlCommand);
                resizeByControlCommand = NULL;
            } else
                if (figureIsSelected)
                {
//                    newX = (GLdouble)pe->x()/
//                            scene->width()/(scene->nSca * scene->ratio) * 2.0;
//                    newY = (GLdouble)(scene->height() -  pe->y())/
//                            scene->width()/(scene->nSca * scene->ratio) * 2.0;

                    float x, y;
                    scene->getWorldCoord(pe->pos(), x, y);
                    newX = x;
                    newY = y;
                    RoadElement::undoStack->push(new MoveCommand(selectedElement, oldX, oldY, newX, newY, scene));

                }
        leftButtonIsPressed = false;
        figureIsSelected = false;
        controlIsSelected = false;
    }
    ptrMousePosition = pe->pos();
}

void SelectedState::wheelEvent(QWheelEvent *pe)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::wheelEvent(QWheelEvent *pe)\n";
    if ((pe->delta())>0) scene->scalePlus();
    else
        if ((pe->delta())<0) scene->scaleMinus();

    scene->updateGL();
}

void SelectedState::keyPressEvent(QKeyEvent *pe)
{
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

        //    case Qt::Key_Delete:
        //    {
        //        s = "Qt::Key_Delete";
        //        if (selectedElements.size() > 0)
        //        {
        //            RoadElement::undoStack->push(new DeleteCommand(selectedElements, model, stateManager, properties, scene));
        //        }
        //        else
        //        {
        //            RoadElement::undoStack->push(new DeleteCommand(selectedElement, model, stateManager, properties, groupIndex, elementIndex, scene));
        //        }
        //    }
        //        break;
    case Qt::Key_Control:
        s = "Qt::Key_Control";
        keyboardKey = pe->key();
        break;
    default:
        break;
    }
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::keyPressEvent(QKeyEvent *pe), key = " << s << "\n";
    scene->updateGL();
}

void SelectedState::dragEnterEvent(QDragEnterEvent *event)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::dragEnterEvent(QDragEnterEvent *event)\n";
    event->acceptProposedAction();
}

void SelectedState::dropEvent(QDropEvent *event)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::dropEvent(QDropEvent *event)\n";
    stateManager->defaultState->dropEvent(event);
}

void SelectedState::setSelectedElement(int elementIndex, int groupIndex)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::setSelectedElement(int elementIndex = "
                                       << elementIndex << ", int groupIndex = " << groupIndex << ")\n";
    if (groupIndex >= model->getNumberOfGroups() || groupIndex < 0)
    {
        QMessageBox::critical(0, "Ошибка", "SelectedState::setSelectedElement(int elementIndex, int groupIndex) In model group index = " + QString::number(groupIndex) + " out of range",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->errorLog() << "SelectedState::setSelectedElement(int elementIndex, int groupIndex) In model group index = " << groupIndex << " out of range\n";
        QApplication::exit(0);
    }
    if (elementIndex >= model->getGroup(groupIndex).size() || elementIndex < 0)
    {
        QMessageBox::critical(0, "Ошибка", "In model element index = " + QString::number(elementIndex) + " out of range",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->errorLog() << "In model element index = " << elementIndex << " out of range\n";
        QApplication::exit(0);
    }
    this->elementIndex = elementIndex;
    this->groupIndex = groupIndex;
}

void SelectedState::setSelectedElement(RoadElement *element)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::setSelectedElement(RoadElement *element)\n";
    if (element == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "RoadElement* element = NULL, program terminates",
                              QMessageBox::Yes);
        if (log)
            Logger::getLogger()->errorLog() << "RoadElement* element = NULL, program terminates\n";
        QApplication::exit(0);
    }
    this->selectedElement = element;
}

void SelectedState::setLogging(bool status)
{
    log = status;
    Logger::getLogger()->infoLog() << "--------------------\n";
    Logger::getLogger()->infoLog() << "SelectedState::setLogging(bool status)"
                                   << " status = " << status << "\n";
    Logger::getLogger()->infoLog() << "--------------------\n";
}

SelectedState::~SelectedState()
{
    stateManager = NULL;
    model = NULL;
    scene = NULL;
    properties = NULL;
    selectedElement = NULL;

    delete contextMenu;
    contextMenu = NULL;

    delete fixAction;
    fixAction = NULL;
    delete combineAction;
    combineAction = NULL;
    delete breakAction;
    breakAction = NULL;
}

void SelectedState::clearProperties(QVBoxLayout *layout)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::clearProperties(QFormLayout *layout)\n";
    if (layout == NULL)
    {
        QMessageBox::critical(0, "Ошибка", "QFormLayout *layout = NULL, cannot claerProperties, program terminates");
        if (log)
            Logger::getLogger()->errorLog() << "QFormLayout *layout = NULL, cannot claerProperties, program terminates\n";
        QApplication::exit(0);
    }
//    while(layout->count() > 0)
//    {
//        QLayoutItem *item = layout->takeAt(0);
//        delete item->widget();
//        delete item;
//    }


}

bool SelectedState::tryToSelectFigures(QPoint mp, QList<RoadElement*>& elements)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::tryToSelectFigures(QPoint mp, QList<RoadElement*>& elements)\n";
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

    for (QList<RoadElement*>::iterator it = elements.begin(); it != elements.end(); ++it)
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


    hitsForFigure=glRenderMode(GL_RENDER); // число совпадений и переход в режим рисования
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::tryToSelectFigures(QPoint mp, QList<RoadElement*>& elements), hits = "
                                       << hitsForFigure << "\n";
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

bool SelectedState::tryToSelectAllFigures(QPoint mp)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::tryToSelectAllFigures(QPoint mp)\n";
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
    for (int j = model->getNumberOfGroups() - 2; j >= 0; --j)
    {
        int element = 0;
        for (QList<RoadElement*>::iterator it = model->getGroup(j).begin();
             it != model->getGroup(j).end(); ++it, ++element)
        {
            if (element == elementIndex && j == groupIndex)
                continue;
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
        Logger::getLogger()->infoLog() << "SelectedState::tryToSelectAllFigures(QPoint mp), hits = "
                                       << hitsForFigure << "\n";
    if (hitsForFigure > 0) // есть совпадания и нет ошибок
    {
        i = selectBuffer[3] - 1; // имя фигуры верхняя фигура
        for (int j = model->getNumberOfGroups() - 2; j >= 0; --j)
        {
            if (i < model->getGroup(j).size())
            {
                //stateManager->selectedState->setSelectedElement(i, j);
                /*
                if (i != elementIndex || j != groupIndex)
                {
                    elementIndex = i;
                    groupIndex = j;
                }
                */
                QList<RoadElement*>::iterator it = model->getGroup(j).begin();
                for(int k = 0; k < i; ++k)
                    ++it;
                elementIterator = it;
                (*it)->setSelectedStatus(true);
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

bool SelectedState::tryToSelectControlsInSelectedFigure(QPoint mp)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::tryToSelectControlsInSelectedFigure(QPoint mp)\n";
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

    QList<RoadElement*>::iterator it = model->getGroup(groupIndex).begin();
    for (int j = 0; j < elementIndex; ++j)
        ++it;

    for (int i = 1; i < (*it)->getNumberOfControls() + 1; ++i)
    {
        glPushMatrix();
        glScalef(nSca, nSca, nSca);
        gluLookAt(xDelta,yDelta,0.5,
                  xDelta,yDelta,-10,
                  0,1,0);
        glLoadName(i); // загрузить имя на вершину стека имён
        (*it)->drawControlElement(i - 1, 15.0f, 30.0f);

        glPopMatrix();
    }


    hitsForControl = glRenderMode(GL_RENDER); // число совпадений и переход в режим рисования
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::tryToSelectControlsInSelectedFigure(QPoint mp), hits = "
                                       << hitsForControl << "\n";
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

bool SelectedState::tryToSelectControlsInSelectedFigure(QPoint mp, RoadElement *element, int &index)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::tryToSelectControlsInSelectedFigure(QPoint mp, RoadElement *element, int &index)\n";
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
        Logger::getLogger()->infoLog() << "SelectedState::tryToSelectControlsInSelectedFigure(QPoint mp, RoadElement *element, int &index), hits = "
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

bool SelectedState::tryToSelectFigures(QPoint mp, RoadElement *&element)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::tryToSelectFigures(QPoint mp, RoadElement *&element)\n";
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
    for (int j = model->getNumberOfGroups() - 2; j >= 0; --j)
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
        Logger::getLogger()->infoLog() << "SelectedState::tryToSelectFigures(QPoint mp, RoadElement *&element), hits = "
                                       << hitsForFigure << "\n";

    if (hitsForFigure > 0) // есть совпадания и нет ошибок
    {
        i = selectBuffer[3] - 1; // имя фигуры верхняя фигура
        for (int j = model->getNumberOfGroups() - 2; j >= 0; --j)
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

void SelectedState::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::getWindowCoord(double x, double y, double z, double &wx, double &wy, double &wz)\n";
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    gluProject (x, y, z, mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
    wy=viewport[3]-wy;
}

void SelectedState::createMenu()
{
    //contextMenu->clear();
    contextMenu = new QMenu(scene);
    contextMenu->addAction(fixAction);
    contextMenu->addAction(combineAction);
    contextMenu->addAction(breakAction);
}

void SelectedState::createActions()
{
    fixAction = new QAction(QObject::tr("Зафиксировать"), scene);

    fixAction->setCheckable(true);
    fixAction->setChecked(false);
    //std::list<RoadElement*>::iterator it = model->getGroup(groupIndex).begin();
    //for (int j = 0; j < elementIndex; ++j)
    //    ++it;
    //QObject::connect(fixAction, SIGNAL(toggled(bool)),(*it), SLOT(setFixed(bool)));

    combineAction = new QAction(QObject::tr("Объединить"), scene);
    QObject::connect(combineAction, SIGNAL(triggered()), this, SLOT(combineElements()));

    breakAction = new QAction(QObject::tr("Разбить"), scene);
    QObject::connect(breakAction, SIGNAL(triggered()), this, SLOT(breakElements()));
}

void SelectedState::combineElements()
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::combineElements()\n";
    if (selectedElements.size() > 1)
    {
        CompositeRoad* element = new CompositeRoad();
        for (QList<RoadElement*>::iterator it = selectedElements.begin();
             it != selectedElements.end();
             ++it)
        {
            element->addElement((*it));

            for (int i = 0; i < model->getNumberOfGroups(); ++i)
            {
                for (QList<RoadElement*>::iterator iter = model->getGroup(i).begin();
                     iter != model->getGroup(i).end();
                     ++iter)
                {
                    if ((*it) == (*iter))
                    {
                        model->getGroup(i).erase(iter);
                        break;
                    }
                }
            }

        }

        model->getGroup(0).push_back(element);
        selectedElements.clear();
        element->setSelectedStatus(false);
        scene->setCursor(Qt::ArrowCursor);
        stateManager->setState(stateManager->defaultState);
        scene->updateGL();
    }
}

void SelectedState::breakElements()
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::breakElements()\n";
    if (selectedElement->getName() != "CompositeRoad")
        return;
    selectedElements.clear();
    for (int i = 0; i < selectedElement->getNumberOfElements(); ++i)
    {
        RoadElement* element = selectedElement->getElement(i);
        if (element == NULL)\
            qDebug() << "element null";
        int layer = element->getLayer();
        selectedElements.push_back(element);
        model->getGroup(layer).push_back(element);

        qDebug() << "number:" << i;
    }
    model->getGroup(0).removeOne(selectedElement);

    selectedElement = NULL;
    qDebug() << "Good";
}

void SelectedState::clear()
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::clear()\n";
    if (selectedElement)
    {
        selectedElement->clearProperties(properties);
        selectedElement->setSelectedStatus(false);
    }
    selectedElement = NULL;
    rightButtonIsPressed = false;
    leftButtonIsPressed = false;
    controlIsSelected = false;
    figureIsSelected = false;
    elementIndex = -1;
    groupIndex = -1;
    controlIndex = -1;
    for (QList<RoadElement*>::iterator it = selectedElements.begin();
         it != selectedElements.end(); ++it)
    {
        (*it)->setSelectedStatus(false);
        (*it) = NULL;
    }
    selectedElements.clear();
    resizeByControlCommand = NULL;
    keyboardKey = -1;
}


void SelectedState::keyReleaseEvent(QKeyEvent *pe)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::keyReleaseEvent(QKeyEvent *pe)\n";
    if (pe->key() == Qt::Key_Control)
    {
        keyboardKey = 0;
    }
}


QString SelectedState::getName()
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::getName()\n";
    return "SelectedState";
}


void SelectedState::contextMenuEvent(QContextMenuEvent *pe)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::contextMenuEvent(QContextMenuEvent *pe)\n";
    if (selectedElements.size() > 1)
    {
        if (tryToSelectFigures(pe->pos(), selectedElements) == true)
        {
            combineAction->setVisible(true);
            breakAction->setVisible(false);
            contextMenu->exec(pe->globalPos());
        }

    }
    else
        if (selectedElement->getName() == "CompositeRoad")
        {
            combineAction->setVisible(false);
            breakAction->setVisible(true);
            contextMenu->exec(pe->globalPos());
        }
        else
        {
            //combineAction->setVisible(false);
            //breakAction->setVisible(true);
        }
    //contextMenu->exec(pe->globalPos());
}

bool SelectedState::tryToSelectFigures(QPoint mp)
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::tryToSelectFigures(QPoint mp)\n";
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

    glPushMatrix();
    glScalef(nSca, nSca, nSca);
    gluLookAt(xDelta,yDelta,0.5,
              xDelta,yDelta,-10,
              0,1,0);
    glLoadName(i++); // загрузить имя на вершину стека имён
    selectedElement->drawFigure();
    glPopMatrix();


    hitsForFigure = glRenderMode(GL_RENDER); // число совпадений и переход в режим рисования
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::tryToSelectFigures(QPoint mp), hits = "
                                       << hitsForFigure << "\n";

    if (hitsForFigure > 0) // есть совпадания и нет ошибок
    {

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


void SelectedState::copy()
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::copy()\n";
    QClipboard *buffer = QApplication::clipboard();
    RoadElementMimeData* data = NULL;
    if (selectedElements.size() > 0)
    {
        data = new RoadElementMimeData(selectedElements);
    }
    else
    {
        data = new RoadElementMimeData(selectedElement);
    }
    buffer->setMimeData(data);
}

void SelectedState::paste()
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::paste()\n";
    RoadElement::undoStack->push(new PasteCommand(stateManager,properties,scene,model));
}


void SelectedState::cut()
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::cut()\n";
    copy();
    del();
}

void SelectedState::del()
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::del()\n";

    if (selectedElements.size() > 0)
    {
        int result = QMessageBox::warning(0, tr("Удаление элементов"),
                                     tr("Вы действительно хотите удалить выделенные элементы?"),
                                     QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::Yes)
            RoadElement::undoStack->push(new DeleteCommand(selectedElements, model, stateManager, properties, scene));
    }
    else
    {
        int result = QMessageBox::warning(0, tr("Удаление элемента"),
                                     tr("Вы действительно хотите удалить выделенный элемент?"),
                                     QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::Yes)
            RoadElement::undoStack->push(new DeleteCommand(selectedElement, model, stateManager, properties, groupIndex, elementIndex, scene));
    }
}


void SelectedState::saveToPresets()
{
    if (log)
        Logger::getLogger()->infoLog() << "SelectedState::saveToPresets()\n";
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
        if (selectedElements.size() > 0)
        {
//            CompositeRoad* road = new CompositeRoad();
//            for (int i = 0; i < selectedElements.size(); ++i)
//            {
//                road->addElement(selectedElements[i]);
//            }
//            JSONFileManager::saveFile(fileName, road);
            JSONFileManager::saveFile(fileName, selectedElements);
        }
        else
            JSONFileManager::saveFile(fileName, selectedElement);
        stateManager->processTemplate();

    }
}

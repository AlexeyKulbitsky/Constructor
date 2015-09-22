#include "multiselectedstate.h"

MultiSelectedState::MultiSelectedState()
{
    this->stateManager = NULL;
    this->model = NULL;
    this->scene = NULL;

    rightButtonIsPressed = false;
    leftButtonIsPressed = false;
    controlIsSelected = false;
    figureIsSelected = false;

    elementIndex = -1;
    groupIndex = -1;
    controlIndex = -1;
}

MultiSelectedState::MultiSelectedState(StateManager *manager, Model *model, Scene2D *scene, QFormLayout *properties)
{
    this->stateManager = manager;
    this->model = model;
    this->scene = scene;
    this->properties = properties;

    rightButtonIsPressed = false;
    leftButtonIsPressed = false;
    controlIsSelected = false;
    figureIsSelected = false;

    elementIndex = -1;
    groupIndex = -1;
    controlIndex = -1;
    createActions();
    createMenu();
}

MultiSelectedState::~MultiSelectedState()
{

}

void MultiSelectedState::clearProperties(QFormLayout *layout)
{
    while(QLayoutItem* child = layout->takeAt(0))
    {
        delete child->widget();
        delete child;
    }
}

bool MultiSelectedState::tryToSelectFigures(QPoint mp)
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
        i = selectBuffer[3] - 1; // имя фигуры верхняя фигура
        for (int j = model->getNumberOfGroups() - 1; j >= 0; --j)
        {
            if (i < model->getGroup(j).size())
            {
                elementIndex = i;
                groupIndex = j;
                std::list<RoadElement*>::iterator it = model->getGroup(j).begin();
                for(int k = 0; k < i; ++k)
                    ++it;
                //(*it)->setSelectedStatus(true);
                //scene->setCursor(Qt::SizeAllCursor);
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

void MultiSelectedState::createActions()
{
    combineAction = new QAction(QObject::tr("Объединить"), scene);
    QObject::connect(combineAction, SIGNAL(triggered()), this, SLOT(combineElements()));
}

void MultiSelectedState::createMenu()
{
    contextMenu = new QMenu(scene);
    contextMenu->addAction(combineAction);
}



void MultiSelectedState::mousePressEvent(QMouseEvent *pe)
{
    ptrMousePosition = pe->pos();
    switch (pe->button())
    {
    case Qt::LeftButton:
    {
        if (keyboardKey == Qt::Key_Control)
        {
            leftButtonIsPressed = true;
            if (this->tryToSelectFigures(pe->pos()) == true)
            {

                for (std::list<ElementInfo>::iterator it = selectedElements.begin();
                     it != selectedElements.end();
                     ++it)
                {
                    if (it->elementIndex == elementIndex &&
                            it->groupIndex == groupIndex)
                    {
                        std::list<RoadElement*>::iterator el = model->getGroup(groupIndex).begin();
                        for(int k = 0; k < elementIndex; ++k)
                            ++el;
                        (*el)->setSelectedStatus(false);
                        selectedElements.erase(it);
                        scene->setCursor(Qt::SizeAllCursor);
                        scene->updateGL();
                        return;
                    }
                }
                std::list<RoadElement*>::iterator it = model->getGroup(groupIndex).begin();
                for(int k = 0; k < elementIndex; ++k)
                    ++it;
                (*it)->setSelectedStatus(true);
                scene->setCursor(Qt::SizeAllCursor);
                addSelectedElementToTheGroup(elementIndex, groupIndex);
            }
            else
            {

            }
        }
    }
        break;
    default:
        break;
    }

    scene->updateGL();
}

void MultiSelectedState::mouseMoveEvent(QMouseEvent *pe)
{
    if (this->tryToSelectFigures(pe->pos()) == true)
    {
        for (std::list<ElementInfo>::iterator it = selectedElements.begin();
             it != selectedElements.end();
             ++it)
        {
            if (it->elementIndex == elementIndex &&
                    it->groupIndex == groupIndex)
            {
                scene->setCursor(Qt::SizeAllCursor);
                break;
            }
        }

    }
    else
    {
        scene->setCursor(Qt::ArrowCursor);
    }
    ptrMousePosition = pe->pos();
    scene->updateGL();
}

void MultiSelectedState::mouseReleaseEvent(QMouseEvent *pe)
{
}

void MultiSelectedState::wheelEvent(QWheelEvent *pe)
{
}

void MultiSelectedState::keyPressEvent(QKeyEvent *pe)
{
    keyboardKey = pe->key();
}

void MultiSelectedState::keyReleaseEvent(QKeyEvent *)
{
    keyboardKey = 0;
}

void MultiSelectedState::dragEnterEvent(QDragEnterEvent *event)
{
}

void MultiSelectedState::dropEvent(QDropEvent *event)
{
}



void MultiSelectedState::addSelectedElementToTheGroup(int elementIndex, int groupIndex)
{
    ElementInfo info;
    info.elementIndex = elementIndex;
    info.groupIndex = groupIndex;

    selectedElements.push_back(info);
}

void MultiSelectedState::clearSelectedElementsGroup()
{
    selectedElements.clear();
}


QString MultiSelectedState::getName()
{
    return "MultiSelectedState";
}


void MultiSelectedState::contextMenuEvent(QContextMenuEvent *pe)
{
    contextMenu->exec(pe->globalPos());
}

void MultiSelectedState::combineElements()
{
    if (selectedElements.size() > 1)
    {
        std::list<RoadElement*> temp;
        CompositeRoad* element = new CompositeRoad();
        for (std::list<ElementInfo>::iterator it = selectedElements.begin();
             it != selectedElements.end();
             ++it)
        {
            std::list<RoadElement*>::iterator el = model->getGroup(it->groupIndex).begin();
            for(int k = 0; k < it->elementIndex; ++k)
                ++el;
            element->addElement((*el));
            temp.push_back((*el));
        }
        std::list<RoadElement*>::iterator tmp = temp.begin();

        for (std::list<ElementInfo>::iterator it = selectedElements.begin();
             it != selectedElements.end();
             ++it)
        {
            model->getGroup(it->groupIndex).remove((*tmp));
            tmp++;
        }
        model->getGroup(0).push_back(element);

        element->setSelectedStatus(false);
        scene->setCursor(Qt::ArrowCursor);
        stateManager->setState(stateManager->defaultState);
        scene->updateGL();
    }
}
